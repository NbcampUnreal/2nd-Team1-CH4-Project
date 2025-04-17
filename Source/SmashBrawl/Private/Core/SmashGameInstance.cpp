#include "Core/SmashGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include <Online/OnlineSessionNames.h>
#include "Engine/World.h"

USmashGameInstance::USmashGameInstance()
{
	PlayerTeam = { 1, 2, 3, 1 };
	AliveArray = { true, true, false, false };
	CPU = { false, false, false, false };
	PlayerCharacters = {
		ESmashCharacter::Fighter,
		ESmashCharacter::Fighter,
		ESmashCharacter::Gunner,
		ESmashCharacter::Hats
	};
}

void USmashGameInstance::Init()
{
	Super::Init();

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USmashGameInstance::OnJoinSessionComplete);
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USmashGameInstance::OnCreateSessionComplete);
			UE_LOG(LogTemp, Warning, TEXT("✅ OnlineSubsystem 초기화 성공: %s"), *Subsystem->GetSubsystemName().ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ OnlineSubsystem을 찾을 수 없습니다"));
	}
}

void USmashGameInstance::HostSession()
{
	UE_LOG(LogTemp, Warning, TEXT("🔧 HostSession() 함수 진입"));

	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("❌ SessionInterface가 유효하지 않음 (Steam 로그인을 했을 때만 발생하는지 확인 필요)"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("🟢 SessionInterface 유효, 세션 생성 시도"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.NumPublicConnections = 4;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bAllowJoinInProgress = true;

	// ✅ SteamID를 세션에 포함 (CreateSession 전에!)
	FString HostId = IOnlineSubsystem::Get()->GetIdentityInterface()->GetUniquePlayerId(0)->ToString();
	SessionSettings.Set(TEXT("HostSteamID"), HostId, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	UE_LOG(LogTemp, Warning, TEXT("🆔 호스트 SteamID 저장: %s"), *HostId);

	FName SessionName = TEXT("SmashSession");
	SessionInterface->CreateSession(0, SessionName, SessionSettings);
	UE_LOG(LogTemp, Warning, TEXT("📡 세션 생성 시도 중..."));
}


void USmashGameInstance::FindSession()
{
	if (!SessionInterface.IsValid()) return;

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults = 20;
	SessionSearch->PingBucketSize = 50;

	// 🔍 Steam 세션 필터링
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals); // Steam 전용 설정

	UE_LOG(LogTemp, Warning, TEXT("🔍 세션 검색 중..."));
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USmashGameInstance::OnFindSessionsComplete);
}

void USmashGameInstance::JoinSession()
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;

	// 현재 접속 중인 플레이어의 Steam ID를 가져오기
	FString MySteamID = IOnlineSubsystem::Get()->GetIdentityInterface()->GetUniquePlayerId(0)->ToString();
	UE_LOG(LogTemp, Warning, TEXT("🧑 내 SteamID: %s"), *MySteamID);

	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		if (!SearchResult.IsValid()) continue;

		FString HostSteamID;
		if (SearchResult.Session.SessionSettings.Get(TEXT("HostSteamID"), HostSteamID))
		{
			UE_LOG(LogTemp, Warning, TEXT("📜 세션 HostSteamID: %s"), *HostSteamID);

			// 자기 자신 세션은 스킵
			if (HostSteamID == MySteamID)
			{
				UE_LOG(LogTemp, Warning, TEXT("⚠️ 내 세션은 제외 (SteamID: %s)"), *HostSteamID);
				continue;
			}

			// ✅ 조건 만족한 세션 참가 시도
			UE_LOG(LogTemp, Warning, TEXT("🔗 세션 참가 시도: %s"), *SearchResult.GetSessionIdStr());

			// ✅ 강제 설정 일치 (Steam workaround)
			const_cast<FOnlineSessionSearchResult&>(SearchResult).Session.SessionSettings.bUsesPresence = true;
			const_cast<FOnlineSessionSearchResult&>(SearchResult).Session.SessionSettings.bUseLobbiesIfAvailable = true;

			bool bSuccess = SessionInterface->JoinSession(0, TEXT("SmashSession"), SearchResult);
			if (bSuccess)
			{
				UE_LOG(LogTemp, Warning, TEXT("📡 JoinSession 호출 성공"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("❌ JoinSession 호출 실패"));
			}

			return; // 첫 번째 유효한 세션만 참가
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("❓ HostSteamID 키가 존재하지 않음, 세션 ID: %s"), *SearchResult.GetSessionIdStr());
		}
	}

	UE_LOG(LogTemp, Error, TEXT("❌ 유효한 세션을 찾지 못했습니다"));
}

void USmashGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (!SessionSearch.IsValid()) return;

	UE_LOG(LogTemp, Warning, TEXT("🔍 세션 검색 완료 (%d개 결과)"), SessionSearch->SearchResults.Num());

	// ✅ 검색된 세션들 디버깅 출력
	for (int32 Index = 0; Index < SessionSearch->SearchResults.Num(); ++Index)
	{
		const FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[Index];
		UE_LOG(LogTemp, Warning, TEXT("📜 세션 %d: ID: %s, Valid: %s"),
			Index,
			*Result.GetSessionIdStr(),
			Result.IsValid() ? TEXT("YES") : TEXT("NO"));
	}

	if (bWasSuccessful && SessionSearch->SearchResults.Num() > 0 && !bHasJoinedSession)
	{
		UE_LOG(LogTemp, Warning, TEXT("✅ 세션 검색 성공"));
		bHasJoinedSession = true;
		JoinSession(); // 단 한 번만!
	}
}


void USmashGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("✅ 세션 생성 성공: %s"), *SessionName.ToString());
		bShouldShowHostSuccess = true;

		UWorld* World = GetWorld();
		if (World && World->GetNetMode() != NM_Client) // ✅ HasAuthority() 대신 사용
		{
			ShowWidget(HostSuccessWidgetClass);
			FTimerHandle TimerHandle;
			World->GetTimerManager().SetTimer(TimerHandle, [World]()
				{
					UE_LOG(LogTemp, Warning, TEXT("🌍 딜레이 후 맵 이동"));
					FString MapPath = TEXT("/Game/PlatformFighterKit/Maps/Menu/OnlineLobby?listen");
					World->ServerTravel(MapPath);
				}, 2.0f, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("❌ 클라이언트이므로 ServerTravel 무시"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 세션 생성 실패"));
	}
}

void USmashGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString ConnectString;
	if (SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
	{
		UWorld* World = GetWorld();
		if (!World)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ OnJoinSessionComplete: GetWorld() 반환값이 NULL입니다."));
			return;
		}

		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
		if (PC)
		{
			UE_LOG(LogTemp, Warning, TEXT("✅ 세션 참가 성공, 접속 주소: %s"), *ConnectString);
			ShowWidget(ClientSuccessWidgetClass);

			// 중복 ClientTravel 방지
			static bool bTravelDone = false;
			if (!bTravelDone)
			{
				bTravelDone = true;
				PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ PlayerController를 찾지 못했습니다."));
		}
	}
	else
	{
		// ✳️ 실패 로그 보강
		UE_LOG(LogTemp, Error, TEXT("❌ 접속 주소를 얻지 못했습니다 (SessionName: %s, Result: %d)"), *SessionName.ToString(), (int32)Result);

		switch (Result)
		{
		case EOnJoinSessionCompleteResult::SessionIsFull:
			UE_LOG(LogTemp, Error, TEXT("🔴 세션이 가득 찼습니다"));
			break;
		case EOnJoinSessionCompleteResult::SessionDoesNotExist:
			UE_LOG(LogTemp, Error, TEXT("🔴 세션이 존재하지 않습니다"));
			break;
		case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
			UE_LOG(LogTemp, Error, TEXT("🔴 세션 주소를 가져오지 못했습니다"));
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("🔴 알 수 없는 오류"));
			break;
		}
	}
}


void USmashGameInstance::ShowWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	UWorld* World = GetWorld();
	if (!World || !WidgetClass) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC) return;

	ActiveWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
	if (ActiveWidget)
	{
		ActiveWidget->AddToViewport();
		PC->bShowMouseCursor = true;
	}
}
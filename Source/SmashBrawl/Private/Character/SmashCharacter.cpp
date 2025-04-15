#include "SmashBrawl/Public/Character//SmashCharacter.h"

#include "EnhancedInputComponent.h"
#include "AbilitySystem/BaseAbility.h"
#include "AbilitySystem/SmashAbilitySystemComponent.h"
#include "Character/Camera/SmashCameraComponent.h"
#include "Character/Components/FXComponent.h"
#include "Character/Components/SmashStateSystem.h"
#include "Character/Components/SmashCharacterMovementComponent.h"
#include "Character/Components/SmashCharacterStats.h"
#include "Character/Components/SmashCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/SmashGameInstance.h"
#include "Core/SmashGameState.h"
#include "Core/SmashPlatFighterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/HUD/InGame/UW_HUD_PlayerName.h"

DEFINE_LOG_CATEGORY(LogSmashCharacter);


ASmashCharacter::ASmashCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
	        .SetDefaultSubobjectClass<USmashCharacterMovementComponent>(ASSTCharacter::CharacterMovementComponentName)
	        .SetDefaultSubobjectClass<USmashCameraComponent>(TEXT("FollowCamera")))
{
	// Tick 설정 - 초기에는 비활성화, StartUp에서 활성화
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.SetTickFunctionEnable(false);

	SmashCameraComponent = Cast<USmashCameraComponent>(GetFollowCamera());
	SmashCharacterMovementComponent = Cast<USmashCharacterMovementComponent>(GetCharacterMovement());
	SmashStateSystem = CreateDefaultSubobject<USmashStateSystem>(TEXT("SmashCharacterState"));
	SmashCharacterStatsComponent = CreateDefaultSubobject<USmashCharacterStats>(TEXT("SmashCharacterStatsComponent"));
	SmashCombatComponent = CreateDefaultSubobject<USmashCombatComponent>(TEXT("SmashCombatComponent"));
	AbilitySystemComponent = CreateDefaultSubobject<USmashAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	FXComponent = CreateDefaultSubobject<UFXComponent>(TEXT("FXComponent"));

	// 초기화 상태 및 기본값 설정
	bInitialized = false;
	Attacks = ESmashAttacks::None;
	Direction = ESmashDirection::None;
	AbilityType = ESmashAbilityTypes::None;
}

void ASmashCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 상태 및 메타데이터
	DOREPLIFETIME(ASmashCharacter, StateInfo);
	DOREPLIFETIME(ASmashCharacter, LastHit);
	DOREPLIFETIME(ASmashCharacter, PlayerNo);
	DOREPLIFETIME(ASmashCharacter, Team);

	// 입력 및 위치 데이터
	DOREPLIFETIME(ASmashCharacter, MoveInputValue);
	DOREPLIFETIME(ASmashCharacter, UpDownInputValue);
	DOREPLIFETIME(ASmashCharacter, Direction);
	DOREPLIFETIME(ASmashCharacter, Location);
	DOREPLIFETIME(ASmashCharacter, YPos);
	DOREPLIFETIME(ASmashCharacter, ZPos);
	DOREPLIFETIME(ASmashCharacter, FootZPos);
	DOREPLIFETIME(ASmashCharacter, LocationFeet);

	// 상태 플래그
	DOREPLIFETIME(ASmashCharacter, bAttachedAbil);
	DOREPLIFETIME(ASmashCharacter, bLedgeCooldown);
	DOREPLIFETIME(ASmashCharacter, bLanded);
	DOREPLIFETIME(ASmashCharacter, bDizzied);
	DOREPLIFETIME(ASmashCharacter, bPushed);
	DOREPLIFETIME(ASmashCharacter, bCanSmash);
	DOREPLIFETIME(ASmashCharacter, bBufferdInput);
	DOREPLIFETIME(ASmashCharacter, bBufferdDirection);
	DOREPLIFETIME(ASmashCharacter, bHitRest);
	DOREPLIFETIME(ASmashCharacter, bSpiked);

	// 전투 관련
	DOREPLIFETIME(ASmashCharacter, AbilityType);
	DOREPLIFETIME(ASmashCharacter, Attacks);
}

void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 땅에 닿았을 때 이벤트 핸들러 등록
	LandedDelegate.AddDynamic(this, &ASmashCharacter::OnLandedSmash);

	// 약간의 딜레이 후에 초기화 시작 (0.5초)
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASmashCharacter::StartUp, 0.5f, false);

	// 게임 시작 5초 후 카메라 모드 초기화 (타이밍이 중요)
	FTimerHandle CameraInitTimer;
	GetWorld()->GetTimerManager().SetTimer(CameraInitTimer, this, &ASmashCharacter::InitializeCameraMode, 5.0f, false);

	FTimerHandle UpdateTargetsTimer;
	GetWorld()->GetTimerManager().SetTimer(UpdateTargetsTimer, [this]()
	{
		if (SmashCameraComponent && SmashCameraComponent->GetCameraMode() == ECameraMode::Group)
		{
			TArray<AActor*> AllCharacters;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASmashCharacter::StaticClass(), AllCharacters);

			for (AActor* Actor : AllCharacters)
			{
				if (Actor != this && !SmashCameraComponent->GetTargetActors().Contains(Actor))
				{
					SmashCameraComponent->AddTargetActor(Actor);
				}
			}
		}
	}, 5.0f, true); // 5초마다 반복
}

void ASmashCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASmashCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 초기화 완료 확인
	if (!bInitialized)
	{
		return;
	}

	// 주요 틱 로직
	FacingCheck();

	Multicast_SmashDetection();

	UpdateLocations();

	// 컴포넌트 틱 호출
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->MainTick();
	}

	if (FXComponent)
	{
		FXComponent->FXMainLoop();
	}
}

void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 입력 컴포넌트가 EnhancedInputComponent인지 확인
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 기본 이동 입력 바인딩
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this,
		                                   &ASmashCharacter::ResetMoveInput);
		EnhancedInputComponent->BindAction(IA_UpDown, ETriggerEvent::Triggered, this, &ASmashCharacter::UpDownAxis);
		EnhancedInputComponent->BindAction(IA_UpDown, ETriggerEvent::Completed, this,
		                                   &ASmashCharacter::ResetUpDownAxis);

		// 공격 입력 바인딩
		EnhancedInputComponent->BindAction(IA_BasicAttack, ETriggerEvent::Triggered, this,
		                                   &ASmashCharacter::BasicAttackPressed);
		EnhancedInputComponent->BindAction(IA_BasicAttack, ETriggerEvent::Completed, this,
		                                   &ASmashCharacter::BasicAttackReleased);
		EnhancedInputComponent->BindAction(IA_SpecialAttack, ETriggerEvent::Triggered, this,
		                                   &ASmashCharacter::SpecialAttackPressed);
		EnhancedInputComponent->BindAction(IA_SpecialAttack, ETriggerEvent::Completed, this,
		                                   &ASmashCharacter::SpecialAttackReleased);

		// 도발 입력 바인딩
		// EnhancedInputComponent->BindAction(IA_TauntUp, ETriggerEvent::Triggered, this, &ASmashCharacter::TauntUpPressed);
		// EnhancedInputComponent->BindAction(IA_TauntRight, ETriggerEvent::Triggered, this, &ASmashCharacter::TauntRightPressed);
		// EnhancedInputComponent->BindAction(IA_TauntLeft, ETriggerEvent::Triggered, this, &ASmashCharacter::TauntLeftPressed);
		// EnhancedInputComponent->BindAction(IA_TauntDown, ETriggerEvent::Triggered, this, &ASmashCharacter::TauntDownPressed);

		EnhancedInputComponent->BindAction(IA_SpecialAttack, ETriggerEvent::Triggered, this,
		                                   &ASmashCharacter::SpecialAttackPressed);
		EnhancedInputComponent->BindAction(IA_SpecialAttack, ETriggerEvent::Completed, this,
		                                   &ASmashCharacter::SpecialAttackReleased);

		EnhancedInputComponent->BindAction(IA_Dodge, ETriggerEvent::Started, this, &ASmashCharacter::DodgePressed);

		EnhancedInputComponent->BindAction(IA_CameraToggle, ETriggerEvent::Started, this,
		                                   &ASmashCharacter::ToggleCameraMode);

		EnhancedInputComponent->BindAction(IA_Grab, ETriggerEvent::Started, this, &ASmashCharacter::GrabPressed);

		// EnhancedInputComponent->BindAction(IA_TauntUp, ETriggerEvent::Triggered, this, &ASmashCharacter::TauntUpPressed);
		// EnhancedInputComponent->BindAction(IA_TauntRight, ETriggerEvent::Triggered, this, &ASmashCharacter::TauntRightPressed);
		// EnhancedInputComponent->BindAction(IA_TauntLeft, ETriggerEvent::Triggered, this, &ASmashCharacter::TauntLeftPressed);
		// EnhancedInputComponent->BindAction(IA_TauntDown, ETriggerEvent::Triggered, this, &ASmashCharacter::TauntDownPressed);
	}
}

void ASmashCharacter::StartUp()
{
	// 위젯 설정
	SetupHUDWidget();

	// 기본 설정 초기화
	SetUp();

	// 코스튬 설정
	SetCostume();

	// Tick 활성화를 위한 타이머
	FTimerHandle TickActivationHandle;
	GetWorld()->GetTimerManager().SetTimer(TickActivationHandle, [this]()
	{
		// Tick 활성화
		SetActorTickEnabled(true);

		// LastHit 초기화 (서버에서만)
		if (HasAuthority())
		{
			LastHit = this;
		}

		// 이동 상태 초기화
		SetMovementState(FSmashPlayerMovement(true, true, true, true));

		// 능력 시스템 초기화
		InitializeAbilitySystem();

		// 초기화 완료 표시
		bInitialized = true;
	}, 0.5f, false);
}

void ASmashCharacter::SetupHUDWidget()
{
	// 위젯 컴포넌트 확인 및 설정
	if (WidgetComponent)
	{
		if (UUW_HUD_PlayerName* HUD_PlayerName = Cast<UUW_HUD_PlayerName>(WidgetComponent->GetUserWidgetObject()))
		{
			HUD_PlayerName->Player = this;
			HUD_PlayerName->PlayerNo = PlayerNo;
		}
		else
		{
			UE_LOG(LogSmashCharacter, Error, TEXT("WBP_HUD_PlayerName을 위젯컴포넌트 위젯에 설정해주세요."));
		}
	}
	else
	{
		UE_LOG(LogSmashCharacter, Error, TEXT("WidgetComponent가 설정되지 않았습니다."));
	}
}

void ASmashCharacter::InitializeAbilitySystem()
{
	// 능력 시스템 컴포넌트 널 체크
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogSmashCharacter, Error, TEXT("AbilitySystemComponent가 설정되지 않았습니다."));
		return;
	}

	// 레벨 인트로 설정
	if (IsValid(AbilitySystemComponent->LevelIntro))
	{
		AbilitySystemComponent->LevelIntro->bActive = true;
	}

	// 상태 시스템 초기화 및 널 체크
	if (SmashStateSystem)
	{
		SmashStateSystem->TryChangeState(ESmashPlayerStates::Ability);
	}
	else
	{
		UE_LOG(LogSmashCharacter, Error, TEXT("StateSystem이 설정되지 않았습니다."));
	}
}

void ASmashCharacter::SetUp()
{
	// 게임 인스턴스 설정
	SetupGameInstance();

	// 게임 상태 설정
	SetupGameState();

	// 서버에서만 수행해야 하는 작업
	if (HasAuthority())
	{
		SetupGameMode();
	}

	// 모든 클라이언트에서 수행할 작업
	SyncClientPlayerData();
}

void ASmashCharacter::SetupGameInstance()
{
	// 게임 인스턴스 가져오기
	SmashGameInstance = Cast<USmashGameInstance>(GetGameInstance());
	if (!SmashGameInstance)
	{
		UE_LOG(LogSmashCharacter, Error, TEXT("맵 프로젝트에서 USmashGameInstance를 설정해주세요!!!"));
	}
}

void ASmashCharacter::SetupGameState()
{
	// 게임 상태 가져오기
	SmashGameState = Cast<ASmashGameState>(UGameplayStatics::GetGameState(this));
	if (!SmashGameState)
	{
		UE_LOG(LogSmashCharacter, Error, TEXT("게임모드에서 USmashGameInstance를 설정해주세요!!!"));
	}
}

void ASmashCharacter::SetupGameMode()
{
	// 게임 모드 가져오기 (서버에서만)
	SmashGameMode = Cast<ASmashPlatFighterGameMode>(UGameplayStatics::GetGameMode(this));
	if (!SmashGameMode)
	{
		UE_LOG(LogSmashCharacter, Error, TEXT("게임모드에를 GM_SmashPlatFighterGameMode로 설정해주세요."));
		return;
	}

	// 캐릭터 설정
	if (SmashGameInstance && PlayerNo >= 0 && PlayerNo < SmashGameInstance->PlayerCharacters.Num())
	{
		Character = SmashGameInstance->PlayerCharacters[PlayerNo];
	}

	// 능력 부착
	AttachAbilities();

	// 카메라 업데이트
	UpdateCamera();
}

void ASmashCharacter::SyncClientPlayerData()
{
	// 클라이언트-서버 동기화 (모든 클라이언트에서 실행)
	if (SmashGameInstance && PlayerNo >= 0 && PlayerNo < SmashGameInstance->AliveArray.Num())
	{
		SmashGameInstance->AliveArray[PlayerNo] = true;

		if (PlayerNo < SmashGameInstance->PlayerTeam.Num())
		{
			Team = SmashGameInstance->PlayerTeam[PlayerNo];
		}
	}
	else
	{
		UE_LOG(LogSmashCharacter, Error, TEXT("PlayerNo(%d)가 올바른 범위가 아니거나 SmashGameInstance가 없습니다."), PlayerNo);
	}
}

void ASmashCharacter::SetCostume()
{
	// // 재질 설정 (팀별 색상)
	// if (TeamOptionMaterials.IsValidIndex(PlayerNo) && TeamOptionMaterials[PlayerNo])
	// {
	// 	UMaterialInstance* TeamMaterial = TeamOptionMaterials[PlayerNo];
	// 	if (GetMesh())
	// 	{
	// 		Material = GetMesh()->CreateDynamicMaterialInstance(0, TeamMaterial);
	// 	}
	// }
	// else
	// {
	// 	UE_LOG(LogSmashCharacter, Warning, TEXT("Team Material 인덱스에 메테리얼을 제대로 설정하세요."));
	// }
}

void ASmashCharacter::AttachAbilities()
{
	// 서버에서만 능력 부착 (권한 체크)
	if (HasAuthority() && !bAttachedAbil && AbilitySystemComponent)
	{
		AbilitySystemComponent->AttachAllAbilities();
		bAttachedAbil = true; // 복제됨
	}
}

void ASmashCharacter::UpdateCamera()
{
	// SST 플러그인의 카메라 로직 (미구현)
	UE_LOG(LogSmashCharacter, Display, TEXT("SST 플러그인의 카메라 로직을 사용할 예정입니다. 미구현"));
}

//---------------------------------------------------------------------
// 상태 및 움직임 관리
//---------------------------------------------------------------------

void ASmashCharacter::RespawnEvent_Implementation()
{
	if (LifeCount <= 0)
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		return;
	}

	LifeCount--;

	SmashStateSystem->TryChangeState(ESmashPlayerStates::Dead);

	AbilitySystemComponent->Multicast_Respawning();

	UClass* RespawnClass = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/PlatformFighterKit/Blueprints/LevelObjects/BP_RespwanLocations.BP_RespwanLocations_C"));
	UClass* RespawnFoothold = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/PlatformFighterKit/Blueprints/PlayerObjects/BP_respwan.BP_respwan_C"));
	
	if (!RespawnClass) return;

	TArray<AActor*> RespawnPoints;
	UGameplayStatics::GetAllActorsOfClass(this, RespawnClass, RespawnPoints);

	if (RespawnPoints.Num() == 0) return;

	int32 Index = FMath::RandRange(0, RespawnPoints.Num() - 1);
	AActor* RespawnPoint = RespawnPoints[Index];

	if (!RespawnPoint) return;

	if (RespawnFoothold)
	{
		FVector SpawnLocation = RespawnPoint->GetActorLocation();
		FRotator SpawnRotation = FRotator::ZeroRotator;

		AActor* SpawnFoothold = GetWorld()->SpawnActor<AActor>(RespawnFoothold, SpawnLocation, SpawnRotation);

		if (SpawnFoothold)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor successfully spawned!"));
		}
	}
	
	FVector RespawnLocation = FVector(RespawnPoint->GetActorLocation().X, RespawnPoint->GetActorLocation().Y, RespawnPoint->GetActorLocation().Z + 300.0f);
	SetActorLocation(RespawnLocation);
	SetActorRotation(RespawnPoint->GetActorRotation());

	SmashStateSystem->TryChangeState(ESmashPlayerStates::Idle);
}

void ASmashCharacter::FacingCheck()
{
	// 움직임 컴포넌트 확인
	if (!SmashCharacterMovementComponent)
	{
		return;
	}

	// 방향 전환 속성 동기화 (현재 값과 다를 때만 업데이트)
	if (SmashCharacterMovementComponent->bCanFlip != StateInfo.PlayerMovement.bCanFlipping)
	{
		SmashCharacterMovementComponent->bCanFlip = StateInfo.PlayerMovement.bCanFlipping;
	}
}

void ASmashCharacter::Multicast_SmashDetection_Implementation()
{
	// 스매시 공격 감지 로직 (모든 클라이언트에서 실행)
	if (StateInfo.PlayerMovement.bCanAttack)
	{
		// 우측 이동 감지
		if (MoveInputValue >= 0.8f)
		{
			if (DoOnceMoveDirection.Execute())
			{
				StateInfo.PlayCondition.bCanSmash = true;
				Direction = ESmashDirection::Forward;
			}
		}
		// 좌측 이동 감지
		else if (MoveInputValue <= -0.8f)
		{
			if (DoOnceMoveDirection.Execute())
			{
				StateInfo.PlayCondition.bCanSmash = true;
				Direction = ESmashDirection::Forward;
			}
		}
		// 중립 상태일 때 DoOnce 리셋
		else if (!(MoveInputValue >= 0.8f || MoveInputValue <= -0.8f))
		{
			DoOnceMoveDirection.Reset();
		}

		// 위 방향 입력 감지
		if (UpDownInputValue >= 0.8f)
		{
			if (DoOnceUpDirection.Execute())
			{
				Direction = ESmashDirection::Up;
				StateInfo.PlayCondition.bCanSmash = true;
			}
		}
		// 아래 방향 입력 감지
		else if (UpDownInputValue <= -0.8f)
		{
			if (DoOnceDownDirection.Execute())
			{
				Direction = ESmashDirection::Down;
				StateInfo.PlayCondition.bCanSmash = true;
			}
		}
		// 중립 상태일 때 DoOnce 리셋
		else if (UpDownInputValue >= 0.8f || UpDownInputValue <= -0.8f)
		{
			DoOnceUpDirection.Reset();
			DoOnceDownDirection.Reset();
		}

		// 스매시 가능 상태 리셋 타이머
		if (StateInfo.PlayCondition.bCanSmash)
		{
			FTimerHandle DelayHandle;
			GetWorld()->GetTimerManager().SetTimer(DelayHandle, [this]()
			{
				StateInfo.PlayCondition.bCanSmash = false;
			}, 0.1f, false);
		}
	}
	else
	{
		StateInfo.PlayCondition.bCanSmash = false;
	}
}

void ASmashCharacter::UpdateLocations()
{
	// 위치 정보 업데이트
	Location = GetActorLocation();
	YPos = Location.Y;
	ZPos = Location.Z;

	// 캡슐 컴포넌트가 있는지 확인
	if (GetCapsuleComponent())
	{
		FootZPos = Location.Z - GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	}
	else
	{
		FootZPos = Location.Z;
	}

	LocationFeet = FVector(Location.X, Location.Y, FootZPos);
}

void ASmashCharacter::SetMovementState(FSmashPlayerMovement SetMovement)
{
	// 서버에서만 상태 변경 (권한 체크)
	if (HasAuthority())
	{
		StateInfo.PlayerMovement = SetMovement;

		// 방향 전환 상태를 Movement 컴포넌트에 즉시 반영
		if (SmashCharacterMovementComponent)
		{
			SmashCharacterMovementComponent->bCanFlip = SetMovement.bCanFlipping;
		}
	}
}

void ASmashCharacter::SetCanFlip(bool bNewCanFlip)
{
	// 서버에서 실행되는 경우
	if (HasAuthority())
	{
		// 현재 상태 정보를 유지하며 방향 전환 속성만 업데이트
		StateInfo.PlayerMovement.bCanFlipping = bNewCanFlip;

		// 나머지 PlayerMovement 값 유지하며 업데이트
		SetMovementState(StateInfo.PlayerMovement);

		// Movement 컴포넌트에 즉시 반영
		if (SmashCharacterMovementComponent)
		{
			SmashCharacterMovementComponent->bCanFlip = bNewCanFlip;
		}
	}
	else
	{
		// 클라이언트에서는 서버에 RPC로 요청
		Server_SetCanFlip(bNewCanFlip);
	}
}

bool ASmashCharacter::CanFlip() const
{
	return StateInfo.PlayerMovement.bCanFlipping;
}

void ASmashCharacter::Server_SetCanFlip_Implementation(bool bNewCanFlip)
{
	SetCanFlip(bNewCanFlip);
}

void ASmashCharacter::SetStateInfo(const FSmashPlayerStateInfo& NewStateInfo)
{
	// 서버에서만 상태 정보 변경
	if (HasAuthority())
	{
		StateInfo = NewStateInfo;
		OnRep_StateInfo();
	}
}

void ASmashCharacter::Server_RequestSetStateInfo_Implementation(const FSmashPlayerStateInfo& NewStateInfo)
{
	// 서버에서만 실행
	if (HasAuthority())
	{
		SetStateInfo(NewStateInfo);
	}
}

void ASmashCharacter::Client_UpdateStateInfo_Implementation(const FSmashPlayerStateInfo& NewStateInfo)
{
	// 서버에서 클라이언트로 상태 정보 업데이트 (권한이 없는 클라이언트에서는 직접 수정 불가)
	if (!HasAuthority())
	{
		StateInfo = NewStateInfo;
		OnRep_StateInfo();
	}
}

void ASmashCharacter::OnRep_StateInfo()
{
	// Movement 컴포넌트에 방향 전환 속성 업데이트
	if (SmashCharacterMovementComponent)
	{
		SmashCharacterMovementComponent->bCanFlip = StateInfo.PlayerMovement.bCanFlipping;
	}
}

void ASmashCharacter::OnRep_LastHit()
{
	// LastHit이 변경되었을 때 필요한 처리
}

void ASmashCharacter::OnRep_PlayerNo()
{
	// PlayerNo가 변경되었을 때 UI 업데이트
	if (WidgetComponent)
	{
		if (UUW_HUD_PlayerName* HUD_PlayerName = Cast<UUW_HUD_PlayerName>(WidgetComponent->GetUserWidgetObject()))
		{
			HUD_PlayerName->PlayerNo = PlayerNo;
		}
	}
}

//---------------------------------------------------------------------
// 이펙트 및 액션 처리
//---------------------------------------------------------------------

void ASmashCharacter::SpawnFeetFX()
{
	// 달리기 이펙트 재생
	if (SprintFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, SprintFX, LocationFeet);
	}
}

void ASmashCharacter::TauntAction(ESmashDirection ActionDirection)
{
	// 도발 액션 처리 (특정 상태일 때만 가능)
	if (SmashStateSystem &&
		SmashStateSystem->GetCurrentState() != ESmashPlayerStates::Ability &&
		StateInfo.PlayerMovement.bCanAttack)
	{
		SmashStateSystem->TryChangeState(ESmashPlayerStates::Ability);
		AbilityType = ESmashAbilityTypes::Taunt;
		Direction = ActionDirection;
	}
}

void ASmashCharacter::DodgeDelayEvent()
{
	bDodgeDelay = true;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]()
	{
		bDodgeDelay = false;
	});

	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, TimerDelegate, DodgeDelayTimer, false);
}

void ASmashCharacter::ScreenShake(float RumbleInten, float RumbleDuration)
{
}

void ASmashCharacter::LedgeGrab()
{
	HitStates = ESmashHitState::Invincible;

	SmashCharacterMovementComponent->StopMovementImmediately();

	FVector NewLocation = FVector(LedgeLocation.X, LedgeLocation.Y, LedgeLocation.Z - 60.0f);
	GetCapsuleComponent()->SetWorldLocation(NewLocation);

	SmashCharacterMovementComponent->GravityScale = 0.0f;

	SmashStateSystem->TryChangeState(ESmashPlayerStates::Ledge);

	AbilitySystemComponent->Ledge->bActive = true;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]()
	{
		HitStates = ESmashHitState::Normal;
	});

	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, TimerDelegate, 1.0f, false);
}

void ASmashCharacter::InitializeCameraMode()
{
	if (SmashCameraComponent)
	{
		// 새로운 카메라 초기화 함수 호출 (호스트는 그룹 모드, 클라이언트는 개인 모드)
		SmashCameraComponent->InitializeCameraForPlayer(true);
		UE_LOG(LogSmashCharacter, Display, TEXT("카메라 모드가 초기화되었습니다."));
	}
}

void ASmashCharacter::ToggleCameraMode()
{
	if (SmashCameraComponent)
	{
		// 현재 모드 확인 후 토글 (로컬에서만 적용됨)
		if (SmashCameraComponent->GetCameraMode() == ECameraMode::Group)
		{
			SmashCameraComponent->SetCameraMode(ECameraMode::Default, 1.0f);
			UE_LOG(LogSmashCharacter, Display, TEXT("카메라 모드가 개인 모드로 변경되었습니다."));
		}
		else
		{
			SmashCameraComponent->SetCameraMode(ECameraMode::Group, 1.0f);
			UE_LOG(LogSmashCharacter, Display, TEXT("카메라 모드가 그룹 모드로 변경되었습니다."));
		}
	}
}

void ASmashCharacter::RequestCameraShake(float Intensity, float Duration, float Falloff)
{
	// 기존 카메라 흔들림 요청 유지
	if (SmashGameMode)
	{
		SmashGameMode->ShakeCamera(Intensity, Duration, Falloff);
	}

	// 로컬 카메라에도 직접 흔들림 적용
	if (SmashCameraComponent)
	{
		SmashCameraComponent->ShakeCamera(Intensity, Duration, Falloff);
	}
}

void ASmashCharacter::PlayReviveEffect()
{
	// 부활 이펙트 - 주변 카메라에 약간의 흔들림
	RequestCameraShake(2.0f, 0.5f, 1.0f);
}

void ASmashCharacter::Launch_Implementation(FVector LaunchVector, bool bXYOver, bool bZOver)
{
	LaunchCharacter(LaunchVector, bXYOver, bZOver);
}

void ASmashCharacter::Dizzy()
{
	// 캐릭터를 어지러움 상태로 설정
	SetMovementState(FSmashPlayerMovement(false, false, false, false));

	// 어지러움 상태 타이머 설정
	FTimerHandle DizzyHandle;
	GetWorld()->GetTimerManager().SetTimer(DizzyHandle, [this]()
	{
		bDizzied = false;

		if (SmashStateSystem)
		{
			SmashStateSystem->TryChangeState(ESmashPlayerStates::Idle);
		}
	}, 2.0f, false);
}

void ASmashCharacter::LandedAction()
{
	// 착지 이펙트 재생
	if (P_Landed)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, P_Landed, LocationFeet);
	}

	// 캡슐 높이 설정
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHeight);
	}

	// 착지 상태 설정
	bLanded = true;

	// 이동 모드 설정
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

	// 상태 시스템이 있는지 확인
	if (SmashStateSystem)
	{
		ESmashPlayerStates CurrentState = SmashStateSystem->GetCurrentState();

		// 특정 상태가 아닐 때만 상태 변경
		if (!(CurrentState == ESmashPlayerStates::Ability || CurrentState == ESmashPlayerStates::Tumble))
		{
			SetCanFlip(true);
			SmashStateSystem->TryChangeState(ESmashPlayerStates::Idle);

			// 어지러움 상태 체크
			if (bDizzied)
			{
				SmashStateSystem->TryChangeState(ESmashPlayerStates::Dizzy);
			}
		}
	}
}

void ASmashCharacter::OnLandedSmash(const FHitResult& Hit)
{
	// 착지 액션 실행
	LandedAction();

	// 착지 후 짧은 딜레이 설정
	FTimerHandle LandedHandle;
	GetWorld()->GetTimerManager().SetTimer(LandedHandle, [this]()
	{
		bLanded = false;
		LastHit = this;
	}, 0.01f, false);
}

//---------------------------------------------------------------------
// 입력 처리
//---------------------------------------------------------------------

void ASmashCharacter::Move(const struct FInputActionValue& Value)
{
	// 컨트롤러와 이동 컴포넌트 확인
	if (!Controller || !SmashCharacterMovementComponent)
	{
		return;
	}

	// 이동 입력 처리
	MoveInputValue = Value.Get<float>();

	// 상태 변경 시도 (걷기/달리기 상태로)
	ESmashPlayerStates CurrentState = SmashStateSystem->GetCurrentState();
	bool CanAirWalk = CurrentState == ESmashPlayerStates::Fall || CurrentState == ESmashPlayerStates::Jump;
	if (SmashStateSystem->TryChangeState(ESmashPlayerStates::WalkAndRun) || CanAirWalk)
	{
		// 입력 방향 설정
		SmashCharacterMovementComponent->AddInputVector(FVector::ForwardVector * MoveInputValue);
	}
}

void ASmashCharacter::UpDownAxis(const FInputActionValue& InputActionValue)
{
	// 컨트롤러와 이동 컴포넌트 확인
	if (Controller && SmashCharacterMovementComponent)
	{
		UpDownInputValue = InputActionValue.Get<float>();
	}
}

void ASmashCharacter::JumpOrDrop_Implementation()
{
	// 웅크린 상태에서는 플랫폼 아래로 떨어지기 시도
	if (bIsCrouched)
	{
		if (SSTCharacterMovementComponent)
		{
			SSTCharacterMovementComponent->WantsToPlatformDrop = true;
		}
	}
	else
	{
		// 점프 가능한 상태일 때만 점프 실행
		if (StateInfo.PlayerMovement.bCanJump)
		{
			Jump();

			if (SmashStateSystem)
			{
				SmashStateSystem->TryChangeState(ESmashPlayerStates::Jump);
			}
		}
	}
}

void ASmashCharacter::ResetMoveInput(const FInputActionValue& Value)
{
	// 이동 입력 값 리셋
	MoveInputValue = 0.0f;
}

void ASmashCharacter::ResetUpDownAxis(const FInputActionValue& InputActionValue)
{
	UpDownInputValue = 0.0f;
}

void ASmashCharacter::UpdateDirection()
{
	// 입력 값에 따라 방향 설정
	if (UpDownInputValue >= 0.5)
	{
		Direction = ESmashDirection::Up;
	}
	else if (UpDownInputValue <= -0.5)
	{
		Direction = ESmashDirection::Down;
	}
	else if (MoveInputValue >= 0.5)
	{
		// 캐릭터가 오른쪽을 보고 있는지 확인
		if (SmashCharacterMovementComponent && SmashCharacterMovementComponent->IsFacingRight())
		{
			Direction = ESmashDirection::Forward;
		}
		else
		{
			Direction = ESmashDirection::Back;
		}
	}
	else if (MoveInputValue <= -0.5)
	{
		// 캐릭터가 오른쪽을 보고 있는지 확인
		if (SmashCharacterMovementComponent && SmashCharacterMovementComponent->IsFacingRight())
		{
			Direction = ESmashDirection::Back;
		}
		else
		{
			Direction = ESmashDirection::Forward;
		}
	}
	else
	{
		Direction = ESmashDirection::None;
	}
}

void ASmashCharacter::BasicAttack()
{
	// 방패 상태가 아니고 공격 가능한 상태일 때
	if (StateInfo.PlayerMovement.bCanAttack)
	{
		// 공중 상태일 때는 공중 공격
		if (GetCharacterMovement() && GetCharacterMovement()->IsFalling())
		{
			SmashStateSystem->TryChangeState(ESmashPlayerStates::Ability);
			AbilityType = ESmashAbilityTypes::Air;
			return;
		}

		// 스매시 공격 가능 상태일 때
		if (StateInfo.PlayCondition.bCanSmash)
		{
			SmashStateSystem->TryChangeState(ESmashPlayerStates::Ability);
			AbilityType = ESmashAbilityTypes::Smash;
			StateInfo.PlayCondition.bCanSmash = false;
			return;
		}

		// 걷기/달리기 상태일 때는 기타 공격
		if (SmashStateSystem->GetCurrentState() == ESmashPlayerStates::WalkAndRun)
		{
			SmashStateSystem->TryChangeState(ESmashPlayerStates::Ability);
			AbilityType = ESmashAbilityTypes::Other;
			return;
		}

		// 기본 상태에서는 기본 공격
		SmashStateSystem->TryChangeState(ESmashPlayerStates::Ability);
		AbilityType = ESmashAbilityTypes::Basic;
	}
}

void ASmashCharacter::CrouchDrop_Implementation()
{
	// 웅크리기 가능한 상태일 때
	if (CanCrouch())
	{
		if (SmashStateSystem)
		{
			SmashStateSystem->TryChangeState(ESmashPlayerStates::Crouch);
		}
		Crouch();
	}
}

void ASmashCharacter::StopCrouchDrop_Implementation()
{
	Super::StopCrouchDrop_Implementation();

	// 웅크리기 해제 시 상태 변경
	if (SmashStateSystem)
	{
		SmashStateSystem->TryChangeState(ESmashPlayerStates::Idle);
	}
}

void ASmashCharacter::BasicAttackPressed(const FInputActionValue& InputActionValue)
{
	// 공격 버튼 상태 설정
	bAttackButton = true;

	// 공격 가능한 상태일 때만 처리
	if (SmashStateSystem && SmashStateSystem->GetCurrentState() != ESmashPlayerStates::Ability && StateInfo.
		PlayerMovement.bCanAttack)
	{
		// 방향 업데이트
		UpdateDirection();

		// 기본 공격 실행
		BasicAttack();

		// 한 번만 실행되도록 DoOnce 사용
		if (DoOnceBasicAttackPressed.Execute())
		{
			bAttackButtonReleased = false;
		}
	}
}

void ASmashCharacter::BasicAttackReleased(const FInputActionValue& InputActionValue)
{
	// 공격 버튼 릴리즈 상태 설정
	bAttackButton = false;
	bAttackButtonReleased = true;
}

void ASmashCharacter::SpecialAttackPressed(const FInputActionValue& InputActionValue)
{
	// 특수 공격 버튼 상태 설정
	bSpecialAttackButton = true;

	// 방향 업데이트
	UpdateDirection();

	// 특수 공격 상태로 변경
	if (SmashStateSystem)
	{
		SmashStateSystem->TryChangeState(ESmashPlayerStates::Ability);
	}
	AbilityType = ESmashAbilityTypes::Special;
}

void ASmashCharacter::SpecialAttackReleased(const FInputActionValue& InputActionValue)
{
	// 특수 공격 버튼 릴리즈 상태 설정
	bSpecialAttackButton = false;
	bSpecialAttackButtonReleased = true;
}

void ASmashCharacter::DodgePressed(const FInputActionValue& InputActionValue)
{
	// SetMovementState(FSmashPlayerMovement(false, false, false, false));

	if (bDodgeDelay)
		return;

	if (SmashStateSystem->GetCurrentState() == ESmashPlayerStates::Jump || SmashStateSystem->GetCurrentState() ==
		ESmashPlayerStates::Fall)
	{
		SmashStateSystem->TryChangeState(ESmashPlayerStates::Ability);
		AbilityType = ESmashAbilityTypes::Dodge;
		Direction = ESmashDirection::Up;
		return;
	}


	if (GetMoveInputValue() >= 0.3f || GetMoveInputValue() <= -0.3f)
	{
		if (SmashCharacterMovementComponent->IsFacingRight())
		{
			Direction = ESmashDirection::Forward;
		}
		else
		{
			Direction = ESmashDirection::Back;
		}
		SmashStateSystem->TryChangeState(ESmashPlayerStates::Ability);
		AbilityType = ESmashAbilityTypes::Dodge;
	}
	else if (GetUpDownInputValue() <= -0.4f)
	{
		Direction = ESmashDirection::Down;
		SmashStateSystem->TryChangeState(ESmashPlayerStates::Ability);
		AbilityType = ESmashAbilityTypes::Dodge;
	}
}

void ASmashCharacter::GrabPressed(const FInputActionValue& InputActionValue)
{
	if (SmashStateSystem->GetCurrentState() != ESmashPlayerStates::Idle) return;

	SmashStateSystem->TryChangeState(ESmashPlayerStates::Ability);
	AbilityType = ESmashAbilityTypes::Grab;
}

void ASmashCharacter::TauntUpPressed(const FInputActionValue& InputActionValue)
{
	TauntAction(ESmashDirection::Up);
}

void ASmashCharacter::TauntRightPressed(const FInputActionValue& InputActionValue)
{
	TauntAction(ESmashDirection::Forward);
}

void ASmashCharacter::TauntLeftPressed(const FInputActionValue& InputActionValue)
{
	TauntAction(ESmashDirection::Back);
}

void ASmashCharacter::TauntDownPressed(const FInputActionValue& InputActionValue)
{
	TauntAction(ESmashDirection::Down);
}

//---------------------------------------------------------------------
// 인터페이스 구현 - IInterface_SmashCombat
//---------------------------------------------------------------------

ESmashPlayerStates ASmashCharacter::GetPlayerState_Implementation()
{
	return SmashStateSystem ? SmashStateSystem->GetCurrentState() : ESmashPlayerStates::Idle;
}

ESmashAttacks ASmashCharacter::GetAttackTypes_Implementation()
{
	return Attacks;
}

ESmashAbilityTypes ASmashCharacter::GetAbilityTypes_Implementation()
{
	return AbilityType;
}

ESmashDirection ASmashCharacter::GetDirection_Implementation()
{
	return Direction;
}

int32 ASmashCharacter::GetSuper_Implementation()
{
	return SmashCharacterStatsComponent ? SmashCharacterStatsComponent->GetSuper() : 0;
}

FTransform ASmashCharacter::GetAbilitySpawnTransform_Implementation()
{
	return IInterface_SmashCombat::GetAbilitySpawnTransform_Implementation();
}

void ASmashCharacter::SetSuper_Implementation(int32 NewSuper)
{
	if (SmashCharacterStatsComponent)
	{
		SmashCharacterStatsComponent->SetSuper(NewSuper);
	}
}

void ASmashCharacter::SetAttacks_Implementation(ESmashAttacks NewAttacks)
{
	Attacks = NewAttacks;
}

void ASmashCharacter::BufferButtons_Implementation()
{
	bAttackButtonReleased = !bAttackButton;
	bSpecialAttackButtonReleased = !bSpecialAttackButton;
}

void ASmashCharacter::ClearBuffer_Implementation()
{
	BufferMove = ESmashBuffer::None;
	BufferDirection = ESmashDirection::None;
	bBufferdInput = false;
	bBufferdDirection = false;
}

bool ASmashCharacter::bHitConditions()
{
	return true;
}

void ASmashCharacter::TakeDamage(int32 DamageAmount, ESmashAttackType AttackType, bool bIsRightDirection)
{
	SmashCombatComponent->TakeDamage(DamageAmount, AttackType, bIsRightDirection);
}

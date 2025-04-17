#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "Character/SmashCharacter.h"
#include "Kismet/BlueprintPlatformLibrary.h"

#include "SmashGameInstance.generated.h"

UCLASS()
class SMASHBRAWL_API USmashGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

public:
	USmashGameInstance();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void HostSession();

	UFUNCTION(BlueprintCallable)
	void FindSession();

	UFUNCTION()
	void OnFindSessionsComplete(bool bWasSuccessful);

	UFUNCTION()
	void JoinSession();

	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void ShowWidget(TSubclassOf<UUserWidget> WidgetClass);


	bool bHasJoinedSession = false;
	UPROPERTY()
	bool bShouldShowHostSuccess = false;
	UUserWidget* ActiveWidget = nullptr;

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HostSuccessWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ClientSuccessWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SmashGameInstance")
	ESmashInGameMode SubMode = ESmashInGameMode::Stock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SmashGameInstance")
	TArray<bool> CPU;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SmashGameInstance")
	TArray<ESmashCharacter> PlayerCharacters;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SmashGameInstance")
	TArray<bool> AliveArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SmashGameInstance")
	TArray<int32> PlayerTeam;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SmashGameInstance")
	int32 StockCount = 3;
};

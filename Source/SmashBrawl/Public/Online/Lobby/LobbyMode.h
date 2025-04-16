// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyMode.generated.h"

/**
 * 
 */

class ALobbyGameState;
DECLARE_MULTICAST_DELEGATE(FOnPlayerJoin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchStart);
UCLASS()
class SMASHBRAWL_API ALobbyMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALobbyMode();
	virtual void BeginPlay() override;
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
public:
	
	virtual void StartMatch() override;
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	virtual void ServerTravelLevel();
	
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	bool bCanJoinPlayer();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void PlayerReady(APlayerController* NewPlayer);
	
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void PlayerUnReady(APlayerController* NewPlayer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	FString PlayMapPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	TObjectPtr<ALobbyGameState> LobbyGameState;

	FTimerHandle PlayTimerHandle;
	
	FOnMatchStart OnMatchStart;
	FOnPlayerJoin OnPlayerJoin;
};

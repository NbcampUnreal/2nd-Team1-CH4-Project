// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API ALobbyGameState : public AGameState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void AddJoinPlayer(const APlayerController* NewPlayer);
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void RemoveJoinPlayer(const APlayerController* NewPlayer);
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	int32 GetNumJoinPlayers() const;

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void AddReadPlayer(const APlayerController* NewPlayer);
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void RemoveReadPlayer(const APlayerController* NewPlayer);
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	int32 GetNumReadPlayers() const;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	TArray<APlayerState*> GetJoinedPlayerStates() const
	{
		return JoinPlayers;
	}
	
public:
	UPROPERTY(Replicated)
	TArray<APlayerState*> ReadyPlayers;
	UPROPERTY(Replicated)
	TArray<APlayerState*> JoinPlayers;

};

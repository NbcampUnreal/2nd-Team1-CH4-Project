// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SMASHBRAWL_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void ChangeReadyState();
	
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	bool GetIsReady() const;
	
public:
	UPROPERTY(Replicated)
	FString PlayerNickname;

	UFUNCTION(BlueprintCallable)
	FString GetPlayerNickname() const { return PlayerNickname; }
	UFUNCTION(BlueprintCallable)
	void SetPlayerNickname(const FString& Name) { PlayerNickname = Name; }

	UPROPERTY(Replicated)
	bool bReady;
	
};

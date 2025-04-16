// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class ALobbyPlayerState;
/**
 * 
 */
UCLASS()
class SMASHBRAWL_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController();
	virtual void BeginPlay() override;

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void ServerPlayerReady();
	UFUNCTION(Server, BlueprintCallable, Reliable)
	void ServerPlayerUnReady();
	UFUNCTION(Server, BlueprintCallable, Reliable)
	void ServerDebugPlayerStart();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	TSubclassOf<UUserWidget> UserWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	TObjectPtr<UUserWidget> UserWidget;

	TObjectPtr<ALobbyPlayerState> LobbyPlayerState;
};



#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SmashPlatFighterGameMode.generated.h"

class ASmashCharacter;

/**
 * 대난투 스타일 게임의 게임 모드.
 * 멀티플레이 레이드를 위한 기능을 제공합니다.
 */
UCLASS()
class SMASHBRAWL_API ASmashPlatFighterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASmashPlatFighterGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	/** 게임 종료 위젯 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> EndGameWidgetClass;
	
	virtual void Logout(AController* Exiting) override;

    /** 보스 등록 함수 */
    UFUNCTION(BlueprintCallable, Category = "Boss")
    void RegisterBoss(AActor* BossActor);

    /** 카메라를 그룹 모드로 전환 */
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SwitchToGroupMode(bool bEnableGroupMode = true);

	/** 게임 종료 */
	UFUNCTION(NetMulticast, Reliable, Category = "GameSet")
	void Multicast_EndGame();
    


private:
    /** 등록된 플레이어 목록 */
    UPROPERTY()
    TArray<ASmashCharacter*> RegisteredPlayers;

    /** 등록된 보스 액터 */
    UPROPERTY()
    AActor* BossActor;
};
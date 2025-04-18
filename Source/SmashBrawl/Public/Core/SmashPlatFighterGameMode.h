// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SmashPlatFighterGameMode.generated.h"

class ASmashCharacter;
class USmashCameraComponent;

// 보스 이벤트를 처리하기 위한 인터페이스
UINTERFACE(MinimalAPI, Blueprintable)
class UInterface_BossEvents : public UInterface
{
	GENERATED_BODY()
};

class SMASHBRAWL_API IInterface_BossEvents
{
	GENERATED_BODY()

public:
	// 보스 페이즈 변경 처리
	virtual void HandleBossPhaseChange(int32 NewPhase) = 0;
	
	// 보스 등록
	virtual void RegisterBoss(AActor* BossActor) = 0;
	
	// 보스 소개 시퀀스 재생
	virtual void PlayBossIntroSequence(AActor* BossActor, float Duration) = 0;
};

// 게임 이벤트를 처리하기 위한 인터페이스
UINTERFACE(MinimalAPI, Blueprintable)
class UInterface_GameEvents : public UInterface
{
	GENERATED_BODY()
};

class SMASHBRAWL_API IInterface_GameEvents
{
	GENERATED_BODY()

public:
	// 승리 시퀀스 재생
	virtual void PlayVictorySequence() = 0;
	
	// 패배 시퀀스 재생
	virtual void PlayDefeatSequence() = 0;
};

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
	virtual void Logout(AController* Exiting) override;

public:
	/** 보스 페이즈 변화 처리 함수 */
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void HandleBossPhaseChange(int32 NewPhase);

	/** 보스 등록 함수 */
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void RegisterBoss(AActor* BossActor);

	/** 게임 종료 처리 (승리/패배) */
	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void HandleGameOver(bool bVictory);

	/** 카메라 흔들림 효과 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void ShakeCamera(float Intensity, float Duration, float Falloff = 1.0f);

	/** 보스 소개 시퀀스 재생 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void PlayBossIntroSequence(AActor* BossActor, float Duration = 5.0f);

private:
	/** 마스터 카메라 찾기 */
	USmashCameraComponent* FindMasterCamera();
	
	/** 모든 플레이어에게 페이즈 변경 알림 */
	void NotifyAllPlayersAboutPhaseChange(int32 NewPhase);

	/** 등록된 플레이어 목록 */
	UPROPERTY()
	TArray<ASmashCharacter*> RegisteredPlayers;
    
    /** 페이즈 변경 효과음 */
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    class USoundBase* PhaseChangeSound;
};
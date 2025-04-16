#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/SmashTypes.h"
#include "SmashAbilitySystemComponent.generated.h"

class ASmashCharacter;
class ABaseAbility;

/**
 * 스매시 브라울 게임의 캐릭터 능력 시스템을 관리하는 컴포넌트
 * 모든 공격, 회피, 특수 동작 등의 능력을 관리하고 활성화합니다.
 */
UCLASS(ClassGroup=("Smash"), meta=(BlueprintSpawnableComponent))
class SMASHBRAWL_API USmashAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USmashAbilitySystemComponent();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//------------------------------------------------------------
	// 주요 시스템 기능
	//------------------------------------------------------------
public:
	/**
	 * 메인 틱 함수 - 능력 상태를 업데이트하고 관리합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="SmashAbility System|Core")
	void MainTick();

	/**
	 * 버퍼 호출 처리
	 * @param NewBuffer - 처리할 버퍼 타입
	 */
	UFUNCTION(BlueprintCallable, Category="SmashAbility System|Core")
	void BufferCall(ESmashBuffer NewBuffer);

	/**
	 * 차지되지 않은 모든 능력 종료
	 * @param Caller - 호출한 능력 (종료에서 제외됨)
	 */
	UFUNCTION(BlueprintCallable, Category="SmashAbility System|Core")
	void EndAllNonChargedAbilities(ABaseAbility* Caller);

	/**
	 * 추가 능력 활성화
	 * @param Selection - 활성화할 추가 능력 인덱스
	 */
	UFUNCTION(BlueprintCallable, Category="SmashAbility System|Core")
	void ActivateExtraAbility(int32 Selection);

	/**
	 * 플레이어가 절벽에 매달린 상태일 때 호출
	 */
	UFUNCTION(BlueprintCallable, Category="SmashAbility System|Core")
	void OnLedge();

	//------------------------------------------------------------
	// 능력 관리 함수
	//------------------------------------------------------------
public:
	/**
	 * 모든 능력 부착
	 */
	UFUNCTION(BlueprintCallable, Category="SmashAbility System|Management")
	void AttachAllAbilities();

	/**
	 * 능력 활성화
	 * @param InAbility - 활성화할 능력
	 */
	UFUNCTION(BlueprintCallable, Category="SmashAbility System|Management")
	void ActivateAbility(ABaseAbility* InAbility);

	/**
	 * 능력 부착 유틸리티 함수
	 * @param AbilityClass - 부착할 능력 클래스
	 * @return 생성된 능력 인스턴스
	 */
	UFUNCTION(BlueprintCallable, Category="SmashAbility System|Management")
	ABaseAbility* AttachAbility(const TSubclassOf<ABaseAbility>& AbilityClass);

private:
	/**
	 * 방향 기반 능력 활성화
	 * @param Direction - 현재 방향
	 * @param AttackType - 공격 타입 (설정될 값)
	 * @param Ability - 활성화할 능력
	 */
	void ActivateDirectionalAbility(ESmashDirection Direction, ESmashAttacks AttackType, ABaseAbility* Ability);

	/**
	 * 방향에 따른 능력 선택 및 활성화
	 * @param UpAbility - 위쪽 방향 능력
	 * @param DownAbility - 아래쪽 방향 능력  
	 * @param ForwardAbility - 앞쪽 방향 능력
	 * @param BackAbility - 뒤쪽 방향 능력 (null 가능)
	 * @param NeutralAbility - 중립 방향 능력 (null 가능)
	 * @param UpAttackType - 위쪽 공격 타입
	 * @param DownAttackType - 아래쪽 공격 타입
	 * @param ForwardAttackType - 앞쪽 공격 타입
	 * @param BackAttackType - 뒤쪽 공격 타입 (기본값 = ForwardAttackType)
	 * @param NeutralAttackType - 중립 공격 타입
	 */
	void ProcessDirectionalAbility(
		ABaseAbility* UpAbility,
		ABaseAbility* DownAbility,
		ABaseAbility* ForwardAbility,
		ABaseAbility* BackAbility = nullptr,
		ABaseAbility* NeutralAbility = nullptr,
		ESmashAttacks UpAttackType = ESmashAttacks::None,
		ESmashAttacks DownAttackType = ESmashAttacks::None,
		ESmashAttacks ForwardAttackType = ESmashAttacks::None,
		ESmashAttacks BackAttackType = ESmashAttacks::None,
		ESmashAttacks NeutralAttackType = ESmashAttacks::None
	);

	//------------------------------------------------------------
	// 네트워크 멀티캐스트 함수들
	//------------------------------------------------------------
public:
	/**
	 * 능력 선택에 따른 호출
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="SmashAbility System|Network")
	void Multicast_WitchAbility();

	/**
	 * 리스폰 처리
	 */
	UFUNCTION(NetMulticast, UnReliable, BlueprintCallable, Category="SmashAbility System|Network")
	void Multicast_Respawning();

	/**
	 * 기본 공격 호출
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="SmashAbility System|Network")
	void Multicast_BasicAttack();

	/**
	 * 특수 공격 호출
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="SmashAbility System|Network")
	void Multicast_SpecialAttack();

	/**
	 * 공중 공격 호출
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="SmashAbility System|Network")
	void Multicast_AirAttack();

	/**
	 * 스매시 공격 호출
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="SmashAbility System|Network")
	void Multicast_SmashAttack();

	/**
	 * 회피 호출
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="SmashAbility System|Network")
	void Multicast_Dodge();

	/**
	 * 도발 호출
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="SmashAbility System|Network")
	void Multicast_Taunts();

	/**
	 * 던지기 호출
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="SmashAbility System|Network")
	void Multicast_Throw();

	/**
	 * 엎드리기 호출
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="SmashAbility System|Network")
	void Multicast_Prone();

	//------------------------------------------------------------
	// 상태 관련 변수들
	//------------------------------------------------------------
public:
	/**
	 * 사용된 기술 카운트
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SmashAbility System|Stats")
	int32 UsedMovesCount;

	/**
	 * 마지막으로 사용된 기술 이름
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SmashAbility System|Stats")
	FString UsedMoves;

	/**
	 * 대미지 스케일 변수 - 반복적으로 같은 기술 사용 시 대미지 감소
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility System|Stats")
	TArray<float> DamageScale;

	/**
	 * 최근 사용한 5개 기술
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SmashAbility System|Stats")
	TArray<ABaseAbility*> Last5Moves;

	//------------------------------------------------------------
	// 핵심 참조 변수
	//------------------------------------------------------------
public:
	/**
	 * 부모 캐릭터 참조
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SmashAbility System|References")
	TObjectPtr<ASmashCharacter> Parent;


	//------------------------------------------------------------
	// 기본 공격 능력 클래스 및 인스턴스
	//------------------------------------------------------------
public:
	// 중립 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Basic Attacks")
	TSubclassOf<ABaseAbility> NeutralAttackClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Basic Attacks")
	TObjectPtr<ABaseAbility> NeutralAttack;

	// 상단 기울임 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Basic Attacks")
	TSubclassOf<ABaseAbility> TiltUpClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Basic Attacks")
	TObjectPtr<ABaseAbility> TiltUp;

	// 하단 기울임 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Basic Attacks")
	TSubclassOf<ABaseAbility> TiltDownClass;
	UPROPERTY(Replicated,BlueprintReadOnly, Category="SmashAbility System|Basic Attacks")
	TObjectPtr<ABaseAbility> TiltDown;

	// 전방 기울임 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Basic Attacks")
	TSubclassOf<ABaseAbility> TiltForwardClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Basic Attacks")
	TObjectPtr<ABaseAbility> TiltForward;

	// 대시 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Basic Attacks")
	TSubclassOf<ABaseAbility> DashAttackClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Basic Attacks")
	TObjectPtr<ABaseAbility> DashAttack;

	//------------------------------------------------------------
	// 특수 공격 능력 클래스 및 인스턴스
	//------------------------------------------------------------
public:
	// 중립 특수 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Special Attacks")
	TSubclassOf<ABaseAbility> SpecialNeutralClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Special Attacks")
	TObjectPtr<ABaseAbility> SpecialNeutral;

	// 상단 특수 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Special Attacks")
	TSubclassOf<ABaseAbility> SpecialUpClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Special Attacks")
	TObjectPtr<ABaseAbility> SpecialUp;

	// 하단 특수 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Special Attacks")
	TSubclassOf<ABaseAbility> SpecialDownClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Special Attacks")
	TObjectPtr<ABaseAbility> SpecialDown;

	// 전방 특수 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Special Attacks")
	TSubclassOf<ABaseAbility> SpecialForwardClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Special Attacks")
	TObjectPtr<ABaseAbility> SpecialForward;

	// 슈퍼 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Special Attacks")
	TSubclassOf<ABaseAbility> SuperAttackClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Special Attacks")
	TObjectPtr<ABaseAbility> SuperAttack;


	//------------------------------------------------------------
	// 공중 공격 능력 클래스 및 인스턴스
	//------------------------------------------------------------
public:
	// 중립 공중 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Air Attacks")
	TSubclassOf<ABaseAbility> AirNeutralClass;
	UPROPERTY(Replicated,BlueprintReadOnly, Category="SmashAbility System|Air Attacks")
	TObjectPtr<ABaseAbility> AirNeutral;

	// 상단 공중 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Air Attacks")
	TSubclassOf<ABaseAbility> AirUpClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Air Attacks")
	TObjectPtr<ABaseAbility> AirUp;

	// 하단 공중 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Air Attacks")
	TSubclassOf<ABaseAbility> AirDownClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Air Attacks")
	TObjectPtr<ABaseAbility> AirDown;

	// 전방 공중 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Air Attacks")
	TSubclassOf<ABaseAbility> AirForwardClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Air Attacks")
	TObjectPtr<ABaseAbility> AirForward;

	// 후방 공중 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Air Attacks")
	TSubclassOf<ABaseAbility> AirBackClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Air Attacks")
	TObjectPtr<ABaseAbility> AirBack;

	//------------------------------------------------------------
	// 스매시 공격 능력 클래스 및 인스턴스
	//------------------------------------------------------------
public:
	// 상단 스매시 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Smash Attacks")
	TSubclassOf<ABaseAbility> SmashUpClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Smash Attacks")
	TObjectPtr<ABaseAbility> SmashUp;

	// 하단 스매시 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SmashAbility System|Smash Attacks")
	TSubclassOf<ABaseAbility> SmashDownClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Smash Attacks")
	TObjectPtr<ABaseAbility> SmashDown;

	// 전방 스매시 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Smash Attacks")
	TSubclassOf<ABaseAbility> SmashForwardClass;
	UPROPERTY( Replicated,BlueprintReadOnly, Category="SmashAbility System|Smash Attacks")
	TObjectPtr<ABaseAbility> SmashForward;

	//------------------------------------------------------------
	// 회피 능력 클래스 및 인스턴스
	//------------------------------------------------------------
public:
	// 공중 회피
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Dodge")
	TSubclassOf<ABaseAbility> DodgeAirClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Dodge")
	TObjectPtr<ABaseAbility> DodgeAir;

	// 제자리 회피
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Dodge")
	TSubclassOf<ABaseAbility> DodgeSpotClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Dodge")
	TObjectPtr<ABaseAbility> DodgeSpot;

	// 후방 회피
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Dodge")
	TSubclassOf<ABaseAbility> DodgeBackClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Dodge")
	TObjectPtr<ABaseAbility> DodgeBack;

	// 전방 회피
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Dodge")
	TSubclassOf<ABaseAbility> DodgeForwardClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Dodge")
	TObjectPtr<ABaseAbility> DodgeForward;

	//------------------------------------------------------------
	// 잡기 및 던지기 능력 클래스 및 인스턴스
	//------------------------------------------------------------
public:
	// 기본 잡기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Grab and Throw")
	TSubclassOf<ABaseAbility> GrabClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Grab and Throw")
	TObjectPtr<ABaseAbility> Grab;

	// 잡기 중 펀치
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Grab and Throw")
	TSubclassOf<ABaseAbility> PummleClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Grab and Throw")
	TObjectPtr<ABaseAbility> Pummle;

	// 상단 던지기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Grab and Throw")
	TSubclassOf<ABaseAbility> ThrowUpClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Grab and Throw")
	TObjectPtr<ABaseAbility> ThrowUp;

	// 하단 던지기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Grab and Throw")
	TSubclassOf<ABaseAbility> ThrowDownClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Grab and Throw")
	TObjectPtr<ABaseAbility> ThrowDown;

	// 후방 던지기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Grab and Throw")
	TSubclassOf<ABaseAbility> ThrowBackwardClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Grab and Throw")
	TObjectPtr<ABaseAbility> ThrowBackward;

	// 전방 던지기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Grab and Throw")
	TSubclassOf<ABaseAbility> ThrowForwardClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Grab and Throw")
	TObjectPtr<ABaseAbility> ThrowForward;

	//------------------------------------------------------------
	// 도발 능력 클래스 및 인스턴스
	//------------------------------------------------------------
public:
	// 상단 도발
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Taunts")
	TSubclassOf<ABaseAbility> TauntUpClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Taunts")
	TObjectPtr<ABaseAbility> TauntUp;

	// 하단 도발
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Taunts")
	TSubclassOf<ABaseAbility> TauntDownClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Taunts")
	TObjectPtr<ABaseAbility> TauntDown;

	// 왼쪽 도발
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Taunts")
	TSubclassOf<ABaseAbility> TauntLeftClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Taunts")
	TObjectPtr<ABaseAbility> TauntLeft;

	// 오른쪽 도발
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Taunts")
	TSubclassOf<ABaseAbility> TauntRightClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Taunts")
	TObjectPtr<ABaseAbility> TauntRight;

	//------------------------------------------------------------
	// 기타 능력 클래스 및 인스턴스
	//------------------------------------------------------------
public:
	// 엎드린 상태에서 서기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Prone")
	TSubclassOf<ABaseAbility> ProneStandClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Prone")
	TObjectPtr<ABaseAbility> ProneStand;

	// 엎드린 상태에서 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Prone")
	TSubclassOf<ABaseAbility> ProneAttackClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Prone")
	TObjectPtr<ABaseAbility> ProneAttack;

	// 절벽 공격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Ledge")
	TSubclassOf<ABaseAbility> LedgeAttackClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Ledge")
	TObjectPtr<ABaseAbility> LedgeAttack;

	// 절벽 잡기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Ledge")
	TSubclassOf<ABaseAbility> LedgeClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Ledge")
	TObjectPtr<ABaseAbility> Ledge;

	// 레벨 인트로
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Misc")
	TSubclassOf<ABaseAbility> LevelIntroClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Misc")
	TObjectPtr<ABaseAbility> LevelIntro;

	// 리스폰 능력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Misc")
	TSubclassOf<ABaseAbility> RespawnAbilityClass;

	// 아이템 사용
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Misc")
	TSubclassOf<ABaseAbility> ItemsClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Misc")
	TObjectPtr<ABaseAbility> Items;

	//------------------------------------------------------------
	// 추가 능력 클래스 및 인스턴스
	//------------------------------------------------------------
public:
	// 추가 능력 1
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Extra Abilities")
	TSubclassOf<ABaseAbility> ExtraAbility1Class;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Extra Abilities")
	TObjectPtr<ABaseAbility> ExtraAbility1;

	// 추가 능력 2
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Extra Abilities")
	TSubclassOf<ABaseAbility> ExtraAbility2Class;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Extra Abilities")
	TObjectPtr<ABaseAbility> ExtraAbility2;

	// 추가 능력 3
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Extra Abilities")
	TSubclassOf<ABaseAbility> ExtraAbility3Class;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Extra Abilities")
	TObjectPtr<ABaseAbility> ExtraAbility3;

	// 추가 능력 4
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SmashAbility System|Extra Abilities")
	TSubclassOf<ABaseAbility> ExtraAbility4Class;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="SmashAbility System|Extra Abilities")
	TObjectPtr<ABaseAbility> ExtraAbility4;
};

#include "SmashBrawl/Public/AbilitySystem/SmashAbilitySystemComponent.h"

#include "AbilitySystem/BaseAbility.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "SmashBrawl/Public/Interfaces/Interface_SmashCombat.h"

USmashAbilitySystemComponent::USmashAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	UsedMovesCount = 0;
	DamageScale.Add(1.0f);
	DamageScale.Add(0.9f);
	DamageScale.Add(0.8f);
	DamageScale.Add(0.7f);
	DamageScale.Add(0.6f);
}

void USmashAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// 부모 캐릭터 참조 설정
	Parent = Cast<ACharacter>(GetOwner());
	if (!Parent)
	{
		UE_LOG(LogTemp, Error, TEXT("부모는 캐릭터가 아닙니다"));
		return;
	}
}

void USmashAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USmashAbilitySystemComponent::MainTick()
{
	// 능력 상태이지만 특정 공격 타입이 없는 경우, 적절한 능력 활성화
	if (IInterface_SmashCombat::Execute_GetPlayerState(Parent) == EPlayerStates::Ability &&	IInterface_SmashCombat::Execute_GetAttackTypes(Parent) == EAttacks::None)
	{
		Multicast_WitchAbility();
	}
}

void USmashAbilitySystemComponent::BufferCall(ESmashBuffer NewBuffer)
{
	IInterface_SmashCombat::Execute_BufferButtons(Parent);

	// 버퍼 타입에 따라 적절한 함수 호출
	switch (NewBuffer)
	{
	case ESmashBuffer::Attack:
		Multicast_BasicAttack();
		break;
	case ESmashBuffer::Special:
		Multicast_SpecialAttack();
		break;
	default:
		break;
	}
}

void USmashAbilitySystemComponent::EndAllNonChargedAbilities(ABaseAbility* Caller)
{
	// 모든 BaseAbility 액터 검색
	TArray<AActor*> Abilities;
	UGameplayStatics::GetAllActorsOfClass(this, ABaseAbility::StaticClass(), Abilities);

	// 차지되지 않은 모든 능력 종료 (호출자 제외)
	for (AActor* AbilityActor : Abilities)
	{
		ABaseAbility* Ability = Cast<ABaseAbility>(AbilityActor);
		if (!Ability)
			continue;

		if (Ability->Parent == Parent && Ability->ChargeLevel == 0.0f && Caller != Ability)
		{
			Ability->EndAbility();
		}
	}
}

void USmashAbilitySystemComponent::ActivateExtraAbility(int32 Selection)
{
	switch (Selection)
	{
	case 0:
		ActivateAbility(ExtraAbility1);
		break;
	case 1:
		ActivateAbility(ExtraAbility2);
		break;
	case 2:
		ActivateAbility(ExtraAbility3);
		break;
	case 3:
		ActivateAbility(ExtraAbility4);
		break;
	default:
		break;
	}
}

void USmashAbilitySystemComponent::OnLedge()
{
	// 미구현 (향후 구현 예정)
}

void USmashAbilitySystemComponent::AttachAllAbilities()
{
	// 기본 공격 능력 부착
	NeutralAttack = AttachAbility(NeutralAttackClass);
	TiltUp = AttachAbility(TiltUpClass);
	TiltDown = AttachAbility(TiltDownClass);
	TiltForward = AttachAbility(TiltForwardClass);
	DashAttack = AttachAbility(DashAttackClass);

	// 특수 공격 능력 부착
	SpecialNeutral = AttachAbility(SpecialNeutralClass);
	SpecialUp = AttachAbility(SpecialUpClass);
	SpecialDown = AttachAbility(SpecialDownClass);
	SpecialForward = AttachAbility(SpecialForwardClass);
	SuperAttack = AttachAbility(SuperAttackClass);

	// 공중 공격 능력 부착
	AirNeutral = AttachAbility(AirNeutralClass);
	AirForward = AttachAbility(AirForwardClass);
	AirBack = AttachAbility(AirBackClass);
	AirUp = AttachAbility(AirUpClass);
	AirDown = AttachAbility(AirDownClass);

	// 스매시 공격 능력 부착
	SmashUp = AttachAbility(SmashUpClass);
	SmashForward = AttachAbility(SmashForwardClass);
	SmashDown = AttachAbility(SmashDownClass);

	// 잡기 및 던지기 능력 부착
	Grab = AttachAbility(GrabClass);
	Pummle = AttachAbility(PummleClass);
	ThrowUp = AttachAbility(ThrowUpClass);
	ThrowDown = AttachAbility(ThrowDownClass);
	ThrowBackward = AttachAbility(ThrowBackwardClass);
	ThrowForward = AttachAbility(ThrowForwardClass);

	// 회피 능력 부착
	DodgeAir = AttachAbility(DodgeAirClass);
	DodgeBack = AttachAbility(DodgeBackClass);
	DodgeForward = AttachAbility(DodgeForwardClass);
	DodgeSpot = AttachAbility(DodgeSpotClass);

	// 도발 능력 부착
	TauntUp = AttachAbility(TauntUpClass);
	TauntDown = AttachAbility(TauntDownClass);
	TauntLeft = AttachAbility(TauntLeftClass);
	TauntRight = AttachAbility(TauntRightClass);

	// 기타 및 특수 능력 부착
	LevelIntro = AttachAbility(LevelIntroClass);
	LedgeAttack = AttachAbility(LedgeAttackClass);
	ProneAttack = AttachAbility(ProneAttackClass);
	ProneStand = AttachAbility(ProneStandClass);
	RespawnAbility = AttachAbility(RespawnAbilityClass);
	Ledge = AttachAbility(LedgeClass);
	Items = AttachAbility(ItemsClass);

	// 추가 능력 부착
	ExtraAbility1 = AttachAbility(ExtraAbility1Class);
	ExtraAbility2 = AttachAbility(ExtraAbility2Class);
	ExtraAbility3 = AttachAbility(ExtraAbility3Class);
	ExtraAbility4 = AttachAbility(ExtraAbility4Class);
}

void USmashAbilitySystemComponent::ActivateAbility(ABaseAbility* InAbility)
{
	// 유효성 검사
	if (!IsValid(InAbility))
	{
		UE_LOG(LogTemp, Warning, TEXT("유효하지 않은 능력으로 호출되는 활성화 가능성."));
		return;
	}

	// 능력 활성화
	InAbility->bActive = true;

	// 버퍼 초기화 및 사용 카운트 리셋
	IInterface_SmashCombat::Execute_ClearBuffer(Parent);
	UsedMovesCount = 0;

	// 최근 사용 능력 히스토리 업데이트
	Last5Moves.Insert(InAbility, 0);
	if (Last5Moves.Num() > 5)
		Last5Moves.RemoveAt(Last5Moves.Num() - 1);

	// 사용 히스토리 문자열 구성 및 중복 사용 카운트 계산
	UsedMoves = FString();
	for (int i = Last5Moves.Num() - 1; i >= 0; i--)
	{
		FString MoveName = UKismetSystemLibrary::GetDisplayName(Last5Moves[i]);
		MoveName.Append(",");
		MoveName.Append(UsedMoves);
		UsedMoves = MoveName;

		if (Last5Moves[i] == InAbility)
		{
			UsedMovesCount++;
		}
	}
}

ABaseAbility* USmashAbilitySystemComponent::AttachAbility(const TSubclassOf<ABaseAbility>& AbilityClass)
{
	// 유효성 검사
	if (!IsValid(AbilityClass))
		return nullptr;

	// 능력 액터 생성 및 설정
	FTransform SpawnTransform = IInterface_SmashCombat::Execute_GetAbilitySpawnTransform(Parent);
	ABaseAbility* NewAbility = GetWorld()->SpawnActorDeferred<ABaseAbility>(
		AbilityClass,
		SpawnTransform,
		Parent,
		Parent,
		ESpawnActorCollisionHandlingMethod::Undefined,
		ESpawnActorScaleMethod::SelectDefaultAtRuntime
	);

	if (NewAbility)
	{
		NewAbility->Parent = Parent;
		UGameplayStatics::FinishSpawningActor(NewAbility, SpawnTransform, ESpawnActorScaleMethod::SelectDefaultAtRuntime);
		NewAbility->AttachToActor(Parent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	return NewAbility;
}

// 새 헬퍼 메서드: 방향 기반 능력 활성화
void USmashAbilitySystemComponent::ActivateDirectionalAbility(ESmashDirection Direction, EAttacks AttackType, ABaseAbility* Ability)
{
	if (IsValid(Ability))
	{
		IInterface_SmashCombat::Execute_SetAttacks(Parent, AttackType);
		ActivateAbility(Ability);
	}
}

// 새 헬퍼 메서드: 방향에 따른 능력 선택 및 활성화
void USmashAbilitySystemComponent::ProcessDirectionalAbility(ABaseAbility* UpAbility, ABaseAbility* DownAbility,
                                                             ABaseAbility* ForwardAbility, ABaseAbility* BackAbility, ABaseAbility* NeutralAbility,
                                                             EAttacks UpAttackType,
                                                             EAttacks DownAttackType,
                                                             EAttacks ForwardAttackType,
                                                             EAttacks BackAttackType,
                                                             EAttacks NeutralAttackType)
{
	// 현재 방향 가져오기
	const ESmashDirection CurrentDirection = IInterface_SmashCombat::Execute_GetDirection(Parent);

	// 방향에 따라 적절한 능력 활성화
	switch (CurrentDirection)
	{
	case ESmashDirection::Up:
		if (UpAbility)
			ActivateDirectionalAbility(CurrentDirection, UpAttackType, UpAbility);
		break;

	case ESmashDirection::Down:
		if (DownAbility)
			ActivateDirectionalAbility(CurrentDirection, DownAttackType, DownAbility);
		break;

	case ESmashDirection::Forward:
		if (ForwardAbility)
			ActivateDirectionalAbility(CurrentDirection, ForwardAttackType, ForwardAbility);
		break;

	case ESmashDirection::Back:
		if (BackAbility)
			ActivateDirectionalAbility(CurrentDirection, BackAttackType, BackAbility);
		else if (ForwardAbility) // 후방 능력이 없으면 전방 능력 사용
			ActivateDirectionalAbility(CurrentDirection, ForwardAttackType, ForwardAbility);
		break;

	case ESmashDirection::None:
		if (NeutralAbility)
			ActivateDirectionalAbility(CurrentDirection, NeutralAttackType, NeutralAbility);
		break;

	default: break;
	}
}

void USmashAbilitySystemComponent::Multicast_BasicAttack_Implementation()
{
	ProcessDirectionalAbility(
		TiltUp, // 위쪽 능력
		TiltDown, // 아래쪽 능력
		TiltForward, // 앞쪽 능력
		nullptr, // 뒤쪽 능력 (없음, 앞쪽 사용)
		NeutralAttack, // 중립 능력
		EAttacks::TiltUp, // 위쪽 공격 타입
		EAttacks::TiltDown, // 아래쪽 공격 타입
		EAttacks::TiltForward, // 앞쪽 공격 타입
		EAttacks::TiltForward, // 뒤쪽 공격 타입
		EAttacks::Neutral // 중립 공격 타입
	);
}

void USmashAbilitySystemComponent::Multicast_SpecialAttack_Implementation()
{
	// 현재 방향 가져오기
	const ESmashDirection CurrentDirection = IInterface_SmashCombat::Execute_GetDirection(Parent);

	// 중립 방향이고 슈퍼 게이지가 꽉 찬 경우 슈퍼 공격 활성화
	if (CurrentDirection == ESmashDirection::None && IInterface_SmashCombat::Execute_GetSuper(Parent) == 100)
	{
		IInterface_SmashCombat::Execute_SetSuper(Parent, 0);
		IInterface_SmashCombat::Execute_SetAttacks(Parent, EAttacks::Super);
		ActivateAbility(SuperAttack);
		return;
	}

	// 일반 특수 공격 처리
	ProcessDirectionalAbility(
		SpecialUp, // 위쪽 능력
		SpecialDown, // 아래쪽 능력
		SpecialForward, // 앞쪽 능력
		nullptr, // 뒤쪽 능력 (없음, 앞쪽 사용)
		SpecialNeutral, // 중립 능력
		EAttacks::SpecialUp, // 위쪽 공격 타입
		EAttacks::SpecialDown, // 아래쪽 공격 타입
		EAttacks::SpecialForward, // 앞쪽 공격 타입
		EAttacks::SpecialForward, // 뒤쪽 공격 타입
		EAttacks::SpecialNeutral // 중립 공격 타입
	);
}

void USmashAbilitySystemComponent::Multicast_AirAttack_Implementation()
{
	ProcessDirectionalAbility(
		AirUp, // 위쪽 능력
		AirDown, // 아래쪽 능력
		AirForward, // 앞쪽 능력
		AirBack, // 뒤쪽 능력
		AirNeutral, // 중립 능력
		EAttacks::AirUp, // 위쪽 공격 타입
		EAttacks::AirDown, // 아래쪽 공격 타입
		EAttacks::AirForward, // 앞쪽 공격 타입
		EAttacks::AirBack, // 뒤쪽 공격 타입
		EAttacks::AirUp // 중립 공격 타입 (AirUp 사용)
	);
}

void USmashAbilitySystemComponent::Multicast_SmashAttack_Implementation()
{
	ProcessDirectionalAbility(
		SmashUp, // 위쪽 능력
		SmashDown, // 아래쪽 능력
		SmashForward, // 앞쪽 능력
		nullptr, // 뒤쪽 능력 (없음)
		nullptr, // 중립 능력 (없음)
		EAttacks::SmashUp, // 위쪽 공격 타입
		EAttacks::SmashDown, // 아래쪽 공격 타입
		EAttacks::SmashForward // 앞쪽 공격 타입
	);
}

void USmashAbilitySystemComponent::Multicast_Dodge_Implementation()
{
	ProcessDirectionalAbility(
		DodgeAir, // 위쪽 능력
		DodgeSpot, // 아래쪽 능력
		DodgeForward, // 앞쪽 능력
		DodgeBack, // 뒤쪽 능력
		nullptr, // 중립 능력 (없음)
		EAttacks::DodgeAir, // 위쪽 공격 타입
		EAttacks::DodgeSpot, // 아래쪽 공격 타입
		EAttacks::DodgeForward, // 앞쪽 공격 타입
		EAttacks::DodgeBack // 뒤쪽 공격 타입
	);
}

void USmashAbilitySystemComponent::Multicast_Taunts_Implementation()
{
	// 도발은 공격 타입 설정이 필요 없으므로 간소화된 버전 사용
	const ESmashDirection CurrentDirection = IInterface_SmashCombat::Execute_GetDirection(Parent);
	switch (CurrentDirection)
	{
	case ESmashDirection::Up:
		ActivateAbility(TauntUp);
		break;
	case ESmashDirection::Down:
		ActivateAbility(TauntDown);
		break;
	case ESmashDirection::Forward:
		ActivateAbility(TauntRight);
		break;
	case ESmashDirection::Back:
		ActivateAbility(TauntLeft);
		break;
	default: break;
	}
}

void USmashAbilitySystemComponent::Multicast_Throw_Implementation()
{
	const ESmashDirection CurrentDirection = IInterface_SmashCombat::Execute_GetDirection(Parent);

	switch (CurrentDirection)
	{
	case ESmashDirection::Up:
		ActivateAbility(ThrowUp);
		break;
	case ESmashDirection::Down:
		ActivateAbility(ThrowDown);
		break;
	case ESmashDirection::Forward:
		ActivateAbility(ThrowForward);
		break;
	case ESmashDirection::Back:
		ActivateAbility(ThrowBackward);
		break;
	default: break;
	}
}

void USmashAbilitySystemComponent::Multicast_Prone_Implementation()
{
	const ESmashDirection CurrentDirection = IInterface_SmashCombat::Execute_GetDirection(Parent);

	switch (CurrentDirection)
	{
	case ESmashDirection::Up:
		ActivateAbility(ProneStand);
		break;
	case ESmashDirection::Down:
		ActivateAbility(ProneAttack);
		break;
	default: break;
	}
}

void USmashAbilitySystemComponent::Multicast_Respawning_Implementation()
{
	if (IInterface_SmashCombat::Execute_GetPlayerState(Parent) == EPlayerStates::Dead)
		RespawnAbility->bActive = true;
}

void USmashAbilitySystemComponent::Multicast_WitchAbility_Implementation()
{
	const EAbilityTypes CurrentAbilityTypes = IInterface_SmashCombat::Execute_GetAbilityTypes(Parent);

	// 현재 능력 타입에 따라 적절한 함수 호출
	switch (CurrentAbilityTypes)
	{
	case EAbilityTypes::Basic:
		Multicast_BasicAttack();
		break;
	case EAbilityTypes::Special:
		Multicast_SpecialAttack();
		break;
	case EAbilityTypes::Air:
		Multicast_AirAttack();
		break;
	case EAbilityTypes::Smash:
		Multicast_SmashAttack();
		break;
	case EAbilityTypes::Dodge:
		Multicast_Dodge();
		break;
	case EAbilityTypes::Taunt:
		Multicast_Taunts();
		break;
	case EAbilityTypes::Throw:
		Multicast_Throw();
		break;
	case EAbilityTypes::Prone:
		Multicast_Prone();
		break;
	case EAbilityTypes::Grab:
		ActivateAbility(Grab);
		break;
	case EAbilityTypes::Other:
		ActivateAbility(DashAttack);
		break;

	default: break;
	}
}

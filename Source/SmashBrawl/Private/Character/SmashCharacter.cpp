#include "SmashBrawl/Public/Character//SmashCharacter.h"


#include "EnhancedInputComponent.h"
#include "AbilitySystem/BaseAbility.h"
#include "AbilitySystem/SmashAbilitySystemComponent.h"
#include "Actors/Shield.h"
#include "Character/FXComponent.h"
#include "Character/StateSystem.h"
#include "Character/Components/SmashCharacterMovementComponent.h"
#include "Character/Components/SmashCharacterStats.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/SmashGameInstance.h"
#include "Core/SmashGameState.h"
#include "Core/SmashPlatFighterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/HUD/InGame/UW_HUD_PlayerName.h"

class ABrawlPlayerController;

DEFINE_LOG_CATEGORY(LogSmashCharacter);


ASmashCharacter::ASmashCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<USmashCharacterMovementComponent>(ASSTCharacter::CharacterMovementComponentName))
{
	// Enable ticking but initially set to false during StartUp
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.SetTickFunctionEnable(false);


	// 컴포넌트 생성
	SmashCharacterMovementComponent = Cast<USmashCharacterMovementComponent>(GetCharacterMovement());
	AbilitySystemComponent = CreateDefaultSubobject<USmashAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	StateSystem = CreateDefaultSubobject<UStateSystem>(TEXT("CharacterState"));
	SmashCharacterStatsComponent = CreateDefaultSubobject<USmashCharacterStats>(TEXT("SmashCharacterStatsComponent"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	FXComponent = CreateDefaultSubobject<UFXComponent>(TEXT("FXComponent"));

	// 초기화 상태
	bInitialized = false;
}

void ASmashCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASmashCharacter, LastHit);
	DOREPLIFETIME(ASmashCharacter, StateInfo);
	DOREPLIFETIME(ASmashCharacter, PlayerNo);
	DOREPLIFETIME(ASmashCharacter, bAttachedAbil);
	DOREPLIFETIME(ASmashCharacter, Team);
	DOREPLIFETIME(ASmashCharacter, MoveInputValue);
	DOREPLIFETIME(ASmashCharacter, Direction);
	DOREPLIFETIME(ASmashCharacter, Location);
	DOREPLIFETIME(ASmashCharacter, YPos);
	DOREPLIFETIME(ASmashCharacter, ZPos);
	DOREPLIFETIME(ASmashCharacter, FootZPos);
	DOREPLIFETIME(ASmashCharacter, LocationFeet);
	DOREPLIFETIME(ASmashCharacter, bLedgeCooldown);
	DOREPLIFETIME(ASmashCharacter, AbilityType);
	DOREPLIFETIME(ASmashCharacter, bLanded);
	DOREPLIFETIME(ASmashCharacter, bPushed);
}

void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();

	LandedDelegate.AddDynamic(this, &ASmashCharacter::OnLandedSmash);

	// 초기화 지연 시간 단축 (이전 0.25초)
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASmashCharacter::StartUp, 0.1f, false);
}

void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 추가 입력 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 이동 액션 Completed 이벤트에 바인딩 추가
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASmashCharacter::ResetMoveInput);

		EnhancedInputComponent->BindAction(IA_UpDown, ETriggerEvent::Triggered, this, &ASmashCharacter::UpDownAxis);


		EnhancedInputComponent->BindAction(IA_BasicAttack, ETriggerEvent::Triggered, this, &ASmashCharacter::BasicAttackPressed);
		EnhancedInputComponent->BindAction(IA_BasicAttack, ETriggerEvent::Completed, this, &ASmashCharacter::BasicAttackReleased);

		EnhancedInputComponent->BindAction(IA_SpecialAttack, ETriggerEvent::Triggered, this, &ASmashCharacter::SpecialAttackPressed);
		EnhancedInputComponent->BindAction(IA_SpecialAttack, ETriggerEvent::Completed, this, &ASmashCharacter::SpecialAttackReleased);

		EnhancedInputComponent->BindAction(IA_TauntUp, ETriggerEvent::Triggered, this, &ASmashCharacter::TauntUpPressed);
		EnhancedInputComponent->BindAction(IA_TauntRight, ETriggerEvent::Triggered, this, &ASmashCharacter::TauntRightPressed);
		EnhancedInputComponent->BindAction(IA_TauntLeft, ETriggerEvent::Triggered, this, &ASmashCharacter::TauntLeftPressed);
		EnhancedInputComponent->BindAction(IA_TauntDown, ETriggerEvent::Triggered, this, &ASmashCharacter::TauntDownPressed);
	}
}

void ASmashCharacter::StartUp()
{
	// 위젯 설정 및 널 체크
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

	// 기본 설정
	SetUp();

	// 코스튬 설정 및 널 체크
	SetCostume();

	// Tick 활성화를 위한 타이머 시간 단축 (이전 1.0초)
	FTimerHandle TickActivationHandle;
	GetWorld()->GetTimerManager().SetTimer(TickActivationHandle, [this]()
	{
		SetActorTickEnabled(true);

		// LastHit 초기화
		if (GetLocalRole() == ROLE_Authority)
		{
			LastHit = this;
		}

		// 방패 부착 및 널 체크
		AttachShield();

		// 이동 상태 초기화
		SetMovementState(FSmashPlayerMovement(false, false, false, false));

		// 능력 시스템 초기화 및 널 체크
		if (AbilitySystemComponent)
		{
			if (IsValid(AbilitySystemComponent->LevelIntro))
			{
				AbilitySystemComponent->LevelIntro->bActive = true;

				// AI 로직 (미구현)
				UE_LOG(LogSmashCharacter, Warning, TEXT("AI코드 로직 미구현입니다.."));
			}
		}
		else
		{
			UE_LOG(LogSmashCharacter, Error, TEXT("AbilitySystemComponent가 설정되지 않았습니다."));
		}

		// 상태 시스템 초기화 및 널 체크
		if (StateSystem)
		{
			StateSystem->ChangeState(ESmashPlayerStates::Ability);
		}
		else
		{
			UE_LOG(LogSmashCharacter, Error, TEXT("StateSystem이 설정되지 않았습니다."));
		}

		// 초기화 완료 표시
		bInitialized = true;
	}, 0.5f, false); // 1.0초에서 0.5초로 단축
}

void ASmashCharacter::SetUp()
{
	// 게임 인스턴스 설정 및 널 체크
	SmashGameInstance = Cast<USmashGameInstance>(GetGameInstance());
	if (!SmashGameInstance)
	{
		UE_LOG(LogSmashCharacter, Error, TEXT("맵 프로젝트에서 USmashGameInstance를 설정해주세요!!!"));
		return;
	}

	// 게임 상태 설정 및 널 체크
	SmashGameState = Cast<ASmashGameState>(UGameplayStatics::GetGameState(this));
	if (!SmashGameState)
	{
		UE_LOG(LogSmashCharacter, Error, TEXT("게임모드에서 USmashGameInstance를 설정해주세요!!!"));
		return;
	}

	// 서버에서만 수행해야 하는 작업
	if (HasAuthority())
	{
		SmashGameMode = Cast<ASmashPlatFighterGameMode>(UGameplayStatics::GetGameMode(this));
		if (!SmashGameMode)
		{
			UE_LOG(LogSmashCharacter, Error, TEXT("게임모드에를 GM_SmashPlatFighterGameMode로 설정해주세요."));
			return;
		}

		// 캐릭터 설정
		Character = SmashGameInstance->PlayerCharacters[PlayerNo];

		// 능력 부착
		AttachAbilities();

		// 카메라 업데이트
		UpdateCamera();
	}

	// 모든 클라이언트에서 수행할 작업 (클라이언트-서버 동기화 문제)
	// HasAuthority() 체크가 없는 작업은 모든 클라이언트에서 실행됨
	if (SmashGameInstance && PlayerNo >= 0 && PlayerNo < SmashGameInstance->AliveArray.Num())
	{
		SmashGameInstance->AliveArray[PlayerNo] = true;
		Team = SmashGameInstance->PlayerTeam[PlayerNo];
	}
	else
	{
		UE_LOG(LogSmashCharacter, Error, TEXT("PlayerNo(%d)가 올바른 범위가 아니거나 SmashGameInstance가 없습니다."), PlayerNo);
	}
}

void ASmashCharacter::SetCostume()
{
	// 재질 설정 및 널 체크
	if (TeamOptionMaterials.IsValidIndex(PlayerNo) && TeamOptionMaterials[PlayerNo])
	{
		UMaterialInstance* TeamMaterial = TeamOptionMaterials[PlayerNo];
		Material = GetMesh() ? GetMesh()->CreateDynamicMaterialInstance(0, TeamMaterial) : nullptr;
	}
	else
	{
		UE_LOG(LogSmashCharacter, Warning, TEXT("Team Material 인덱스에 메테리얼을 제대로 설정하세요."));
	}
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

void ASmashCharacter::AttachShield()
{
	// 방패 설정 및 널 체크
	if (!ShieldClass)
	{
		UE_LOG(LogSmashCharacter, Display, TEXT("Config|ShieldClass를 설정해주세요"));
		return;
	}

	if (!GetMesh())
	{
		UE_LOG(LogSmashCharacter, Error, TEXT("Mesh 컴포넌트가 없어 Shield를 부착할 수 없습니다."));
		return;
	}

	// Spawn 전 유효성 체크
	if (HasAuthority() && ShieldClass && GetWorld())
	{
		Shield = GetWorld()->SpawnActorDeferred<AShield>(ShieldClass, GetMesh()->GetComponentToWorld(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		if (Shield)
		{
			Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), ShieldSocket);
			Shield->FinishSpawning(GetMesh()->GetComponentToWorld());
		}
		else
		{
			UE_LOG(LogSmashCharacter, Error, TEXT("Shield 생성에 실패했습니다."));
		}
	}
}

void ASmashCharacter::UpdateCamera()
{
	// SST 플러그인의 카메라 로직 (미구현)
	UE_LOG(LogSmashCharacter, Display, TEXT("SST 플러그인의 카메라 로직을 사용할 생각입니다. 미 구현"));
}

void ASmashCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 초기화 완료 체크
	if (!bInitialized)
	{
		return;
	}

	FacingCheck();

	Multicast_SmashDetection();
	UpdateLocations();
	UpdateFlashing();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->MainTick();
	}

	if (FXComponent)
	{
		FXComponent->FXMainLoop();
	}
}

void ASmashCharacter::FacingCheck()
{
	if (!SmashCharacterMovementComponent)
	{
		return;
	}

	// 현재 값과 다를 때만 업데이트 (성능 최적화)
	if (SmashCharacterMovementComponent->bCanFlip != StateInfo.PlayerMovement.bCanFlipping)
	{
		SmashCharacterMovementComponent->bCanFlip = StateInfo.PlayerMovement.bCanFlipping;
	}

	// 여기에 추가적인 방향 확인 로직 구현
	// 예: 대상 방향으로 캐릭터 모델 회전, 애니메이션 처리 등
}

void ASmashCharacter::Multicast_SmashDetection_Implementation()
{
	if (StateInfo.PlayerMovement.bCanAttack)
	{
		if (MoveInputValue >= 0.8f)
		{
			if (DoOnceMoveDirection.Execute())
			{
				StateInfo.PlayCondition.bCanSmash = true;
				Direction = ESmashDirection::Forward;
			}
		}
		else if (MoveInputValue <= -0.8f)
		{
			if (DoOnceMoveDirection.Execute())
			{
				StateInfo.PlayCondition.bCanSmash = true;
				Direction = ESmashDirection::Forward;
			}
		}
		else if (!(MoveInputValue >= 0.8f || MoveInputValue <= -0.8f))
		{
			DoOnceMoveDirection.Reset();
		}


		if (UpDownInputValue >= 0.8f)
		{
			if (DoOnceUpDirection.Execute())
			{
				Direction = ESmashDirection::Up;
				StateInfo.PlayCondition.bCanSmash = true;
			}
		}
		else if (UpDownInputValue <= -0.8f)
		{
			if (DoOnceDownDirection.Execute())
			{
				Direction = ESmashDirection::Down;
				StateInfo.PlayCondition.bCanSmash = true;
			}
		}
		else if (UpDownInputValue >= 0.8f || UpDownInputValue <= -0.8f)
		{
			DoOnceUpDirection.Reset();
			DoOnceDownDirection.Reset();
		}

		if (StateInfo.PlayCondition.bCanSmash)
		{
			// 0.1초 딜레이 추가
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
	Location = GetActorLocation();
	YPos = Location.Y;
	ZPos = Location.Z;
	FootZPos = Location.Z - GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	LocationFeet = FVector(Location.X, Location.Y, FootZPos);
}

void ASmashCharacter::UpdateFlashing()
{
	if (!IsValid(Material))
		return;

	if (bFlashing)
	{
		Material->SetScalarParameterValue(FName("Flash"), 0.0f);
	}
	else
	{
		Material->SetScalarParameterValue(FName("Flash"), 1.0f);
	}

	Material->SetScalarParameterValue(FName("Invon"), InvonFlash);
	Material->SetScalarParameterValue(FName("Smash"), SmashFlash);
	Material->SetScalarParameterValue(FName("Hit"), HitFlash);
	Material->SetScalarParameterValue(FName("FreeFall"), FreeFallFlash);

	if (StateSystem->GetCurrentState() == ESmashPlayerStates::FreeFall || StateSystem->GetCurrentState() == ESmashPlayerStates::Hit || bIsSmashFlash ||
		(HitStates == ESmashHitState::Intangible || HitStates == ESmashHitState::Invincible))
	{
		bFlashing = true;
	}
	else
	{
		bFlashing = false;
	}

	if (StateSystem->GetCurrentState() == ESmashPlayerStates::FreeFall)
	{
		Material->SetVectorParameterValue(FName("FlashColor"), FLinearColor(0.008f, 0.02f, 0.02f, 1.0f));
	}
	if (StateSystem->GetCurrentState() == ESmashPlayerStates::Hit)
	{
		Material->SetVectorParameterValue(FName("FlashColor"), FLinearColor(0.6f, 0.0f, 0.03f, 1.0f));
	}
	if (HitStates == ESmashHitState::Intangible || HitStates == ESmashHitState::Invincible)
	{
		Material->SetVectorParameterValue(FName("FlashColor"), FLinearColor(0.7f, 0.93f, 0.77f, 1.0f));
	}

	if (bIsSmashFlash)
	{
		Material->SetVectorParameterValue(FName("FlashColor"), FLinearColor(0.81f, 0.48f, 0.01f, 1.0f));
	}
}

void ASmashCharacter::SpawnFeetFX()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, SprintFX, LocationFeet);
}

void ASmashCharacter::TauntAction(ESmashDirection ActionDirection)
{
	if (StateSystem->GetCurrentState() != ESmashPlayerStates::Ability && StateInfo.PlayerMovement.bCanAttack)
	{
		StateSystem->ChangeState(ESmashPlayerStates::Ability);
		AbilityType = ESmashAbilityTypes::Taunt;
		Direction = ActionDirection;
	}
}

void ASmashCharacter::Dizzy()
{
	SetMovementState(FSmashPlayerMovement(false, false, false, false));

	FTimerHandle DizzyHandle;
	GetWorld()->GetTimerManager().SetTimer(DizzyHandle, [this]()
	{
		bDizzied = false;
		StateSystem->ChangeState(ESmashPlayerStates::Idle);
	}, 2.0f, false);
}

float ASmashCharacter::NockBackCalculate()
{
	float Value0 = AbilitySystemComponent->DamageScale[AbilitySystemComponent->UsedMovesCount];
	float Value1 = SmashCharacterStatsComponent->Percent / 10.0f;
	float Value2 = (SmashCharacterStatsComponent->Percent * Damage) / 20.f;

	float Value3 = Value0 * (Value1 + Value2);
	float Value4 = (200.0f / (SmashCharacterStatsComponent->Weight + 100.0f)) * 1.4f;

	float Value5 = (Value3 * Value4) + 18.0f;
	float Value6 = (SmashCharacterStatsComponent->Percent / 7.0f) * HitScale;
	float Value7 = (Value5 *Value6) + BaseKnock;

	Knockback = Value7 * DamRatios;
	return Knockback;
}

void ASmashCharacter::ShieldHit(ESmashDirection NewDirection)
{
	// float Knockback = NockBackCalculate();
	// ApplyShieldKnockBack(Knockback, Direction);
}

void ASmashCharacter::LandedAction()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, P_Landed, LocationFeet);
	GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHeight);
	bLanded = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	if (!(StateSystem->GetCurrentState() == ESmashPlayerStates::Shield ||
		StateSystem->GetCurrentState() == ESmashPlayerStates::Ability ||
		StateSystem->GetCurrentState() == ESmashPlayerStates::Tumble))
	{
		SetCanFlip(true);
		StateSystem->ChangeState(ESmashPlayerStates::Idle);

		if (bDizzied)
			StateSystem->ChangeState(ESmashPlayerStates::Dizzy);
	}
}

void ASmashCharacter::OnLandedSmash(const FHitResult& Hit)
{
	LandedAction();

	FTimerHandle LandedHandle;
	GetWorld()->GetTimerManager().SetTimer(LandedHandle, [this]()
	{
		bLanded = false;
		LastHit = this;
	}, 0.01f, false);
}

void ASmashCharacter::Move(const struct FInputActionValue& Value)
{
	if (Controller && SmashCharacterMovementComponent)
	{
		if (StateInfo.PlayerMovement.bCanMove)
		{
			MoveInputValue = Value.Get<float>();
			SmashCharacterMovementComponent->AddInputVector(FVector::ForwardVector * MoveInputValue);
			//FootStep->DetectFeet();
			if (StateSystem->GetCurrentState() == ESmashPlayerStates::Idle && !bPushed && !(MoveInputValue == 0.0f))
			{
				StateSystem->ChangeState(ESmashPlayerStates::WalkAndRun);
			}
		}
	}
}

void ASmashCharacter::UpDownAxis(const FInputActionValue& InputActionValue)
{
	if (Controller && SmashCharacterMovementComponent)
	{
		UpDownInputValue = InputActionValue.Get<float>();
	}
}

void ASmashCharacter::JumpOrDrop_Implementation()
{
	if (bIsCrouched) // attempt to drop through platform, if any
	{
		SSTCharacterMovementComponent->WantsToPlatformDrop = true;
	}
	else
	{
		Jump();

		StateSystem->ChangeState(ESmashPlayerStates::Jump);

		SmashCharacterMovementComponent->GravityScale = SmashCharacterStatsComponent->DefaultGravityScale;

		bool bCondition = MoveInputValue > 0.0f && !IsFacingRight() || MoveInputValue < 0.0f && IsFacingRight() || MoveInputValue == 0.0f;
		if (bCondition && SmashCharacterMovementComponent->IsFalling())
		{
			GetMovementComponent()->StopMovementImmediately();
		}

		//아래로직은 2단점프일 경우 인것 같습니다.
		int32 LocalDirection = 0;
		if (IsFacingRight())
		{
			if (MoveInputValue < 0.0f)
			{
				LocalDirection = 1; //Right <
			}
			else
			{
				LocalDirection = 2; //Right >
			}
		}
		else
		{
			if (MoveInputValue > 0.0f)
			{
				LocalDirection = 3; //Left <
			}
			else
			{
				LocalDirection = 4; //Left <
			}
		}


		if (LocalDirection == 1 || LocalDirection == 3) Direction = ESmashDirection::Back;
		else if (LocalDirection == 2 || LocalDirection == 4) Direction = ESmashDirection::Forward;

		bLedgeCooldown = true;

		FTimerHandle LedgeCooldownHandle;
		GetWorld()->GetTimerManager().SetTimer(LedgeCooldownHandle, [this]()
		{
			bLedgeCooldown = false;
		}, 0.2f, false);
	}
}

void ASmashCharacter::ResetMoveInput(const FInputActionValue& Value)
{
	MoveInputValue = 0.0f;
}

void ASmashCharacter::BasicAttack()
{
	if (StateSystem->GetCurrentState() == ESmashPlayerStates::Shield)
	{
		StateSystem->ChangeState(ESmashPlayerStates::Ability);
		AbilityType = ESmashAbilityTypes::Grab;
		return;
	}

	if (StateSystem->GetCurrentState() != ESmashPlayerStates::Shield && StateInfo.PlayerMovement.bCanAttack)
	{
		if (GetCharacterMovement()->IsFalling())
		{
			StateSystem->ChangeState(ESmashPlayerStates::Ability);
			AbilityType = ESmashAbilityTypes::Air;
			return;
		}

		if (StateInfo.PlayCondition.bCanSmash)
		{
			StateSystem->ChangeState(ESmashPlayerStates::Ability);
			AbilityType = ESmashAbilityTypes::Smash;
			StateInfo.PlayCondition.bCanSmash = false;
			return;
		}

		if (StateSystem->GetCurrentState() != ESmashPlayerStates::WalkAndRun)
		{
			StateSystem->ChangeState(ESmashPlayerStates::Ability);
			AbilityType = ESmashAbilityTypes::Other;
			return;
		}

		StateSystem->ChangeState(ESmashPlayerStates::Ability);
		AbilityType = ESmashAbilityTypes::Basic;
	}
}

void ASmashCharacter::BasicAttackPressed(const FInputActionValue& InputActionValue)
{
	bAttackButton = true;

	if (StateSystem->GetCurrentState() != ESmashPlayerStates::Ability && StateInfo.PlayerMovement.bCanAttack)
	{
		BasicAttack();

		if (DoOnceBasicAttackPressed.Execute())
			bAttackButtonReleased = false;
	}
}

void ASmashCharacter::BasicAttackReleased(const FInputActionValue& InputActionValue)
{
	bAttackButton = false;
	bAttackButtonReleased = true;
}

void ASmashCharacter::SpecialAttackPressed(const FInputActionValue& InputActionValue)
{
	bSpecialAttackButton = true;
	if (StateSystem->GetCurrentState() != ESmashPlayerStates::Ability && StateInfo.PlayerMovement.bCanAttack)
	{
		StateSystem->ChangeState(ESmashPlayerStates::Ability);
		AbilityType = ESmashAbilityTypes::Special;
	}
}

void ASmashCharacter::SpecialAttackReleased(const FInputActionValue& InputActionValue)
{
	bSpecialAttackButton = false;
	bSpecialAttackButtonReleased = true;
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

void ASmashCharacter::SetMovementState(FSmashPlayerMovement SetMovement)
{
	// 서버에서만 상태 변경 (권한 체크)
	if (GetLocalRole() == ROLE_Authority)
	{
		StateInfo.PlayerMovement = SetMovement;

		// 즉시 방향 전환 상태를 Movement 컴포넌트에 반영
		if (SmashCharacterMovementComponent)
		{
			SmashCharacterMovementComponent->bCanFlip = SetMovement.bCanFlipping;
		}

		// 클라이언트에 상태 정보 업데이트 전파 (OnRep_StateInfo가 호출됨)
	}
	else
	{
		// 클라이언트에서는 서버에 요청
		// (여기서는 SetCanFlip과 같은 개별 필드 변경 메서드를 통해 처리)
	}
}

void ASmashCharacter::SetCanFlip(bool bNewCanFlip)
{
	// 서버에서만 실행되도록 변경
	if (GetLocalRole() == ROLE_Authority)
	{
		StateInfo.PlayerMovement.bCanFlipping = bNewCanFlip;

		// 나머지 PlayerMovement 값은 유지하며 값 업데이트
		SetMovementState(StateInfo.PlayerMovement);

		// 즉시 movement 컴포넌트에도 반영
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

void ASmashCharacter::Server_SetCanFlip_Implementation(bool bNewCanFlip)
{
	SetCanFlip(bNewCanFlip);
}

void ASmashCharacter::Client_UpdateStateInfo_Implementation(const FSmashPlayerStateInfo& NewStateInfo)
{
	// 서버에서 클라이언트로 상태 정보 업데이트 (권한이 없는 클라이언트에서는 직접 수정 불가)
	if (GetLocalRole() != ROLE_Authority)
	{
		StateInfo = NewStateInfo;
		OnRep_StateInfo();
	}
}

bool ASmashCharacter::CanFlip() const
{
	return StateInfo.PlayerMovement.bCanFlipping;
}

void ASmashCharacter::SetStateInfo(const FSmashPlayerStateInfo& NewStateInfo)
{
	// 서버에서만 상태 정보 변경
	if (GetLocalRole() == ROLE_Authority)
	{
		StateInfo = NewStateInfo;
		OnRep_StateInfo();

		// 모든 클라이언트에 상태 정보 업데이트 (필요시)
		// Client_UpdateStateInfo(NewStateInfo);
	}
}

void ASmashCharacter::OnRep_StateInfo()
{
	// Movement 컴포넌트에 방향 전환 속성 업데이트
	if (SmashCharacterMovementComponent)
	{
		SmashCharacterMovementComponent->bCanFlip = StateInfo.PlayerMovement.bCanFlipping;
	}

	// 추가적인 상태 처리 (애니메이션, 이펙트 등)
	// UpdateCharacterVisuals();
}

void ASmashCharacter::OnRep_LastHit()
{
	// LastHit이 변경되었을 때 처리할 코드
	if (LastHit)
	{
		// 추가 처리 (사운드, 파티클 등)
	}
}

void ASmashCharacter::OnRep_PlayerNo()
{
	// PlayerNo가 변경되었을 때 처리할 코드
	// UI 업데이트 등
	if (WidgetComponent)
	{
		if (UUW_HUD_PlayerName* HUD_PlayerName = Cast<UUW_HUD_PlayerName>(WidgetComponent->GetUserWidgetObject()))
		{
			HUD_PlayerName->PlayerNo = PlayerNo;
		}
	}
}

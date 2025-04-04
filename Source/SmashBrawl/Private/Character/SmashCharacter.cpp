#include "SmashBrawl/Public/Character//SmashCharacter.h"


#include "EnhancedInputComponent.h"
#include "AbilitySystem/BaseAbility.h"
#include "AbilitySystem/SmashAbilitySystemComponent.h"
#include "Actors/Shield.h"
#include "Character/FXComponent.h"
#include "Character/StateSystem.h"
#include "Character/Components/SmashCharacterMovementComponent.h"
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

	SmashCharacterMovementComponent = Cast<USmashCharacterMovementComponent>(GetCharacterMovement());

	// 컴포넌트 생성
	AbilitySystemComponent = CreateDefaultSubobject<USmashAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	StateSystem = CreateDefaultSubobject<UStateSystem>(TEXT("CharacterState"));
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	FXComponent = CreateDefaultSubobject<UFXComponent>(TEXT("FXComponent"));

	// 초기화 상태
	bInitialized = false;

	// FX 기본값 설정
	FlashDuration = 0.5f;
	FlashTimeRemaining = 0.0f;
	bIsFlashing = false;
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
}

void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();

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
		SetMovementState(FSmashPlayerMovement(false, false, false, true));

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

	// 널 체크 추가된 각 기능 호출
	FacingCheck();
	SprintCheck();


	SmashDetection();
	UpdateLocations();
	UpdateFlashing();
	UpdateWalkRun();
	UpdateStates();

	// 컴포넌트 업데이트 - 널 체크 추가
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

// 미구현된 함수들의 기본 구현
void ASmashCharacter::SprintCheck()
{
	// 나 레프트나 라이트 입력값이 눌렸어. 현재 상태가 IDLE 거나 WALK /RUN 다음로직을 실행한다.
	//발자국이펙트를 생성하고
	//상태를 Sprint로 바꾼다.


	// TODO: 달리기 상태 확인 및 처리
	// 예시 구현:
	// if (!IsInitialized() || !SmashCharacterMovementComponent) return;
	// 
	// bool bWantsToSprint = StateInfo.PlayerMovement.bWantsToSprint;
	// if (bWantsToSprint && SmashCharacterMovementComponent->CanSprint())
	// {
	//     SmashCharacterMovementComponent->SetSprinting(true);
	// }
	// else
	// {
	//     SmashCharacterMovementComponent->SetSprinting(false);
	// }
}

void ASmashCharacter::SmashDetection()
{
	// TODO: 스매시 공격 감지 및 처리
	// 예시 구현:
	// if (!IsInitialized() || !AbilitySystemComponent) return;
	// 
	// 스매시 입력 감지 로직
	// 스매시 공격 발동 처리
}

void ASmashCharacter::UpdateLocations()
{
	// TODO: 위치 업데이트 처리
	// 예시 구현:
	// if (!IsInitialized()) return;
	// 
	// 캐릭터 위치에 따른 특별 처리
	// 맵 경계 체크 등
}

void ASmashCharacter::UpdateFlashing()
{
	// TODO: 피격 시 깜빡임 효과 처리
	// 예시 구현:
	if (!IsInitialized()) return;

	if (bIsFlashing)
	{
		FlashTimeRemaining -= GetWorld()->GetDeltaSeconds();
		if (FlashTimeRemaining <= 0)
		{
			bIsFlashing = false;
			// 깜빡임 효과 종료 처리
		}
	}
}

void ASmashCharacter::UpdateWalkRun()
{
	// TODO: 걷기/달리기 상태 업데이트
	// 예시 구현:
	// if (!IsInitialized() || !SmashCharacterMovementComponent) return;
	// 
	// 걷기/달리기 상태 업데이트 로직
}

void ASmashCharacter::UpdateStates()
{
	// TODO: 캐릭터 상태 업데이트
	// 예시 구현:
	// if (!IsInitialized() || !StateSystem) return;
	// 
	// 현재 상태에 따른 추가 처리
}

void ASmashCharacter::Move(const struct FInputActionValue& Value)
{
	if (Controller && SmashCharacterMovementComponent)
	{
		// 입력값 저장
		MoveInputValue = Value.Get<float>();

		// 기존 이동 로직
		SmashCharacterMovementComponent->AddInputVector(FVector::ForwardVector * MoveInputValue);
	}
}

void ASmashCharacter::ResetMoveInput(const FInputActionValue& Value)
{
	MoveInputValue = 0.0f;
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
		// 피격 효과 처리
		bIsFlashing = true;
		FlashTimeRemaining = FlashDuration;

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

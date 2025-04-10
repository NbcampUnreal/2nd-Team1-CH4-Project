#include "Character/Action/PlayMontageAction.h"


#include "Character/CharacterState/BaseCharacterState.h"
#include "Character/SmashCharacter.h"
#include "Character/Components/SmashStateSystem.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

UPlayMontageAction::UPlayMontageAction()
{
	// 기본 액션 타입 설정
	ActionType = ESmashActionType::PlayMontage;

	// 기본값 초기화
	bAnimEndNext = false;
	NextState = ESmashPlayerStates::Idle;
	Montage = nullptr;
	PlayRate = 1.0f;
	StartSectionName = NAME_None;
	bRestartIfPlaying = true;
	bIsPlaying = false;
	BlendOutTime = 0.25f;
	bChangeStateOnInterrupted = false;
}

void UPlayMontageAction::InitAction_Implementation(UBaseCharacterState* InOwnerState)
{
	// 부모 클래스 초기화 호출
	Super::InitAction_Implementation(InOwnerState);

	bIsPlaying = false;
	MontageEndedDelegateHandle.Reset();
}

bool UPlayMontageAction::Execute_Implementation()
{
	if (CanExecute())
		return PlayMontage();

	return false;
}

bool UPlayMontageAction::CanExecute_Implementation()
{
	// 상위 클래스 조건 체크
	if (!Super::CanExecute_Implementation())
	{
		return false;
	}

	// 몽타주 유효성 검사
	if (!Montage)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPlayMontageAction::CanExecute - Montage is null"));
		return false;
	}

	// 캐릭터 참조 유효성 검사
	ASmashCharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPlayMontageAction::CanExecute - Owner character is null"));
		return false;
	}

	// 스켈레탈 메시 컴포넌트 검사
	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
	if (!MeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPlayMontageAction::CanExecute - Character mesh is null"));
		return false;
	}

	// 애니메이션 인스턴스 검사
	UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPlayMontageAction::CanExecute - Animation instance is null"));
		return false;
	}

	return true;
}

bool UPlayMontageAction::PlayMontage()
{
	// 캐릭터 및 애니메이션 인스턴스 가져오기
	ASmashCharacter* OwnerCharacter = GetOwnerCharacter();
	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
	UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();

	// 이미 재생 중인 경우 처리
	if (bIsPlaying)
	{
		if (!bRestartIfPlaying)
		{
			return true; // 이미 재생 중이고 다시 시작하지 않는 설정이면 성공으로 간주
		}

		// 기존 재생 중인 몽타주 중지
		AnimInstance->Montage_Stop(BlendOutTime, Montage);

		// 기존 델리게이트 제거
		CleanupDelegates();
	}

	// 몽타주 재생
	float Duration = AnimInstance->Montage_Play(Montage, PlayRate, EMontagePlayReturnType::MontageLength, 0.0f);

	if (Duration <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPlayMontageAction::PlayMontage - 몽타주를 연주하지 못했습니다 %s"), *Montage->GetName());
		return false;
	}

	// 특정 섹션에서 시작 (섹션 이름이 유효한 경우)
	if (StartSectionName != NAME_None)
	{
		AnimInstance->Montage_JumpToSection(StartSectionName, Montage);
	}
	
	AnimInstance->OnMontageEnded.AddDynamic(this, &UPlayMontageAction::OnMontageEnded);

	bIsPlaying = true;

	UE_LOG(LogTemp, Log, TEXT("UPlayMontageAction::PlayMontage - 몽타주 연주를 시작했습니다 %s"), *Montage->GetName());
	return true;
}

bool UPlayMontageAction::StopMontage(float InBlendOutTime)
{
	// 현재 재생 중이 아니면 실패
	if (!bIsPlaying)
	{
		return false;
	}

	// 캐릭터 참조 가져오기
	ASmashCharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter || !OwnerCharacter->GetMesh())
	{
		return false;
	}

	// 애니메이션 인스턴스 가져오기
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return false;
	}

	// 몽타주 중지
	AnimInstance->Montage_Stop(InBlendOutTime, Montage);

	// 델리게이트는 OnMontageEnded에서 정리됨
	return true;
}

void UPlayMontageAction::OnMontageEnded(UAnimMontage* EndedMontage, bool bInterrupted)
{
	// 다른 몽타주가 종료된 경우 무시
	if (EndedMontage != Montage)
	{
		return;
	}

	// 재생 상태 변경
	bIsPlaying = false;

	// 델리게이트 정리
	CleanupDelegates();

	// 인터럽트 처리 (중간에 중단된 경우)
	if (bInterrupted && !bChangeStateOnInterrupted)
	{
		UE_LOG(LogTemp, Log, TEXT("UPlayMontageAction::OnMontageEnded - 몽타주는 상태를 바꾸지 않고 중단되었습니다"));
		return;
	}

	// 몽타주 완료 후 다음 상태로 전환
	if (bAnimEndNext && OwnerState)
	{
		if (USmashStateSystem* StateSystem = OwnerState->OwnerStateSystem)
		{
			UE_LOG(LogTemp, Log, TEXT("UPlayMontageAction::OnMontageEnded - 상태 변경 %s"), *UEnum::GetValueAsString(NextState));
			StateSystem->TryChangeState(NextState);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UPlayMontageAction::OnMontageEnded - 상태 시스템은 NULL입니다"));
		}
	}
}

float UPlayMontageAction::GetMontagePlayProgress() const
{
	// 재생 중이 아니거나 몽타주가 없으면 0 반환
	if (!bIsPlaying || !Montage)
	{
		return 0.0f;
	}

	// 캐릭터 참조 및 애니메이션 인스턴스 확인
	ASmashCharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter || !OwnerCharacter->GetMesh())
	{
		return 0.0f;
	}

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return 0.0f;
	}

	// 현재 위치를 총 길이로 나누어 진행률 계산
	const float Position = AnimInstance->Montage_GetPosition(Montage);
	const float Length = Montage->GetPlayLength();

	return Length > 0.0f ? FMath::Clamp(Position / Length, 0.0f, 1.0f) : 0.0f;
}

void UPlayMontageAction::CleanupDelegates()
{
	// 캐릭터 및 애니메이션 인스턴스 확인
	ASmashCharacter* OwnerCharacter = GetOwnerCharacter();
	if (OwnerCharacter && OwnerCharacter->GetMesh())
	{
		if (UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &UPlayMontageAction::OnMontageEnded);
		}
	}

	MontageEndedDelegateHandle.Reset();
}

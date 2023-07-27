#include "Characters/Monster/AIControllers/C_AIControllerBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/Monster/C_Monster.h"
#include "Characters/Player/C_Mage.h"

#include "Utilities/CLog.h"

AC_AIControllerBase::AC_AIControllerBase()
{
	FString path = L"";

	{
		AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
		SensingSight = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");
		SetPerceptionComponent(*AIPerceptionComponent);

		/////////////////////////////////////////////////////////////////////////////
		// �ʱ�ȭ
		/////////////////////////////////////////////////////////////////////////////
		/// �þ� ����
		SensingSight->SightRadius = 3000.0f;
		/// ���� �þ� ����
		SensingSight->LoseSightRadius = 3500.0f;
		/// �þ� ����(����)
		SensingSight->PeripheralVisionAngleDegrees = 90.0f;
		/// ���� �ִ� �ð�
		//SensingSight->SetMaxAge(5.0f);
		/////////////////////////////////////////////////////////////////////////////

		/// �� ��ü ����
		SensingSight->DetectionByAffiliation.bDetectEnemies = true;
		/// �Ʊ� ��ü ����
		SensingSight->DetectionByAffiliation.bDetectFriendlies = false;
		/// �߸� ��ü ����
		SensingSight->DetectionByAffiliation.bDetectNeutrals = true;

		/// ���� ���� ���
		GetPerceptionComponent()->SetDominantSense(*SensingSight->GetSenseImplementation());
		/// ���� ��� �߰�
		GetPerceptionComponent()->ConfigureSense(*SensingSight);
	}
}

void AC_AIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AC_AIControllerBase::OnSensingTarget);
	//TargetKeyDelegate.BindUFunction(this, "UpdateTargetKey", nullptr);
}

void AC_AIControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Character->AddMovementInput(FVector(10.0f, 0.0f, 0.0f));
	//MoveToLocation(FVector(10.0f, 0.0f, 0.0f));
}

void AC_AIControllerBase::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

}

void AC_AIControllerBase::OnAttacking()
{
}

void AC_AIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn == nullptr) return;

	Character = Cast<AC_Monster>(InPawn);
	if (Character->GetBehaviorTree() == nullptr) return;

	UBlackboardComponent* blackboard = nullptr;
	if (UseBlackboard(Character->GetBehaviorTree()->GetBlackboardAsset(), blackboard))
		RunBehaviorTree(Character->GetBehaviorTree());

	Blackboard = blackboard;
}

void AC_AIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();

}

void AC_AIControllerBase::OnSensingTarget(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() == true)
	{
		if (Actor != nullptr)
		{
			AC_Mage* target = Cast<AC_Mage>(Actor);
			if (target != nullptr)
			{
				Blackboard.Get()->SetValueAsBool(L"HasLineOfSight", true);

				if (Blackboard.Get()->GetValueAsObject(L"Target") == nullptr)
					Blackboard.Get()->SetValueAsObject(L"Target", Actor);

				//GetWorld()->GetTimerManager().ClearTimer(TargetKeyHandle);
				//Blackboard.Get()->SetValueAsBool(L"HasLineOfSight", true);

				//CLog::Print(L"HasLineOfSight == true", 3.0f, FColor::Yellow);
			}
		}
	}

	else
	{
		//GetWorld()->GetTimerManager().SetTimer(TargetKeyHandle, 8.0f, false);
		Blackboard.Get()->SetValueAsBool(L"HasLineOfSight", false);

		CLog::Print(L"HasLineOfSight == false", 3.0f, FColor::Yellow);
	}
}
#include "AISystem/BTService_UpdateTarget.h"
#include "AISystem/BaseAIController.h"
#include "Core/BaseAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTService_UpdateTarget::UBTService_UpdateTarget()
{
	NodeName = "Update Target";
	Interval = 0.5f; // Check twice per second
	RandomDeviation = 0.1f;

	// Accept only actors
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateTarget, TargetActorKey), AActor::StaticClass());
	CanSeeTargetKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateTarget, CanSeeTargetKey));
	LastKnownLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateTarget, LastKnownLocationKey));
}

void UBTService_UpdateTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	// Get target from blackboard
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!TargetActor)
	{
		// No target
		BlackboardComp->SetValueAsBool(CanSeeTargetKey.SelectedKeyName, false);
		return;
	}

	// Check if target is valid
	if (!IsValid(TargetActor))
	{
		AIController->ClearCurrentTarget();
		TimeSinceLastSeen = 0.0f;
		return;
	}

	// Check if target is dead
	if (ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor))
	{
		if (TargetCharacter->GetMesh() && TargetCharacter->GetMesh()->IsSimulatingPhysics())
		{
			// Target is ragdolled (dead)
			AIController->ClearCurrentTarget();
			TimeSinceLastSeen = 0.0f;
			return;
		}
	}

	// Check line of sight
	bool bCanSeeTarget = AIController->CanSeeTarget();
	BlackboardComp->SetValueAsBool(CanSeeTargetKey.SelectedKeyName, bCanSeeTarget);

	if (bCanSeeTarget)
	{
		// Update last known location
		BlackboardComp->SetValueAsVector(LastKnownLocationKey.SelectedKeyName, TargetActor->GetActorLocation());
		TimeSinceLastSeen = 0.0f;
	}
	else
	{
		// Lost sight
		TimeSinceLastSeen += DeltaSeconds;

		// Clear target if lost for too long
		if (bClearTargetIfLost && TimeSinceLastSeen >= LostTargetTimeout)
		{
			UE_LOG(LogTemp, Display, TEXT(">>> BTService_UpdateTarget: Lost target for %.1fs, clearing"), TimeSinceLastSeen);
			AIController->ClearCurrentTarget();
			TimeSinceLastSeen = 0.0f;
		}
	}
}

FString UBTService_UpdateTarget::GetStaticDescription() const
{
	return FString::Printf(TEXT("Update target info (timeout: %.1fs)"), LostTargetTimeout);
}

#include "AISystem/BTTask_StartCombat.h"
#include "AISystem/BaseAIController.h"
#include "Core/BaseAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_StartCombat::UBTTask_StartCombat()
{
	NodeName = "Start Combat";
	bNotifyTick = false;

	// Accept only actors
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_StartCombat, TargetActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_StartCombat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	ABaseAICharacter* AICharacter = AIController->GetAICharacter();
	if (!AICharacter)
	{
		return EBTNodeResult::Failed;
	}

	// Get target from blackboard
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> BTTask_StartCombat: [%s] no target in blackboard"), *AICharacter->GetName());
		return EBTNodeResult::Failed;
	}

	// Start shooting
	AICharacter->StartShooting(TargetActor);
	UE_LOG(LogTemp, Display, TEXT(">>> BTTask_StartCombat: [%s] started shooting at [%s]"), 
		*AICharacter->GetName(), 
		*TargetActor->GetName());

	return EBTNodeResult::Succeeded;
}

FString UBTTask_StartCombat::GetStaticDescription() const
{
	return FString::Printf(TEXT("Start shooting at: %s"), *TargetActorKey.SelectedKeyName.ToString());
}

#include "AISystem/BTTask_StopCombat.h"
#include "AISystem/BaseAIController.h"
#include "Core/BaseAICharacter.h"

UBTTask_StopCombat::UBTTask_StopCombat()
{
	NodeName = "Stop Combat";
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_StopCombat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	// Stop shooting
	AICharacter->StopShooting();
	UE_LOG(LogTemp, Display, TEXT(">>> BTTask_StopCombat: [%s] stopped shooting"), *AICharacter->GetName());

	return EBTNodeResult::Succeeded;
}

FString UBTTask_StopCombat::GetStaticDescription() const
{
	return TEXT("Stop shooting");
}

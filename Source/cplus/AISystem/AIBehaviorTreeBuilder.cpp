#include "AISystem/AIBehaviorTreeBuilder.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/Composites/BTComposite_Selector.h"
#include "BehaviorTree/Composites/BTComposite_Sequence.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BehaviorTree/BTService.h"
#include "AISystem/BTTask_EquipWeapon.h"
#include "AISystem/BTTask_StartCombat.h"
#include "AISystem/BTService_UpdateTarget.h"
#include "AISystem/BTDecorator_HasWeapon.h"

UBehaviorTree* UAIBehaviorTreeBuilder::CreateCombatBehaviorTree(UObject* Outer, TSubclassOf<ABaseWeapon> WeaponClass)
{
	if (!Outer)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> AIBehaviorTreeBuilder: No Outer object provided"));
		return nullptr;
	}

	// Create Behavior Tree
	UBehaviorTree* BehaviorTree = NewObject<UBehaviorTree>(Outer, UBehaviorTree::StaticClass(), NAME_None, RF_Public | RF_Standalone);
	if (!BehaviorTree)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> AIBehaviorTreeBuilder: Failed to create BehaviorTree"));
		return nullptr;
	}

	// Create Blackboard
	UBlackboardData* Blackboard = CreateCombatBlackboard(BehaviorTree);
	if (!Blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> AIBehaviorTreeBuilder: Failed to create Blackboard"));
		return nullptr;
	}

	BehaviorTree->BlackboardAsset = Blackboard;

	// Create Root Selector
	UBTComposite_Selector* RootSelector = NewObject<UBTComposite_Selector>(BehaviorTree);
	BehaviorTree->RootNode = RootSelector;

	// === COMBAT BRANCH ===
	
	// Combat Sequence (outer)
	UBTComposite_Sequence* CombatSequence = NewObject<UBTComposite_Sequence>(BehaviorTree);
	RootSelector->Children.Add(FBTCompositeChild());
	RootSelector->Children[0].ChildComposite = CombatSequence;

	// Add decorator: Has Target (checks if TargetActor is set)
	// Note: We'll use a simple check in the service instead

	// Add service: Update Target
	UBTService_UpdateTarget* UpdateTargetService = NewObject<UBTService_UpdateTarget>(BehaviorTree);
	UpdateTargetService->TargetActorKey.SelectedKeyName = FName("TargetActor");
	UpdateTargetService->CanSeeTargetKey.SelectedKeyName = FName("CanSeeTarget");
	UpdateTargetService->LastKnownLocationKey.SelectedKeyName = FName("LastKnownLocation");
	CombatSequence->Services.Add(UpdateTargetService);

	// Task 1: Move To Target
	UBTTask_MoveTo* MoveToTask = NewObject<UBTTask_MoveTo>(BehaviorTree);
	// Use reflection to set protected BlackboardKey
	if (FProperty* KeyProperty = MoveToTask->GetClass()->FindPropertyByName(FName("BlackboardKey")))
	{
		FBlackboardKeySelector* KeySelector = KeyProperty->ContainerPtrToValuePtr<FBlackboardKeySelector>(MoveToTask);
		if (KeySelector)
		{
			KeySelector->SelectedKeyName = FName("TargetActor");
		}
	}
	MoveToTask->AcceptableRadius = 300.0f; // Stop 3m from target (combat range)
	CombatSequence->Children.Add(FBTCompositeChild());
	CombatSequence->Children[0].ChildTask = MoveToTask;

	// Task 2: Start Combat (after reaching target)
	UBTTask_StartCombat* StartCombatTask = NewObject<UBTTask_StartCombat>(BehaviorTree);
	StartCombatTask->TargetActorKey.SelectedKeyName = FName("TargetActor");
	CombatSequence->Children.Add(FBTCompositeChild());
	CombatSequence->Children[1].ChildTask = StartCombatTask;

	// === IDLE BRANCH ===

	// Idle Sequence
	UBTComposite_Sequence* IdleSequence = NewObject<UBTComposite_Sequence>(BehaviorTree);
	RootSelector->Children.Add(FBTCompositeChild());
	RootSelector->Children[1].ChildComposite = IdleSequence;

	// Task: Wait
	UBTTask_Wait* WaitTask = NewObject<UBTTask_Wait>(BehaviorTree);
	WaitTask->WaitTime = 2.0f;
	WaitTask->RandomDeviation = 1.0f;
	IdleSequence->Children.Add(FBTCompositeChild());
	IdleSequence->Children[0].ChildTask = WaitTask;

	UE_LOG(LogTemp, Display, TEXT(">>> AIBehaviorTreeBuilder: Created combat BehaviorTree"));

	return BehaviorTree;
}

UBlackboardData* UAIBehaviorTreeBuilder::CreateCombatBlackboard(UObject* Outer)
{
	if (!Outer)
	{
		return nullptr;
	}

	// Create Blackboard
	UBlackboardData* Blackboard = NewObject<UBlackboardData>(Outer, UBlackboardData::StaticClass(), NAME_None, RF_Public | RF_Standalone);
	if (!Blackboard)
	{
		return nullptr;
	}

	// Add TargetActor key (Object)
	FBlackboardEntry TargetActorEntry;
	TargetActorEntry.EntryName = FName("TargetActor");
	TargetActorEntry.KeyType = NewObject<UBlackboardKeyType_Object>(Blackboard);
	Cast<UBlackboardKeyType_Object>(TargetActorEntry.KeyType)->BaseClass = AActor::StaticClass();
	Blackboard->Keys.Add(TargetActorEntry);

	// Add LastKnownLocation key (Vector)
	FBlackboardEntry LastKnownLocationEntry;
	LastKnownLocationEntry.EntryName = FName("LastKnownLocation");
	LastKnownLocationEntry.KeyType = NewObject<UBlackboardKeyType_Vector>(Blackboard);
	Blackboard->Keys.Add(LastKnownLocationEntry);

	// Add CanSeeTarget key (Bool)
	FBlackboardEntry CanSeeTargetEntry;
	CanSeeTargetEntry.EntryName = FName("CanSeeTarget");
	CanSeeTargetEntry.KeyType = NewObject<UBlackboardKeyType_Bool>(Blackboard);
	Blackboard->Keys.Add(CanSeeTargetEntry);

	// Add IsInCombat key (Bool)
	FBlackboardEntry IsInCombatEntry;
	IsInCombatEntry.EntryName = FName("IsInCombat");
	IsInCombatEntry.KeyType = NewObject<UBlackboardKeyType_Bool>(Blackboard);
	Blackboard->Keys.Add(IsInCombatEntry);

	UE_LOG(LogTemp, Display, TEXT(">>> AIBehaviorTreeBuilder: Created combat Blackboard with %d keys"), Blackboard->Keys.Num());

	return Blackboard;
}

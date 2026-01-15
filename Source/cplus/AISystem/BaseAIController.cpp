#include "AISystem/BaseAIController.h"
#include "AISystem/AIBehaviorTreeBuilder.h"
#include "Core/BaseAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

ABaseAIController::ABaseAIController()
{
	// Create AI Perception component
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	// Create sight config
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngle;
	SightConfig->SetMaxAge(MaxAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AutoSuccessRange;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	// Create hearing config
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = HearingRange;
	HearingConfig->SetMaxAge(MaxAge);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;

	// Create damage config
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	DamageConfig->SetMaxAge(MaxAge);

	// Add senses to perception
	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->ConfigureSense(*HearingConfig);
	AIPerception->ConfigureSense(*DamageConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	// Enable auto-possess AI
	bStartAILogicOnPossess = true;
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();

	// Bind perception events
	if (AIPerception)
	{
		AIPerception->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnPerceptionUpdated);
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnTargetPerceptionUpdated);
	}
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Cache AI character
	AICharacter = Cast<ABaseAICharacter>(InPawn);

	if (!AICharacter)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> AI CONTROLLER: Possessed pawn is not ABaseAICharacter!"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT(">>> AI CONTROLLER: Possessed [%s]"), *AICharacter->GetName());

	// Create behavior tree if needed
	if (!BehaviorTree && bAutoCreateBehaviorTree)
	{
		UE_LOG(LogTemp, Display, TEXT(">>> AI CONTROLLER: Auto-creating BehaviorTree..."));
		BehaviorTree = UAIBehaviorTreeBuilder::CreateCombatBehaviorTree(this, DefaultWeaponClass);
		
		if (BehaviorTree)
		{
			UE_LOG(LogTemp, Display, TEXT(">>> AI CONTROLLER: BehaviorTree created successfully"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT(">>> AI CONTROLLER: Failed to create BehaviorTree"));
			return;
		}
	}

	// Run behavior tree
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		UE_LOG(LogTemp, Display, TEXT(">>> AI CONTROLLER: Running BehaviorTree: %s"), *BehaviorTree->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> AI CONTROLLER: No BehaviorTree assigned!"));
	}
}

void ABaseAIController::OnUnPossess()
{
	// Stop behavior tree
	if (BrainComponent)
	{
		BrainComponent->StopLogic(TEXT("Unpossessed"));
	}

	AICharacter = nullptr;
	CurrentTarget = nullptr;

	Super::OnUnPossess();
}

void ABaseAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	// This is called whenever any actor's perception is updated
	// We handle target selection in OnTargetPerceptionUpdated
}

void ABaseAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !AICharacter)
	{
		return;
	}

	// Ignore if actor is dead
	if (ACharacter* TargetCharacter = Cast<ACharacter>(Actor))
	{
		if (!IsValid(TargetCharacter))
		{
			return;
		}
	}

	// Check if this is a player
	bool bIsPlayer = Actor->IsA(ACharacter::StaticClass()) && Cast<ACharacter>(Actor)->IsPlayerControlled();

	if (!bIsPlayer)
	{
		return; // Only target players for now
	}

	// Successfully sensed
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Display, TEXT(">>> AI PERCEPTION: [%s] detected [%s] via %s"), 
			*AICharacter->GetName(), 
			*Actor->GetName(),
			*Stimulus.Type.Name.ToString());

		// Set as current target
		SetCurrentTarget(Actor);

		// Update blackboard
		if (UBlackboardComponent* BB = GetBlackboardComponent())
		{
			BB->SetValueAsBool(CanSeeTargetKey, true);
			BB->SetValueAsVector(LastKnownLocationKey, Stimulus.StimulusLocation);
			BB->SetValueAsBool(IsInCombatKey, true);
		}
	}
	// Lost sight
	else
	{
		UE_LOG(LogTemp, Display, TEXT(">>> AI PERCEPTION: [%s] lost sight of [%s]"), 
			*AICharacter->GetName(), 
			*Actor->GetName());

		// Update blackboard
		if (UBlackboardComponent* BB = GetBlackboardComponent())
		{
			BB->SetValueAsBool(CanSeeTargetKey, false);
			BB->SetValueAsVector(LastKnownLocationKey, Stimulus.StimulusLocation);
		}

		// Don't clear target immediately - let BT handle search behavior
	}
}

void ABaseAIController::SetCurrentTarget(AActor* NewTarget)
{
	if (CurrentTarget == NewTarget)
	{
		return;
	}

	CurrentTarget = NewTarget;

	// Update blackboard
	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->SetValueAsObject(TargetActorKey, CurrentTarget);
		
		if (CurrentTarget)
		{
			BB->SetValueAsVector(LastKnownLocationKey, CurrentTarget->GetActorLocation());
			UE_LOG(LogTemp, Display, TEXT(">>> AI CONTROLLER: [%s] target set to [%s]"), 
				AICharacter ? *AICharacter->GetName() : TEXT("Unknown"),
				*CurrentTarget->GetName());
		}
	}
}

void ABaseAIController::ClearCurrentTarget()
{
	CurrentTarget = nullptr;

	// Update blackboard
	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->ClearValue(TargetActorKey);
		BB->SetValueAsBool(CanSeeTargetKey, false);
		BB->SetValueAsBool(IsInCombatKey, false);
		
		UE_LOG(LogTemp, Display, TEXT(">>> AI CONTROLLER: [%s] target cleared"), 
			AICharacter ? *AICharacter->GetName() : TEXT("Unknown"));
	}
}

bool ABaseAIController::CanSeeTarget() const
{
	if (!CurrentTarget || !AIPerception)
	{
		return false;
	}

	FActorPerceptionBlueprintInfo Info;
	AIPerception->GetActorsPerception(CurrentTarget, Info);

	// Check if sight stimulus is active
	for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISenseConfig_Sight>())
		{
			return Stimulus.WasSuccessfullySensed();
		}
	}

	return false;
}

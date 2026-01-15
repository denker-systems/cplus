#include "AnimNotifyState_MeleeCollision.h"
#include "CombatSystem/Core/CombatComponent.h"
#include "GameFramework/Actor.h"

void UAnimNotifyState_MeleeCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	TimeSinceLastTrace = 0.0f;
	
	if (MeshComp && MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("AnimNotifyState_MeleeCollision: Begin - Socket: %s"), *SocketName.ToString());
	}
}

void UAnimNotifyState_MeleeCollision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	TimeSinceLastTrace += FrameDeltaTime;

	if (TimeSinceLastTrace >= TraceFrequency)
	{
		PerformTrace(MeshComp);
		TimeSinceLastTrace = 0.0f;
	}
}

void UAnimNotifyState_MeleeCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("AnimNotifyState_MeleeCollision: End"));
	}
}

void UAnimNotifyState_MeleeCollision::PerformTrace(USkeletalMeshComponent* MeshComp)
{
	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	UCombatComponent* CombatComp = Owner->FindComponentByClass<UCombatComponent>();

	if (!CombatComp)
	{
		return;
	}

	FMeleeCollisionInfo CollisionInfo;
	CollisionInfo.CollisionType = CollisionType;
	CollisionInfo.SocketName = SocketName;
	CollisionInfo.TraceRadius = TraceRadius;
	CollisionInfo.bDrawDebug = bDrawDebug;

	CombatComp->PerformMeleeTrace(CollisionInfo);
}

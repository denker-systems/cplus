#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CombatSystem/Core/CombatTypes.h"
#include "AnimNotifyState_MeleeCollision.generated.h"

UCLASS()
class CPLUS_API UAnimNotifyState_MeleeCollision : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Collision")
	EMeleeCollisionType CollisionType = EMeleeCollisionType::RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Collision")
	FName SocketName = FName("hand_r");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Collision")
	float TraceRadius = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Collision")
	bool bDrawDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Collision")
	float TraceFrequency = 0.016f;

protected:
	void PerformTrace(USkeletalMeshComponent* MeshComp);

private:
	float TimeSinceLastTrace = 0.0f;
};

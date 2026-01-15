#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatSystem/Core/CombatTypes.h"
#include "CombatInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class CPLUS_API ICombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void TakeCombatDamage(const FHitData& HitData, AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool CanBeDamaged() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void OnHitReaction(EHitReaction ReactionType, const FVector& HitDirection);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool IsAlive() const;
};

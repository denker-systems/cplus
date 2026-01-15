#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BasePlayerAnimInstance.generated.h"

class ABasePlayerCharacter;

UCLASS()
class CPLUS_API UBasePlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	TObjectPtr<ABasePlayerCharacter> PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Direction = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsDead = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Weapon")
	bool bIsArmed = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Weapon")
	bool bIsPistol = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Weapon")
	bool bIsRifle = false;
};

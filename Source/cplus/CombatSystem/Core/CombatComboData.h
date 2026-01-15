#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatTypes.h"
#include "CombatComboData.generated.h"

UCLASS(BlueprintType)
class CPLUS_API UCombatComboData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	FName ComboID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	FText ComboName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	FText ComboDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	TArray<FComboAttack> ComboChain;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	float ComboResetTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	bool bLoopCombo = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	bool bRequiresPerfectTiming = false;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("CombatCombo", GetFName());
	}

	UFUNCTION(BlueprintCallable, Category = "Combo")
	int32 GetComboLength() const { return ComboChain.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Combo")
	bool IsValidComboIndex(int32 Index) const { return ComboChain.IsValidIndex(Index); }

	UFUNCTION(BlueprintCallable, Category = "Combo")
	FComboAttack GetComboAttack(int32 Index) const
	{
		return ComboChain.IsValidIndex(Index) ? ComboChain[Index] : FComboAttack();
	}
};

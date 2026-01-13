#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "BaseCharacter.generated.h"

/**
 * Base Character
 * 
 * Root class for all characters in the game (players and AI)
 * Provides common functionality: movement, animation, GameplayTags
 */
UCLASS(Abstract)
class CPLUS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

	/** Gameplay tags for this character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FGameplayTagContainer CharacterTags;

public:
	/** Get character tags */
	UFUNCTION(BlueprintPure, Category = "Character")
	FGameplayTagContainer GetCharacterTags() const { return CharacterTags; }

	/** Add tag to character */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void AddCharacterTag(FGameplayTag Tag);

	/** Remove tag from character */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void RemoveCharacterTag(FGameplayTag Tag);

	/** Check if character has tag */
	UFUNCTION(BlueprintPure, Category = "Character")
	bool HasCharacterTag(FGameplayTag Tag) const;
};

#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "QuestInteractable.h"
#include "FriendlyAICharacter.generated.h"

/**
 * Friendly AI Character
 * 
 * Base class for friendly NPCs (quest givers, merchants, allies)
 * Provides: Dialog system, Quest giving, Interaction
 */
UCLASS(Abstract)
class CPLUS_API AFriendlyAICharacter : public ABaseAICharacter, public IQuestInteractable
{
	GENERATED_BODY()

public:
	AFriendlyAICharacter();

protected:
	virtual void BeginPlay() override;

	/** Quest giver component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UQuestGiverComponent* QuestGiver;

	/** NPC identifier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FName NPCID;

public:
	// IQuestInteractable interface
	virtual void Interact_Implementation(AActor* Interactor) override;

	/** Get NPC ID */
	UFUNCTION(BlueprintPure, Category = "NPC")
	FName GetNPCID() const { return NPCID; }
};

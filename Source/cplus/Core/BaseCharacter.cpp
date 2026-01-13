#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::AddCharacterTag(FGameplayTag Tag)
{
	CharacterTags.AddTag(Tag);
}

void ABaseCharacter::RemoveCharacterTag(FGameplayTag Tag)
{
	CharacterTags.RemoveTag(Tag);
}

bool ABaseCharacter::HasCharacterTag(FGameplayTag Tag) const
{
	return CharacterTags.HasTag(Tag);
}

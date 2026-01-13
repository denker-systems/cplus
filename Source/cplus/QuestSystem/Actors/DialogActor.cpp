#include "DialogActor.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"

ADialogActor::ADialogActor()
{
	PrimaryActorTick.bCanEverTick = false;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	RootComponent = AudioComponent;
	AudioComponent->bAutoActivate = false;
}

void ADialogActor::PlayDialogSequence(const TArray<FQuestDialogData>& Dialogs)
{
	if (Dialogs.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("DialogActor: No dialogs to play"));
		return;
	}

	CurrentDialogs = Dialogs;
	CurrentDialogIndex = 0;
	PlayNextDialog();
}

void ADialogActor::PlayNextDialog()
{
	if (CurrentDialogIndex >= CurrentDialogs.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("DialogActor: Dialog sequence complete"));
		return;
	}

	const FQuestDialogData& Dialog = CurrentDialogs[CurrentDialogIndex];

	// Play voice over
	if (Dialog.VoiceOver && AudioComponent)
	{
		AudioComponent->SetSound(Dialog.VoiceOver);
		AudioComponent->Play();
	}

	// TODO: Show dialog UI with text and portrait
	// TODO: Play NPC animation

	UE_LOG(LogTemp, Log, TEXT("DialogActor: Playing dialog %d: %s"), 
		CurrentDialogIndex, *Dialog.DialogText.ToString());

	// Schedule next dialog
	float Duration = Dialog.DisplayDuration;
	if (Duration <= 0.0f && Dialog.VoiceOver)
	{
		Duration = Dialog.VoiceOver->GetDuration();
	}
	if (Duration <= 0.0f)
	{
		Duration = 3.0f; // Default
	}

	CurrentDialogIndex++;
	GetWorld()->GetTimerManager().SetTimer(DialogTimerHandle, this, &ADialogActor::PlayNextDialog, Duration, false);
}

void ADialogActor::StopDialog()
{
	GetWorld()->GetTimerManager().ClearTimer(DialogTimerHandle);
	
	if (AudioComponent)
	{
		AudioComponent->Stop();
	}

	CurrentDialogs.Empty();
	CurrentDialogIndex = 0;
}

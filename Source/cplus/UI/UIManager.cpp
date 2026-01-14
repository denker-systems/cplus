// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"
#include "PauseMenuWidget.h"
#include "QuestSystem/UI/QuestUIManager.h"
#include "Blueprint/UserWidget.h"

UUIManager::UUIManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Create QuestUIManager as sub-component
	QuestUIManager = CreateDefaultSubobject<UQuestUIManager>(TEXT("QuestUIManager"));
}

void UUIManager::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT(">>> UI MANAGER: BeginPlay called"));

	// Create pause menu widget
	CreatePauseMenuWidget();

	UE_LOG(LogTemp, Display, TEXT(">>> UI MANAGER: Initialization complete"));
}

void UUIManager::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UUIManager::CreatePauseMenuWidget()
{
	if (!PauseMenuWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> UI MANAGER: PauseMenuWidgetClass not set"));
		return;
	}

	PauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);
	if (PauseMenuWidget)
	{
		PauseMenuWidget->AddToViewport();
		UE_LOG(LogTemp, Display, TEXT(">>> UI MANAGER: Pause menu widget created"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(">>> UI MANAGER: Failed to create pause menu widget"));
	}
}

void UUIManager::TogglePauseMenu()
{
	if (!PauseMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> UI MANAGER: PauseMenuWidget is null"));
		return;
	}

	PauseMenuWidget->TogglePauseMenu();
}

void UUIManager::ShowPauseMenu()
{
	if (!PauseMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> UI MANAGER: PauseMenuWidget is null"));
		return;
	}

	PauseMenuWidget->ShowPauseMenu();
}

void UUIManager::HidePauseMenu()
{
	if (!PauseMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT(">>> UI MANAGER: PauseMenuWidget is null"));
		return;
	}

	PauseMenuWidget->HidePauseMenu();
}

bool UUIManager::IsPauseMenuVisible() const
{
	return PauseMenuWidget && PauseMenuWidget->IsPauseMenuVisible();
}

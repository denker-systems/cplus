// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"
#include "PauseMenuWidget.h"
#include "InventoryWidget.h"
#include "QuestSystem/UI/QuestUIManager.h"
#include "InventorySystem/InventoryComponent.h"
#include "Blueprint/UserWidget.h"

UUIManager::UUIManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUIManager::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT(">>> UI MANAGER: BeginPlay called"));

	// Create QuestUIManager as a proper component on the owner actor
	if (AActor* Owner = GetOwner())
	{
		QuestUIManager = NewObject<UQuestUIManager>(Owner, UQuestUIManager::StaticClass(), TEXT("QuestUIManager"));
		if (QuestUIManager)
		{
			// Pass widget classes from UIManager to QuestUIManager
			QuestUIManager->NotificationWidgetClass = NotificationWidgetClass;
			QuestUIManager->JournalWidgetClass = JournalWidgetClass;
			QuestUIManager->QuestGiverWidgetClass = QuestGiverWidgetClass;
			QuestUIManager->QuestCompletionWidgetClass = QuestCompletionWidgetClass;
			
			QuestUIManager->RegisterComponent();
			UE_LOG(LogTemp, Display, TEXT(">>> UI MANAGER: QuestUIManager created and registered"));
		}
	}

	// Create pause menu widget
	CreatePauseMenuWidget();

	// Create inventory widget
	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport(50);
			
			// Find inventory component on owner and link it
			if (AActor* Owner = GetOwner())
			{
				if (UInventoryComponent* Inventory = Owner->FindComponentByClass<UInventoryComponent>())
				{
					InventoryWidget->SetInventoryComponent(Inventory);
				}
			}
			UE_LOG(LogTemp, Display, TEXT(">>> UI MANAGER: Inventory widget created"));
		}
	}

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

void UUIManager::ToggleInventory()
{
	if (InventoryWidget)
	{
		InventoryWidget->ToggleInventory();
	}
}

void UUIManager::ShowInventory()
{
	if (InventoryWidget)
	{
		InventoryWidget->ShowInventory();
	}
}

void UUIManager::HideInventory()
{
	if (InventoryWidget)
	{
		InventoryWidget->HideInventory();
	}
}

bool UUIManager::IsInventoryVisible() const
{
	return InventoryWidget && InventoryWidget->IsInventoryVisible();
}

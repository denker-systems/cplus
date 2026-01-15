#include "InventoryWidget.h"
#include "InventorySystem/InventoryComponent.h"
#include "InventorySystem/InventoryItem.h"
#include "InventorySystem/ItemDefinition.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TitleText)
	{
		TitleText->SetText(FText::FromString(TEXT("INVENTORY")));
		TitleText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnCloseClicked);
	}

	// Start hidden
	SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryWidget::SetInventoryComponent(UInventoryComponent* InInventory)
{
	// Unbind from old
	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChanged.RemoveDynamic(this, &UInventoryWidget::OnInventoryChanged);
	}

	InventoryComponent = InInventory;

	// Bind to new
	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChanged.AddDynamic(this, &UInventoryWidget::OnInventoryChanged);
		RefreshInventory();
	}
}

void UInventoryWidget::RefreshInventory()
{
	ClearItemEntries();

	if (!InventoryComponent)
	{
		return;
	}

	// Add entry for each item
	TArray<FInventoryItem> Items = InventoryComponent->GetAllItems();
	for (const FInventoryItem& Item : Items)
	{
		AddItemEntry(Item);
	}

	// Update weight display
	if (WeightText)
	{
		WeightText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		if (InventoryComponent->bUseWeightLimit)
		{
			FString WeightStr = FString::Printf(TEXT("Weight: %.1f / %.1f"),
				InventoryComponent->GetCurrentWeight(),
				InventoryComponent->MaxWeight);
			WeightText->SetText(FText::FromString(WeightStr));
		}
		else
		{
			FString CountStr = FString::Printf(TEXT("Items: %d / %d"),
				InventoryComponent->GetItemCount(),
				InventoryComponent->MaxSlots);
			WeightText->SetText(FText::FromString(CountStr));
		}
	}
}

void UInventoryWidget::AddItemEntry(const FInventoryItem& Item)
{
	if (!ItemListContainer || !Item.ItemData)
	{
		return;
	}

	// Create horizontal box for entry
	UHorizontalBox* EntryBox = NewObject<UHorizontalBox>(this);

	// Create item name text
	UTextBlock* NameText = NewObject<UTextBlock>(this);
	FString DisplayText = FString::Printf(TEXT("%s x%d"),
		*Item.ItemData->ItemName.ToString(),
		Item.Quantity);
	NameText->SetText(FText::FromString(DisplayText));

	// Set color based on rarity
	NameText->SetColorAndOpacity(FSlateColor(Item.ItemData->RarityColor));

	// Set font size
	FSlateFontInfo FontInfo = NameText->GetFont();
	FontInfo.Size = 16;
	NameText->SetFont(FontInfo);

	EntryBox->AddChild(NameText);

	// Add to container
	ItemListContainer->AddChild(EntryBox);
}

void UInventoryWidget::ClearItemEntries()
{
	if (ItemListContainer)
	{
		ItemListContainer->ClearChildren();
	}
}

void UInventoryWidget::ShowInventory()
{
	SetVisibility(ESlateVisibility::Visible);
	RefreshInventory();

	// Set input mode to UI
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
}

void UInventoryWidget::HideInventory()
{
	SetVisibility(ESlateVisibility::Hidden);

	// Restore input mode
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}
}

void UInventoryWidget::ToggleInventory()
{
	if (IsInventoryVisible())
	{
		HideInventory();
	}
	else
	{
		ShowInventory();
	}
}

bool UInventoryWidget::IsInventoryVisible() const
{
	return GetVisibility() == ESlateVisibility::Visible;
}

void UInventoryWidget::OnCloseClicked()
{
	HideInventory();
}

void UInventoryWidget::OnInventoryChanged(UItemDefinition* Item, int32 NewQuantity)
{
	// Auto-refresh when inventory changes
	if (IsInventoryVisible())
	{
		RefreshInventory();
	}
}

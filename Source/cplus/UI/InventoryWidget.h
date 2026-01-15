#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
class UVerticalBox;
class UTextBlock;
class UButton;
struct FInventoryItem;

/**
 * Basic Inventory Widget
 * 
 * Simple list-based inventory display.
 * Blueprint binds: ItemListContainer, TitleText, CloseButton
 */
UCLASS()
class CPLUS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Initialize with inventory component
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory UI")
	void SetInventoryComponent(UInventoryComponent* InInventory);

	/**
	 * Refresh the item list display
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory UI")
	void RefreshInventory();

	/**
	 * Show the inventory
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory UI")
	void ShowInventory();

	/**
	 * Hide the inventory
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory UI")
	void HideInventory();

	/**
	 * Toggle visibility
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory UI")
	void ToggleInventory();

	/**
	 * Is inventory visible?
	 */
	UFUNCTION(BlueprintPure, Category = "Inventory UI")
	bool IsInventoryVisible() const;

protected:
	virtual void NativeConstruct() override;

	/** Inventory component reference */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory UI")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	// ===== WIDGET BINDINGS =====

	/** Container for item entries */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> ItemListContainer;

	/** Title text */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	/** Close button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	/** Weight display (optional) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> WeightText;

private:
	UFUNCTION()
	void OnCloseClicked();

	UFUNCTION()
	void OnInventoryChanged(UItemDefinition* Item, int32 NewQuantity);

	/** Create a text entry for an item */
	void AddItemEntry(const FInventoryItem& Item);

	/** Clear all item entries */
	void ClearItemEntries();
};

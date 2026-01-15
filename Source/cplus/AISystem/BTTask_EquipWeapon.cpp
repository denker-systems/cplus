#include "AISystem/BTTask_EquipWeapon.h"
#include "AISystem/BaseAIController.h"
#include "Core/BaseAICharacter.h"
#include "WeaponSystem/Components/WeaponComponent.h"
#include "WeaponSystem/Actors/BaseWeapon.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_EquipWeapon::UBTTask_EquipWeapon()
{
	NodeName = "Equip Weapon";
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_EquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> BTTask_EquipWeapon: No AIController"));
		return EBTNodeResult::Failed;
	}

	ABaseAICharacter* AICharacter = AIController->GetAICharacter();
	if (!AICharacter)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> BTTask_EquipWeapon: No AICharacter"));
		return EBTNodeResult::Failed;
	}

	UWeaponComponent* WeaponComp = AICharacter->GetWeaponComponent();
	if (!WeaponComp)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> BTTask_EquipWeapon: No WeaponComponent"));
		return EBTNodeResult::Failed;
	}

	// Check if already has weapon
	if (bOnlyIfUnarmed && WeaponComp->GetCurrentWeapon())
	{
		UE_LOG(LogTemp, Display, TEXT(">>> BTTask_EquipWeapon: [%s] already has weapon"), *AICharacter->GetName());
		return EBTNodeResult::Succeeded;
	}

	// Check if weapon class is set
	if (!WeaponClass)
	{
		UE_LOG(LogTemp, Error, TEXT(">>> BTTask_EquipWeapon: No WeaponClass set"));
		return EBTNodeResult::Failed;
	}

	// Add weapon class
	AICharacter->AddWeaponClass(WeaponClass);
	UE_LOG(LogTemp, Display, TEXT(">>> BTTask_EquipWeapon: [%s] equipped %s"), 
		*AICharacter->GetName(), 
		*WeaponClass->GetName());

	return EBTNodeResult::Succeeded;
}

FString UBTTask_EquipWeapon::GetStaticDescription() const
{
	return FString::Printf(TEXT("Equip: %s%s"), 
		WeaponClass ? *WeaponClass->GetName() : TEXT("None"),
		bOnlyIfUnarmed ? TEXT(" (if unarmed)") : TEXT(""));
}

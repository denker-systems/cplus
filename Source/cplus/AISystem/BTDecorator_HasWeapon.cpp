#include "AISystem/BTDecorator_HasWeapon.h"
#include "AISystem/BaseAIController.h"
#include "Core/BaseAICharacter.h"
#include "WeaponSystem/Components/WeaponComponent.h"

UBTDecorator_HasWeapon::UBTDecorator_HasWeapon()
{
	NodeName = "Has Weapon";
}

bool UBTDecorator_HasWeapon::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return false;
	}

	ABaseAICharacter* AICharacter = AIController->GetAICharacter();
	if (!AICharacter)
	{
		return false;
	}

	UWeaponComponent* WeaponComp = AICharacter->GetWeaponComponent();
	if (!WeaponComp)
	{
		return false;
	}

	bool bHasWeapon = WeaponComp->GetCurrentWeapon() != nullptr;

	return bInvertCondition ? !bHasWeapon : bHasWeapon;
}

FString UBTDecorator_HasWeapon::GetStaticDescription() const
{
	return bInvertCondition ? TEXT("Has NO weapon") : TEXT("Has weapon");
}

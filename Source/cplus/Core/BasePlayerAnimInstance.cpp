#include "BasePlayerAnimInstance.h"
#include "BasePlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponSystem/Components/WeaponComponent.h"

void UBasePlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<ABasePlayerCharacter>(TryGetPawnOwner());
}

void UBasePlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<ABasePlayerCharacter>(TryGetPawnOwner());
		if (!PlayerCharacter)
		{
			return;
		}
	}

	UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement();
	if (!MovementComponent)
	{
		return;
	}

	FVector Velocity = MovementComponent->Velocity;
	Speed = Velocity.Size2D();
	
	if (Speed > 0.1f)
	{
		FRotator VelocityRotation = Velocity.Rotation();
		FRotator ActorRotation = PlayerCharacter->GetActorRotation();
		float DeltaYaw = VelocityRotation.Yaw - ActorRotation.Yaw;
		Direction = FMath::UnwindDegrees(DeltaYaw);
	}
	else
	{
		Direction = 0.0f;
	}

	bIsInAir = MovementComponent->IsFalling();
	bIsDead = PlayerCharacter->IsDead();

	// Update weapon states
	if (PlayerCharacter->GetWeaponComponent() && PlayerCharacter->GetWeaponComponent()->GetCurrentWeapon())
	{
		bIsArmed = true;
		// TODO: Detect weapon type from WeaponDefinition when implemented
		bIsPistol = true;
		bIsRifle = false;
	}
	else
	{
		bIsArmed = false;
		bIsPistol = false;
		bIsRifle = false;
	}
}

// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.


#include "HaunticaAnimInstance.h"

#include "Hauntica/Player/HaunticaPlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

void UHaunticaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	PlayerCharacter = Cast<AHaunticaPlayerCharacter>(GetOwningActor());
}

void UHaunticaAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	
	AccelerationValue = FVector::DotProduct(Acceleration, ForwardDirection);
}

void UHaunticaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!PlayerCharacter || !PlayerCharacter->GetCharacterMovement())
	{
		return;
	}
	
	bIsSprinting = PlayerCharacter->IsSprinting();
	
	Acceleration = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration();
	ForwardDirection = PlayerCharacter->GetCharacterMovement()->GetForwardVector();
}

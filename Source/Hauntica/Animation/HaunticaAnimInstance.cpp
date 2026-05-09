// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.


#include "HaunticaAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

void UHaunticaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (const AActor* const Owner = GetOwningActor())
	{
		CharacterMovementComponent = Owner->GetComponentByClass<UCharacterMovementComponent>();
	}
}

void UHaunticaAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	
	AccelerationValue = FVector::DotProduct(Acceleration, ForwardDirection);
}

void UHaunticaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (CharacterMovementComponent)
	{
		Acceleration = CharacterMovementComponent->GetCurrentAcceleration();
		ForwardDirection = CharacterMovementComponent->GetForwardVector();
	}
}

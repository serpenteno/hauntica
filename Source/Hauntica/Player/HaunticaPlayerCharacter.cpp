// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.


#include "HaunticaPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AHaunticaPlayerCharacter::AHaunticaPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->MaxWalkSpeed = MaxForwardWalkSpeed;
}

void AHaunticaPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* const EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AHaunticaPlayerCharacter::SetMaxWalkSpeed);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHaunticaPlayerCharacter::Move);
	
	EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AHaunticaPlayerCharacter::Turn);
}

void AHaunticaPlayerCharacter::Move(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();
	
	AddMovementInput(GetActorForwardVector(), FMath::Sign(Value.Y));
}

void AHaunticaPlayerCharacter::SetMaxWalkSpeed(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();
	
	if (Value.Y > 0.0f)
	{
		CurrentTankMovementDirection = EHaunticaTankMovementDirection::Forward;
	}
	else if (Value.Y < 0.0f)
	{
		CurrentTankMovementDirection = EHaunticaTankMovementDirection::Backward;
	}
	else
	{
		CurrentTankMovementDirection = EHaunticaTankMovementDirection::None;
	}

	switch (CurrentTankMovementDirection)
	{
		case EHaunticaTankMovementDirection::Forward:
			GetCharacterMovement()->MaxWalkSpeed = MaxForwardWalkSpeed;
			break;
		
		case EHaunticaTankMovementDirection::Backward:
			GetCharacterMovement()->MaxWalkSpeed = MaxBackwardWalkSpeed;
			break;
		
		default:
			GetCharacterMovement()->MaxWalkSpeed = MaxForwardWalkSpeed;
			break;
	}
}

void AHaunticaPlayerCharacter::Turn(const FInputActionValue& InputValue)
{
	const float Value = InputValue.Get<float>();
	
	const float Degrees = FMath::Sign(Value) * TurnRate * GetWorld()->GetDeltaSeconds();
	
	AddActorLocalRotation(FRotator(0.0f, Degrees, 0.0f));
}

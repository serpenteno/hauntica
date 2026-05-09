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
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AHaunticaPlayerCharacter::StartMoving);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHaunticaPlayerCharacter::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AHaunticaPlayerCharacter::StopMoving);
	
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHaunticaPlayerCharacter::StartSprinting);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHaunticaPlayerCharacter::StopSprinting);
	
	EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AHaunticaPlayerCharacter::Turn);
}

void AHaunticaPlayerCharacter::StartMoving(const FInputActionValue& InputValue)
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
	
	UpdateMaxWalkSpeed();
}

void AHaunticaPlayerCharacter::Move(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();
	
	AddMovementInput(GetActorForwardVector(), FMath::Sign(Value.Y));
}

void AHaunticaPlayerCharacter::StopMoving()
{
	CurrentTankMovementDirection = EHaunticaTankMovementDirection::None;
	UpdateMaxWalkSpeed();
}

void AHaunticaPlayerCharacter::StartSprinting()
{
	bWantsToSprint = true;
	UpdateMaxWalkSpeed();
}

void AHaunticaPlayerCharacter::StopSprinting()
{
	bWantsToSprint = false;
	UpdateMaxWalkSpeed();
}

void AHaunticaPlayerCharacter::Turn(const FInputActionValue& InputValue)
{
	const float Value = InputValue.Get<float>();
	
	const float Degrees = FMath::Sign(Value) * TurnRate * GetWorld()->GetDeltaSeconds();
	
	AddActorLocalRotation(FRotator(0.0f, Degrees, 0.0f));
}

void AHaunticaPlayerCharacter::UpdateMaxWalkSpeed() const
{
	switch (CurrentTankMovementDirection)
	{
		case EHaunticaTankMovementDirection::Forward:
			GetCharacterMovement()->MaxWalkSpeed = bWantsToSprint ? MaxSprintSpeed : MaxForwardWalkSpeed;
			break;
		
		case EHaunticaTankMovementDirection::Backward:
			GetCharacterMovement()->MaxWalkSpeed = MaxBackwardWalkSpeed;
			break;
		
		default:
			GetCharacterMovement()->MaxWalkSpeed = MaxForwardWalkSpeed;
			break;
	}
}

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
		CurrentPlayerState = bWantsToSprint ? EHaunticaPlayerState::Sprinting : EHaunticaPlayerState::Walking;
	}
	else if (Value.Y < 0.0f)
	{
		CurrentPlayerState = EHaunticaPlayerState::WalkingBackward;
	}
	else
	{
		CurrentPlayerState = EHaunticaPlayerState::Idle;
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
	CurrentPlayerState = EHaunticaPlayerState::Idle;
	UpdateMaxWalkSpeed();
}

void AHaunticaPlayerCharacter::StartSprinting()
{
	bWantsToSprint = true;
	
	if (CurrentPlayerState == EHaunticaPlayerState::Walking)
	{
		CurrentPlayerState = EHaunticaPlayerState::Sprinting;
		UpdateMaxWalkSpeed();
	}
}

void AHaunticaPlayerCharacter::StopSprinting()
{
	bWantsToSprint = false;

	if (CurrentPlayerState == EHaunticaPlayerState::Sprinting)
	{
		CurrentPlayerState = EHaunticaPlayerState::Walking;
		UpdateMaxWalkSpeed();	
	}
}

void AHaunticaPlayerCharacter::Turn(const FInputActionValue& InputValue)
{
	const float Value = InputValue.Get<float>();
	
	const float Degrees = FMath::Sign(Value) * TurnRate * GetWorld()->GetDeltaSeconds();
	
	AddActorLocalRotation(FRotator(0.0f, Degrees, 0.0f));
}

void AHaunticaPlayerCharacter::UpdateMaxWalkSpeed() const
{
	switch (CurrentPlayerState)
	{
		case EHaunticaPlayerState::Walking:
			GetCharacterMovement()->MaxWalkSpeed = MaxForwardWalkSpeed;
			break;
		
		case EHaunticaPlayerState::WalkingBackward:
			GetCharacterMovement()->MaxWalkSpeed = MaxBackwardWalkSpeed;
			break;
		
		case EHaunticaPlayerState::Sprinting:
			GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
			break;
		
		default:
			GetCharacterMovement()->MaxWalkSpeed = MaxForwardWalkSpeed;
			break;
	}
}

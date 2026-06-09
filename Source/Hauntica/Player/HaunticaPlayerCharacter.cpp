// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.


#include "HaunticaPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AHaunticaPlayerCharacter::AHaunticaPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->MaxWalkSpeed = MaxForwardWalkSpeed;
}

void AHaunticaPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (CurrentPlayerState != EHaunticaPlayerState::QuickTurning)
	{
		SetActorTickEnabled(false);
		return;
	}
	
	const FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), QuickTurnTargetRotation, DeltaSeconds, 180.0f / QuickTurnDuration);
	
	if (FMath::IsNearlyEqual(NewRotation.Yaw, QuickTurnTargetRotation.Yaw, QuickTurnTargetRotationErrorTolerance))
	{
		SetActorRotation(QuickTurnTargetRotation);
		StopQuickTurn();
		SetActorTickEnabled(false);
	}
	else
	{
		SetActorRotation(NewRotation);
	}
}

void AHaunticaPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* const EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHaunticaPlayerCharacter::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AHaunticaPlayerCharacter::StopMoving);
	
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHaunticaPlayerCharacter::StartSprinting);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHaunticaPlayerCharacter::StopSprinting);
	
	EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AHaunticaPlayerCharacter::Turn);
	
	EnhancedInputComponent->BindAction(QuickTurnAction, ETriggerEvent::Triggered, this, &AHaunticaPlayerCharacter::StartQuickTurn);
}

void AHaunticaPlayerCharacter::Move(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();
	
	if (Value.Y > 0.0f)
	{
		DesiredPlayerState = bWantsToSprint ? EHaunticaPlayerState::Sprinting : EHaunticaPlayerState::Walking;
	}
	else if (Value.Y < 0.0f)
	{
		DesiredPlayerState = EHaunticaPlayerState::WalkingBackward;
	}
	else
	{
		DesiredPlayerState = EHaunticaPlayerState::Idle;
	}
	
	if (!CanMove())
	{
		return;
	}
	
	if (CurrentPlayerState != DesiredPlayerState)
	{
		ApplyDesiredPlayerState();
	}
	
	AddMovementInput(GetActorForwardVector(), FMath::Sign(Value.Y));
}

void AHaunticaPlayerCharacter::StopMoving()
{
	DesiredPlayerState = EHaunticaPlayerState::Idle;
	
	if (!CanMove())
	{
		return;
	}
	
	ApplyDesiredPlayerState();
}

void AHaunticaPlayerCharacter::StartSprinting()
{
	bWantsToSprint = true;
	
	if (CurrentPlayerState == EHaunticaPlayerState::WalkingBackward)
	{
		StartQuickTurn();
		return;
	}
	
	if (DesiredPlayerState == EHaunticaPlayerState::Walking)
	{
		DesiredPlayerState = EHaunticaPlayerState::Sprinting;
		
		if (!CanMove())
		{
			return;
		}
		
		ApplyDesiredPlayerState();
	}
}

void AHaunticaPlayerCharacter::StopSprinting()
{
	bWantsToSprint = false;

	if (DesiredPlayerState == EHaunticaPlayerState::Sprinting)
	{
		DesiredPlayerState = EHaunticaPlayerState::Walking;
		
		if (!CanMove())
		{
			return;
		}
		
		ApplyDesiredPlayerState();
	}
}

void AHaunticaPlayerCharacter::Turn(const FInputActionValue& InputValue)
{
	if (!CanMove())
	{
		return;
	}
	
	const float Value = InputValue.Get<float>();
	
	const float Degrees = FMath::Sign(Value) * TurnRate * GetWorld()->GetDeltaSeconds();
	
	AddActorLocalRotation(FRotator(0.0f, Degrees, 0.0f));
}

void AHaunticaPlayerCharacter::StartQuickTurn()
{
	if (!CanQuickTurn())
	{
		return;
	}
	
	CurrentPlayerState = EHaunticaPlayerState::QuickTurning;
	
	QuickTurnTargetRotation = GetActorRotation();
	QuickTurnTargetRotation.Yaw += 180.0f;
	QuickTurnTargetRotation.Normalize();
	
	SetActorTickEnabled(true);
}

void AHaunticaPlayerCharacter::StopQuickTurn()
{
	ApplyDesiredPlayerState();
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

void AHaunticaPlayerCharacter::ApplyDesiredPlayerState()
{
	CurrentPlayerState = DesiredPlayerState;
	UpdateMaxWalkSpeed();
}

bool AHaunticaPlayerCharacter::CanMove() const
{
	return CurrentPlayerState != EHaunticaPlayerState::QuickTurning;
}

bool AHaunticaPlayerCharacter::CanQuickTurn() const
{
	return CurrentPlayerState != EHaunticaPlayerState::QuickTurning;
}

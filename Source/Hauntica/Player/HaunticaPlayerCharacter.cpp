// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.


#include "HaunticaPlayerCharacter.h"

#include "HaunticaPlayerData.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AHaunticaPlayerCharacter::AHaunticaPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	bUseControllerRotationYaw = false;
}

void AHaunticaPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
#ifdef WITH_EDITOR
	if (!PlayerData)
	{
		const FString Message = FString::Printf(TEXT("Player Data is not set in %s.\nGo to Class Defaults and assign it."), *GetName());
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Message));
	}
#endif
	
	checkf(PlayerData, TEXT("PlayerData is not set in %s!"), *GetName());
	
	if (UCharacterMovementComponent* const MovementComponent = GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = PlayerData->MaxForwardWalkSpeed;
	}
}

void AHaunticaPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (CurrentPlayerState != EHaunticaPlayerState::QuickTurning)
	{
		SetActorTickEnabled(false);
		return;
	}
	
	const FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), QuickTurnTargetRotation, DeltaSeconds, 180.0f / PlayerData->QuickTurnDuration);
	
	if (FMath::IsNearlyEqual(NewRotation.Yaw, QuickTurnTargetRotation.Yaw, PlayerData->QuickTurnTargetRotationErrorTolerance))
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
	
	if (MoveAction)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHaunticaPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AHaunticaPlayerCharacter::StopMoving);
	}
	
	if (SprintAction)
	{
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHaunticaPlayerCharacter::StartSprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHaunticaPlayerCharacter::StopSprinting);
	}
	
	if (TurnAction)
	{
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AHaunticaPlayerCharacter::Turn);
	}
	
	if (QuickTurnAction)
	{
		EnhancedInputComponent->BindAction(QuickTurnAction, ETriggerEvent::Triggered, this, &AHaunticaPlayerCharacter::StartQuickTurn);
	}
}

void AHaunticaPlayerCharacter::Move(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();
	
	DesiredPlayerState = CalculatePlayerStateFromInput(Value.Y);
	
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
	
	if (!GetWorld())
	{
		return;
	}
	
	const float Degrees = FMath::Sign(Value) * PlayerData->TurnRate * GetWorld()->GetDeltaSeconds();
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
	UCharacterMovementComponent* const MovementComponent = GetCharacterMovement();
	
	if (!MovementComponent)
	{
		return;
	}
	
	switch (CurrentPlayerState)
	{
		case EHaunticaPlayerState::Walking:
			MovementComponent->MaxWalkSpeed = PlayerData->MaxForwardWalkSpeed;
			break;
		
		case EHaunticaPlayerState::WalkingBackward:
			MovementComponent->MaxWalkSpeed = PlayerData->MaxBackwardWalkSpeed;
			break;
		
		case EHaunticaPlayerState::Sprinting:
			MovementComponent->MaxWalkSpeed = PlayerData->MaxSprintSpeed;
			break;
		
		default:
			MovementComponent->MaxWalkSpeed = PlayerData->MaxForwardWalkSpeed;
			break;
	}
}

void AHaunticaPlayerCharacter::ApplyDesiredPlayerState()
{
	CurrentPlayerState = DesiredPlayerState;
	UpdateMaxWalkSpeed();
}

EHaunticaPlayerState AHaunticaPlayerCharacter::CalculatePlayerStateFromInput(const float InputY) const
{
	if (InputY > 0.0f)
	{
		return bWantsToSprint ? EHaunticaPlayerState::Sprinting : EHaunticaPlayerState::Walking;
	}
	
	if (InputY < 0.0f)
	{
		return EHaunticaPlayerState::WalkingBackward;
	}
	
	return EHaunticaPlayerState::Idle;
}

bool AHaunticaPlayerCharacter::CanMove() const
{
	return CurrentPlayerState != EHaunticaPlayerState::QuickTurning;
}

bool AHaunticaPlayerCharacter::CanQuickTurn() const
{
	return CurrentPlayerState != EHaunticaPlayerState::QuickTurning;
}

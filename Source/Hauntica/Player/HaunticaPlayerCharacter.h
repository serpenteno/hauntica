// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HaunticaPlayerCharacter.generated.h"

struct FInputActionValue;

class UInputAction;

UENUM()
enum class EHaunticaPlayerState
{
	Idle UMETA(DisplayName="Idle"),
	Walking UMETA(DisplayName="Walking"),
	WalkingBackward UMETA(DisplayName="Walking Backward"),
	Sprinting UMETA(DisplayName="Sprinting"),
	QuickTurning UMETA(DisplayName="Quick-Turning")
};

UCLASS()
class HAUNTICA_API AHaunticaPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHaunticaPlayerCharacter();

	//~ Begin AActor Interface
	virtual void Tick(float DeltaSeconds) override;
	//~ End AActor Interface
	
	//~ Begin APawn Interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	//~ End APawn Interface
	
	bool IsSprinting() const
	{
		return CurrentPlayerState == EHaunticaPlayerState::Sprinting;
	}
	
private:
	void Move(const FInputActionValue& InputValue);
	void StopMoving();
	void StartSprinting();
	void StopSprinting();
	void Turn(const FInputActionValue& InputValue);
	void StartQuickTurn();
	void StopQuickTurn();
	
	void UpdateMaxWalkSpeed() const;
	void ApplyDesiredPlayerState();
	
	bool CanMove() const;
	bool CanQuickTurn() const;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> TurnAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> SprintAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> QuickTurnAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement|Tank Controls", meta=(ForceUnits="cm/s"))
	float MaxForwardWalkSpeed = 160.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement|Tank Controls", meta=(ForceUnits="cm/s"))
	float MaxBackwardWalkSpeed = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ForceUnits="cm/s"))
	float MaxSprintSpeed = 300.0f;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Movement")
	bool bWantsToSprint = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement|Tank Controls", meta=(ForceUnits="deg/s"))
	float TurnRate = 200.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ForceUnits="s", ClampMin="0.0"))
	float QuickTurnDuration = 1.0f;
	
	UPROPERTY(Transient)
	FRotator QuickTurnTargetRotation;
	
	/** Instantly snap to @code QuickTurnTargetRotation@endcode if the difference between the current rotation and the target rotation is less than or equal to this value */
	UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ForceUnits="deg", ClampMin="0.0", ClampMax="180.0"))
	float QuickTurnTargetRotationErrorTolerance = 1.0f;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Movement")
	EHaunticaPlayerState CurrentPlayerState = EHaunticaPlayerState::Idle;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Movement")
	EHaunticaPlayerState DesiredPlayerState = EHaunticaPlayerState::Idle;
};

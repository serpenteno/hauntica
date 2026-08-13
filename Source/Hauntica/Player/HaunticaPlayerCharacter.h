// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HaunticaPlayerCharacter.generated.h"

struct FInputActionValue;

class UHaunticaInteractionComponent;
class UHaunticaPlayerData;
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
	virtual void BeginPlay() override;
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
	void Interact();
	
	void UpdateMaxWalkSpeed() const;
	void ApplyDesiredPlayerState();
	EHaunticaPlayerState CalculatePlayerStateFromInput(const float InputY) const;
	
	bool CanMove() const;
	bool CanQuickTurn() const;
	bool CanInteract() const;
	
	UPROPERTY(EditDefaultsOnly, Category="Config")
	TObjectPtr<UHaunticaPlayerData> PlayerData;

	UPROPERTY(EditDefaultsOnly, Category="Config|Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Config|Input")
	TObjectPtr<UInputAction> SprintAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Config|Input")
	TObjectPtr<UInputAction> TurnAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Config|Input")
	TObjectPtr<UInputAction> QuickTurnAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Config|Input")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Movement")
	bool bWantsToSprint = false;
	
	UPROPERTY(Transient)
	FRotator QuickTurnTargetRotation;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Movement")
	EHaunticaPlayerState CurrentPlayerState = EHaunticaPlayerState::Idle;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Movement")
	EHaunticaPlayerState DesiredPlayerState = EHaunticaPlayerState::Idle;
	
	UPROPERTY(VisibleAnywhere, Category="Interaction")
	TObjectPtr<UHaunticaInteractionComponent> InteractionComponent;
};

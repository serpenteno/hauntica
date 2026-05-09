// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HaunticaPlayerCharacter.generated.h"

struct FInputActionValue;

class UInputAction;

UENUM()
enum class EHaunticaTankMovementDirection
{
	None UMETA(DisplayName="None"),
	Forward UMETA(DisplayName="Forward"),
	Backward UMETA(DisplayName="Backward")
};

UCLASS()
class HAUNTICA_API AHaunticaPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHaunticaPlayerCharacter();

	//~ Begin APawn Interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	//~ End APawn Interface
	
private:
	void StartMoving(const FInputActionValue& InputValue);
	void Move(const FInputActionValue& InputValue);
	void StopMoving();
	void Turn(const FInputActionValue& InputValue);
	
	void UpdateMaxWalkSpeed() const;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> TurnAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement|Tank Controls", meta=(ForceUnits="cm/s"))
	float MaxForwardWalkSpeed = 160.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement|Tank Controls", meta=(ForceUnits="cm/s"))
	float MaxBackwardWalkSpeed = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement|Tank Controls", meta=(ForceUnits="deg/s"))
	float TurnRate = 200.0f;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Movement|Tank Controls")
	EHaunticaTankMovementDirection CurrentTankMovementDirection = EHaunticaTankMovementDirection::None;
};

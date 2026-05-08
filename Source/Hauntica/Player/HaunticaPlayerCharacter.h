// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HaunticaPlayerCharacter.generated.h"

struct FInputActionValue;

class UInputAction;

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
	void Move(const FInputActionValue& InputValue);
	void SetMaxWalkSpeed(const FInputActionValue& InputValue);
	void Turn(const FInputActionValue& InputValue);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> TurnAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Tank Controls", meta=(AllowPrivateAccess="true", ForceUnits="cm/s"))
	float MaxForwardWalkSpeed = 160.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Tank Controls", meta=(AllowPrivateAccess="true", ForceUnits="cm/s"))
	float MaxBackwardWalkSpeed = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Tank Controls", meta=(AllowPrivateAccess="true", ForceUnits="deg/s"))
	float TurnRate = 200.0f;
};

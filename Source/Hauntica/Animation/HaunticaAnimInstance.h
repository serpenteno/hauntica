// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HaunticaAnimInstance.generated.h"

class AHaunticaPlayerCharacter;
class UCharacterMovementComponent;

UCLASS()
class HAUNTICA_API UHaunticaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	//~ Begin UAnimInstance Interface
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ End UAnimInstance Interface
	
private:
	UPROPERTY(Transient, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	bool bIsSprinting;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	float AccelerationValue;
	
	UPROPERTY(Transient)
	FVector Acceleration;
	
	UPROPERTY(Transient)
	FVector ForwardDirection;
	
	UPROPERTY(Transient)
	TObjectPtr<AHaunticaPlayerCharacter> PlayerCharacter;
};

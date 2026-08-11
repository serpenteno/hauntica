// Copyright 2026 Kamil Fiącek, Wiktoria Boćko, Konrad Pietrzak (RozrywkaFan), Wiktor Kaźmierczak (hmbtoro), Antonina Gudyma (antonya). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HaunticaPlayerData.generated.h"

class UInputAction;

UCLASS()
class HAUNTICA_API UHaunticaPlayerData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ForceUnits="cm/s", ClampMin="0.0"))
	float MaxForwardWalkSpeed = 160.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ForceUnits="cm/s", ClampMin="0.0"))
	float MaxBackwardWalkSpeed = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ForceUnits="cm/s", ClampMin="0.0"))
	float MaxSprintSpeed = 300.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ForceUnits="deg/s", ClampMin="0.0"))
	float TurnRate = 200.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ForceUnits="s", ClampMin="0.0"))
	float QuickTurnDuration = 0.5f;
	
	/** Instantly snap to @code QuickTurnTargetRotation@endcode if the difference between the current rotation and the target rotation is less than or equal to this value */
	UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(ForceUnits="deg", ClampMin="0.0", ClampMax="180.0"))
	float QuickTurnTargetRotationErrorTolerance = 1.0f;
};

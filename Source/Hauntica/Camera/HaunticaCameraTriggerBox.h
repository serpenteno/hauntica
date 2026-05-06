// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "HaunticaCameraTriggerBox.generated.h"

class UArrowComponent;

UCLASS()
class HAUNTICA_API AHaunticaCameraTriggerBox final : public ATriggerBox
{
	GENERATED_BODY()

public:
	AHaunticaCameraTriggerBox();
	
private:
	UFUNCTION()
	void OnPlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnPlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Camera to set the view target to when the player walks through the trigger box in its forward direction */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	TObjectPtr<ACameraActor> ForwardCameraActor;
	
	/** Camera to set the view target to when the player walks through the trigger box in its backward direction */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	TObjectPtr<ACameraActor> BackwardCameraActor;
	
	/** Indicates the forward direction of the trigger box */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> ForwardArrow;
	
	/** Indicates the backward direction of the trigger box */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> BackwardArrow;
};

// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "HaunticaCameraTriggerBox.generated.h"

class UArrowComponent;

UENUM()
enum class EHaunticaCameraTriggerSide : uint8
{
	/** If the player continues to walk in the same direction, ForwardCameraActor will be set as the view target */
	Forward UMETA(DisplayName="Forward"),
	/** If the player continues to walk in the same direction, BackwardCameraActor will be set as the view target */
	Backward UMETA(DisplayName="Backward")
};

UCLASS()
class HAUNTICA_API AHaunticaCameraTriggerBox final : public ATriggerBox
{
	GENERATED_BODY()

public:
	AHaunticaCameraTriggerBox();
	
	//~ Begin AActor Interface
	virtual void Tick(float DeltaSeconds) override;
	//~ End AActor Interface
	
protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface
	
private:
	UFUNCTION()
	void OnPlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnPlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintGetter)
	EHaunticaCameraTriggerSide GetTriggerRelativePlayerSide() const;
	
	bool IsPlayerPawn(const AActor* const OtherActor) const;

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
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Player")
	EHaunticaCameraTriggerSide LastPlayerSide;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Player")
	TObjectPtr<APawn> PlayerPawn;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Player")
	TObjectPtr<APlayerController> PlayerController;
};

// Copyright 2026 Kamil Fiącek, Wiktoria Boćko, Konrad Pietrzak (RozrywkaFan), Wiktor Kaźmierczak (hmbtoro), Antonina Gudyma (antonya). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "HaunticaWorldSettings.generated.h"

UCLASS()
class HAUNTICA_API AHaunticaWorldSettings final : public AWorldSettings
{
	GENERATED_BODY()
	
public:
	ACameraActor* GetDefaultCameraActor() const
	{
		return DefaultCameraActor;
	}
	
protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface
	
private:
	UPROPERTY(EditInstanceOnly, Category="Camera")
	TObjectPtr<ACameraActor> DefaultCameraActor;
};

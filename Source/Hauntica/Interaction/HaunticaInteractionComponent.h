// Copyright 2026 Kamil Fiącek, Wiktoria Boćko, Konrad Pietrzak (RozrywkaFan), Wiktor Kaźmierczak (hmbtoro), Antonina Gudyma (antonya). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HaunticaInteractionComponent.generated.h"

class AHaunticaPlayerCharacter;

UCLASS(ClassGroup=("Hauntica"), meta=(BlueprintSpawnableComponent))
class HAUNTICA_API UHaunticaInteractionComponent final : public UActorComponent
{
	GENERATED_BODY()

public:
	UHaunticaInteractionComponent();
	
	bool TryInteract(AHaunticaPlayerCharacter* const Instigator) const;
	
	void AddInteractableActor(AActor* const Actor);
	void RemoveInteractableActor(AActor* const Actor);
	
private:
	AActor* FindBestInteractableActor() const;
	
	UPROPERTY(Transient, VisibleInstanceOnly)
	TArray<TObjectPtr<AActor>> InteractableActors;
};

// Copyright 2026 Kamil Fiącek, Wiktoria Boćko, Konrad Pietrzak (RozrywkaFan), Wiktor Kaźmierczak (hmbtoro), Antonina Gudyma (antonya). All Rights Reserved.


#include "HaunticaInteractionComponent.h"

#include "HaunticaInteractable.h"

UHaunticaInteractionComponent::UHaunticaInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UHaunticaInteractionComponent::TryInteract(AActor* const Instigator) const
{
	UE_LOG(LogTemp, Display, TEXT("%s: Trying to interact..."), *Instigator->GetName())
	
	AActor* const Actor = FindBestInteractableActor();
	
	if (!Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to interact"), *Instigator->GetName())
		return false;
	}
	
	// All actors stored in InteractableActors, including cast-from Actor, must implement IHaunticaInteractable interface. We shouldn't continue otherwise.
	IHaunticaInteractable* InteractableActor = CastChecked<IHaunticaInteractable>(Actor);
	
	InteractableActor->Interact(Instigator);
	return true;
}

void UHaunticaInteractionComponent::AddInteractableActor(AActor* const Actor)
{
	check(Actor);
	InteractableActors.AddUnique(Actor);
}

void UHaunticaInteractionComponent::RemoveInteractableActor(AActor* const Actor)
{
	check(Actor);
	// Stop at first instance of Actor found as InteractableActors contains unique actors only.
	InteractableActors.RemoveSingle(Actor);
}

AActor* UHaunticaInteractionComponent::FindBestInteractableActor() const
{
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	
	float BestDistance = FLT_MAX;
	AActor* BestActor = nullptr;
	
	for (AActor* const Actor : InteractableActors)
	{
		const float Distance = FVector::DistSquared2D(OwnerLocation, Actor->GetActorLocation());
		
		if (Distance < BestDistance)
		{
			BestDistance = Distance;
			BestActor = Actor;
		}
	}
	
	return BestActor;
}

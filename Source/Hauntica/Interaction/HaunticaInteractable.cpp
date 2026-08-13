// Copyright 2026 Kamil Fiącek, Wiktoria Boćko, Konrad Pietrzak (RozrywkaFan), Wiktor Kaźmierczak (hmbtoro), Antonina Gudyma (antonya). All Rights Reserved.


#include "HaunticaInteractable.h"

void IHaunticaInteractable::Interact(AActor* const Interactor)
{
	UE_LOG(LogTemp, Display, TEXT("%s: Interacting with %s"), *Interactor->GetName(), *_getUObject()->GetName())
}

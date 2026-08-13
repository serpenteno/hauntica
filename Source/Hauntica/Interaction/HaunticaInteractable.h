// Copyright 2026 Kamil Fiącek, Wiktoria Boćko, Konrad Pietrzak (RozrywkaFan), Wiktor Kaźmierczak (hmbtoro), Antonina Gudyma (antonya). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HaunticaInteractable.generated.h"

UINTERFACE()
class UHaunticaInteractable : public UInterface
{
	GENERATED_BODY()
};

class HAUNTICA_API IHaunticaInteractable
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* const Interactor);
};

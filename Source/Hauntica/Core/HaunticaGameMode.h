// Copyright 2026 Kamil Fiącek, Wiktoria Boćko, Konrad Pietrzak (RozrywkaFan), Wiktor Kaźmierczak (hmbtoro). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HaunticaGameMode.generated.h"

UCLASS()
class HAUNTICA_API AHaunticaGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHaunticaGameMode();

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface
};

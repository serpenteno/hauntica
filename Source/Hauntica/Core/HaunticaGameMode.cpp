// Copyright 2026 Kamil Fiącek, Wiktoria Boćko, Konrad Pietrzak (RozrywkaFan), Wiktor Kaźmierczak (hmbtoro). All Rights Reserved.


#include "HaunticaGameMode.h"

#include "Hauntica/Player/HaunticaPlayerCharacter.h"

AHaunticaGameMode::AHaunticaGameMode()
{
	DefaultPawnClass = AHaunticaPlayerCharacter::StaticClass();
}

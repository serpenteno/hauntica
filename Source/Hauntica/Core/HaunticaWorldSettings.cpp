// Copyright 2026 Kamil Fiącek, Wiktoria Boćko, Konrad Pietrzak (RozrywkaFan), Wiktor Kaźmierczak (hmbtoro), Antonina Gudyma (antonya). All Rights Reserved.


#include "HaunticaWorldSettings.h"

void AHaunticaWorldSettings::BeginPlay()
{
	Super::BeginPlay();
	
	ensureAlwaysMsgf(DefaultCameraActor, TEXT("Default Camera Actor is not set in World Settings!"));
}

// Copyright 2026 Kamil Fiącek, Wiktoria Boćko, Konrad Pietrzak (RozrywkaFan), Wiktor Kaźmierczak (hmbtoro). All Rights Reserved.


#include "HaunticaGameMode.h"

#include "HaunticaWorldSettings.h"
#include "Hauntica/Player/HaunticaPlayerCharacter.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

AHaunticaGameMode::AHaunticaGameMode()
{
	DefaultPawnClass = AHaunticaPlayerCharacter::StaticClass();
}

void AHaunticaGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	const AHaunticaWorldSettings* const WorldSettings = Cast<AHaunticaWorldSettings>(GetWorldSettings());
	
	if (!WorldSettings)
	{
		return;
	}
	
	APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	
	if (!PlayerController)
	{
		return;
	}
	
	PlayerController->SetViewTarget(WorldSettings->GetDefaultCameraActor());
}

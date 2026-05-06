// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.


#include "HaunticaCameraTriggerBox.h"

#include "Components/ArrowComponent.h"
#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"


AHaunticaCameraTriggerBox::AHaunticaCameraTriggerBox()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardDirection"));
	ForwardArrow->SetupAttachment(GetRootComponent());
	ForwardArrow->SetArrowFColor(FColor(0, 255, 0));
	
	BackwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("BackwardDirection"));
	BackwardArrow->SetupAttachment(GetRootComponent());
	BackwardArrow->SetArrowFColor(FColor(255, 0, 0));
	BackwardArrow->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHaunticaCameraTriggerBox::OnPlayerEntered);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &AHaunticaCameraTriggerBox::OnPlayerExited);
}

void AHaunticaCameraTriggerBox::OnPlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != UGameplayStatics::GetPlayerPawn(this, 0))
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Player is entering %s"), *GetName())
}

void AHaunticaCameraTriggerBox::OnPlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != UGameplayStatics::GetPlayerPawn(this, 0))
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Player is exiting %s"), *GetName())
}

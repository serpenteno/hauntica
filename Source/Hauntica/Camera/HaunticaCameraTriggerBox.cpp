// Copyright 2026 Kamil Fiącek, Wiktoria Boćko. All Rights Reserved.


#include "HaunticaCameraTriggerBox.h"

#include "Camera/CameraActor.h"
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

void AHaunticaCameraTriggerBox::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!PlayerController)
	{
		return;
	}
	
	const EHaunticaCameraTriggerSide CurrentPlayerSide = GetTriggerRelativePlayerSide();
	
	if (CurrentPlayerSide == LastPlayerSide)
	{
		return;
	}
	
	ACameraActor* NewViewTarget = nullptr;
	
	switch (LastPlayerSide)
	{
	case EHaunticaCameraTriggerSide::Forward:
		NewViewTarget = ForwardCameraActor;
		break;
		
	case EHaunticaCameraTriggerSide::Backward:
		NewViewTarget = BackwardCameraActor;
		break;
		
	default:
		break;
	}
	
	if (NewViewTarget != PlayerController->GetViewTarget())
	{
		PlayerController->SetViewTarget(NewViewTarget);
	}
	
	LastPlayerSide = CurrentPlayerSide;
}

void AHaunticaCameraTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	
	check(PlayerController);
	PlayerPawn = PlayerController->GetPawn();
}

void AHaunticaCameraTriggerBox::OnPlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsPlayerPawn(OtherActor))
	{
		return;
	}
	
	LastPlayerSide = GetTriggerRelativePlayerSide();

	SetActorTickEnabled(true);
}

void AHaunticaCameraTriggerBox::OnPlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsPlayerPawn(OtherActor))
	{
		return;
	}
	
	SetActorTickEnabled(false);
}

EHaunticaCameraTriggerSide AHaunticaCameraTriggerBox::GetTriggerRelativePlayerSide() const
{
	const FVector PlayerRelativeLocation = GetActorLocation() - PlayerPawn->GetActorLocation();
	const float Dot = FVector::DotProduct(PlayerRelativeLocation, GetActorForwardVector());
	
	return Dot > 0.0f ? EHaunticaCameraTriggerSide::Forward : EHaunticaCameraTriggerSide::Backward;
}

bool AHaunticaCameraTriggerBox::IsPlayerPawn(const AActor* const OtherActor) const
{
	return PlayerPawn && OtherActor == PlayerPawn;
}

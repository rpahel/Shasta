// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/Pawns/ShastaPlayerPawn.h"
#include "Game/ShastaPlayerController.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Logging/StructuredLog.h"

AShastaPlayerPawn::AShastaPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>("Root Collider");
	if(Collider)
		SetRootComponent(Collider);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	if (Camera)
		Camera->SetupAttachment(Collider);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("Floating Pawn Movement");
}

void AShastaPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// Bind to controller events
	if (AShastaPlayerController* controller = Cast<AShastaPlayerController>(GetController()))
	{
		controller->OnMoveDelegate.AddUniqueDynamic(this, &AShastaPlayerPawn::MoveCallback);
		controller->OnRotateDelegate.AddUniqueDynamic(this, &AShastaPlayerPawn::RotateCallback);
		controller->OnFOVChangeDelegate.AddUniqueDynamic(this, &AShastaPlayerPawn::FOVChangeCallback);
	}

	// Adjust spawn altitude
	FVector loc = GetActorLocation();
	loc.Z = FMath::Clamp(loc.Z, MinMaxAltitude.X, MinMaxAltitude.Y);
	SetActorLocation(loc);
}

void AShastaPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementComponent)
	{
		FVector loc = GetActorLocation();
		if (loc.Z < MinMaxAltitude.X || loc.Z > MinMaxAltitude.Y)
		{
			MovementComponent->Velocity.Z = 0;
			loc.Z = FMath::Clamp(loc.Z, MinMaxAltitude.X, MinMaxAltitude.Y);
			SetActorLocation(loc);
		}
	}
}

void AShastaPlayerPawn::MoveCallback(const FVector& InputDirection)
{
	if(!MovementComponent)
		return;

	float altitude = GetActorLocation().Z;
	FVector worldDir = InputDirection.RotateAngleAxis(GetActorRotation().Yaw, FVector::UpVector);

	if ((worldDir.Z > 0 && altitude >= MinMaxAltitude.Y)
		|| (worldDir.Z < 0 && altitude <= MinMaxAltitude.X))
	{
		worldDir.Z = 0;
	}

	MovementComponent->AddInputVector(worldDir);
}

void AShastaPlayerPawn::RotateCallback(const FVector2D& InputDirection)
{
	FRotator rot = GetActorRotation();
	rot.Yaw += InputDirection.X;
	rot.Pitch = FMath::Clamp(rot.Pitch + InputDirection.Y, MinMaxPitch.X, MinMaxPitch.Y);
	SetActorRotation(rot);
}

void AShastaPlayerPawn::FOVChangeCallback(float InputFovDelta)
{
	if(!Camera)
		return;

	Camera->FieldOfView = FMath::Clamp(Camera->FieldOfView - InputFovDelta, MinMaxFov.X, MinMaxFov.Y);
}

// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/Pawns/ShastaPlayerPawn.h"
#include "ActorComponents/PlayerMovementComponent.h"
#include "Interfaces/InputsDependentInterface.h"

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

	PlayerMovement = CreateDefaultSubobject<UPlayerMovementComponent>("Player Movement Component");
}

void AShastaPlayerPawn::BindInputActions(UEnhancedInputComponent* InInputComponent)
{
	for (auto& comp : GetComponentsByInterface(UInputsDependentInterface::StaticClass()))
	{
		if (auto inputDepComp = Cast<IInputsDependentInterface>(comp))
		{
			inputDepComp->BindInputActions(InInputComponent);
		}
	}
}

void AShastaPlayerPawn::FOVChangeCallback(float InputFovDelta)
{
	if(!Camera)
		return;

	Camera->FieldOfView = FMath::Clamp(Camera->FieldOfView - InputFovDelta, MinMaxFov.X, MinMaxFov.Y);
}

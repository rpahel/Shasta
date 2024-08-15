// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#include "Actors/Pawns/ShastaPlayerPawn.h"
#include "ActorComponents/Movement/PlayerMovementComponent.h"
#include "ActorComponents/Camera/PlayerCameraComponent.h"
#include "Interfaces/InputsDependentInterface.h"

#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Logging/StructuredLog.h"

//====================================================================================
//==== PUBLIC CONSTRUCTORS
//====================================================================================

AShastaPlayerPawn::AShastaPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>("Root Collider");
	if(Collider)
		SetRootComponent(Collider);

	Camera = CreateDefaultSubobject<UPlayerCameraComponent>("Camera");
	if (Camera)
		Camera->SetupAttachment(Collider);

	PlayerMovement = CreateDefaultSubobject<UPlayerMovementComponent>("Player Movement Component");
}

//====================================================================================
//==== PUBLIC METHODS
//====================================================================================

void AShastaPlayerPawn::BindInputActions(UEnhancedInputComponent* InInputComponent)
{
	for (auto& comp : GetComponentsByInterface(UInputsDependentInterface::StaticClass()))
	{
		if (auto inputDepComp = Cast<IInputsDependentInterface>(comp))
			inputDepComp->BindInputActions(InInputComponent);
	}
}
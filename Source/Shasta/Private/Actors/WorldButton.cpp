// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/WorldButton.h"
#include "Components/BoxComponent.h"

AWorldButton::AWorldButton()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent((Collider = CreateDefaultSubobject<UBoxComponent>("Collider")));
}

void AWorldButton::Interact()
{
	OnSelected.Broadcast(ButtonType);
}

void AWorldButton::EndPlay(EEndPlayReason::Type Reason)
{
	OnSelected.Clear();
}

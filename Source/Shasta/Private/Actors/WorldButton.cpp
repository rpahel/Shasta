// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/WorldButton.h"
#include "Components/BoxComponent.h"

AWorldButton::AWorldButton()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent((Collider = CreateDefaultSubobject<UBoxComponent>("Collider")));
}

void AWorldButton::DoTheThing()
{
}
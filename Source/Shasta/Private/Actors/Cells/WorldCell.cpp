// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/Cells/WorldCell.h"
#include "Components/ShapeComponent.h"

//====================================================================================
//==== PUBLIC CONSTRUCTORS
//====================================================================================

AWorldCell::AWorldCell()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	DissolverShape = CreateDefaultSubobject<UStaticMeshComponent>("Dissolver Shape");
	if(DissolverShape)
		DissolverShape->SetupAttachment(RootComponent);
}

//====================================================================================
//==== PROTECTED METHODS
//====================================================================================

UStaticMeshComponent* AWorldCell::GetDissolverShape() const
{
	return DissolverShape;
}

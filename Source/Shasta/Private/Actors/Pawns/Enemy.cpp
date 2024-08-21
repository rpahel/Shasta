// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/Pawns/Enemy.h"
#include "Actors/Cells/WorldCell.h"
#include "Actors/Cells/CellModifier.h"
#include "ActorComponents/Movement/PathComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemy::TeleportOnPath(AWorldCell* WorldCell, UPathComponent* Path)
{
	if (!Path || !WorldCell)
		return;

	if(!WorldCell->GetCellModifier())
		return;

	const FVector pos = WorldCell->GetCellModifier()->GetTransform().TransformPositionNoScale(Path->GetSplinePointAt(0, ESplineCoordinateSpace::Local).Position);

	SetActorLocation(pos);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::ProgressOnPath(float DeltaTime)
{
}

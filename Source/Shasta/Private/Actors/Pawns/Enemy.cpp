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

	CurrentPath = Path;
	CurrentWorldCell = WorldCell;

	FSplinePoint point = Path->GetSplinePointAt(0, ESplineCoordinateSpace::Local);
	const FVector pos = WorldCell->GetCellModifier()->GetTransform().TransformPositionNoScale(point.Position);
	const FQuat rot = WorldCell->GetCellModifier()->GetTransform().TransformVectorNoScale(point.LeaveTangent).ToOrientationQuat();

	SetActorLocationAndRotation(pos, rot);

	bCanMove = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProgressOnPath(DeltaTime);
}

void AEnemy::ProgressOnPath(float DeltaTime)
{
	if(!CurrentPath || !bCanMove)
		return;

	if(CurrentPathProgress >= 1)
	{ 
		OnFinishedPath.Broadcast(this);
		bCanMove = false;

		Destroy();
		return;
	}

	CurrentPathProgress += DeltaTime * SpeedFactor;
	const FVector loc = CurrentPath->GetLocationAtTime(CurrentPathProgress, ESplineCoordinateSpace::World);
	const FQuat rot = CurrentPath->GetTangentAtTime(CurrentPathProgress, ESplineCoordinateSpace::World).ToOrientationQuat();
	SetActorLocationAndRotation(loc, rot);
}

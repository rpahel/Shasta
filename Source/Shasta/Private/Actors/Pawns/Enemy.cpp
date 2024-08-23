// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/Pawns/Enemy.h"
#include "Actors/Cells/WorldCell.h"
#include "Actors/Cells/CellModifier.h"
#include "ActorComponents/Movement/PathComponent.h"

#include "Components/CapsuleComponent.h"

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
	CurrentPathProgress = 0;
}

void AEnemy::Die(const FVector& PushDir)
{
	GetWorld()->GetTimerManager().ClearTimer(PathSearchTimerHandle);

	if (UCapsuleComponent* collider = GetComponentByClass<UCapsuleComponent>())
	{
		collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (USkeletalMeshComponent* skeleton = GetComponentByClass<USkeletalMeshComponent>())
	{
		skeleton->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		skeleton->SetSimulatePhysics(true);
		skeleton->AddImpulse(PushDir * 1000);
	}

	SetLifeSpan(10);
	bCanMove = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::EndPlay(EEndPlayReason::Type EndReason)
{
	Super::EndPlay(EndReason);

	OnMovingDelegate.Clear();
	OnStoppedDelegate.Clear();
	OnArrivedAtCenterDelegate.Clear();
	GetWorld()->GetTimerManager().ClearTimer(PathSearchTimerHandle);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProgressOnPath(DeltaTime);

	if (!bCanMove)
	{
		OnStoppedDelegate.Broadcast();

		IdleTimer += DeltaTime;
		if(IdleTimer >= 30)
			Destroy();
	}
}

void AEnemy::ProgressOnPath(float DeltaTime)
{
	if(!CurrentPath || !bCanMove)
		return;

	if(CurrentPathProgress >= 1)
	{ 
		bCanMove = false;

		const FVector dir = (GetActorLocation() - CurrentWorldCell->GetActorLocation()).GetSafeNormal();
		AWorldCell* nextCell = CurrentWorldCell->GetCellInDirection(dir);

		if (nextCell->GetCellType() == ECellType::Center)
		{
			OnArrivedAtCenterDelegate.Broadcast(this);
			SetActorLocation(nextCell->GetActorLocation() - dir * 250);
			SetActorRotation(FVector::VectorPlaneProject(dir, FVector::UpVector).ToOrientationQuat());
			return;
		}

		TArray<UPathComponent*> paths = nextCell->GetValidPaths(GetActorLocation(), EShastaPathType::Ground, true);

		// Si on trouve pas de paths dans la cell, mettre un timer et check a la fin du timer.
		if (paths.IsEmpty())
		{
			GetWorld()->GetTimerManager().SetTimer(
				PathSearchTimerHandle,
				[this]()
				{
					bCanMove = true;
				},
				2,
				false
			);

			return;
		}

		TeleportOnPath(nextCell, paths[FMath::RandHelper(paths.Num())]);

		return;
	}

	OnMovingDelegate.Broadcast();
	CurrentPathProgress += DeltaTime * SpeedFactor;
	const FVector loc = CurrentPath->GetLocationAtTime(CurrentPathProgress, ESplineCoordinateSpace::World);
	const FQuat rot = CurrentPath->GetTangentAtTime(CurrentPathProgress, ESplineCoordinateSpace::World).ToOrientationQuat();
	SetActorLocationAndRotation(loc, rot);
}

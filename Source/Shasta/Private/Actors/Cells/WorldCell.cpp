// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/Cells/WorldCell.h"
#include "Components/ShapeComponent.h"
#include <Logging/StructuredLog.h>

//====================================================================================
//==== PUBLIC CONSTRUCTORS
//====================================================================================

AWorldCell::AWorldCell()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	Pivot = CreateDefaultSubobject<USceneComponent>("Rotate me");
	if (Pivot)
		Pivot->SetupAttachment(RootComponent);

	DissolverShape = CreateDefaultSubobject<UStaticMeshComponent>("Dissolver Shape");
	if(DissolverShape)
		DissolverShape->SetupAttachment(RootComponent);

	Neighbors.Empty();
	Neighbors.Reserve(6);
	SideCenters.Empty();
	SideCenters.Reserve(6);
	for (const FIntPoint& angle : { FIntPoint(0, 60), FIntPoint(60, 120), FIntPoint(120, 180), FIntPoint(180, 240), FIntPoint(240, 300), FIntPoint(300, 360) })
	{
		Neighbors.Add(angle, nullptr);
		SideCenters.Add(angle, FMath::Lerp(FVector::RightVector.RotateAngleAxis(angle.X, FVector::UpVector), FVector::RightVector.RotateAngleAxis(angle.Y, FVector::UpVector), 0.5f));
	}
}

//====================================================================================
//==== PRIVATE OVERRIDES
//====================================================================================

void AWorldCell::Init(AWorldCell* parentCell)
{
	if(!parentCell)
		return;

	SetDistanceFromCenter(parentCell->GetDistanceFromCenter() + 1);
}

int32 AWorldCell::GetDistanceFromCenter() const
{
	return DistanceFromCenter;
}

const TMap<FIntPoint, TObjectPtr<AWorldCell>>& AWorldCell::GetNeighbors() const
{
	return Neighbors;
}

void AWorldCell::GenerateNeighbors()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (auto& pair : Neighbors)
	{
		if(pair.Value)
			continue;

		pair.Value = GetWorld()->SpawnActor<AWorldCell>(
			WorldCellTemplate,
			FTransform(GetActorLocation() + 2 * SideCenters[pair.Key] * CellRadius),
			spawnParams
		);

		if (!pair.Value)
		{
			UE_LOGFMT(LogTemp, Warning, "{0} : Could not generate neighbour at {1}", GetName(), pair.Key.ToString());
			continue;
		}
	}
}

//====================================================================================
//==== PRIVATE OVERRIDES
//====================================================================================

void AWorldCell::BeginPlay()
{
	if (bDrawDebugs)
	{
		FColor colorA(FColor::MakeRandomColor());
		FColor colorB(FColor::MakeRandomColor());
		for (auto& neighbor : Neighbors)
		{
			DrawDebugLine(
				GetWorld(),
				GetActorLocation(),
				GetActorLocation() + FVector::RightVector.RotateAngleAxis(neighbor.Key.X, FVector::UpVector) * CellRadius,
				colorA,
				true
			);

			DrawDebugLine(
				GetWorld(),
				GetActorLocation() + FVector::RightVector.RotateAngleAxis(neighbor.Key.X, FVector::UpVector) * CellRadius,
				GetActorLocation() + FVector::RightVector.RotateAngleAxis(neighbor.Key.Y, FVector::UpVector) * CellRadius,
				colorA,
				true
			);

			DrawDebugLine(
				GetWorld(),
				GetActorLocation(),
				GetActorLocation() + FVector::RightVector.RotateAngleAxis(neighbor.Key.Y, FVector::UpVector) * CellRadius,
				colorA,
				true
			);

			DrawDebugLine(
				GetWorld(),
				GetActorLocation(),
				GetActorLocation() + SideCenters[neighbor.Key] * CellRadius,
				colorB,
				true
			);
		}
	}
}

//====================================================================================
//==== PRIVATE METHODS
//====================================================================================

void AWorldCell::SetDistanceFromCenter(int Distance)
{
	DistanceFromCenter = Distance;
}

UStaticMeshComponent* AWorldCell::GetDissolverShape() const
{
	return DissolverShape;
}

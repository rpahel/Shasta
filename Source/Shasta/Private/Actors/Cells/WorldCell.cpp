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
	RootComponent->SetMobility(EComponentMobility::Static);

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
//==== PUBLIC METHODS
//====================================================================================

#if WITH_EDITOR
void AWorldCell::PingNeighbors()
{
	for (auto& pair : Neighbors)
	{
		if (!pair.Value)
			continue;

		DrawDebugPoint(
			GetWorld(),
			pair.Value->GetActorLocation(),
			25,
			FColor::Red,
			false,
			3,
			10
		);
	}
}
#endif

void AWorldCell::GenerateNeighbors()
{
	TArray<TPair<FIntPoint, TObjectPtr<AWorldCell>>> generatedCells;

	// Generating neighbors in empty spots
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (auto& pair : Neighbors)
	{
		if (pair.Value)
			continue;

		pair.Value = GetWorld()->SpawnActor<AWorldCell>(
			WorldCellTemplate,
			FTransform(GetActorLocation() + 2 * SideCenters[pair.Key] * CellRadius),
			spawnParams
		);

		generatedCells.Add(pair);

		if (!pair.Value)
		{
			UE_LOGFMT(LogTemp, Warning, "{0} : Could not generate neighbour at {1}", GetName(), pair.Key.ToString());
			continue;
		}
	}

	UE_LOGFMT(LogTemp, Log, "{0}->GenerateCells() : Generated {1} cells.", GetName(), generatedCells.Num());

	// Declare self as neighbor
	for (auto& pair : generatedCells)
	{
		if (!pair.Value)
			continue;

		pair.Value->IntroduceAsNeighbor(this, GetOppositeSector(pair.Key)); // Opposite Sector
	}
}

float AWorldCell::GetCellRadius() const
{
	return CellRadius;
}

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

//====================================================================================
//==== PUBLIC STATIC METHODS
//====================================================================================

FIntPoint AWorldCell::GetOppositeSector(const FIntPoint& Sector)
{
	return FIntPoint((Sector.X + 180) % 360, ((Sector.Y + 180) == 360 ? 360 : (Sector.Y + 180) % 360));
}

FIntPoint AWorldCell::GetAdjacentSectorCounterClockwise(const FIntPoint& Sector)
{
	return FIntPoint(Sector.X == 0 ? 300 : Sector.X - 60, Sector.Y == 60 ? 360 : Sector.Y - 60);
}

FIntPoint AWorldCell::GetAdjacentSectorClockwise(const FIntPoint& Sector)
{
	return FIntPoint(Sector.X == 300 ? 0 : Sector.X + 60, Sector.Y == 360 ? 60 : Sector.Y + 60);
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

		DrawDebugPoint(
			GetWorld(),
			GetActorLocation(),
			25,
			colorA,
			false,
			3,
			10
		);

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

void AWorldCell::IntroduceAsNeighbor(AWorldCell* NeighborCell, const FIntPoint& NeighborSector)
{
	if(!NeighborCell)
		return;

	TObjectPtr<AWorldCell>* neighborPtr = Neighbors.Find(NeighborSector);

	if (!neighborPtr->IsNull() && *neighborPtr != NeighborCell)
	{
		UE_LOGFMT(LogTemp, Error, "{0}->IntroduceAsNeighbor() : The incoming {1} neighbor is different from the already present neighbor !", GetName(), NeighborSector.ToString());
		return;
	}

	*neighborPtr = NeighborCell;

	// Add neighbor's neighbors that are adjacent to me to my neighbors
	const FIntPoint oppSector = GetOppositeSector(NeighborSector); // My sector relative to NeighborCell

	if (auto ptr_a = NeighborCell->Neighbors.Find(GetAdjacentSectorCounterClockwise(oppSector)))
	{
		if (auto ptr_b = Neighbors.Find(GetAdjacentSectorClockwise(NeighborSector)))
		{
			if (ptr_b->IsNull())
			{
				if ((*ptr_b = *ptr_a) != nullptr)
				{
					(*ptr_b)->IntroduceAsNeighbor(this, GetOppositeSector(GetAdjacentSectorClockwise(NeighborSector)));
				}
			}
		}
	}

	if (auto ptr_a = NeighborCell->Neighbors.Find(GetAdjacentSectorClockwise(oppSector)))
	{
		if (auto ptr_b = Neighbors.Find(GetAdjacentSectorCounterClockwise(NeighborSector)))
		{
			if (ptr_b->IsNull())
			{
				if ((*ptr_b = *ptr_a) != nullptr)
				{
					(*ptr_b)->IntroduceAsNeighbor(this, GetOppositeSector(GetAdjacentSectorCounterClockwise(NeighborSector)));
				}
			}
		}
	}

	// Ajouter une verification pour les secteurs vides pour etre sur qu'il n'y a pas de cases voisines
}

UStaticMeshComponent* AWorldCell::GetDissolverShape() const
{
	return DissolverShape;
}

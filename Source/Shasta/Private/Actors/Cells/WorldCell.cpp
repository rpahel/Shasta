// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/Cells/WorldCell.h"
#include "Actors/Cells/CellModifier.h"
#include "Actors/Pawns/Enemy.h"
#include "ActorComponents/Movement/PathComponent.h"

#include "Curves/CurveVector.h"
#include "Components/ShapeComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <Logging/StructuredLog.h>

//====================================================================================
//==== PUBLIC CONSTRUCTORS
//====================================================================================

AWorldCell::AWorldCell()
{
	PrimaryActorTick.bCanEverTick = true;

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

void AWorldCell::RotateCellModifier()
{
	if (!CurrentCellModifier)
		return;

	CurrentCellModifier->SetActorRotation(CurrentCellModifier->GetActorQuat() * FRotator(0, 60, 0).Quaternion());
}

float AWorldCell::GetCellRadius() const
{
	return CellRadius;
}

void AWorldCell::SetCellType(ECellType InType)
{
	CellType = InType;
	ChangeCellModifier(InType);
}

ECellType AWorldCell::GetCellType() const
{
	return CellType;
}

int32 AWorldCell::GetDistanceFromCenter() const
{
	return DistanceFromCenter;
}

const TMap<FIntPoint, TObjectPtr<AWorldCell>>& AWorldCell::GetNeighbors() const
{
	return Neighbors;
}

void AWorldCell::ChangeCellModifier(ECellType InCellType)
{
	if (CurrentCellModifier)
		CurrentCellModifier->Destroy();

	if (auto ptr = CellModifiersMap.Find(CellType))
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (CurrentCellModifier = GetWorld()->SpawnActor<ACellModifier>(*ptr, params))
		{
			CurrentCellModifier->SetParentCell(this);
			CurrentCellModifier->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

ACellModifier* AWorldCell::GetCellModifier() const
{
	return CurrentCellModifier;
}

void AWorldCell::StartSpawnEnemyTimer()
{
	if (!bEnemySpawnPoint)
		return;

	GetWorld()->GetTimerManager().SetTimer(
		EnemySpawnTimerHandle,
		[this]()
		{
			SpawnEnemy();
		},
		FMath::FRandRange(EnemySpawnTimeMinMax.X, EnemySpawnTimeMinMax.Y),
		false
	);
}

TArray<UPathComponent*> AWorldCell::GetValidPaths(const FVector& StartPoint, EShastaPathType pathType, bool DeepSearch)
{
	TArray<UPathComponent*> retArr;

	if (!CurrentCellModifier)
		return retArr;

	for (auto& path : GetPathsOfType(pathType))
	{
		if (!path)
			continue;

		//const FSplinePoint point = path->GetSplinePointAt(0, ESplineCoordinateSpace::Local);
		//const FVector pointPos = CurrentCellModifier->GetTransform().TransformPositionNoScale(point.Position);
		const FVector pointPos = path->GetLocationAtTime(0, ESplineCoordinateSpace::World);
		const FVector A = (StartPoint - GetActorLocation()).GetSafeNormal();
		const FVector B = (pointPos - GetActorLocation()).GetSafeNormal();

	#if WITH_EDITOR
		DrawDebugPoint(
			GetWorld(),
			pointPos,
			20,
			FColor::Yellow,
			false,
			5,
			10
		);

		DrawDebugLine(
			GetWorld(),
			GetActorLocation(),
			GetActorLocation() + A * CellRadius,
			FColor::Cyan,
			false,
			5,
			10,
			5
		);

		DrawDebugLine(
			GetWorld(),
			GetActorLocation(),
			GetActorLocation() + B * CellRadius,
			FColor::Magenta,
			false,
			5,
			10,
			5
		);
	#endif // WITH_EDITOR

		// A and B cant be more than 30° apart
		if (A.Dot(B) < cos(FMath::DegreesToRadians(30)))
			continue;

		// Path starts at StartPoint.
		// Now we have to check if PathEndPoint leads to another valid tile if DeepSeach is true.

		if (!DeepSearch)
		{
			retArr.Add(path);
			continue;
		}

		const FVector endPos = path->GetLocationAtTime(1, ESplineCoordinateSpace::World);
		AWorldCell* cell = GetCellInDirection(endPos - GetActorLocation());

		if (!cell)
			continue;

		if (cell->CellType == ECellType::Center)
		{
			retArr.Add(path);
			continue;
		}

		if (cell->CellType != ECellType::Defense)
			continue;

		if (cell->GetValidPaths(endPos, pathType).IsEmpty())
			continue;

		retArr.Add(path);
	}

#if WITH_EDITOR
	for (auto& path : retArr)
	{
		DrawDebugPoint(
			GetWorld(),
			path->GetLocationAtTime(0, ESplineCoordinateSpace::World),
			20,
			FColor::Red,
			false,
			5,
			10
		);
	}
#endif // WITH_EDITOR

	return retArr;
}

AWorldCell* AWorldCell::GetCellInDirection(const FVector& Dir)
{
	for (auto& pair : Neighbors)
	{
		if (!pair.Value)
			continue;

		if (Dir.Dot((pair.Value->GetActorLocation() - GetActorLocation()).GetSafeNormal()) > cos(FMath::DegreesToRadians(30)))
		{
			return pair.Value;
		}
	}

	return nullptr;
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
	Super::BeginPlay();

	StartSpawnEnemyTimer();

#if WITH_EDITOR
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
#endif // WITH_EDITOR

}

void AWorldCell::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(EnemySpawnTimerHandle);
}

#if WITH_EDITOR
void AWorldCell::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const FName name = PropertyChangedEvent.GetPropertyName();

	if (name == GET_MEMBER_NAME_CHECKED(AWorldCell, CellType))
		ChangeCellModifier(CellType);
}
#endif // WITH_EDITOR

//====================================================================================
//==== PRIVATE METHODS
//====================================================================================

void AWorldCell::SpawnEnemy()
{
	if (!EnemyTemplate || !bEnemySpawnPoint)
		return;
	
	TArray<TPair<AWorldCell*, TArray<UPathComponent*>>> candidates;

	for (auto& pair : Neighbors)
	{
		if(!pair.Value)
			continue;

		TArray<UPathComponent*> paths = pair.Value->GetValidPaths(GetActorLocation() + (pair.Value->GetActorLocation() - GetActorLocation()) * 0.5f, EShastaPathType::Ground, true);

		if(paths.IsEmpty())
			continue;

		candidates.Add({ pair.Value, paths });
	}

	if (candidates.IsEmpty())
	{
		StartSpawnEnemyTimer();
		return;
	}

	TPair<AWorldCell*, TArray<UPathComponent*>> chosenPair = candidates[FMath::RandHelper(candidates.Num())];
	UPathComponent* chosenPath = chosenPair.Value[FMath::RandHelper(chosenPair.Value.Num())];

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (AEnemy* enemy = GetWorld()->SpawnActor<AEnemy>(EnemyTemplate, chosenPath->GetLocationAtTime(0, ESplineCoordinateSpace::World), FRotator(), params))
		enemy->TeleportOnPath(chosenPair.Key, chosenPath);

	StartSpawnEnemyTimer();
}

TArray<UPathComponent*> AWorldCell::GetPaths() const
{
	TArray<UPathComponent*> ret;

	if (!CurrentCellModifier)
		return ret;

	return CurrentCellModifier->GetPaths();
}

TArray<UPathComponent*> AWorldCell::GetPathsOfType(EShastaPathType PathType) const
{
	TArray<UPathComponent*> ret;

	if (!CurrentCellModifier)
		return ret;

	ret = CurrentCellModifier->GetPaths().FilterByPredicate(
		[PathType](UPathComponent* path)
		{
			return path->GetTypes().Contains(PathType);
		}
	);

	return ret;
}

void AWorldCell::SetDistanceFromCenter(int Distance)
{
	DistanceFromCenter = Distance;
}

void AWorldCell::IntroduceAsNeighbor(AWorldCell* NeighborCell, const FIntPoint& NeighborSector)
{
	if (!NeighborCell)
		return;

	TObjectPtr<AWorldCell>* neighborPtr = Neighbors.Find(NeighborSector);

	if (neighborPtr && *neighborPtr && *neighborPtr != NeighborCell)
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
			if (!*ptr_b)
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
			if (!*ptr_b)
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

TArray<AWorldCell*> AWorldCell::GenerateNeighbors(const TSubclassOf<AWorldCell>& InTemplate)
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
			InTemplate,
			FTransform(GetActorLocation() + 2 * SideCenters[pair.Key] * CellRadius),
			spawnParams
		);

		if (!pair.Value)
		{
			UE_LOGFMT(LogTemp, Warning, "{0} : Could not generate neighbour at {1}", GetName(), pair.Key.ToString());
			continue;
		}

		pair.Value->SetDistanceFromCenter(GetDistanceFromCenter() + 1);
		generatedCells.Add(pair);
	}

	UE_LOGFMT(LogTemp, Log, "{0}->GenerateCells() : Generated {1} cells.", GetName(), generatedCells.Num());

	TArray<AWorldCell*> retArr;

	// Declare self as neighbor
	for (auto& pair : generatedCells)
	{
		if (!pair.Value)
			continue;

		pair.Value->IntroduceAsNeighbor(this, GetOppositeSector(pair.Key)); // Opposite Sector
		retArr.Add(pair.Value);
	}

	return retArr;
}

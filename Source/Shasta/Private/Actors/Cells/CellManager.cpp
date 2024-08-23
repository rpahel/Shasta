// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/Cells/CellManager.h"
#include "Actors/Cells/CellDissolver.h"
#include "Actors/Cells/WorldCell.h"

#include <Logging/StructuredLog.h>
#include <Kismet/GameplayStatics.h>

//====================================================================================
//==== PRIVATE METHODS
//====================================================================================

void ACellManager::PlayTransition()
{
	if(CellDissolver)
		CellDissolver->PlayTransition();
}

void ACellManager::PlayTransition(AWorldCell* InCell)
{
	CellDissolver->SetActorLocation(InCell->GetActorLocation());

	if (CellDissolver)
		CellDissolver->PlayTransition();

	CellToChange = InCell;
}

void ACellManager::BeginEnemySpawn()
{
	for (auto& cell : WorldCellArray)
	{
		if(!cell)
			continue;

		cell->StartSpawnEnemyTimer();
	}

	OnBeginEnemySpawn.Broadcast();
}

bool ACellManager::IsCurrentCellInCooldown(const FVector& InLocation) const
{
	AWorldCell* found = GetCellAt(InLocation);

	if (found->IsInCooldown())
	{
		if(found->GetCellType() == ECellType::Defense)
			OnCooldownCheckTriggered.Broadcast();

		return true;
	}

	return false;
}

void ACellManager::ChangeCurrentCellTo(const FVector& InLocation, const FName& ModifierName)
{
	AWorldCell* found = GetCellAt(InLocation);
	found->SetNewDefenseModifierName(ModifierName);
	PlayTransition(found);
}

AWorldCell* ACellManager::GetCellAt(const FVector& InLocation) const
{
	const FVector adjusted(InLocation.X, InLocation.Y, 0);

	AWorldCell* returnCell = nullptr;
	float lowestSize = CellDistance * CellDistance;

	for (auto& cell : WorldCellArray)
	{
		float size = (cell->GetActorLocation() - adjusted).SizeSquared();

		if (size <= lowestSize)
		{
			returnCell = cell;
			lowestSize = size;
		}
	}

	return returnCell;
}

bool ACellManager::IsInTransition() const
{
	return CellDissolver->IsInTransition();
}

void ACellManager::BeginPlay()
{
	CellDissolver = Cast<ACellDissolver>(UGameplayStatics::GetActorOfClass(GetWorld(), ACellDissolver::StaticClass()));
	CellDissolver->OnMidPointDelegate.AddUniqueDynamic(this, &ACellManager::OnDissolverMidPointCallback);

	for (auto& cell : WorldCellArray)
	{
		CellDistance = cell->GetDistanceToEdgeCenter() * 2;
		break;
	}
}

void ACellManager::GenerateCells()
{
	if (!WorldCellArray.IsEmpty())
	{
		for (int i = WorldCellArray.Num() - 1; i >= 0; --i)
		{
			if(IsValid(WorldCellArray[i]))
				WorldCellArray[i]->Destroy();
		}
	}

	WorldCellArray.Empty();

	// Generating neighbors in empty spots
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWorldCell* centerCell = GetWorld()->SpawnActor<AWorldCell>(WorldCellTemplate, spawnParams);

	if(!centerCell)
		return;

	centerCell->SetDistanceFromCenter(0);
	WorldCellArray.Add(centerCell);

	TArray<AWorldCell*> tmpArr(WorldCellArray);
	TArray<AWorldCell*> holderArr;
	for (size_t i = 1; i <= NumberOfRings; i++)
	{
		for (auto& cell : tmpArr)
		{
			if(!cell)
				continue;

			holderArr.Append(cell->GenerateNeighbors(WorldCellTemplate));
		}

		WorldCellArray.Append(holderArr);
		tmpArr = holderArr;
		holderArr.Empty();
	}

	for (auto& cell : WorldCellArray)
	{
		int32 distance = cell->GetDistanceFromCenter();
		cell->SetCellType(distance == 0 ? ECellType::Center : (distance == NumberOfRings ? ECellType::Border : ECellType::Defense));

#if WITH_EDITOR
		cell->SetFolderPath(FName(FString::Printf(TEXT("World Cells/Ring %d"), distance)));
#endif // WITH_EDITOR
	}
}

void ACellManager::OnDissolverMidPointCallback()
{
	CellToChange->ApplyNewCellModifier();
	CellToChange = nullptr;
}
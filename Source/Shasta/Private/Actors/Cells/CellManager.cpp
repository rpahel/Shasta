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

void ACellManager::BeginPlay()
{
	CellDissolver = Cast<ACellDissolver>(UGameplayStatics::GetActorOfClass(GetWorld(), ACellDissolver::StaticClass()));
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
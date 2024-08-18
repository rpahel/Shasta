// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Editor/CellManagerEditorSystem.h"
#include "Actors/Cells/WorldCell.h"

#include <Logging/StructuredLog.h>

//====================================================================================
//==== PUBLIC METHODS
//====================================================================================

void UCellManagerEditorSystem::RegisterWorldCell(AWorldCell* InWorldCell)
{
	if(!InWorldCell)
		return;

	if (auto key = WorldCellPositionMap.FindKey(InWorldCell))
		UnregisterWorldCell(InWorldCell);

	const FVector& pos = InWorldCell->GetActorLocation();
	WorldCellPositionMap.Add(pos, InWorldCell);
	OnWorldCellRegistered.Broadcast(InWorldCell, pos);
}

void UCellManagerEditorSystem::UnregisterWorldCell(AWorldCell* InWorldCell)
{
	if (!InWorldCell)
		return;

	if (auto key = WorldCellPositionMap.FindKey(InWorldCell))
		WorldCellPositionMap.Remove(*key);

	OnWorldCellUnregistered.Broadcast(InWorldCell);
}

//====================================================================================
//==== PRIVATE OVERRIDES
//====================================================================================

void UCellManagerEditorSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOGFMT(LogTemp, Warning, "I am alive");
}
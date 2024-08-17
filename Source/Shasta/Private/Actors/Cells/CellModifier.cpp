// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/Cells/CellModifier.h"
#include "ActorComponents/Movement/PathComponent.h"

#include <Logging/StructuredLog.h>

//====================================================================================
//==== PUBLIC METHODS
//====================================================================================

const TMultiMap<EShastaPathType, TObjectPtr<UPathComponent>>& ACellModifier::GetPaths() const
{
	return Paths;
}

void ACellModifier::SetParentCell(AWorldCell* WorldCell)
{
	ParentCell = WorldCell;
}

//====================================================================================
//==== PRIVATE OVERRIDES
//====================================================================================

void ACellModifier::BeginPlay()
{
	Super::BeginPlay();
	
	Paths.Empty();
	for (auto& comp : GetComponents())
	{
		if (UPathComponent* path = Cast<UPathComponent>(comp))
		{
			for (auto& type : path->GetTypes())
			{
				Paths.Add(type, path);
			}
		}
	}

	auto enumClassToFString = [](EShastaPathType inValue) -> FName
		{
			switch (inValue)
			{
				case EShastaPathType::None:
					return "None";

				case EShastaPathType::Ground:
					return "Ground";

				case EShastaPathType::Aerial:
					return "Aerial";

				case EShastaPathType::Narrow:
					return "Narrow";

				case EShastaPathType::Climbable:
					return "Climbable";

				default:
					return "Error";
			}
		};

	UE_LOGFMT(LogTemp, Log, "{0} - {1} Paths found : ", GetName(), Paths.Num());
	for (auto& pair : Paths)
		UE_LOGFMT(LogTemp, Log, "Type {0} : {1} ", enumClassToFString(pair.Key), pair.Value->GetName());
}
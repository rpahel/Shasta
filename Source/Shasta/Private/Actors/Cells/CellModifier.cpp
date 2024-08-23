// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/Cells/CellModifier.h"
#include "ActorComponents/Movement/PathComponent.h"

#include <Logging/StructuredLog.h>

//====================================================================================
//==== PUBLIC METHODS
//====================================================================================

const TArray<UPathComponent*>& ACellModifier::GetPaths()
{
	if (Paths.IsEmpty())
	{
		TArray<USceneComponent*> arr;
		RootComponent->GetChildrenComponents(true, arr);
		for (auto& comp : arr)
		{
			if (UPathComponent* path = Cast<UPathComponent>(comp))
				Paths.Add(path);
		}
	}

	return Paths;
}

void ACellModifier::SetParentCell(AWorldCell* WorldCell)
{
	ParentCell = WorldCell;
}

float ACellModifier::GetCooldown() const
{
	return Cooldown;
}

float ACellModifier::GetLifeTime() const
{
	return LifeTime;
}

//====================================================================================
//==== PRIVATE OVERRIDES
//====================================================================================

void ACellModifier::BeginPlay()
{
	Super::BeginPlay();
	
	Paths.Empty();

	GetPaths();

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
}
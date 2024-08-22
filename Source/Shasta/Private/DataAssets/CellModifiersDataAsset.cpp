// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "DataAssets/CellModifiersDataAsset.h"

const TMap<FName, TSubclassOf<ACellModifier>>& UCellModifiersDataAsset::GetCellModifiers() const
{
	return CellModifiers;
}

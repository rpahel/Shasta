// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "DataAssets/CellModifiersDataAsset.h"

const TArray<FCellModifierName>& UCellModifiersDataAsset::GetCellModifiers() const
{
	return CellModifiers;
}

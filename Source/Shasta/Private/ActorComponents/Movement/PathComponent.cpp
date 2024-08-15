// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "ActorComponents/Movement/PathComponent.h"

//====================================================================================
//==== PUBLIC METHODS
//====================================================================================

bool UPathComponent::IsOfType(EShastaPathType PathType) const
{
    return Types.Contains(PathType);
}

const TSet<EShastaPathType>& UPathComponent::GetTypes() const
{
    return Types;
}

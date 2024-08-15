// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "PathComponent.generated.h"

UENUM()
enum class EShastaPathType : uint8
{
	None = 0,
	Normal = 1,
	Aerial = 2,
	Narrow = 3,
	Climbable = 4
};

/**
 * 
 */
UCLASS(
	ClassGroup = ("Shasta"),
	meta = (BlueprintSpawnableComponent),
	AutoExpandCategories = ("Shasta|Path"),
	meta = (ShortTooltip = "To be filled.")
)
class SHASTA_API UPathComponent : public USplineComponent
{
	GENERATED_BODY()
	
private:
	//==== Exposed Fields ====

	UPROPERTY(EditAnywhere, Category = "Shasta|Path")
	TSet<EShastaPathType> Types;

public:
	//==== Methods ====

	bool IsOfType(EShastaPathType PathType) const;
	const TSet<EShastaPathType>& GetTypes() const;
};

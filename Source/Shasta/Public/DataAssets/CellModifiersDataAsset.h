// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CellModifiersDataAsset.generated.h"

class ACellModifier;

USTRUCT()
struct FCellModifierName
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	FName CellModifierName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACellModifier> CellModifierTemplate;
};

/**
 * 
 */
UCLASS()
class SHASTA_API UCellModifiersDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TArray<FCellModifierName> CellModifiers;

public:
	const TArray<FCellModifierName>& GetCellModifiers() const;
};

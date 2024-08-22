// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CellModifiersDataAsset.generated.h"

class ACellModifier;
enum class ECellType : uint8;

//USTRUCT()
//struct FCellModifierName
//{
//	GENERATED_BODY();
//
//	UPROPERTY(EditAnywhere)
//	TMap<FName, TSubclassOf<ACellModifier>> DefenseModifiers;
//};

/**
 * 
 */
UCLASS()
class SHASTA_API UCellModifiersDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TMap<FName, TSubclassOf<ACellModifier>> CellModifiers;

public:
	const TMap<FName, TSubclassOf<ACellModifier>>& GetCellModifiers() const;
};

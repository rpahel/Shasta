// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellManager.generated.h"

class AWorldCell;

/**
 * 
 */
UCLASS(
	AutoExpandCategories = ("Shasta|Cell Manager")
)
class SHASTA_API ACellManager : public AActor
{
	GENERATED_BODY()

private:
	//==== Exposed Fields ====

	UPROPERTY(EditAnywhere, Category = "Shasta|Cell Manager")
	TSubclassOf<AWorldCell> WorldCellTemplate;

	//==== Hidden Fields ====

	UPROPERTY(EditAnywhere, Category = "Shasta|Cell Manager")
	int32 NumberOfRings = 2;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Cell Manager")
	TArray<TObjectPtr<AWorldCell>> WorldCellArray;

public:
	//==== Methods ====

private:
	//==== Methods ====

	UFUNCTION(CallInEditor, Category = "Shasta")
	void GenerateCells();
};

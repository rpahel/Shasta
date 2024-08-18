// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "CellManagerEditorSystem.generated.h"

class AWorldCell;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCellManagerWorldCellSignature, AWorldCell*, OutWorldCell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCellManagerWorldCellVectorSignature, AWorldCell*, OutWorldCell, const FVector&, OutLocation);

/**
 * 
 */
UCLASS()
class SHASTA_API UCellManagerEditorSystem : public UEditorSubsystem
{
	GENERATED_BODY()
	
public:
	//==== Delegates ====

	UPROPERTY(BlueprintAssignable)
	FCellManagerWorldCellSignature OnWorldCellUnregistered;

	UPROPERTY(BlueprintAssignable)
	FCellManagerWorldCellVectorSignature OnWorldCellRegistered;

private:
	//==== Hidden Fields ====

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Cell Manager System")
	TMap<FVector, TObjectPtr<AWorldCell>> WorldCellPositionMap;

public:
	//==== Methods ====

	void RegisterWorldCell(AWorldCell* InWorldCell);
	void UnregisterWorldCell(AWorldCell* InWorldCell);

private:
	//==== Overrides ====

	void Initialize(FSubsystemCollectionBase& Collection) override;
};

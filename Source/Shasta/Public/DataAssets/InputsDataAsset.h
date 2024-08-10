// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "EnhancedInputLibrary.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputsDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class SHASTA_API UInputsDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintGetter = GetInputActionsMap)
	TMap<FName, TObjectPtr<UInputAction>> InputActionsMap;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetInputMappingContextsMap)
	TMap<FName, TObjectPtr<UInputMappingContext>> InputMappingContextsMap;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<FName, UInputAction*> GetInputActionsMap()
	{
		TMap<FName, UInputAction*> retMap;
		retMap.Reserve(InputActionsMap.Num());
		for (auto& pair : InputActionsMap)
			retMap.Add(pair.Key, pair.Value);

		return retMap;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<FName, UInputMappingContext*> GetInputMappingContextsMap()
	{
		TMap<FName, UInputMappingContext*> retMap;
		retMap.Reserve(InputMappingContextsMap.Num());
		for (auto& pair : InputMappingContextsMap)
			retMap.Add(pair.Key, pair.Value);

		return retMap;
	}
};

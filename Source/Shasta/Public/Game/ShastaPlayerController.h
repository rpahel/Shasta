// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShastaPlayerController.generated.h"

class UInputsDataAsset;

/**
 * 
 */
UCLASS(
	AutoCollapseCategories = ("Shasta|Inputs")
)
class SHASTA_API AShastaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintGetter = GetInputsDataAsset, Category = "Shasta|Inputs")
	TObjectPtr<UInputsDataAsset> InputsDataAsset;

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInputsDataAsset* GetInputsDataAsset()
	{
		return InputsDataAsset;
	}
};

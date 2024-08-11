// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "EnhancedInputLibrary.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputsDataAsset.generated.h"

USTRUCT(BlueprintType)
struct SHASTA_API FIMCPriority
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere)
	int32 DefaultPriority;
};

/**
 *
 */
UCLASS()
class SHASTA_API UInputsDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<UInputAction>> InputActionsMap;

	UPROPERTY(EditAnywhere)
	TMap<FName, FIMCPriority> InputMappingContextsMap;

public:
	const TMap<FName, TObjectPtr<UInputAction>>& GetInputActionsMap()
	{
		return InputActionsMap;
	}

	const TMap<FName, FIMCPriority>& GetInputMappingContextsMap()
	{
		return InputMappingContextsMap;
	}
};

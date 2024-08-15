// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnhancedInputComponent.h"
#include "InputsDependentInterface.generated.h"

class AShastaPlayerController;
class UInputAction;
struct FInputActionInstance;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInputsDependentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHASTA_API IInputsDependentInterface
{
	GENERATED_BODY()

public:
	virtual void BindInputActions(UEnhancedInputComponent* InInputComponent) = 0;
};

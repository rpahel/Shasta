// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "Interfaces/InputsDependentInterface.h"

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "PlayerCameraComponent.generated.h"

/**
 * 
 */
UCLASS(
	ClassGroup = ("Shasta Camera"),
	meta = (BlueprintSpawnableComponent),
	AutoExpandCategories = ("Shasta|PlayerCamera"),
	meta = (ShortTooltip = "To be filled.")
)
class SHASTA_API UPlayerCameraComponent : public UCameraComponent, public IInputsDependentInterface
{
	GENERATED_BODY()
	
private:
	//==== Exposed Fields ====

	UPROPERTY(EditAnywhere, Category = "Shasta|PlayerCamera")
	FVector2D MinMaxFov = FVector2D(70, 100);

	UPROPERTY(EditAnywhere, Category = "Shasta|PlayerCamera|Inputs")
	float Sensitivity = 1;

	UPROPERTY(EditAnywhere, Category = "Shasta|PlayerCamera|Inputs")
	TObjectPtr<UInputAction> FOVInputAction = nullptr;

private:
	//==== IInputsDependent Implementation ====

	virtual void BindInputActions(UEnhancedInputComponent* InInputComponent) override;

private:
	//==== Event Handlers ====

	UFUNCTION()
	void FOVChangedCallback(const FInputActionInstance& InputInstance);
};

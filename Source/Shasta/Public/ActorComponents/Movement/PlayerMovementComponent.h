// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "ActorComponents/Movement/ShastaMovementComponent.h"
#include "Interfaces/InputsDependentInterface.h"

#include "CoreMinimal.h"
#include "PlayerMovementComponent.generated.h"

UCLASS(
	ClassGroup = ("Shasta"),
	meta = (BlueprintSpawnableComponent),
	AutoExpandCategories = ("Shasta|Movement"),
	meta = (ShortTooltip = "To be filled.")
)
class SHASTA_API UPlayerMovementComponent : public UShastaMovementComponent, public IInputsDependentInterface
{
	GENERATED_BODY()

private:
	//==== Exposed Fields ====

	UPROPERTY(EditAnywhere, Category = "Shasta|Movement")
	FVector2D MinMaxPitch = FVector2D(-89, 89);

	UPROPERTY(EditAnywhere, Category = "Shasta|Movement")
	FVector2D MinMaxAltitude = FVector2D(100, 500);

	UPROPERTY(EditAnywhere, meta=(ClampMin = 0), Category = "Shasta|Movement|Inputs")
	float RotationSensitivity = 1;

	UPROPERTY(EditAnywhere, Category = "Shasta|Movement|Inputs")
	TObjectPtr<UInputAction> MoveInputAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Shasta|Movement|Inputs")
	TObjectPtr<UInputAction> RotationInputAction = nullptr;

public:
	//==== IInputsDependent Implementation ====

	virtual void BindInputActions(UEnhancedInputComponent* InInputComponent) override;

private:
	//==== Overrides ====

	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//==== Event Handlers ====

	UFUNCTION()
	void MoveCallback(const FInputActionInstance& InputInstance);

	UFUNCTION()
	void MoveCompletedCallback(const FInputActionInstance& InputInstance);

	UFUNCTION()
	void RotationCallback(const FInputActionInstance& InputInstance);
};

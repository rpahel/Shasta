// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "Interfaces/InputsDependentInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShastaPlayerPawn.generated.h"

class UPlayerMovementComponent;
class USphereComponent;
class UCameraComponent;

UCLASS()
class SHASTA_API AShastaPlayerPawn : public APawn, public IInputsDependentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Shasta|Camera")
	FVector2D MinMaxFov = FVector2D(70, 100);

	//==== Components ====

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Components")
	TObjectPtr<USphereComponent> Collider;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Components")
	TObjectPtr<UPlayerMovementComponent> PlayerMovement;

public:
	AShastaPlayerPawn();

	//==== IInputsDependent Implementation ====

	virtual void BindInputActions(UEnhancedInputComponent* InInputComponent) override;

private:
	UFUNCTION()
	void FOVChangeCallback(float InputFovDelta);
};

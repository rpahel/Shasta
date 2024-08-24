// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "Interfaces/InputsDependentInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShastaPlayerPawn.generated.h"

class UPlayerMovementComponent;
class USphereComponent;
class UPlayerCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShastaPlayerPawnSignature);

UCLASS()
class SHASTA_API AShastaPlayerPawn : public APawn, public IInputsDependentInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FShastaPlayerPawnSignature OnLost;

private:
	//==== Components ====

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Components")
	TObjectPtr<USphereComponent> Collider;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Components")
	TObjectPtr<UPlayerCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Components")
	TObjectPtr<UPlayerMovementComponent> PlayerMovement;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Components")
	bool bLost = false;

public:
	AShastaPlayerPawn();

	void Lose(const FVector& AttackerLocation);

	//==== IInputsDependent Implementation ====

	virtual void BindInputActions(UEnhancedInputComponent* InInputComponent) override;

private:

	void EndPlay(EEndPlayReason::Type Reason) override;
};

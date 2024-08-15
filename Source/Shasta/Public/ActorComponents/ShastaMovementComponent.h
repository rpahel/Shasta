// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "ShastaMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMovementSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMovementVectorSignature, FVector, Direction, float, NormalizedSpeed);

UCLASS(Abstract)
class SHASTA_API UShastaMovementComponent : public UFloatingPawnMovement
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FMovementVectorSignature OnMoveDelegate;

	UPROPERTY(BlueprintAssignable)
	FMovementSignature OnStopDelegate;

private:
	//==== Hidden Fields ====

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Movement|Debug")
	bool bCanMove = true;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Movement|Debug")
	bool bIsMoving = false;

public:
	//==== Methods ====

	bool GetIsMoving();
	virtual void SetCanMove(bool CanMove);
	virtual bool GetCanMove();
	virtual void MoveTowards(const FVector& Direction);

private:
	//==== Methods ====

	void SetIsMoving(bool IsMoving);
};

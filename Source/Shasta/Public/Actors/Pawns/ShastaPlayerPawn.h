// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShastaPlayerPawn.generated.h"

class UFloatingPawnMovement;
class USphereComponent;
class UCameraComponent;

UCLASS()
class SHASTA_API AShastaPlayerPawn : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Shasta|Movement")
	FVector2D MinMaxPitch = FVector2D(-89, 89);

	UPROPERTY(EditAnywhere, Category = "Shasta|Movement")
	FVector2D MinMaxAltitude = FVector2D(100, 500);

	UPROPERTY(EditAnywhere, Category = "Shasta|Camera")
	FVector2D MinMaxFov = FVector2D(70, 100);

	//==== Components ====

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collider;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

public:
	AShastaPlayerPawn();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MoveCallback(const FVector& InputDirection);

	UFUNCTION()
	void RotateCallback(const FVector2D& InputDirection);

	UFUNCTION()
	void FOVChangeCallback(float InputFovDelta);
};

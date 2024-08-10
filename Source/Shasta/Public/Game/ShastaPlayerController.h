// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShastaPlayerController.generated.h"

class UInputsDataAsset;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

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

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Inputs|Debug")
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem;

public:
	UFUNCTION(BlueprintCallable)
	UInputMappingContext* AddMappingContext(const FName& InIMCName, int32 InPriority = -1);

	UFUNCTION(BlueprintCallable)
	void RemoveMappingContext(UInputMappingContext* InIMC);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type InReason) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInputsDataAsset* GetInputsDataAsset() const;

private:
	void SetupInputMappingContext();
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void MovementCallback(const FVector& InDirection) const;

	UFUNCTION(BlueprintCallable)
	void CameraRotationCallback(const FVector2D& InDirection) const;

	UFUNCTION(BlueprintCallable)
	void FOVCallback(float InDelta) const;
};

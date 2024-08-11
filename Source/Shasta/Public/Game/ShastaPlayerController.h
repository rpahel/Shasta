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

	virtual void BindActions();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInputsDataAsset* GetInputsDataAsset() const;

private:
	void SetupInputMappingContext();
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void MovementCallback(const FInputActionInstance& InputInstance);

	UFUNCTION()
	void CameraRotationCallback(const FInputActionInstance& InputInstance);

	UFUNCTION()
	void FOVCallback(const FInputActionInstance& InputInstance);

	UFUNCTION()
	void SelectCallback(const FInputActionInstance& InputInstance);

	UFUNCTION()
	void ContextualCallback(const FInputActionInstance& InputInstance);

	UFUNCTION()
	void CancelCallback(const FInputActionInstance& InputInstance);
};

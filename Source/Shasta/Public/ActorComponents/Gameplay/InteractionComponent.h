// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "Interfaces/InputsDependentInterface.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class AModifierSelector;

UCLASS(
	ClassGroup = ("Shasta"),
	meta = (BlueprintSpawnableComponent),
	AutoExpandCategories = ("Shasta|Gameplay"),
	meta = (ShortTooltip = "To be filled.")
)
class SHASTA_API UInteractionComponent : public UActorComponent, public IInputsDependentInterface
{
	GENERATED_BODY()

private:
	//==== Exposed Fields ====

	UPROPERTY(EditAnywhere, Category = "Shasta|Gameplay|Inputs")
	TSubclassOf<AModifierSelector> ModifierSelectorTemplate;

	UPROPERTY(EditAnywhere, Category = "Shasta|Gameplay|Inputs")
	TObjectPtr<UInputAction> InteractAction = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Gameplay|Inputs")
	TObjectPtr<UInputAction> OpenMenuAction = nullptr;

	//==== Hidden Fields ====

	UPROPERTY(EditAnywhere, Category = "Shasta|Gameplay|Inputs")
	TObjectPtr<AModifierSelector> ModifierSelector = nullptr;

	UPROPERTY(EditAnywhere, Category = "Shasta|Gameplay|Inputs")
	bool bCanOpenMenu = false;

public:
	void SetCanOpenMenu(bool CanOpenMenu);

	//==== IInputsDependent Implementation ====

	virtual void BindInputActions(UEnhancedInputComponent* InInputComponent) override;

private:
	//==== Overrides ====

	void BeginPlay() override;

	//==== Event Handlers ====

	UFUNCTION()
	void InteractCallback(const FInputActionInstance& InputInstance);

	UFUNCTION()
	void OpenMenuCallback(const FInputActionInstance& InputInstance);
};

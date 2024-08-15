// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShastaPlayerController.generated.h"

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

USTRUCT()
struct FMappingContextPriority
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> MappingContext = nullptr;

	UPROPERTY(EditAnywhere)
	int32 Priority = 0;
};

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
	//==== Exposed Fields ====

	UPROPERTY(EditAnywhere, Category = "Shasta|Inputs")
	TMap<FName, FMappingContextPriority> MappingContexts;

	//==== Hidden Fields ====

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Inputs|Debug")
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem;

public:
	//==== Methods ====

	void AddMappingContext(const FName& InIMCName);
	void RemoveMappingContext(const FName& InIMCName);

protected:
	virtual void BeginPlay() override;
	virtual void BindActions();

private:
	void SetupInputMappingContext();
	virtual void SetupInputComponent() override;
};

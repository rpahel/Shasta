// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldButton.generated.h"

UENUM()
enum class EWorldButtonType : uint8
{
	None,
	Play,
	Quit,
	LowQuality,
	MediumQuality,
	HighQuality
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWorldButtonTypeSignature, EWorldButtonType, OutType);

UCLASS()
class SHASTA_API AWorldButton : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FWorldButtonTypeSignature OnSelected;

private:
	UPROPERTY(EditAnywhere)
	EWorldButtonType ButtonType = EWorldButtonType::None;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> Collider;

public:	
	AWorldButton();
	void Interact();

private:
	void EndPlay(EEndPlayReason::Type Reason) override;
};

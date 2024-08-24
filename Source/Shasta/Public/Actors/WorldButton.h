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
	Options,
	Quit,
	LowQuality,
	MediumQuality,
	HighQuality
};

UCLASS()
class SHASTA_API AWorldButton : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	EWorldButtonType ButtonType = EWorldButtonType::None;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> Collider;

public:	
	AWorldButton();
	void DoTheThing();
};

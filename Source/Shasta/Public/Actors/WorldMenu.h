// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldMenu.generated.h"

class AWorldButton;
enum class EWorldButtonType : uint8;

UCLASS()
class SHASTA_API AWorldMenu : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AWorldButton>> Buttons;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundWave> ButtonSelectSound;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundWave> StartGameMusic;

public:	
	AWorldMenu();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnSelectCallback(EWorldButtonType InType);
};

// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellModifier.generated.h"

class AWorldCell;
class UPathComponent;
enum class EShastaPathType : uint8;

UCLASS()
class SHASTA_API ACellModifier : public AActor
{
	GENERATED_BODY()

private:
	//==== Exposed Fields ====

	UPROPERTY(EditDefaultsOnly, Category = "Shasta|Cell Defense")
	TSoftObjectPtr<UMaterialInstance> CellIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Shasta|Cell Defense")
	float Cooldown = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Shasta|Cell Defense")
	float LifeTime = 10;

	//==== Hidden Fields ====

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Cell Defense|Debug")
	FName DefenseName;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Cell Defense|Debug")
	TObjectPtr<AWorldCell> ParentCell;

	TArray<TObjectPtr<UPathComponent>> Paths;

public:
	//==== Methods ====

	const TArray<UPathComponent*>& GetPaths();
	void SetParentCell(AWorldCell* WorldCell);
	float GetCooldown() const;
	float GetLifeTime() const;

private:
	//==== Overrides ====

	void BeginPlay() override;
};

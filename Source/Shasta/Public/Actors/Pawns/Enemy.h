// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

class UPathComponent;
class AWorldCell;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyEnemySignature, AEnemy*, OutEnemy);

UCLASS(
	AutoExpandCategories = "Shasta|Enemy"
)
class SHASTA_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	FEnemyEnemySignature OnFinishedPath;

private:
	UPROPERTY(EditAnywhere, Category = "Shasta|Enemy")
	float SpeedFactor = 0.1f;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Enemy|Debug")
	TObjectPtr<AWorldCell> CurrentWorldCell;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Enemy|Debug")
	TObjectPtr<UPathComponent> CurrentPath;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Enemy|Debug")
	float CurrentPathProgress = 0;

	UPROPERTY(VisibleAnywhere, Category = "Shasta|Enemy|Debug")
	bool bCanMove = false;

public:
	AEnemy();

	void TeleportOnPath(AWorldCell* WorldCell, UPathComponent* Path);

private:	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void ProgressOnPath(float DeltaTime);
};

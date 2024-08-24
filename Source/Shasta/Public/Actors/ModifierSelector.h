// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModifierSelector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FModifierSelectorSignature);

UCLASS()
class SHASTA_API AModifierSelector : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FModifierSelectorSignature OnOpenDelegate;

	UPROPERTY(BlueprintAssignable)
	FModifierSelectorSignature OnCloseDelegate;

private:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class USoundWave> ModifierSelectorSound;

	UPROPERTY(EditAnywhere)
	float CloseDistance = 300;

	UPROPERTY(EditAnywhere)
	float AnimationTime = 1;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> OpenCurve;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> CloseCurve;

	UPROPERTY(VisibleAnywhere)
	bool bIsOpen = false;

	UPROPERTY(VisibleAnywhere)
	bool bInAnimation = false;

	UPROPERTY(VisibleAnywhere)
	float AnimationProgress = 0;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> OwnerActor;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ACellManager> CellManager;

	UPROPERTY(VisibleAnywhere)
	FVector CloseAnimationStartPos;

public:	
	AModifierSelector();
	void SetOwnerActor(AActor* InOwner);
	void Open();
	void Close();
	void Select(UPrimitiveComponent* Component);
	bool IsInAnimation() const;

private:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	ACellManager* GetCellManager();
};

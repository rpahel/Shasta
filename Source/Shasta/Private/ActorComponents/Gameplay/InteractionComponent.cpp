// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/Gameplay/InteractionComponent.h"
#include "Actors/ModifierSelector.h"
#include "Actors/WorldButton.h"

//====================================================================================
//==== IINPUTSDEPENDENT IMPLEMENTATION
//====================================================================================

void UInteractionComponent::SetCanOpenMenu(bool CanOpenMenu)
{
	bCanOpenMenu = CanOpenMenu;
}

void UInteractionComponent::BindInputActions(UEnhancedInputComponent* InInputComponent)
{
	if (!InInputComponent)
		return;

	if (InteractAction)
		InInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &UInteractionComponent::InteractCallback);

	if (OpenMenuAction)
		InInputComponent->BindAction(OpenMenuAction, ETriggerEvent::Completed, this, &UInteractionComponent::OpenMenuCallback);
}

//====================================================================================
//==== PRIVATE OVERRIDES
//====================================================================================

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ModifierSelector = GetWorld()->SpawnActor<AModifierSelector>(ModifierSelectorTemplate, params);
	ModifierSelector->SetActorLocation(FVector(0, 0, -10000));
	ModifierSelector->SetActorHiddenInGame(true);
	ModifierSelector->SetOwnerActor(GetOwner());
}

//====================================================================================
//==== PRIVATE EVENT HANDLERS
//====================================================================================

void UInteractionComponent::InteractCallback(const FInputActionInstance& InputInstance)
{
	FHitResult hit;
	const FVector start = GetOwner()->GetActorLocation();
	const FVector end = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 1000;

	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_GameTraceChannel1))
	{
		if (hit.GetComponent() && hit.GetComponent()->ComponentHasTag("ModifierSelector"))
		{
			if(!ModifierSelector->IsInAnimation())
				ModifierSelector->Select(hit.GetComponent());
			return;
		}

		if (AWorldButton* button = Cast<AWorldButton>(hit.GetActor()))
			button->Interact();
	}
}

void UInteractionComponent::OpenMenuCallback(const FInputActionInstance& InputInstance)
{
	if(bCanOpenMenu)
		ModifierSelector->Open();
}
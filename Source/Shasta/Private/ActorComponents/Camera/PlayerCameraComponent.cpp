// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "ActorComponents/Camera/PlayerCameraComponent.h"

//====================================================================================
//==== IINPUTSDEPENDENT IMPLEMENTATION
//====================================================================================

void UPlayerCameraComponent::BindInputActions(UEnhancedInputComponent* InInputComponent)
{
	if (!InInputComponent)
		return;

	if (FOVInputAction)
		InInputComponent->BindAction(FOVInputAction, ETriggerEvent::Triggered, this, &UPlayerCameraComponent::FOVChangedCallback);
}

//====================================================================================
//==== PRIVATE EVENT HANDLERS
//====================================================================================

void UPlayerCameraComponent::FOVChangedCallback(const FInputActionInstance& InputInstance)
{
	SetFieldOfView(FMath::Clamp(FieldOfView - InputInstance.GetValue().Get<float>() * Sensitivity, MinMaxFov.X, MinMaxFov.Y));
}

// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Game/ShastaPlayerController.h"
#include "DataAssets/InputsDataAsset.h"

#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Logging/StructuredLog.h"

UInputMappingContext* AShastaPlayerController::AddMappingContext(const FName& InIMCName, int32 InPriority)
{
	if (!InputSubsystem || !InputsDataAsset)
		return nullptr;

	const FIMCPriority* ptr = InputsDataAsset->GetInputMappingContextsMap().Find(InIMCName);

	if (!ptr || !ptr->InputMappingContext)
		return nullptr;

	InputSubsystem->AddMappingContext(ptr->InputMappingContext, InPriority == -1 ? ptr->DefaultPriority : InPriority);
	return ptr->InputMappingContext;
}

void AShastaPlayerController::RemoveMappingContext(UInputMappingContext* InIMC)
{
	if(!InputSubsystem || !InIMC)
		return;

	InputSubsystem->RemoveMappingContext(InIMC);
}

void AShastaPlayerController::BeginPlay()
{
	UE_LOGFMT(LogTemp, Warning, "AShastaPlayerController::BeginPlay()");
	SetInputMode(FInputModeGameOnly());
}

void AShastaPlayerController::EndPlay(EEndPlayReason::Type InReason)
{
	OnMoveDelegate.Clear();
	OnRotateDelegate.Clear();
	OnFOVChangeDelegate.Clear();
}

void AShastaPlayerController::BindActions()
{
	if(!InputsDataAsset)
		return;

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	if (!EIC)
		return;

	auto& inputActionsMap = InputsDataAsset->GetInputActionsMap();

	if (auto action = inputActionsMap.FindRef("Movement"))
	{
		EIC->BindAction(action, ETriggerEvent::Triggered, this, &AShastaPlayerController::MovementCallback);
	}

	if (auto action = inputActionsMap.FindRef("Camera"))
	{
		EIC->BindAction(action, ETriggerEvent::Triggered, this, &AShastaPlayerController::CameraRotationCallback);
	}

	if (auto action = inputActionsMap.FindRef("FOV"))
	{
		EIC->BindAction(action, ETriggerEvent::Triggered, this, &AShastaPlayerController::FOVCallback);
	}

	if (auto action = inputActionsMap.FindRef("Select"))
	{
		EIC->BindAction(action, ETriggerEvent::Completed, this, &AShastaPlayerController::SelectCallback);
	}

	if (auto action = inputActionsMap.FindRef("Contextual"))
	{
		EIC->BindAction(action, ETriggerEvent::Completed, this, &AShastaPlayerController::ContextualCallback);
	}

	if (auto action = inputActionsMap.FindRef("Cancel"))
	{
		EIC->BindAction(action, ETriggerEvent::Completed, this, &AShastaPlayerController::CancelCallback);
	}
}

UInputsDataAsset* AShastaPlayerController::GetInputsDataAsset() const
{
	return InputsDataAsset;
}

void AShastaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UE_LOGFMT(LogTemp, Warning, "AShastaPlayerController::SetupInputComponent()");
	SetupInputMappingContext();
	BindActions();
}

void AShastaPlayerController::SetupInputMappingContext()
{
	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!InputSubsystem)
	{
		UE_LOGFMT(LogTemp, Error, "AShastaPlayerController::SetUpInputMappingContext() -> Could not get InputSubsystem !");
		return;
	}

	InputSubsystem->ClearAllMappings();
	AddMappingContext("Default");
}

void AShastaPlayerController::MovementCallback(const FInputActionInstance& InputInstance)
{
	FVector inputValue = InputInstance.GetValue().Get<FVector>();
	OnMoveDelegate.Broadcast(inputValue);
}

void AShastaPlayerController::CameraRotationCallback(const FInputActionInstance& InputInstance)
{
	FVector2D inputValue = InputInstance.GetValue().Get<FVector2D>();
	OnRotateDelegate.Broadcast(inputValue * CameraSensitivity);
}

void AShastaPlayerController::FOVCallback(const FInputActionInstance& InputInstance)
{
	float inputValue = InputInstance.GetValue().Get<float>();
	OnFOVChangeDelegate.Broadcast(inputValue);
}

void AShastaPlayerController::SelectCallback(const FInputActionInstance& InputInstance)
{
	UE_LOGFMT(LogTemp, Log, "AShastaPlayerController::SelectCallback");
}

void AShastaPlayerController::ContextualCallback(const FInputActionInstance& InputInstance)
{
	UE_LOGFMT(LogTemp, Log, "AShastaPlayerController::ContextualCallback");
}

void AShastaPlayerController::CancelCallback(const FInputActionInstance& InputInstance)
{
	UE_LOGFMT(LogTemp, Log, "AShastaPlayerController::CancelCallback");
}

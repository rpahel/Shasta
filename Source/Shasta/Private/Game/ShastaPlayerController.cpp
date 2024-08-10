// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Game/ShastaPlayerController.h"
#include "DataAssets/InputsDataAsset.h"

#include "EnhancedInputSubsystems.h"
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
	UE_LOGFMT(LogTemp, Warning, "AShastaPlayerController::EndPlay()");
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

void AShastaPlayerController::MovementCallback(const FVector& InDirection) const
{
	UE_LOGFMT(LogTemp, Log, "AShastaPlayerController::MovementCallback : {0}", InDirection.ToString());
}

void AShastaPlayerController::CameraRotationCallback(const FVector2D& InDirection) const
{
	UE_LOGFMT(LogTemp, Log, "AShastaPlayerController::CameraRotationCallback : {0}", InDirection.ToString());
}

void AShastaPlayerController::FOVCallback(float InDelta) const
{
	UE_LOGFMT(LogTemp, Log, "AShastaPlayerController::FOVCallback : {0}", InDelta);
}

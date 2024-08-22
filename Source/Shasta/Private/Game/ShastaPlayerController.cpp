// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Game/ShastaPlayerController.h"
#include "Actors/Pawns/ShastaPlayerPawn.h"

#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "Logging/StructuredLog.h"

void AShastaPlayerController::AddMappingContext(const FName& InIMCName)
{
	if (!InputSubsystem)
		return;

	if (auto found = MappingContexts.Find(InIMCName))
		InputSubsystem->AddMappingContext(found->MappingContext, found->Priority);
}

void AShastaPlayerController::RemoveMappingContext(const FName& InIMCName)
{
	if (!InputSubsystem)
		return;

	if (auto found = MappingContexts.Find(InIMCName))
		InputSubsystem->RemoveMappingContext(found->MappingContext);
}

void AShastaPlayerController::BeginPlay()
{
	SetInputMode(FInputModeGameOnly());
	BindActions();
	if (UGameUserSettings* settings = GEngine->GetGameUserSettings())
	{
		settings->SetFullscreenMode(EWindowMode::Fullscreen);
		settings->SetOverallScalabilityLevel(3);
		settings->ApplySettings(false);
	}
}

void AShastaPlayerController::BindActions()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	if (!EIC)
		return;

	if (auto playerPawn = Cast<AShastaPlayerPawn>(GetPawn()))
		playerPawn->BindInputActions(EIC);
}

void AShastaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
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
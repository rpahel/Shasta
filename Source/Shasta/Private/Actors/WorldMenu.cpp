// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/WorldMenu.h"
#include "Actors/WorldButton.h"
#include "Actors/Cells/CellManager.h"
#include "Actors/Pawns/ShastaPlayerPawn.h"
#include "ActorComponents/Gameplay/InteractionComponent.h"
#include "GameFramework/GameUserSettings.h"

#include "Kismet/GameplayStatics.h"
#include <Logging/StructuredLog.h>

AWorldMenu::AWorldMenu()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AWorldMenu::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimerForNextTick(
		[this]()
		{
			TArray<AActor*> arr;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldButton::StaticClass(), arr);
			Buttons.Reserve(arr.Num());
			for (AActor* button : arr)
			{
				auto& ref = Buttons.Add_GetRef(Cast<AWorldButton>(button));
				ref->OnSelected.AddUniqueDynamic(this, &AWorldMenu::OnSelectCallback);
			}
		}
	);
}

void AWorldMenu::OnSelectCallback(EWorldButtonType InType)
{
	switch (InType)
	{
		case EWorldButtonType::Play:
		{
			for (int i = Buttons.Num() - 1; i >= 0; i--)
				Buttons[i]->Destroy();

			Buttons.Empty();

			if(ACellManager* manager = Cast<ACellManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACellManager::StaticClass())))
				manager->BeginEnemySpawn();

			if (AShastaPlayerPawn* player = Cast<AShastaPlayerPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AShastaPlayerPawn::StaticClass())))
			{
				if (UInteractionComponent* interactComp = player->GetComponentByClass<UInteractionComponent>())
				{
					interactComp->SetCanOpenMenu(true);
				}
			}

			break;
		}

		case EWorldButtonType::Quit:
		{
			UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
			break;
		}

		case EWorldButtonType::LowQuality:
		{
			if (UGameUserSettings* settings = GEngine->GetGameUserSettings())
			{
				settings->SetOverallScalabilityLevel(0);
				settings->SetResolutionScaleNormalized(0.9f);
				settings->ApplySettings(false);
			}

			break;
		}

		case EWorldButtonType::MediumQuality:
		{
			if (UGameUserSettings* settings = GEngine->GetGameUserSettings())
			{
				settings->SetOverallScalabilityLevel(1);
				settings->SetResolutionScaleNormalized(0.95f);
				settings->ApplySettings(false);
			}

			break;
		}

		case EWorldButtonType::HighQuality:
		{
			if (UGameUserSettings* settings = GEngine->GetGameUserSettings())
			{
				settings->SetOverallScalabilityLevel(2);
				settings->SetResolutionScaleNormalized(1);
				settings->ApplySettings(false);
			}

			break;
		}

		default:
			break;
	}
}

// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "Actors/WorldMenu.h"
#include "Actors/WorldButton.h"

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
		case EWorldButtonType::None:
		{
			UE_LOGFMT(LogTemp, Log, "AWorldMenu::OnSelectCallback -> None Clicked.");
			break;
		}

		case EWorldButtonType::Play:
		{
			UE_LOGFMT(LogTemp, Log, "AWorldMenu::OnSelectCallback -> Play Clicked.");
			break;
		}

		case EWorldButtonType::Quit:
		{
			UE_LOGFMT(LogTemp, Log, "AWorldMenu::OnSelectCallback -> Quit Clicked.");
			UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
			break;
		}

		case EWorldButtonType::LowQuality:
		{
			UE_LOGFMT(LogTemp, Log, "AWorldMenu::OnSelectCallback -> LowQuality Clicked.");
			break;
		}

		case EWorldButtonType::MediumQuality:
		{
			UE_LOGFMT(LogTemp, Log, "AWorldMenu::OnSelectCallback -> MediumQuality Clicked.");
			break;
		}

		case EWorldButtonType::HighQuality:
		{
			UE_LOGFMT(LogTemp, Log, "AWorldMenu::OnSelectCallback -> HighQuality Clicked.");
			break;
		}

		default:
			break;
	}
}

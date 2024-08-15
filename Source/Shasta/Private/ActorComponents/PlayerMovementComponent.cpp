// Fill out your copyright notice in the Description page of Project Settings.


#include "Shasta/Public/ActorComponents/PlayerMovementComponent.h"

//====================================================================================
//==== IINPUTSDEPENDENT IMPLEMENTATION
//====================================================================================

void UPlayerMovementComponent::BindInputActions(UEnhancedInputComponent* InInputComponent)
{
	if (!InInputComponent)
		return;

	if (MoveInputAction)
	{
		InInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &UPlayerMovementComponent::MoveCallback);
		InInputComponent->BindAction(MoveInputAction, ETriggerEvent::Completed, this, &UPlayerMovementComponent::MoveCompletedCallback);
		InInputComponent->BindAction(MoveInputAction, ETriggerEvent::Canceled, this, &UPlayerMovementComponent::MoveCompletedCallback);
	}

	if (RotationInputAction)
		InInputComponent->BindAction(RotationInputAction, ETriggerEvent::Triggered, this, &UPlayerMovementComponent::RotationCallback);
}

//====================================================================================
//==== PRIVATE OVERRIDES
//====================================================================================

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Adjust spawn altitude
	FVector loc = GetActorLocation();
	loc.Z = FMath::Clamp(loc.Z, MinMaxAltitude.X, MinMaxAltitude.Y);
	GetOwner()->SetActorLocation(loc);
}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector loc = GetActorLocation();
	if (loc.Z < MinMaxAltitude.X || loc.Z > MinMaxAltitude.Y)
	{
		Velocity.Z = 0;
		loc.Z = FMath::Clamp(loc.Z, MinMaxAltitude.X, MinMaxAltitude.Y);
		GetOwner()->SetActorLocation(loc);
	}
}

//====================================================================================
//==== PRIVATE EVENT HANDLERS
//====================================================================================

void UPlayerMovementComponent::MoveCallback(const FInputActionInstance& InputInstance)
{
	const float altitude = GetActorLocation().Z;
	FVector worldDir = InputInstance.GetValue().Get<FVector>().RotateAngleAxis(GetOwner()->GetActorRotation().Yaw, FVector::UpVector);

	if ((worldDir.Z > 0 && altitude >= MinMaxAltitude.Y)
		|| (worldDir.Z < 0 && altitude <= MinMaxAltitude.X))
	{
		worldDir.Z = 0;
	}

	MoveTowards(worldDir);
}

void UPlayerMovementComponent::MoveCompletedCallback(const FInputActionInstance& InputInstance)
{
	MoveTowards(FVector::ZeroVector);
}

void UPlayerMovementComponent::RotationCallback(const FInputActionInstance& InputInstance)
{
	const FVector2D inputDirection = InputInstance.GetValue().Get<FVector2D>();
	FRotator rot = GetOwner()->GetActorRotation();
	rot.Yaw += inputDirection.X;
	rot.Pitch = FMath::Clamp(rot.Pitch + inputDirection.Y, MinMaxPitch.X, MinMaxPitch.Y);
	GetOwner()->SetActorRotation(rot);
}

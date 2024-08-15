// Copyright (c) 2024, Raphael Coimbra, Killian Diakouka. All rights reserved.


#include "ActorComponents/Movement/ShastaMovementComponent.h"
#include "Game/ShastaPlayerController.h"

//====================================================================================
//==== PUBLIC METHODS
//====================================================================================

bool UShastaMovementComponent::GetIsMoving()
{
	return bIsMoving;
}

void UShastaMovementComponent::SetCanMove(bool CanMove)
{
	if (bCanMove == CanMove)
		return;

	bCanMove = CanMove;

	if(!bCanMove)
		bIsMoving = false;
}

bool UShastaMovementComponent::GetCanMove()
{
	return bCanMove;
}

void UShastaMovementComponent::MoveTowards(const FVector& Direction)
{
	if(bCanMove && !Direction.Equals(FVector::ZeroVector))
	{
		bIsMoving = true;
		Super::AddInputVector(Direction);
		OnMoveDelegate.Broadcast(Direction, Velocity.SizeSquared() / (MaxSpeed * MaxSpeed));
	}

	else if(bIsMoving)
	{
		OnStopDelegate.Broadcast();
		bIsMoving = false;
	}
}

//====================================================================================
//==== PRIVATE METHODS
//====================================================================================

void UShastaMovementComponent::SetIsMoving(bool IsMoving)
{
	bIsMoving = IsMoving;
}

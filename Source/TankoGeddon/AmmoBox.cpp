// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include <Components/StaticMeshComponent.h>
#include "TankPawn.h"

// Sets default values
AAmmoBox::AAmmoBox(): AThingBox()
{
	
}

void AAmmoBox::ActionWithTank(ATankPawn* Tank)
{
	Tank->SetupCannon(CannonClass);
}



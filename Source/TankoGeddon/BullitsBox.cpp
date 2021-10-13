// Fill out your copyright notice in the Description page of Project Settings.


#include "BullitsBox.h"
#include "ThingBox.h"
#include "TankPawn.h"
#include <Components/ArrowComponent.h>
#include <Components/TextRenderComponent.h>

// Sets default values
ABullitsBox::ABullitsBox() : AThingBox()
{
	TextPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("TextPoint"));
	TextPoint->SetupAttachment(RootComponent);

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(TextPoint);
	Text->SetWorldRotation(FRotator{90.f, 0.f, 0.f});
	Text->SetWorldScale3D({3.f, 3.f, 3.f});
}

void ABullitsBox::BeginPlay()
{
	Super::BeginPlay();
	Text->SetText(FString::FromInt(BullitsCount));
}

void ABullitsBox::ActionWithTank(ATankPawn* Tank)
{
	Tank->AddBullits(BullitsCount);
}
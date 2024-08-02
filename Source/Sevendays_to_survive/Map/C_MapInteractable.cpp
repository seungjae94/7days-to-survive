// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/C_MapInteractable.h"

#include "Map/UI/C_InteractionMessageWidgetComponent.h"

AC_MapInteractable::AC_MapInteractable()
{
    bReplicates = true;

    InteractionMessageWidgetComponent = CreateDefaultSubobject<UC_InteractionMessageWidgetComponent>(TEXT("Interaction Widget Component"));
    InteractionMessageWidgetComponent->SetupAttachment(GetRootComponent());
    InteractionMessageWidgetComponent->SetIsReplicated(true);
}

void AC_MapInteractable::BeginPlay()
{
    Super::BeginPlay();
}


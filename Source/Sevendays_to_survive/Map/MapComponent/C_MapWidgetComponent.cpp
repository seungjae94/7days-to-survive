// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/MapComponent/C_MapWidgetComponent.h"

UC_MapWidgetComponent::UC_MapWidgetComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UC_MapWidgetComponent::ShowWidget(int _Index)
{
    GetWidget()->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UC_MapWidgetComponent::HideWidget(int _Index)
{
    GetWidget()->SetVisibility(ESlateVisibility::Collapsed);
}

void UC_MapWidgetComponent::BeginPlay()
{
    Super::BeginPlay();
    InitWidget();
    HideWidget();
}

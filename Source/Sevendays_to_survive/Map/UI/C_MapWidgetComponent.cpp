// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/UI/C_MapWidgetComponent.h"

UC_MapWidgetComponent::UC_MapWidgetComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UC_MapWidgetComponent::ShowWidget()
{
    GetWidget()->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UC_MapWidgetComponent::HideWidget()
{
    GetWidget()->SetVisibility(ESlateVisibility::Collapsed);
}

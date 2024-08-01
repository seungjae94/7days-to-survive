// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "C_InteractionWidgetComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_InteractionWidgetComponent : public UWidgetComponent
{
    GENERATED_BODY()

public:
    UC_InteractionWidgetComponent();

    virtual void ShowWidget() PURE_VIRTUAL(ShowWidget, );
    virtual void HideWidget() PURE_VIRTUAL(HideWidget, );
};

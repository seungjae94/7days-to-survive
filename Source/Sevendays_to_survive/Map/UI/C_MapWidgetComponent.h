// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "C_MapWidgetComponent.generated.h"

UCLASS(Abstract, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_MapWidgetComponent : public UWidgetComponent
{
    GENERATED_BODY()

public:
    UC_MapWidgetComponent();

    UFUNCTION(BlueprintCallable)
    virtual void ShowWidget();

    UFUNCTION(BlueprintCallable)
    virtual void HideWidget();
};

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

    void SetIsViewable(bool _bIsViewable);
    bool IsViewable() const;

    UFUNCTION(BlueprintCallable)
    virtual void ShowWidget(int _Index = 0);

    UFUNCTION(BlueprintCallable)
    virtual void HideWidget(int _Index = 0);

protected:
    void BeginPlay() override;

private:
    bool bIsViewable = true;
};

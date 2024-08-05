// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/MapComponent/C_MapWidgetComponent.h"
#include "C_MapMessageComponent.generated.h"

class UC_MapMessageWidget;

UCLASS(Abstract, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_MapMessageComponent : public UC_MapWidgetComponent
{
    GENERATED_BODY()

public:
    UC_MapMessageComponent();

    virtual void MapInteract() PURE_VIRTUAL(MapInteract, );

    void ShowWidget(int _Index) override;

    bool IsInteractOnServer() const;
    void SetIsInteractOnServer(bool _bIsInteractOnServer);

protected:
    void BeginPlay() override;

    UFUNCTION()
    virtual void SetMessage() PURE_VIRTUAL(SetMessage, );

    UFUNCTION()
    UC_MapMessageWidget* GetMessageWidget();

private:
    UPROPERTY()
    TSubclassOf<UUserWidget> MessageWidgetClass;

    bool bIsInteractOnServer = false;
};

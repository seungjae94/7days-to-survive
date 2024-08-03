// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/UI/C_MapWidgetComponent.h"
#include "C_MapActorInteractionComponent.generated.h"

class UC_InteractionMessageWidget;

UCLASS(Abstract, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_MapActorInteractionComponent : public UC_MapWidgetComponent
{
    GENERATED_BODY()

public:
    UC_MapActorInteractionComponent();

    virtual void MapInteract() PURE_VIRTUAL(MapInteract, );

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void SetMessage() PURE_VIRTUAL(SetMessage, );

    UFUNCTION()
    UC_InteractionMessageWidget* GetMessageWidget();

private:
    UPROPERTY()
    TSubclassOf<UUserWidget> MessageWidgetClass;
};

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

    UFUNCTION(NetMulticast, Reliable)
    void SetMessage();
    virtual void SetMessage_Implementation() PURE_VIRTUAL(SetMessage_Implementation, );

    virtual void MapInteract() PURE_VIRTUAL(MapInteract,);

protected:
    virtual void BeginPlay() override;

protected:
    UC_InteractionMessageWidget* MessageWidget = nullptr;
};

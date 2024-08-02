// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/UI/C_MapActorInteractionComponent.h"
#include "C_DoorInteractionComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_DoorInteractionComponent : public UC_MapActorInteractionComponent
{
    GENERATED_BODY()

public:
    void MapInteract() override;

    void SetMessage_Implementation() override;
};

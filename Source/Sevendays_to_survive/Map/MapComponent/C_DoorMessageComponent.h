// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/MapComponent/C_MapMessageComponent.h"
#include "C_DoorMessageComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_DoorMessageComponent : public UC_MapMessageComponent
{
    GENERATED_BODY()

public:
    void MapInteract() override;

private:
    void SetMessage() override;
};

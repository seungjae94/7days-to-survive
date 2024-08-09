// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Map/C_MapEnums.h"
#include "Map/C_ItemRows.h"
#include "Player/Global/C_PlayerEnum.h"
#include "STS/C_STSMacros.h"
#include "C_Items.generated.h"

class UC_MapDataObject;

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_Item : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure)
    bool IsCraftable() const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName Id;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FC_ItemBaseData BaseData;
};

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_MaterialItem : public UC_Item
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FC_MaterialItemData MaterialData;
};

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_WeaponItem : public UC_Item
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FC_WeaponItemData WeaponData;
};

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_ConsumableItem : public UC_Item
{
    GENERATED_BODY()

public:
    void Use(UWorld* _World) const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FC_ConsumableItemData ConsumableData;
};

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_BuildingPartItem : public UC_Item
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FC_BuildingPartItemData BuildingPartData;
};
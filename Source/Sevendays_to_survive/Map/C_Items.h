// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Map/C_MapEnums.h"
#include "Player/Global/C_PlayerEnum.h"
#include "STS/C_STSMacros.h"
#include "C_Items.generated.h"

class UC_MapDataObject;
struct FC_ItemRow;
struct FC_MaterialRow;
struct FC_WeaponRow;
struct FC_ConsumableRow;
struct FC_ItemBuildingPartRow;

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_Item : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure)
    bool IsCraftable() const;

    UFUNCTION(BlueprintPure)
    FName GetId() const;

    UFUNCTION(BlueprintPure)
    FString GetName() const;

    UFUNCTION(BlueprintPure)
    EItemType GetType() const;

    UFUNCTION(BlueprintPure)
    UTexture2D* GetIcon() const;

    UFUNCTION(BlueprintPure)
    const TMap<FName, int>& GetCraftMaterials() const;

    UFUNCTION(BlueprintPure)
    int GetDropWeight() const;

protected:
    FName Id;
    const FC_ItemRow* ItemRow = nullptr;

    template<typename ItemRowType>
    const ItemRowType* RowCast() const
    {
        if (nullptr == ItemRow)
        {
            STS_FATAL("[%s] ItemRow is unset.", TEXT(__FUNCTION__));
            return nullptr;
        }

        return reinterpret_cast<const ItemRowType*>(ItemRow);
    }
};

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_MaterialItem : public UC_Item
{
    GENERATED_BODY()

    friend UC_MapDataObject;

public:
    UFUNCTION(BlueprintPure)
    int GetMaxCount() const;
};

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_WeaponItem : public UC_Item
{
    GENERATED_BODY()

    friend UC_MapDataObject;

public:
    UFUNCTION(BlueprintPure)
    int GetDamage() const;

    UFUNCTION(BlueprintPure)
    bool GetIsStatic() const;

    UFUNCTION(BlueprintPure)
    EStaticItemSlot GetStaticItemSlot() const;

    UFUNCTION(BlueprintPure)
    UStaticMesh* GetStaticMesh() const;

    UFUNCTION(BlueprintPure)
    ESkerItemSlot GetSkeletalItemSlot() const;
};

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_ConsumableItem : public UC_Item
{
    GENERATED_BODY()

    friend UC_MapDataObject;

public:
    void Use(UWorld* _World) const;

    UFUNCTION(BlueprintPure)
    int GetHp() const;

    UFUNCTION(BlueprintPure)
    int GetStamina() const;
};

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_BuildingPartItem : public UC_Item
{
    GENERATED_BODY()

    friend UC_MapDataObject;

public:
    UFUNCTION(BlueprintPure)
    int GetMaxHp() const;

    UFUNCTION(BlueprintPure)
    UStaticMesh* GetMesh() const;

    UFUNCTION(BlueprintPure)
    TSubclassOf<AActor> GetActorClass() const;

    UFUNCTION(BlueprintPure)
    TEnumAsByte<ETraceTypeQuery> GetTraceType() const;
};
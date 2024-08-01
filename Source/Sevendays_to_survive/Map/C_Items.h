// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Map/C_MapEnums.h"
#include "Player/Global/C_PlayerEnum.h"
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FName Id;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    EItemType Type = EItemType::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UTexture2D* Icon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TMap<FName, int> CraftMaterials;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    int DropWeight = 0;

protected:
    void Init(FName _Id, FC_ItemRow* _ItemRow);
};

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_ItemMaterial : public UC_Item
{
    GENERATED_BODY()

    friend UC_MapDataObject;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    int MaxCount = 0;

private:
    void Init(FName _Id, FC_MaterialRow* _ItemRow);
};

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_Weapon : public UC_Item
{
    GENERATED_BODY()

    friend UC_MapDataObject;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Damage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsStatic = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EStaticItemSlot StaticItemSlot = EStaticItemSlot::SlotMax;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* StaticMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESkerItemSlot SkeletalItemSlot = ESkerItemSlot::SlotMax;

private:
    void Init(FName _Id, FC_WeaponRow* _ItemRow);
};

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_Consumable : public UC_Item
{
    GENERATED_BODY()

    friend UC_MapDataObject;

public:
    void Use(UWorld* _World) const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    int Hp = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    int Stamina = 0;

private:
    void Init(FName _Id, FC_ConsumableRow* _ItemRow);
};

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_ItemBuildingPart : public UC_Item
{
    GENERATED_BODY()

    friend UC_MapDataObject;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    int MaxHp = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UStaticMesh* Mesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<ETraceTypeQuery> TraceType = ETraceTypeQuery::TraceTypeQuery1;

private:
    void Init(FName _Id, FC_ItemBuildingPartRow* _ItemRow);
};
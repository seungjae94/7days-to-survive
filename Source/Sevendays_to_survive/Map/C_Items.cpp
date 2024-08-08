// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/C_Items.h"

#include "Map/C_ItemRows.h"
#include "STS/C_STSGlobalFunctions.h"
#include "Player/Global/C_MapPlayer.h"

bool UC_Item::IsCraftable() const
{
    return !GetCraftMaterials().IsEmpty();
}

FName UC_Item::GetId() const
{
    return Id;
}

FString UC_Item::GetName() const
{
    return ItemRow->Name;
}

EItemType UC_Item::GetType() const
{
    return ItemRow->Type;
}

UTexture2D* UC_Item::GetIcon() const
{
    return ItemRow->Icon;
}

const TMap<FName, int>& UC_Item::GetCraftMaterials() const
{
    return ItemRow->CraftMaterials;
}

int UC_Item::GetDropWeight() const
{
    return ItemRow->DropWeight;
}

void UC_ConsumableItem::Use(UWorld* _World) const
{
    AC_MapPlayer* Player = UC_STSGlobalFunctions::GetMapPlayerCharacter(_World);
    if (false == Player->IsValidLowLevel())
    {
        return;
    }

    Player->AddHp(GetHp());
    Player->Addstamina(GetStamina());
}

int UC_ConsumableItem::GetHp() const
{
    return RowCast<FC_ConsumableRow>()->Hp;
}

int UC_ConsumableItem::GetStamina() const
{
    return RowCast<FC_ConsumableRow>()->Stamina;
}

int UC_MaterialItem::GetMaxCount() const
{
    return RowCast<FC_MaterialRow>()->MaxCount;
}

int UC_WeaponItem::GetDamage() const
{
    return RowCast<FC_WeaponRow>()->Damage;
}

bool UC_WeaponItem::GetIsStatic() const
{
    return RowCast<FC_WeaponRow>()->IsStatic;
}

EStaticItemSlot UC_WeaponItem::GetStaticItemSlot() const
{
    return RowCast<FC_WeaponRow>()->StaticItemSlot;
}

UStaticMesh* UC_WeaponItem::GetStaticMesh() const
{
    return RowCast<FC_WeaponRow>()->StaticMesh;
}

ESkerItemSlot UC_WeaponItem::GetSkeletalItemSlot() const
{
    return RowCast<FC_WeaponRow>()->SkeletalItemSlot;
}

int UC_BuildingPartItem::GetMaxHp() const
{
    return RowCast<FC_ItemBuildingPartRow>()->MaxHp;
}

UStaticMesh* UC_BuildingPartItem::GetMesh() const
{
    return RowCast<FC_ItemBuildingPartRow>()->Mesh;
}

TSubclassOf<AActor> UC_BuildingPartItem::GetActorClass() const
{
    return RowCast<FC_ItemBuildingPartRow>()->ActorClass;
}

TEnumAsByte<ETraceTypeQuery> UC_BuildingPartItem::GetTraceType() const
{
    return RowCast<FC_ItemBuildingPartRow>()->TraceType;
}

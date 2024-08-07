// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/C_Items.h"

#include "Map/C_ItemRows.h"
#include "STS/C_STSGlobalFunctions.h"
#include "Player/Global/C_MapPlayer.h"

bool UC_Item::IsCraftable() const
{
    return GetCraftMaterials().IsEmpty();
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

void UC_Consumable::Use(UWorld* _World) const
{
    AC_MapPlayer* Player = UC_STSGlobalFunctions::GetMapPlayerCharacter(_World);
    if (false == Player->IsValidLowLevel())
    {
        return;
    }

    Player->AddHp(GetHp());
    Player->Addstamina(GetStamina());
}

int UC_Consumable::GetHp() const
{
    return 0;
}

int UC_Consumable::GetStamina() const
{
    return 0;
}

int UC_ItemMaterial::GetMaxCount() const
{
    return RowCast<FC_MaterialRow>()->MaxCount;
}

int UC_Weapon::GetDamage() const
{
    return RowCast<FC_WeaponRow>()->Damage;
}

bool UC_Weapon::GetIsStatic() const
{
    return RowCast<FC_WeaponRow>()->IsStatic;
}

EStaticItemSlot UC_Weapon::GetStaticItemSlot() const
{
    return RowCast<FC_WeaponRow>()->StaticItemSlot;
}

UStaticMesh* UC_Weapon::GetStaticMesh() const
{
    return RowCast<FC_WeaponRow>()->StaticMesh;
}

ESkerItemSlot UC_Weapon::GetSkeletalItemSlot() const
{
    return RowCast<FC_WeaponRow>()->SkeletalItemSlot;
}

int UC_ItemBuildingPart::GetMaxHp() const
{
    return RowCast<FC_ItemBuildingPartRow>()->MaxHp;
}

UStaticMesh* UC_ItemBuildingPart::GetMesh() const
{
    return RowCast<FC_ItemBuildingPartRow>()->Mesh;
}

TSubclassOf<AActor> UC_ItemBuildingPart::GetActorClass() const
{
    return RowCast<FC_ItemBuildingPartRow>()->ActorClass;
}

TEnumAsByte<ETraceTypeQuery> UC_ItemBuildingPart::GetTraceType() const
{
    return RowCast<FC_ItemBuildingPartRow>()->TraceType;
}

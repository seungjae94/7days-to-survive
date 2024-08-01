// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/C_Items.h"

#include "Map/C_ItemRows.h"
#include "STS/C_STSGlobalFunctions.h"
#include "Player/Global/C_MapPlayer.h"

bool UC_Item::IsCraftable() const
{
    return !CraftMaterials.IsEmpty();
}

void UC_Consumable::Use(UWorld* _World) const
{
    AC_MapPlayer* Player = UC_STSGlobalFunctions::GetMapPlayerCharacter(_World);
    if (false == Player->IsValidLowLevel())
    {
        return;
    }

    Player->AddHp(Hp);
    Player->Addstamina(Stamina);
}

// Init functions

void UC_Item::Init(FName _Id, FC_ItemRow* _ItemRow)
{
    if (nullptr == _ItemRow)
    {
        UE_LOG(LogTemp, Fatal, TEXT("Row should be FC_ItemRow type."));
        return;
    }

    Id = _Id;
    Name = _ItemRow->Name;
    Type = _ItemRow->Type;
    Icon = _ItemRow->Icon;
    DropWeight = _ItemRow->DropWeight;
    CraftMaterials = _ItemRow->CraftMaterials;
}

void UC_ItemMaterial::Init(FName _Id, FC_MaterialRow* _ItemRow)
{
    Super::Init(_Id, _ItemRow);

    MaxCount = _ItemRow->MaxCount;
}

void UC_Weapon::Init(FName _Id, FC_WeaponRow* _ItemRow)
{
    Super::Init(_Id, _ItemRow);

    Damage = _ItemRow->Damage;
    IsStatic = _ItemRow->IsStatic;
    StaticItemSlot = _ItemRow->StaticItemSlot;
    StaticMesh = _ItemRow->StaticMesh;
    SkeletalItemSlot = _ItemRow->SkeletalItemSlot;
}

void UC_Consumable::Init(FName _Id, FC_ConsumableRow* _ItemRow)
{
    Super::Init(_Id, _ItemRow);

    Hp = _ItemRow->Hp;
    Stamina = _ItemRow->Stamina;
}

void UC_ItemBuildingPart::Init(FName _Id, FC_ItemBuildingPartRow* _ItemRow)
{
    Super::Init(_Id, _ItemRow);

    MaxHp = _ItemRow->MaxHp;
    Mesh = _ItemRow->Mesh;
    ActorClass = _ItemRow->ActorClass;
    TraceType = _ItemRow->TraceType;
}

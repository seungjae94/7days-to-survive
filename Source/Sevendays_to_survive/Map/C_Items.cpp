// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/C_Items.h"

#include "Map/C_ItemRows.h"
#include "STS/C_STSGlobalFunctions.h"
#include "Player/Global/C_MapPlayer.h"

void UC_Item::Init(FName _Id, FC_ItemRow* _ItemRow)
{
    if (nullptr == _ItemRow)
    {
        UE_LOG(LogTemp, Fatal, TEXT("Row should be a FC_ItemRow type."));
        return;
    }

    Id = _Id;
    Name = _ItemRow->Name;
    Type = _ItemRow->Type;
    Icon = _ItemRow->Icon;
    DropWeight = _ItemRow->DropWeight;
    CraftMaterials = _ItemRow->CraftMaterials;
}

bool UC_Item::IsCraftable() const
{
    return !CraftMaterials.IsEmpty();
}

void UC_ItemMaterial::Init(FName _Id, FC_ItemRow* _ItemRow)
{
    Super::Init(_Id, _ItemRow);

    FC_MaterialRow* Row = reinterpret_cast<FC_MaterialRow*>(_ItemRow);

    if (nullptr == Row)
    {
        UE_LOG(LogTemp, Fatal, TEXT("Row should be a FC_MaterialRow type."));
    }

    MaxCount = Row->MaxCount;
}

void UC_Weapon::Init(FName _Id, FC_ItemRow* _ItemRow)
{
    Super::Init(_Id, _ItemRow);

    FC_WeaponRow* TypeRow = reinterpret_cast<FC_WeaponRow*>(_ItemRow);

    if (nullptr == TypeRow)
    {
        UE_LOG(LogTemp, Fatal, TEXT("Row should be a FC_WeaponRow type."));
        return;
    }

    Damage = TypeRow->Damage;
    IsStatic = TypeRow->IsStatic;
    StaticItemSlot = TypeRow->StaticItemSlot;
    StaticMesh = TypeRow->StaticMesh;
    SkeletalItemSlot = TypeRow->SkeletalItemSlot;
}


void UC_Consumable::Init(FName _Id, FC_ItemRow* _ItemRow)
{
    Super::Init(_Id, _ItemRow);

    FC_ConsumableRow* TypeRow = reinterpret_cast<FC_ConsumableRow*>(_ItemRow);

    if (nullptr == TypeRow)
    {
        UE_LOG(LogTemp, Fatal, TEXT("Row should be a FC_ConsumableRow type."));
        return;
    }

    Hp = TypeRow->Hp;

    Stamina = TypeRow->Stamina;
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

void UC_ItemBuildingPart::Init(FName _Id, FC_ItemRow* _ItemRow)
{
    Super::Init(_Id, _ItemRow);

    FC_ItemBuildingPartRow* TypeRow = reinterpret_cast<FC_ItemBuildingPartRow*>(_ItemRow);

    if (nullptr == TypeRow)
    {
        UE_LOG(LogTemp, Fatal, TEXT("Row should be a FC_ItemBuildingPartRow type."));
        return;
    }

    MaxHp = TypeRow->MaxHp;
    Mesh = TypeRow->Mesh;
    ActorClass = TypeRow->ActorClass;
    TraceType = TypeRow->TraceType;
}

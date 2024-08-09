// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/C_Items.h"

#include "Map/C_ItemRows.h"
#include "STS/C_STSGlobalFunctions.h"
#include "Player/Global/C_MapPlayer.h"

bool UC_Item::IsCraftable() const
{
    return !BaseData.CraftMaterials.IsEmpty();
}

FString UC_Item::GetName() const
{
    return BaseData.Name;
}

void UC_ConsumableItem::Use(UWorld* _World) const
{
    AC_MapPlayer* Player = UC_STSGlobalFunctions::GetMapPlayerCharacter(_World);
    if (false == Player->IsValidLowLevel())
    {
        return;
    }

    Player->AddHp(ConsumableData.Hp);
    Player->Addstamina(ConsumableData.Stamina);
}
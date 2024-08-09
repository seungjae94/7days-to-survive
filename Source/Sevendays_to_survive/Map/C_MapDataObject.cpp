// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/C_MapDataObject.h"
#include "Map/C_Items.h"
#include "Map/C_ItemRows.h"
#include "Map/C_MapDataAsset.h"
#include "STS/C_STSInstance.h"
#include "STS/C_STSGlobalFunctions.h"
#include "STS/C_STSMacros.h"

void UC_MapDataObject::Init(UC_STSInstance* _Inst)
{
    Inst = _Inst;
    Asset = _Inst->GetMapDataAsset();

    Type_To_AccDropIds.Emplace(EItemType::Material);
    Type_To_AccDropIds.Emplace(EItemType::Consumable);
    Type_To_AccDropIds.Emplace(EItemType::Weapon);
    Type_To_AccDropWeights.Emplace(EItemType::Material);
    Type_To_AccDropWeights.Emplace(EItemType::Consumable);
    Type_To_AccDropWeights.Emplace(EItemType::Weapon);

    FString ContextString;

    for (TPair<EItemType, UDataTable*> Pair : Asset->ItemDetailDataTables)
    {
        EItemType ItemType = Pair.Key;
        UDataTable* ItemTable = Pair.Value;

        TArray<FName> RowNames = ItemTable->GetRowNames();

        for (FName RowName : RowNames)
        {
            // Convert FC_ItemRow to UC_Item.
            UC_Item* NewItem = nullptr;
            switch (ItemType)
            {
            case EItemType::Material:
            {
                UC_MaterialItem* CreatedItem = CreateItemObject<UC_MaterialItem, FC_MaterialRow>(Asset->ItemBaseDataTable, ItemTable, RowName, &NewItem);
                CreatedItem->MaterialData = *ItemTable->FindRow<FC_MaterialRow>(RowName, TEXT(""));
                break;
            }
            case EItemType::Weapon:
            {
                UC_WeaponItem* CreatedItem = CreateItemObject<UC_WeaponItem, FC_WeaponRow>(Asset->ItemBaseDataTable, ItemTable, RowName, &NewItem);
                CreatedItem->WeaponData = *ItemTable->FindRow<FC_WeaponRow>(RowName, TEXT(""));
                break;
            }
            case EItemType::Consumable:
            {
                UC_ConsumableItem* CreatedItem = CreateItemObject<UC_ConsumableItem, FC_ConsumableRow>(Asset->ItemBaseDataTable, ItemTable, RowName, &NewItem);
                CreatedItem->ConsumableData = *ItemTable->FindRow<FC_ConsumableRow>(RowName, TEXT(""));
                break;
            }
            case EItemType::BuildingPart:
            {
                UC_BuildingPartItem* CreatedItem = CreateItemObject<UC_BuildingPartItem, FC_ItemBuildingPartRow>(Asset->ItemBaseDataTable, ItemTable, RowName, &NewItem);
                CreatedItem->BuildingPartData = *ItemTable->FindRow<FC_ItemBuildingPartRow>(RowName, TEXT(""));
                break;
            }
            default:
                STS_FATAL("[%s] Item type is unset.", __FUNCTION__);
                return;
            }

            Items.Emplace(RowName, NewItem);

            if (true == NewItem->IsCraftable())
            {
                CraftItems.Add(NewItem->Id);
            }

            if (EItemType::BuildingPart == ItemType)
            {
                continue;
            }

            int PrevAcc = 0;
            if (false == Type_To_AccDropWeights[ItemType].IsEmpty())
            {
                PrevAcc = Type_To_AccDropWeights[ItemType].Last();
            }
            Type_To_AccDropWeights[ItemType].Add(PrevAcc + NewItem->BaseData.DropWeight);
            Type_To_AccDropIds[ItemType].Add(RowName);
        }

    }
}


TMap<FName, int> UC_MapDataObject::GetRandomDropItems()
{
    TMap<FName, int> Result;

    for (TPair<EItemType, int>& Pair : Asset->Type_To_DropPouchCount)
    {
        EItemType Type = Pair.Key;
        int Count = Pair.Value;

        for (int i = 0; i < Count; ++i)
        {
            // None drop test
            int NoneDropTestInt = Inst->GenerateRandomInt(0, 99);

            if (NoneDropTestInt < Asset->NoneDropWeights[Type])
            {
                continue;
            }

            // 0 3 1 2 0
            // 0 3 4 6 6 Acc
            /*
            * Rand -> Index
            * 0 -> 1
            * 1 -> 1
            * 2 -> 1
            * 3 -> 2
            * 4 -> 3
            * 5 -> 3
            */

            TArray<int>& AccDropWeights = Type_To_AccDropWeights[Type];
            int TotalAcc = AccDropWeights.Last();
            int RandomInt = Inst->GenerateRandomInt(0, TotalAcc - 1);
            int Index = BisectRight(AccDropWeights, RandomInt);
            FName Id = Type_To_AccDropIds[Type][Index];

            int MinCount = Asset->Type_To_DropItemMinCount[Type];
            int MaxCount = Asset->Type_To_DropItemMaxCount[Type];
            int RandomCount = Inst->GenerateRandomInt(MinCount, MaxCount);

            Result.Emplace(Id, RandomCount);
        }
    }

    return Result;
}


TArray<FName> UC_MapDataObject::GetCraftItems(int _Page, int _PageSize)
{
    TArray<FName> Result;

    int Page = _Page;
    int MaxPage = GetCraftListMaxPage(_PageSize);

    if (Page < 0)
    {
        Page = 0;
    }

    if (Page > MaxPage)
    {
        Page = MaxPage;
    }

    int Start = Page * _PageSize;
    int End = FMath::Min<int>({ Start + _PageSize - 1, CraftItems.Num() - 1 });

    for (int i = Start; i <= End; ++i)
    {
        Result.Add(CraftItems[i]);
    }

    return Result;
}

int UC_MapDataObject::GetCraftListMaxPage(int _PageSize)
{
    return (CraftItems.Num() - 1) / _PageSize;
}

int UC_MapDataObject::BisectRight(TArray<int>& _Arr, int _Value)
{
    // 0 3 1 2 0 Weights
    // 0 3 4 6 6 AccWeights
    /*
    * Rand -> Index
    * 0 -> 1
    * 1 -> 1
    * 2 -> 1
    * 3 -> 2
    * 4 -> 3
    * 5 -> 3
    */

    int L = 0;
    int R = _Arr.Num() - 1;

    while (R - L > 1)
    {
        int M = (L + R) / 2;

        if (_Value < _Arr[M])
        {
            R = M;
        }
        else
        {
            L = M + 1;
        }
    }

    return R;
}

const UC_Item* UC_MapDataObject::FindItem(FName _Id)
{
    if (true == Items.Contains(_Id))
    {
        return Items[_Id];
    }

    return nullptr;
}
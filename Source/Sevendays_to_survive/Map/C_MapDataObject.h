// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Map/C_MapEnums.h"
#include "Map/C_ItemRows.h"
#include "STS/C_STSMacros.h"
#include "C_MapDataObject.generated.h"

class UC_STSInstance;
class UC_MapDataAsset;
class UC_Item;

UCLASS(BlueprintType)
class SEVENDAYS_TO_SURVIVE_API UC_MapDataObject : public UObject
{
    GENERATED_BODY()

    friend UC_STSInstance;

public:
    UFUNCTION(BlueprintPure)
    TArray<FName> GetCraftItems(int _Page, int _PageSize);

    UFUNCTION(BlueprintPure)
    int GetCraftListMaxPage(int _PageSize);

    UFUNCTION(BlueprintPure)
    TMap<FName, int> GetRandomDropItems();

    UFUNCTION(BlueprintPure)
    const UC_Item* FindItem(FName _Id);

private:
    void Init(UC_STSInstance* _Inst);

    int BisectRight(TArray<int>& _Arr, int _Value);

    template<typename ItemObjectType, typename ItemRowType>
    ItemObjectType* CreateItemObject(UDataTable* _BaseDataTable, UDataTable* _DetailDataTable, FName _RowName, UC_Item** _ItemPtr)
    {
        FC_ItemBaseData* BaseData = _BaseDataTable->FindRow<FC_ItemBaseData>(_RowName, TEXT(""));
        ItemRowType* DetailData = _DetailDataTable->FindRow<ItemRowType>(_RowName, TEXT(""));
        ItemObjectType* ItemObject = NewObject<ItemObjectType>(this);
        if (nullptr == BaseData || nullptr == DetailData || false == ItemObject->IsValidLowLevel())
        {
            STS_FATAL("[%s] Failed to create item object.", TEXT(__FUNCTION__));
            return nullptr;
        }

        ItemObject->Id = _RowName;
        ItemObject->BaseData = *BaseData;
        *_ItemPtr = ItemObject;
        return ItemObject;
    }

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
    TMap<FName, UC_Item*> Items;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
    TArray<FName> CraftItems;

    // 가중치 기반 아이템 드롭
    TMap<EItemType, TArray<FName>> Type_To_AccDropIds;
    TMap<EItemType, TArray<int>> Type_To_AccDropWeights;

    UC_STSInstance* Inst = nullptr;
    UC_MapDataAsset* Asset = nullptr;
};
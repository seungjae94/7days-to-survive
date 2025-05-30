﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/C_ItemSourceHISMA.h"

#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "STS/C_STSInstance.h"
#include "STS/C_STSGlobalFunctions.h"
#include "Map/C_MapDataAsset.h"
#include "Map/C_ItemSourceTableRow.h"
#include "Map/C_Items.h"
#include "Player/Global/C_MapPlayer.h"
#include "Map/Inventory/C_InventoryComponent.h"
#include "STS/C_STSMacros.h"
#include "UI/C_HealthBar.h"
#include "Map/MapComponent/C_InstancedHpBarComponent.h"

AC_ItemSourceHISMA::AC_ItemSourceHISMA()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    HISMComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HierarchicalInstancedStaticMesh"));
    HISMComponent->SetIsReplicated(true);
    SetRootComponent(HISMComponent);

    HpBarComponent = CreateDefaultSubobject<UC_InstancedHpBarComponent>(TEXT("Hp Bar Component"));
    HpBarComponent->SetupAttachment(HISMComponent);
    HpBarComponent->SetIsReplicated(true);
}

void AC_ItemSourceHISMA::BeginPlay()
{
    Super::BeginPlay();

    if (true == Id.IsNone())
    {
        HpBarComponent->SetIsViewable(false);
        return;
    }

    UC_STSInstance* Inst = GetWorld()->GetGameInstanceChecked<UC_STSInstance>();
    UC_MapDataAsset* MapDataAsset = Inst->GetMapDataAsset();

    DropItems = MapDataAsset->GetItemSourceDropItems(Id);

    int MaxHp = MapDataAsset->GetItemSourceMaxHp(Id);
    int InstCount = HISMComponent->GetInstanceCount();

    HpBarComponent->SetMaxHp(MaxHp, InstCount);
}

void AC_ItemSourceHISMA::Damage_Implementation(int _Index, int _Damage, AActor* _HitActor)
{
    if (true == Id.IsNone())
    {
        return;
    }

    HpBarComponent->DecHp(_Index, _Damage);

    // 아이템 획득
    NetMulticast_GainDropItems(Cast<AC_MapPlayer>(_HitActor));

    if (true == HpBarComponent->IsZero(_Index))
    {
        NetMulticast_RemoveInst(_Index);
        return;
    }
}

void AC_ItemSourceHISMA::NetMulticast_GainDropItems_Implementation(AC_MapPlayer* _ItemGainer)
{
    if (false == _ItemGainer->IsValidLowLevel() || false == _ItemGainer->IsLocallyControlled())
    {
        return;
    }

    UC_InventoryComponent* InventoryComponent = _ItemGainer->GetComponentByClass<UC_InventoryComponent>();
    if (nullptr == InventoryComponent)
    {
        STS_FATAL("[%s] InventoryComponent is NULL.", __FUNCTION__);
        return;
    }

    for (TPair<FName, int>& DropItem : DropItems)
    {
        const UC_Item* Item = UC_STSGlobalFunctions::FindItem(GetWorld(), DropItem.Key);
        InventoryComponent->AddItem(Item, DropItem.Value);
    }
}

void AC_ItemSourceHISMA::NetMulticast_RemoveInst_Implementation(int _Index)
{
    HISMComponent->RemoveInstance(_Index);
}

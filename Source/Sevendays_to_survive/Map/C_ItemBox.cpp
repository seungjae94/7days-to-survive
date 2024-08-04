// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/C_ItemBox.h"

#include "STS/C_STSGlobalFunctions.h"
#include "Map/C_ItemPouch.h"
#include "Map/C_MapDataAsset.h"
#include "Map/C_MapDataObject.h"
#include "Map/UI/C_MapDamageTakerComponent.h"

// Sets default values
AC_ItemBox::AC_ItemBox()
{
	bReplicates = true;
}

// Called when the game starts or when spawned
void AC_ItemBox::BeginPlay()
{
	Super::BeginPlay();

	MapDataAsset = UC_STSGlobalFunctions::GetMapDataAsset(GetWorld());
	MapDataMemory = UC_STSGlobalFunctions::GetMapDataObject(GetWorld());
	ItemPouchClass = MapDataAsset->GetItemPouchClass();

	DamageTakerComponent->SetMaxHp(MapDataAsset->GetItemBoxMaxHp());
	DamageTakerComponent->SetHp(MapDataAsset->GetItemBoxMaxHp());
}

// Called every frame
void AC_ItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_ItemBox::PreDestroy()
{
	SpawnItems();
}

void AC_ItemBox::SpawnItems_Implementation()
{
	TMap<FName, int> DropItems = MapDataMemory->GetRandomDropItems();

	for (TPair<FName, int>& ItemAndCount : DropItems)
	{
		AC_ItemPouch* ItemPouch = GetWorld()->SpawnActor<AC_ItemPouch>(ItemPouchClass.Get(), GetActorTransform());
		ItemPouch->SetItemAndCount(ItemAndCount.Key, ItemAndCount.Value);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/C_ItemPouch.h"

#include "STS/C_STSGlobalFunctions.h"
#include "Map/Inventory/C_InventoryComponent.h"
#include "Map/C_MapDataAsset.h"
#include "Map/PlayerComponent/C_MapInteractionComponent.h"
#include "Map/MapComponent/C_ItemMessageComponent.h"

// Sets default values
AC_ItemPouch::AC_ItemPouch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(StaticMeshComponent);

	ItemMessageComp = CreateDefaultSubobject<UC_ItemMessageComponent>(TEXT("Interaction Widget Component"));
	ItemMessageComp->SetupAttachment(StaticMeshComponent);
	ItemMessageComp->SetIsReplicated(true);
}

void AC_ItemPouch::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AC_ItemPouch::SetItemAndCount_Implementation(FName _Id, int _Count)
{
	ItemMessageComp->SetItemAndCount(_Id, _Count);
}

// Called when the game starts or when spawned
void AC_ItemPouch::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AC_ItemPouch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


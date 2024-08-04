// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/BuildingSystem/C_BuildingPart.h"

#include "STS/C_STSGlobalFunctions.h"
#include "Map/C_MapDataAsset.h"
#include "Map/C_MapDataObject.h"
#include "Map/MapComponent/C_HpBarComponent.h"

AC_BuildingPart::AC_BuildingPart()
{
}

void AC_BuildingPart::BeginPlay()
{
	Super::BeginPlay();

	MapDataAsset = UC_STSGlobalFunctions::GetMapDataAsset(GetWorld());
	MapDataMemory = UC_STSGlobalFunctions::GetMapDataObject(GetWorld());
}

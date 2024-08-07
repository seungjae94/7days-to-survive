// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/C_MapDamageTaker.h"
#include "Components/StaticMeshComponent.h"
#include "C_BuildingPart.generated.h"

class UC_MapDataAsset;
class UC_MapDataObject;

UCLASS()
class SEVENDAYS_TO_SURVIVE_API AC_BuildingPart : public AC_MapDamageTaker
{
	GENERATED_BODY()
	
public:
	AC_BuildingPart();

protected:
	void BeginPlay() override;

private:
	UC_MapDataAsset* MapDataAsset = nullptr;
	UC_MapDataObject* MapDataObject = nullptr;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Map/C_MapEnums.h"
#include "Player/Global/C_PlayerEnum.h"
#include "C_ItemRows.generated.h"

USTRUCT(BlueprintType)
struct SEVENDAYS_TO_SURVIVE_API FC_ItemBaseData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Type = EItemType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int> CraftMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DropWeight = 0;
};

USTRUCT(BlueprintType)
struct SEVENDAYS_TO_SURVIVE_API FC_MaterialItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxCount = 0;
};

USTRUCT(BlueprintType)
struct SEVENDAYS_TO_SURVIVE_API FC_WeaponItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsStatic = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStaticItemSlot StaticItemSlot = EStaticItemSlot::SlotMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkerItemSlot SkeletalItemSlot = ESkerItemSlot::SlotMax;
};

USTRUCT(BlueprintType)
struct SEVENDAYS_TO_SURVIVE_API FC_ConsumableItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Hp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Stamina = 0;
};

USTRUCT(BlueprintType)
struct SEVENDAYS_TO_SURVIVE_API FC_BuildingPartItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETraceTypeQuery> TraceType = ETraceTypeQuery::TraceTypeQuery1;
};
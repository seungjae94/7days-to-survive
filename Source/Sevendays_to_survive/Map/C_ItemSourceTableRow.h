// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Map/C_Items.h"
#include "C_ItemSourceTableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SEVENDAYS_TO_SURVIVE_API FC_ItemSourceTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Hp = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int> DropItems;
};

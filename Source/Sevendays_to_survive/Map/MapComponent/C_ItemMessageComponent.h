// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/MapComponent/C_MapMessageComponent.h"
#include "C_ItemMessageComponent.generated.h"

class UC_Item;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_ItemMessageComponent : public UC_MapMessageComponent
{
    GENERATED_BODY()

public:
    UC_ItemMessageComponent();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void SetItemAndCount(FName _ItemId, int _Count);
	void SetItemAndCount_Implementation(FName _ItemId, int _Count);

	void MapInteract() override;

private:
	UPROPERTY(ReplicatedUsing = SetMessage)
	FName ItemId;

	UPROPERTY(ReplicatedUsing = SetMessage)
	int Count = 0;

	void SetMessage() override;
};

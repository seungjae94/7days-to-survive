// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/UI/C_MapActorInteractionComponent.h"
#include "C_PickUpItemComponent.generated.h"

class UC_Item;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_PickUpItemComponent : public UC_MapActorInteractionComponent
{
    GENERATED_BODY()

public:
    UC_PickUpItemComponent();

	void SetMessage_Implementation() override;

	UFUNCTION(Server, Reliable)
	void SetItemAndCount(const UC_Item* _Item, int _Count);
	void SetItemAndCount_Implementation(const UC_Item* _Item, int _Count);

	void MapInteract() override;

private:
	const UC_Item* Item = nullptr;
	int Count = 0;
};

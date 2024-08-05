// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/BuildingSystem/C_BuildingPart.h"
#include "C_Door.generated.h"

class UC_DoorMessageComponent;

UCLASS()
class SEVENDAYS_TO_SURVIVE_API AC_Door : public AC_BuildingPart
{
	GENERATED_BODY()

public:
	AC_Door();

private:
	UPROPERTY()
	UC_DoorMessageComponent* DoorMessageComp = nullptr;
};

	

// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/BuildingSystem/C_Door.h"
#include "STS/C_STSGlobalFunctions.h"
#include "Map/MapComponent/C_DoorMessageComponent.h"

AC_Door::AC_Door()
{
    DoorMessageComp = CreateDefaultSubobject<UC_DoorMessageComponent>(TEXT("Door Message Component"));
    DoorMessageComp->SetupAttachment(SMComponent);
    DoorMessageComp->SetIsReplicated(true);
}
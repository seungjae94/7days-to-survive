﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Main_Levels/C_7Days_Play_GameMode.h"

#include "STS/C_STSInstance.h" // 게임인스턴스
#include "Kismet/GameplayStatics.h"
#include "Map/C_ItemBox.h"
#include "Sevendays_to_survive.h"

AC_7Days_Play_GameMode::AC_7Days_Play_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AC_7Days_Play_GameMode::BeginPlay()
{
	Super::BeginPlay();

	//SpawnItemBoxes();
}

void AC_7Days_Play_GameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	EndingCheck();
}

void AC_7Days_Play_GameMode::EndingCheck()
{
	UC_STSInstance* Inst = GetGameInstance<UC_STSInstance>();
	if (nullptr == Inst)
	{
		return;
	}

	if (Inst->Game_End == false)
	{
		return;
	}
	GetWorld()->ServerTravel(TEXT("7Days_Ending_Level?listen"));
}

void AC_7Days_Play_GameMode::SpawnItemBoxes()
{
	UC_STSInstance* Inst = GetGameInstance<UC_STSInstance>();
	if (nullptr == Inst)
	{
		return;
	}

	float AreaWidth = (TotalArea.Max.Y - TotalArea.Min.Y) / ItemBoxSpawnColumnCount;
	float AreaHeight = (TotalArea.Max.X - TotalArea.Min.X) / ItemBoxSpawnRowCount;

	for (int x = 0; x < ItemBoxSpawnRowCount; ++x)
	{
		for (int y = 0; y < ItemBoxSpawnColumnCount; ++y)
		{
			if (x >= ItemBoxSpawnBorderWidth
				&& ItemBoxSpawnRowCount - 1 - x >= ItemBoxSpawnBorderWidth
				&& y >= ItemBoxSpawnBorderWidth
				&& ItemBoxSpawnColumnCount - 1 - y >= ItemBoxSpawnBorderWidth)
			{
				continue;
			}

			float AreaStartY = TotalArea.Min.Y + y * AreaWidth;
			float AreaStartX = TotalArea.Min.X + x * AreaHeight;

			FBox2D Area = FBox2D{
				{AreaStartX, AreaStartY},
				{AreaStartX + AreaHeight, AreaStartY + AreaWidth}
			};

			for (int c = 0; c < ItemBoxSpawnCountPerArea; ++c)
			{
				FVector RandomVec = Inst->GenerateRandomVector(Area);
				RandomVec.Z = 10000.0f;

				FHitResult OutHit;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(this);

				GetWorld()->LineTraceSingleByChannel(
					OutHit, 
					RandomVec, 
					RandomVec + FVector::DownVector * RandomVec.Z,
					ECC_Visibility,
					Params
				);

				AC_ItemBox* ItemBox = GetWorld()->SpawnActor<AC_ItemBox>(ItemBoxClass.Get());
				ItemBox->SetActorLocation(OutHit.Location);
			}
		}
	}
}


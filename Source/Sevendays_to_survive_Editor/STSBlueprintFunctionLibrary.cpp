﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "STSBlueprintFunctionLibrary.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"
#include "Map/C_ItemRows.h"
#include <Kismet/DataTableFunctionLibrary.h>

TArray<FString> USTSBlueprintFunctionLibrary::ItemNameList;
FString USTSBlueprintFunctionLibrary::FolderName;
TMap<FString, TArray<FString>> USTSBlueprintFunctionLibrary::AllNameList;



void USTSBlueprintFunctionLibrary::ItemDataTableSetting(UDataTable* DataTable, FString& _FolderName)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	FString DefaultFile = TEXT("");
	FString DefaultPath = FPaths::ProjectContentDir();
	FString dir = FPaths::ProjectSavedDir();
	FString Result;

	if (DesktopPlatform) {
		DesktopPlatform->OpenDirectoryDialog(
			NULL,
			TEXT("Select Directory"),	// 탐색기 제목
			DefaultPath,	// 열고싶은 탐색기 경로
			Result	// 선택한 디렉토리 경로가 여기에 들어옴
		);
	}

	Result += TEXT("/*.*");

	TArray<FString> FoundFileName;

	IFileManager::Get().FindFiles(FoundFileName, *Result, true, true);

	// IFileManager::Get().FindFiles(FoundFileName, *Result, *fileExt);

	//내가 특정한 폴더의 레퍼런스 경로가 Result인데 절대경로임 상대경로로 바꿔주는 함수있냐

	if (nullptr == DataTable)
	{
		return;
	}


	TArray<FString> PathElements;

	Result.ParseIntoArray(PathElements, TEXT("Content"));

	//레퍼런스 경로 뒤에 붙여줄 것
	FString FragmentPath = PathElements[1];

	//type명 마지막 선택 폴더명 넣어주기
	TArray<FString> FolderNameElements;

	FragmentPath.ParseIntoArray(FolderNameElements, TEXT("InventoryIcon"), true);

	FolderName = FolderNameElements[1];

	FolderName = FolderName.Replace(TEXT("*.*"), TEXT(""));

	FolderName = FolderName.Replace(TEXT("/"), TEXT(""));

	_FolderName = FolderName;

	FString FinalPath = TEXT("/Script/Engine.Texture2D'/Game") + FragmentPath;

	FinalPath = FinalPath.Replace(TEXT("*.*"), TEXT(""));

	for (size_t i = 0; i < FoundFileName.Num(); i++)
	{
		FC_ItemBaseData ItemDataTable = FC_ItemBaseData(); // [ Name: X, Type : X, ...  ]


		FString FileName = FoundFileName[i];
		FileName = FileName.Replace(TEXT(".uasset"), TEXT(""));
		FString IconPath = FinalPath + FileName + "." + FileName + "'";
		FileName[0] = TChar<TCHAR>::ToUpper(FileName[0]);


		int a = 0;

		IconPath;
		FolderName;


		UEnum* Enum = StaticEnum<EItemType>();

		int Value = Enum->GetIndexByName(*FolderName);

		//TSoftClassPtr<UTexture2D> IconMesh;

		//UTexture2D* IconTextrue = LoadObject<UTexture2D*>(nullptr, IconPath);

		UTexture2D* IconTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *IconPath));

		ItemDataTable.Name = FileName;
		ItemDataTable.Type = static_cast<EItemType>(Value);
		ItemDataTable.Icon = IconTexture;

		//기존에 들어있는 내용은 무시하고 새로운 내용만 추가해줘야 함
		FString ContextString;
		FTableRowBase* FoundRow = DataTable->FindRow<FTableRowBase>(*ItemDataTable.Name, ContextString);

		if (nullptr != FoundRow)
		{
			continue;
		}
		
		//새로운 내용만 리스트에 추가
		ItemNameList.Push(FileName);

		DataTable->AddRow(*ItemDataTable.Name, ItemDataTable); // 저 행을 데이터 테이블에 추가
	}

	DataTable->Modify();
	int a = 0;
}

void USTSBlueprintFunctionLibrary::BuildingPartDataTableSetting(UDataTable* DataTable)
{

	int a = 0;
}

void USTSBlueprintFunctionLibrary::ItemTypeCheck(FString _FolderName, EItemType& ItemType)
{
	//FolderName 받아서 ItemType이랑 
	FString ItemPartName = _FolderName;


	if (TEXT("Consumable") == ItemPartName)
	{
		ItemType = EItemType::Consumable;
	}
	else if (TEXT("Material") == ItemPartName)
	{
		ItemType = EItemType::Material;

	}
	else if (TEXT("Weapon") == ItemPartName)
	{
		ItemType = EItemType::Weapon;
	}

	int ab = 0;

}

void USTSBlueprintFunctionLibrary::ConsumableDataRowSetting(UDataTable* DataTable, EItemType ItemType)
{
	
	for (size_t i = 0; i < ItemNameList.Num(); i++)
	{

		FC_ConsumableItemData ConsumableDataTable = FC_ConsumableItemData();

		FString ConsumableDataName = ItemNameList[i];
		DataTable->AddRow(*ConsumableDataName, ConsumableDataTable);
	}

	ItemNameList.Empty();
	DataTable->Modify();

	ItemNameList.Empty();

	int a = 0;

}

void USTSBlueprintFunctionLibrary::MaterialDataRowSetting(UDataTable* DataTable, EItemType ItemType)
{

	for (size_t i = 0; i < ItemNameList.Num(); i++)
	{
		FC_MaterialItemData MaterialDataTable = FC_MaterialItemData();

		FString MaterialRowDataName = ItemNameList[i];

		DataTable->AddRow(*MaterialRowDataName, MaterialDataTable);
	}

	ItemNameList.Empty();
	DataTable->Modify();

	ItemNameList.Empty();
}

void USTSBlueprintFunctionLibrary::WeaponDataRowSetting(UDataTable* DataTable, EItemType ItemType)
{
	
	for (size_t i = 0; i < ItemNameList.Num(); i++)
	{

		FC_WeaponItemData WeaponDataTable = FC_WeaponItemData();

		FString WeaponRowDataName = ItemNameList[i];
		DataTable->AddRow(*WeaponRowDataName, WeaponDataTable);
	}

	DataTable->Modify();

	ItemNameList.Empty();
}


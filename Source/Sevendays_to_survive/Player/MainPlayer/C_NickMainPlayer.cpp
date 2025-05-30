﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayer/C_NickMainPlayer.h"
#include "Player/Global/C_GlobalAnimInstance.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Player/Global/C_PlayerEnum.h"
#include "Player/Input/C_InputActionDatas.h"
#include "Components/ArrowComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/CapsuleComponent.h"
#include "Map/C_Items.h"
#include "Map/BuildingSystem/C_BuildingComponent.h"
#include "Map/Inventory/C_InventoryComponent.h"
#include "STS/C_STSInstance.h"
#include "STS/C_STSGlobalFunctions.h"




AC_NickMainPlayer::AC_NickMainPlayer()
{
	LocationComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("LocationComponent"));
	LocationComponent->SetupAttachment(RootComponent); // RootComponent에 부착
}

FVector AC_NickMainPlayer::GetComponentLocation()
{
	FVector Location = LocationComponent->GetComponentLocation();
	return Location;
}

void AC_NickMainPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AC_NickMainPlayer, MontageToPlay);
}

void AC_NickMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (true == HasAuthority()) {
		UC_STSInstance* Inst = Cast<UC_STSInstance>(GetGameInstance());
		Inst->AddPlayerArray(this);
	}
}

void AC_NickMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_NickMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

AActor* AC_NickMainPlayer::SpawnMapCamera(const FTransform& _Transform)
{
	if (MapCameraComponent == nullptr) {
		UE_LOG(LogTemp, Fatal, TEXT(""));
	}
	if (GetController() == UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		AActor* Camera = GetWorld()->SpawnActor(MapCameraComponent, &_Transform);
		if (nullptr == Camera) {
			UE_LOG(LogTemp, Fatal, TEXT(""));
		}
		FAttachmentTransformRules AttachRule(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, true);
		Camera->AttachToComponent(GetCapsuleComponent(), AttachRule);
		return Camera;
	}
	return nullptr;
}

void AC_NickMainPlayer::OnQuickSlotSelected(int _Index)
{
	InventoryComp->SetCurQuickSlot(_Index);
	const UC_Item* Item = InventoryComp->GetQuickSlotItem(_Index);

	ChangeNoWeaponServer();
	BuildingComp->HoldBuildingPart(FName());

	if (nullptr == Item)
	{
		return;
	}

	switch (Item->BaseData.Type)
	{
	case EItemType::Weapon:
		OnWeaponSelected(Cast<const UC_WeaponItem>(Item));
		break;
	case EItemType::BuildingPart:
		OnBuildingPartSelected(Cast<const UC_BuildingPartItem>(Item));
		break;
	default:
		break;
	}

}

void AC_NickMainPlayer::OnWeaponSelected(const UC_WeaponItem* _Weapon)
{
	if (true == _Weapon->WeaponData.IsStatic)
	{
		ChangeSlotMeshServer(_Weapon->WeaponData.StaticItemSlot, _Weapon->WeaponData.StaticMesh);
	}
	else
	{
		ChangeSlotSkeletalServer(_Weapon->WeaponData.SkeletalItemSlot);
	}
}

void AC_NickMainPlayer::OnBuildingPartSelected(const UC_BuildingPartItem* _BuildingPart)
{
	BuildingComp->HoldBuildingPart(_BuildingPart->Id);
}


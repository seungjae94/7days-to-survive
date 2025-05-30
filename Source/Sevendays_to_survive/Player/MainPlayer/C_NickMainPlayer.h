// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Global/C_MapPlayer.h"
#include "C_NickMainPlayer.generated.h"

class UAnimMontage;
class UC_WeaponItem;
class UC_BuildingPartItem;

UCLASS()
class SEVENDAYS_TO_SURVIVE_API AC_NickMainPlayer : public AC_MapPlayer
{
	GENERATED_BODY()
public:
	AC_NickMainPlayer();

	FVector GetComponentLocation();

	UFUNCTION(BlueprintCallable)
	void OnQuickSlotSelected(int _Index);

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // 리플리케이트를 설정하기 위한 함수 필수!
	// Called when the game starts or when spawned
	void BeginPlay() override;
	// Called every frame
	void Tick(float DeltaTime) override;

	// 입력 콜백 처리
	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	AActor* SpawnMapCamera(const FTransform& _Transform);

	UFUNCTION(BlueprintCallable)
	void OnWeaponSelected(const UC_WeaponItem* _Weapon);

	UFUNCTION(BlueprintCallable)
	void OnBuildingPartSelected(const UC_BuildingPartItem* _BuildingPart);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* LocationComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> MapCameraComponent;
};

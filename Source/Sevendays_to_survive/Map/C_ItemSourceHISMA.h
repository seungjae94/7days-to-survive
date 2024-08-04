// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Map/C_ItemSourceTableRow.h"
#include "Components/WidgetComponent.h"

#include "C_ItemSourceHISMA.generated.h"

class UC_Item;
class AC_MapPlayer;
class UC_HealthBar;
class UC_InstancedHpBarComponent;

UCLASS()
class SEVENDAYS_TO_SURVIVE_API AC_ItemSourceHISMA : public AActor
{
	GENERATED_BODY()
	
public:
	AC_ItemSourceHISMA();


public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	virtual void Damage(int _Index, int _Damage, AActor* _HitActor);
	void Damage_Implementation(int _Index, int _Damage, AActor* _HitActor);

	UFUNCTION(NetMulticast, Reliable)
	void GainDropItems(AC_MapPlayer* _ItemGainer);
	void GainDropItems_Implementation(AC_MapPlayer* _ItemGainer);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UHierarchicalInstancedStaticMeshComponent* HISMComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UC_InstancedHpBarComponent* HpBarComponent = nullptr;
	
	// Drop items.
	TMap<FName, int> DropItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Id", meta = (AllowPrivateAccess = "true"))
	FName Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta=(AllowPrivateAccess = "true"))
	FC_ItemSourceTableRow Row;
};

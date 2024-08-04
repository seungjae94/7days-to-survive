// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Map/C_Items.h"

#include "C_ItemPouch.generated.h"

class UC_ItemMessageComponent;

UCLASS()
class SEVENDAYS_TO_SURVIVE_API AC_ItemPouch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_ItemPouch();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void SetItemAndCount(FName _Id, int _Count);

	void SetItemAndCount_Implementation(FName _Id, int _Count);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UC_ItemMessageComponent* ItemMessageComp = nullptr;
};

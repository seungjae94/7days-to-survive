// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_MonsterSpawnPoint.generated.h"

UCLASS()
class SEVENDAYS_TO_SURVIVE_API AC_MonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_MonsterSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MonsterSpawn(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SetSpawn(bool _IsSpawn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> SpawnZombie;
	
	UFUNCTION(BlueprintCallable)
	UClass* ZombieClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime = 1.0f;

	UPROPERTY()
	float CoolTime = 0.0f;

	bool CanSpawn = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* SpawnBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* CollisionCheckBoxComponent;

	FVector GetRandomPointInBox() const;

	UFUNCTION(BlueprintCallable)
	void ReduceSpawnArea(FVector2D _ReduceValue);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool SpawnOn = false;

	UPROPERTY()
	uint8 SpawnCount = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 SpawnEndCount = 10;

	UFUNCTION(BlueprintCallable)
	void IncreaseZombie();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsBeginSpawnPoint = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsBoss = false;
};

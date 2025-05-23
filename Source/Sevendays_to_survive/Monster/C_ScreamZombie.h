// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/C_ZombieBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "C_ScreamZombie.generated.h"

/**
 * 
 */
UCLASS()
class SEVENDAYS_TO_SURVIVE_API AC_ScreamZombie : public AC_ZombieBase
{
	GENERATED_BODY()

public:
	AC_ScreamZombie();

	void BeginPlay() override final;
	void Tick(float _DeltaTime) override final;

	void SetName(FString _Name) override final;

	UFUNCTION(NetMulticast, Reliable)
	void Idle() override final;
	void Idle_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void ShoutAttack() override final;
	void ShoutAttack_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Attack() override final;
	void Attack_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void RunAttack() override final;
	void RunAttack_Implementation();

	void Run(FVector _Location) override final;
	void Move(FVector _Location) override final;	

	void OnAttackNotifyBegin()override final;
	void OnAttackNotifyEnd()override final;

	//UFUNCTION(BlueprintCallable, Category = "AI")
	//void MakeNoise(float _Loudness);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool ShoutBegin = false;
};

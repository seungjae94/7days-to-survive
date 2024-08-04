// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/C_MapDamageTaker.h"

#include "STS/C_STSMacros.h"
#include "Map/MapComponent/C_HpBarComponent.h"
#include "Kismet/GameplayStatics.h"

AC_MapDamageTaker::AC_MapDamageTaker()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SMComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM Component"));
	SetRootComponent(SMComponent);

	HpBarComp = CreateDefaultSubobject<UC_HpBarComponent>(TEXT("HpBar"));
	HpBarComp->SetupAttachment(SMComponent);
	HpBarComp->SetIsReplicated(true);
}

void AC_MapDamageTaker::SetMaxHp_Implementation(int _MaxHp)
{
	HpBarComp->SetMaxHp(_MaxHp);
}

float AC_MapDamageTaker::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ReceiveDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return DamageAmount;
}

// Called when the game starts or when spawned
void AC_MapDamageTaker::BeginPlay()
{
	Super::BeginPlay();
}

void AC_MapDamageTaker::ReceiveDamage_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HpBarComp->DecHp(DamageAmount);

	if (true == HpBarComp->IsZero())
	{
		PreDestroy();
		Destroy();
	}
}

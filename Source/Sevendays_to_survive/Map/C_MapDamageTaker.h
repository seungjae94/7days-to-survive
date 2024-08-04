// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/C_MapActor.h"
#include "Components/WidgetComponent.h"
#include "C_MapDamageTaker.generated.h"

class UC_MapDamageTakerComponent;

// It can be damaged and has HP-bar widget.
UCLASS(Abstract)
class SEVENDAYS_TO_SURVIVE_API AC_MapDamageTaker : public AC_MapActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_MapDamageTaker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(Server, Reliable)
	void SetMaxHp(int _MaxHp);
	void SetMaxHp_Implementation(int _MaxHp);

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UFUNCTION(Server, Reliable)
	void ReceiveDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	void ReceiveDamage_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual void PreDestroy() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SMComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UC_MapDamageTakerComponent* DamageTakerComponent = nullptr;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_TestChar_Coach.generated.h"

UCLASS()
class SEVENDAYS_TO_SURVIVE_API AC_TestChar_Coach : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_TestChar_Coach();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

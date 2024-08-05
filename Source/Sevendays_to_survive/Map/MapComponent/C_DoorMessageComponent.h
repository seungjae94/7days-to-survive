// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/MapComponent/C_MapMessageComponent.h"
#include "C_DoorMessageComponent.generated.h"

enum class EDoorState : uint8
{
	Opened,
	Closed,
	Opening,
	Closing,
};

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_DoorMessageComponent : public UC_MapMessageComponent
{
    GENERATED_BODY()

public:
    void MapInteract() override;

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    void SetMessage() override;
	void Open();
	void Close();

	FVector DoorForward;
	FVector DoorRight;
	FVector SpawnLocation;
	FRotator SpawnRotation;
	FVector BoxExtent;

	EDoorState DoorState = EDoorState::Closed;
	float Theta = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float OpenTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	double RotAxisRadius = 5.0;
};

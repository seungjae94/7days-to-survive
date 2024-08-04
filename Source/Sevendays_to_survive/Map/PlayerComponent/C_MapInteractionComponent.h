// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_MapInteractionComponent.generated.h"

class UC_MapMessageComponent;
class UCameraComponent;
class AC_MapPlayer;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_MapInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UC_MapInteractionComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UFUNCTION(BlueprintCallable, Server, Reliable)
    void Server_DestroyActor(AActor* _Actor);
    void Server_DestroyActor_Implementation(AActor* _Actor);
private:
    UFUNCTION(BlueprintPure)
    bool IsServer() const;

    UFUNCTION(BlueprintPure)
    bool IsOwnerLocallyControlled() const;

    UFUNCTION(BlueprintPure)
    FVector GetTraceStartPoint() const;

    UFUNCTION(BlueprintPure)
    FVector GetTraceEndPoint() const;

    UFUNCTION(BlueprintPure)
    FRotator GetCameraRotation() const;

    UFUNCTION(BlueprintCallable)
    void OnMapInteractionKeyDown();

    UFUNCTION(Server, Reliable)
    void Server_MapInteract(UC_MapMessageComponent* _Component);
    void Server_MapInteract_Implementation(UC_MapMessageComponent* _Component);

private:
    UPROPERTY(Category = "HpBar", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    float TraceStartRange = 200.0f;

    UPROPERTY(Category = "HpBar", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    float TraceEndRange = 800.0f;

    UPROPERTY(Category = "HpBar", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FVector BoxHalfSize = { 50.0f, 50.0f, 50.0f };

    AC_MapPlayer* Owner = nullptr;
    UCameraComponent* CameraComponent = nullptr;

    UPROPERTY()
    AActor* ViewingActor = nullptr;

private:
    void View(int _Index, AActor* _Actor);
    void Unview(int _Index);
};

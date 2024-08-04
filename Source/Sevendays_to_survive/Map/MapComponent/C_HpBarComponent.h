// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/MapComponent/C_MapWidgetComponent.h"
#include "C_HpBarComponent.generated.h"

class UC_HealthBar;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_HpBarComponent : public UC_MapWidgetComponent
{
    GENERATED_BODY()

public:
    UC_HpBarComponent();

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(Server, Reliable)
    void SetMaxHp(int _MaxHp);
    void SetMaxHp_Implementation(int _MaxHp);

    UFUNCTION()
    int GetHp() const;

    UFUNCTION(Server, Reliable)
    void SetHp(int _Hp);
    void SetHp_Implementation(int _Hp);

    UFUNCTION(Server, Reliable)
    void DecHp(int _Hp);
    void DecHp_Implementation(int _Hp);

    UFUNCTION()
    bool IsZero() const;

protected:
    virtual void BeginPlay() override;

private:
    UC_HealthBar* GetHpBarWidget();

    UPROPERTY(ReplicatedUsing = OnRep_MaxHp)
    int MaxHp = 0;

    UPROPERTY(ReplicatedUsing = OnRep_Hp)
    int Hp = 0;

    UFUNCTION()
    void OnRep_MaxHp();

    UFUNCTION()
    void OnRep_Hp();
};

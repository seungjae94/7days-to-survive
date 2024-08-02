// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/UI/C_MapWidgetComponent.h"
#include "C_HpBarWidgetComponent.generated.h"

class UC_HealthBar;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_HpBarWidgetComponent : public UC_MapWidgetComponent
{
    GENERATED_BODY()

public:
    UC_HpBarWidgetComponent();

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(Server, Reliable, BlueprintCallable)
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

    UFUNCTION()
    void OnReplicated_MaxHp();

    UFUNCTION()
    void OnReplicated_Hp();

protected:
    virtual void BeginPlay() override;

private:
    UC_HealthBar* HpBarWidget = nullptr;

    UPROPERTY(ReplicatedUsing = OnReplicated_MaxHp)
    int MaxHp = 0;

    UPROPERTY(ReplicatedUsing = OnReplicated_Hp)
    int Hp = 0;
};

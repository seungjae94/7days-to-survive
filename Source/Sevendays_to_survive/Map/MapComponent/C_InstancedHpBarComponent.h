// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/MapComponent/C_MapWidgetComponent.h"
#include "C_InstancedHpBarComponent.generated.h"

class UC_HealthBar;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_InstancedHpBarComponent : public UC_MapWidgetComponent
{
    GENERATED_BODY()

public:
    UC_InstancedHpBarComponent();

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void ShowWidget(int _Index = 0) override;
    void HideWidget() override;

    UFUNCTION(Server, Reliable)
    void SetMaxHp(int _MaxHp, int _InstCount);
    void SetMaxHp_Implementation(int _MaxHp, int _InstCount);

    UFUNCTION()
    int GetHp(int _Index) const;

    UFUNCTION(Server, Reliable)
    void SetHp(int _Index, int _Hp);
    void SetHp_Implementation(int _Index, int _Hp);

    UFUNCTION(Server, Reliable)
    void DecHp(int _Index, int _Hp);
    void DecHp_Implementation(int _Index, int _Hp);

    UFUNCTION()
    bool IsZero(int _Index) const;

private:
    UC_HealthBar* GetHpBarWidget();

    UPROPERTY(ReplicatedUsing = OnRep_MaxHp)
    int MaxHp = 0;

    UPROPERTY(ReplicatedUsing = OnRep_Hp)
    TArray<int> HpArray;

    UFUNCTION()
    void OnRep_MaxHp();

    UFUNCTION()
    void OnRep_Hp();

    bool IsViewing = false;
    int ViewingIndex = 0;
};

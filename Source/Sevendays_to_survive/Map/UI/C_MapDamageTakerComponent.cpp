// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/UI/C_MapDamageTakerComponent.h"

#include "Net/UnrealNetwork.h"
#include "UI/C_HealthBar.h"

UC_MapDamageTakerComponent::UC_MapDamageTakerComponent()
{
    static const TCHAR* ResourcePath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Level/TestLevel/UI/WidgetBP/InGame/BP_ObjectHealthBar.BP_ObjectHealthBar_C'");
    static ConstructorHelpers::FClassFinder<UC_HealthBar> WidgetAsset(ResourcePath);
    if (true == WidgetAsset.Succeeded())
    {
        WidgetClass = WidgetAsset.Class;
    }
    else
    {
        UE_LOG(LogTemp, Fatal, TEXT("Can't find asset."));
        return;
    }
}

void UC_MapDamageTakerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UC_MapDamageTakerComponent, MaxHp);
    DOREPLIFETIME(UC_MapDamageTakerComponent, Hp);
}

void UC_MapDamageTakerComponent::BeginPlay()
{
    Super::BeginPlay();

    GetHpBarWidget()->SetCurHealth(Hp);
    GetHpBarWidget()->SetMaxHealth(MaxHp);

    HideWidget();
}

UC_HealthBar* UC_MapDamageTakerComponent::GetHpBarWidget()
{
    return Cast<UC_HealthBar>(GetWidget());
}

void UC_MapDamageTakerComponent::SetMaxHp_Implementation(int _MaxHp)
{
    MaxHp = _MaxHp;
    GetHpBarWidget()->SetMaxHealth(MaxHp);
}

int UC_MapDamageTakerComponent::GetHp() const
{
    return Hp;
}

void UC_MapDamageTakerComponent::SetHp_Implementation(int _Hp)
{
    Hp = _Hp;
    GetHpBarWidget()->SetCurHealth(Hp);
}

void UC_MapDamageTakerComponent::DecHp_Implementation(int _Hp)
{
    Hp -= _Hp;

    if (Hp <= 0)
    {
        Hp = 0;
    }

    GetHpBarWidget()->SetCurHealth(Hp);
}

bool UC_MapDamageTakerComponent::IsZero() const
{
    return Hp <= 0;
}

void UC_MapDamageTakerComponent::OnRep_MaxHp()
{
    GetHpBarWidget()->SetMaxHealth(MaxHp);
}

void UC_MapDamageTakerComponent::OnRep_Hp()
{
    GetHpBarWidget()->SetCurHealth(Hp);
}
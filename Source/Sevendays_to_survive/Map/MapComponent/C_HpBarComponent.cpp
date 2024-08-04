// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/MapComponent/C_HpBarComponent.h"

#include "Net/UnrealNetwork.h"
#include "UI/C_HealthBar.h"

UC_HpBarComponent::UC_HpBarComponent()
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

void UC_HpBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UC_HpBarComponent, MaxHp);
    DOREPLIFETIME(UC_HpBarComponent, Hp);
}

void UC_HpBarComponent::BeginPlay()
{
    Super::BeginPlay();

    GetHpBarWidget()->SetCurHealth(Hp);
    GetHpBarWidget()->SetMaxHealth(MaxHp);

    HideWidget();
}

UC_HealthBar* UC_HpBarComponent::GetHpBarWidget()
{
    InitWidget();
    return Cast<UC_HealthBar>(GetWidget());
}

void UC_HpBarComponent::SetMaxHp_Implementation(int _MaxHp)
{
    MaxHp = _MaxHp;
    GetHpBarWidget()->SetMaxHealth(MaxHp);
}

int UC_HpBarComponent::GetHp() const
{
    return Hp;
}

void UC_HpBarComponent::SetHp_Implementation(int _Hp)
{
    Hp = _Hp;
    GetHpBarWidget()->SetCurHealth(Hp);
}

void UC_HpBarComponent::DecHp_Implementation(int _Hp)
{
    Hp -= _Hp;

    if (Hp <= 0)
    {
        Hp = 0;
    }

    GetHpBarWidget()->SetCurHealth(Hp);
}

bool UC_HpBarComponent::IsZero() const
{
    return Hp <= 0;
}

void UC_HpBarComponent::OnRep_MaxHp()
{
    GetHpBarWidget()->SetMaxHealth(MaxHp);
}

void UC_HpBarComponent::OnRep_Hp()
{
    GetHpBarWidget()->SetCurHealth(Hp);
}
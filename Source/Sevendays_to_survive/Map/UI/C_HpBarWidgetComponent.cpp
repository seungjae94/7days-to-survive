// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/UI/C_HpBarWidgetComponent.h"

#include "Net/UnrealNetwork.h"
#include "UI/C_HealthBar.h"

UC_HpBarWidgetComponent::UC_HpBarWidgetComponent()
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

void UC_HpBarWidgetComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UC_HpBarWidgetComponent, MaxHp);
    DOREPLIFETIME(UC_HpBarWidgetComponent, Hp);
}

void UC_HpBarWidgetComponent::BeginPlay()
{
    Super::BeginPlay();

    HpBarWidget = Cast<UC_HealthBar>(GetWidget());

    HideWidget();
}

void UC_HpBarWidgetComponent::SetMaxHp_Implementation(int _MaxHp)
{
    MaxHp = _MaxHp;
}

int UC_HpBarWidgetComponent::GetHp() const
{
    return Hp;
}

void UC_HpBarWidgetComponent::SetHp_Implementation(int _Hp)
{
    Hp = _Hp;
}

void UC_HpBarWidgetComponent::DecHp_Implementation(int _Hp)
{
    Hp -= _Hp;

    if (Hp <= 0)
    {
        Hp = 0;
    }
}

bool UC_HpBarWidgetComponent::IsZero() const
{
    return Hp <= 0;
}

void UC_HpBarWidgetComponent::OnReplicated_MaxHp()
{
    HpBarWidget->SetMaxHealth(MaxHp);
}

void UC_HpBarWidgetComponent::OnReplicated_Hp()
{
    HpBarWidget->SetCurHealth(Hp);
}

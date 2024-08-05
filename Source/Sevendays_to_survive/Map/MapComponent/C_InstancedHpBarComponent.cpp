// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/MapComponent/C_InstancedHpBarComponent.h"

#include "Net/UnrealNetwork.h"
#include "UI/C_HealthBar.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "STS/C_STSMacros.h"

UC_InstancedHpBarComponent::UC_InstancedHpBarComponent()
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

void UC_InstancedHpBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UC_InstancedHpBarComponent, MaxHp);
    DOREPLIFETIME(UC_InstancedHpBarComponent, HpArray);
}

void UC_InstancedHpBarComponent::ShowWidget(int _Index)
{
    Super::ShowWidget(_Index);

    if (true == HpArray.IsEmpty())
    {
        return;
    }

    IsViewing = true;
    ViewingIndex = _Index;

    GetHpBarWidget()->SetCurHealth(HpArray[_Index]);
    GetHpBarWidget()->SetMaxHealth(MaxHp);

    UInstancedStaticMeshComponent* ISMComp = GetOwner()->GetComponentByClass<UInstancedStaticMeshComponent>();
    if (false == ISMComp->IsValidLowLevel())
    {
        STS_FATAL("[%s] ISMComp is null.", __FUNCTION__);
        return;
    }

    FTransform Trans;
    ISMComp->GetInstanceTransform(_Index, Trans, true);

    FVector Loc = Trans.GetLocation() + FVector::UpVector * 100.0f;
    SetWorldLocation(Loc);
}

void UC_InstancedHpBarComponent::HideWidget()
{
    Super::HideWidget();
    IsViewing = false;
}

void UC_InstancedHpBarComponent::SetMaxHp_Implementation(int _MaxHp, int _InstCount)
{
    // 서버
    MaxHp = _MaxHp;
    for (int i = 0; i < _InstCount; ++i)
    {
        HpArray.Add(MaxHp);
    }

    GetHpBarWidget()->SetCurHealth(MaxHp);
    GetHpBarWidget()->SetMaxHealth(MaxHp);
}

int UC_InstancedHpBarComponent::GetHp(int _Index) const
{
    return HpArray[_Index];
}

void UC_InstancedHpBarComponent::SetHp_Implementation(int _Index, int _Hp)
{
    // 서버
    HpArray[_Index] = _Hp;

    if (true == IsViewing && ViewingIndex == _Index)
    {
        GetHpBarWidget()->SetCurHealth(HpArray[ViewingIndex]);
    }
}

void UC_InstancedHpBarComponent::DecHp_Implementation(int _Index, int _Hp)
{
    HpArray[_Index] -= _Hp;

    if (0 >= HpArray[_Index])
    {
        HpArray[_Index] = 0;
    }

    if (true == IsViewing && ViewingIndex == _Index)
    {
        GetHpBarWidget()->SetCurHealth(HpArray[ViewingIndex]);
    }
}

bool UC_InstancedHpBarComponent::IsZero(int _Index) const
{
    return HpArray[_Index] <= 0;
}

UC_HealthBar* UC_InstancedHpBarComponent::GetHpBarWidget()
{
    InitWidget();
    return Cast<UC_HealthBar>(GetWidget());
}

void UC_InstancedHpBarComponent::OnRep_MaxHp()
{
    GetHpBarWidget()->SetMaxHealth(MaxHp);
}

void UC_InstancedHpBarComponent::OnRep_Hp()
{
    if (true == IsViewing)
    {
        GetHpBarWidget()->SetCurHealth(HpArray[ViewingIndex]);
    }
}

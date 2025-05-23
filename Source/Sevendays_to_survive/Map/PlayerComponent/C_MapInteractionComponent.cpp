﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/PlayerComponent/C_MapInteractionComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Map/MapComponent/C_MapMessageComponent.h"
#include "Map/C_ItemSourceHISMA.h"
#include "Map/C_MapDamageTaker.h"
#include "Map/BuildingSystem/C_Door.h"
#include "Map/BuildingSystem/C_Door.h"
#include "Player/Global/C_MapPlayer.h"
#include "Camera/CameraComponent.h"
#include "STS/C_STSMacros.h"
#include "Map/MapComponent/C_MapWidgetComponent.h"

UC_MapInteractionComponent::UC_MapInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UC_MapInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    Owner = GetOwner<AC_MapPlayer>();
    CameraComponent = Owner->GetComponentByClass<UCameraComponent>();
}

void UC_MapInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (false == IsOwnerLocallyControlled())
    {
        return;
    }

    FHitResult OutHit;

    bool IsBlocked = UKismetSystemLibrary::BoxTraceSingle(
        GetWorld(),
        GetTraceStartPoint(),
        GetTraceEndPoint(),
        BoxHalfSize,
        FRotator::ZeroRotator,
        UEngineTypes::ConvertToTraceType(ECC_Visibility),
        false,
        TArray{ GetOwner() },
        EDrawDebugTrace::None,
        OutHit,
        true
    );

    if (nullptr == OutHit.GetActor())
    {
        Unview();
        return;
    }

    View(OutHit.Item, OutHit.GetActor());
}

bool UC_MapInteractionComponent::IsServer() const
{
    return UKismetSystemLibrary::IsServer(GetWorld());
}

bool UC_MapInteractionComponent::IsOwnerLocallyControlled() const
{
    return Owner->IsLocallyControlled();
}

FVector UC_MapInteractionComponent::GetTraceStartPoint() const
{
    FVector CameraLocation = CameraComponent->GetComponentLocation();
    FVector CameraForward = CameraComponent->GetForwardVector();
    return CameraLocation + CameraForward * TraceStartRange;
}

FVector UC_MapInteractionComponent::GetTraceEndPoint() const
{
    FVector CameraLocation = CameraComponent->GetComponentLocation();
    FVector CameraForward = CameraComponent->GetForwardVector();
    return CameraLocation + CameraForward * TraceEndRange;
}

FRotator UC_MapInteractionComponent::GetCameraRotation() const
{
    return CameraComponent->GetComponentRotation();
}

void UC_MapInteractionComponent::OnMapInteractionKeyDown()
{
    if (false == IsValid(ViewingActor)) 
    {
        return;
    }
    
    TArray<UC_MapMessageComponent*> Comps;
    ViewingActor->GetComponents<UC_MapMessageComponent>(Comps);
    for (UC_MapMessageComponent* Comp : Comps)
    {
        if (true == Comp->IsInteractOnServer())
        {
            Server_MapInteract(Comp);
        }
        else
        {
            Comp->MapInteract();
        }
    }
}

void UC_MapInteractionComponent::Server_MapInteract_Implementation(UC_MapMessageComponent* _Component)
{
    _Component->MapInteract();
}

void UC_MapInteractionComponent::Server_DestroyActor_Implementation(AActor* _Actor)
{
    if (nullptr == _Actor)
    {
        return;
    }

    _Actor->Destroy();
}

void UC_MapInteractionComponent::View(int _Index, AActor* _Actor)
{
    TArray<UC_MapWidgetComponent*> MapWidgetComps;
    _Actor->GetComponents<UC_MapWidgetComponent>(MapWidgetComps);

    TArray<UC_MapWidgetComponent*> ViewableMapWidgetComps;
    for (UC_MapWidgetComponent* MapWidgetComp : MapWidgetComps)
    {
        if (true == MapWidgetComp->IsViewable())
        {
            ViewableMapWidgetComps.Add(MapWidgetComp);
        }
    }

    // 액터를 안보게 되는 경우
    if (true == ViewableMapWidgetComps.IsEmpty())
    {
        Unview();
        return;
    }

    // 같은 액터를 계속 보는 경우
    if (ViewingActor == _Actor)
    {
        ShowWidgets(ViewableMapWidgetComps, _Index);
        return;
    }

    // 액터를 안보고 있다가 보는 경우
    if (false == IsValid(ViewingActor))
    {
        ViewingActor = _Actor;
        ShowWidgets(ViewableMapWidgetComps, _Index);
        return;
    }

    // 기존에 보고 있던 액터와 다른 액터를 보는 경우
    Unview();
    ViewingActor = _Actor;
    ShowWidgets(ViewableMapWidgetComps, _Index);
}

void UC_MapInteractionComponent::ShowWidgets(TArray<UC_MapWidgetComponent*>& _ViewableMapWidgetComps, int _Index)
{
    for (UC_MapWidgetComponent* MapWidgetComp : _ViewableMapWidgetComps)
    {
        MapWidgetComp->ShowWidget(_Index);
    }
}

void UC_MapInteractionComponent::Unview()
{
    if (true == IsValid(ViewingActor))
    {
        TArray<UC_MapWidgetComponent*> MapWidgetComps;
        ViewingActor->GetComponents<UC_MapWidgetComponent>(MapWidgetComps);

        for (UC_MapWidgetComponent* MapWidgetComp : MapWidgetComps)
        {
            MapWidgetComp->HideWidget();
        }

        ViewingActor = nullptr;
    }
}

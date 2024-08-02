// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/C_MapInteractionComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Map/C_ItemSourceHISMA.h"
#include "Map/C_MapInteractable.h"
#include "Map/C_MapDamageTaker.h"
#include "BuildingSystem/C_Door.h"
#include "Player/Global/C_MapPlayer.h"
#include "Camera/CameraComponent.h"
#include "STS/C_STSMacros.h"
#include "BuildingSystem/C_Door.h"
#include "Map/UI/C_MapWidgetComponent.h"

UC_MapInteractionComponent::UC_MapInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UC_MapInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    Owner = GetOwner<AC_MapPlayer>();
    CameraComponent = Owner->GetComponentByClass<UCameraComponent>();

    IsInteractingMap.Emplace(EMapInteractionTarget::ItemSource, false);
    IsInteractingMap.Emplace(EMapInteractionTarget::MapDamageTaker, false);
    IsInteractingMap.Emplace(EMapInteractionTarget::ItemPouch, false);
    IsInteractingMap.Emplace(EMapInteractionTarget::Door, false);
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

    View(OutHit.GetActor());
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
    
    if (AC_MapInteractable* MapInteractable = Cast<AC_MapInteractable>(ViewingActor))
    {
        Server_MapInteract(MapInteractable);
    }
}

void UC_MapInteractionComponent::Server_MapInteract_Implementation(AC_MapInteractable* _MapInteractable)
{
    _MapInteractable->MapInteract();
}

void UC_MapInteractionComponent::Server_DestroyActor_Implementation(AActor* _Actor)
{
    if (nullptr == _Actor)
    {
        return;
    }

    _Actor->Destroy();
}

//
//void UC_MapInteractionComponent::ViewItemSource(AC_ItemSourceHISMA* _ItemSource, int _Index)
//{
//    // 같은 아이템 소스를 계속 보는 경우
//    if (ViewingItemSource == _ItemSource)
//    {
//        ViewingItemSource->UpdateHpBar(_Index);
//        return;
//    }
//
//    // 다른 아이템 소스를 보게 되는 경우
//    if (true == IsValid(ViewingItemSource))
//    {
//        ViewingItemSource->HideHpBar();
//    }
//    ViewingItemSource = _ItemSource;
//    if (true == IsValid(ViewingItemSource))
//    {
//        ViewingItemSource->UpdateHpBar(_Index);
//    }
//}

void UC_MapInteractionComponent::View(AActor* _Actor)
{
    UC_MapWidgetComponent* MapWidgetComp = _Actor->GetComponentByClass<UC_MapWidgetComponent>();

    if (nullptr == MapWidgetComp)
    {
        Unview();
        return;
    }

    /*if (true == OutHit.GetActor()->IsA<AC_ItemSourceHISMA>())
    {
        ViewItemSource(Cast<AC_ItemSourceHISMA>(OutHit.GetActor()), OutHit.Item);
    }*/

    // 액터를 안보고 있다가 보는 경우 
    if (false == IsValid(ViewingActor))
    {
        MapWidgetComp->ShowWidget();
        return;
    }
    
    // 같은 액터를 계속 보는 경우
    if (ViewingActor == _Actor)
    {
        MapWidgetComp->ShowWidget();
        return;
    }

    // 다른 액터를 보게 되는 경우
    Unview();
    ViewingActor = _Actor;
    MapWidgetComp->ShowWidget();
}

void UC_MapInteractionComponent::Unview()
{
    if (true == IsValid(ViewingActor))
    {
        UC_MapWidgetComponent* MapWidgetComp = ViewingActor->GetComponentByClass<UC_MapWidgetComponent>();
        
        if (nullptr == MapWidgetComp)
        {
            STS_FATAL("[%s] ViewingActor has no MapWidgetComp.", __FUNCTION__);
            return;
        }
        
        MapWidgetComp->HideWidget();
        ViewingActor = nullptr;
    }
}

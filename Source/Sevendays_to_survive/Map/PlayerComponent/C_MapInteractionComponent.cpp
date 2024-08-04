// Fill out your copyright notice in the Description page of Project Settings.

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
        Unview(OutHit.Item);
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
        Comp->MapInteract();
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
    UC_MapWidgetComponent* MapWidgetComp = _Actor->GetComponentByClass<UC_MapWidgetComponent>();

    if (nullptr == MapWidgetComp || false == MapWidgetComp->IsActive())
    {
        Unview(_Index);
        return;
    }

    // 액터를 안보고 있다가 보는 경우 
    if (false == IsValid(ViewingActor))
    {
        ViewingActor = _Actor;
        MapWidgetComp->ShowWidget(_Index);
        return;
    }
    
    // 같은 액터를 계속 보는 경우
    if (ViewingActor == _Actor)
    {
        return;
    }

    // 다른 액터를 보게 되는 경우
    Unview(_Index);
    ViewingActor = _Actor;
    MapWidgetComp->ShowWidget(_Index);
}

void UC_MapInteractionComponent::Unview(int _Index)
{
    if (true == IsValid(ViewingActor))
    {
        UC_MapWidgetComponent* MapWidgetComp = ViewingActor->GetComponentByClass<UC_MapWidgetComponent>();
        
        if (nullptr == MapWidgetComp || false == MapWidgetComp->IsActive())
        {
            STS_FATAL("[%s] ViewingActor has no MapWidgetComp.", __FUNCTION__);
            return;
        }
        
        MapWidgetComp->HideWidget(_Index);
        ViewingActor = nullptr;
    }
}

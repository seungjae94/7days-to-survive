// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/UI/C_InteractionMessageWidgetComponent.h"
#include "C_ItemInfoMessageWidgetComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_ItemInfoMessageWidgetComponent : public UC_InteractionMessageWidgetComponent
{
    GENERATED_BODY()

public:
    UC_ItemInfoMessageWidgetComponent();
//
//    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
//
//    UFUNCTION(Server, Reliable)
//    void SetMessageForItemInfo(const FString& _ItemName, int _Count);
//
//    UFUNCTION(Server, Reliable)
//    void SetMessageForDoor();
//
//protected:
//    virtual void BeginPlay() override;
//
//private:
//    EInteractionMessageType MessageType = EInteractionMessageType::None;
//    UC_InteractionMessageWidget* InteractionMessageWidget = nullptr;
//    FString Message;
};

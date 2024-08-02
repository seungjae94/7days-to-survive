// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/UI/C_MapWidgetComponent.h"
#include "C_InteractionMessageWidgetComponent.generated.h"

class UC_InteractionMessageWidget;

enum class EInteractionMessageType : uint8
{
    None,
    ItemInfo,
    Door,
    MAX,
};

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SEVENDAYS_TO_SURVIVE_API UC_InteractionMessageWidgetComponent : public UC_MapWidgetComponent
{
    GENERATED_BODY()

public:
    UC_InteractionMessageWidgetComponent();

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(Server, Reliable)
    void SetMessageForItemInfo(const FString& _ItemName, int _Count);
    void SetMessageForItemInfo_Implementation(const FString& _ItemName, int _Count);

    UFUNCTION(Server, Reliable)
    void SetMessageForDoor();
    void SetMessageForDoor_Implementation();

protected:
    virtual void BeginPlay() override;

private:
    EInteractionMessageType MessageType = EInteractionMessageType::None;
    UC_InteractionMessageWidget* InteractionMessageWidget = nullptr;
    FString Message;
};

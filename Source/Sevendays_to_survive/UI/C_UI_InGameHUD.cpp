﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/C_UI_InGameHUD.h"
#include "UI/C_UITableRow.h"
#include "STS/C_STSMacros.h"
#include "STS/C_STSInstance.h"
#include "Blueprint/WidgetTree.h"
#include "UI/Inventory/C_UI_InventoryCore.h"
#include "UI/C_UI_InGameMainWidget.h"
#include "UI/C_UI_InGameWidgetCore.h"

void AC_UI_InGameHUD::DrawHUD()
{
    Super::DrawHUD();

    // if (CrosshairTexture)
    // {
    //     // 캔버스 중심을 찾습니다.
    //     FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
    //
    //     // 텍스처 중심이 캔버스 중심에 맞도록 텍스처의 크기 절반 만큼 오프셋을 줍니다.
    //     FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));
    //
    //     // 중심점에 조준선을 그립니다.
    //     FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
    //     TileItem.BlendMode = SE_BLEND_Translucent;
    //     Canvas->DrawItem(TileItem);
    // }
}

UC_UI_InventoryCore* AC_UI_InGameHUD::GetInventoryCore()
{
    UC_UI_InventoryCore* InventoryCore = Cast<UC_UI_InventoryCore>(Widgets[EUIType::Inventory]);
    return InventoryCore;
}

UC_UI_QuickSlot* AC_UI_InGameHUD::GetQuickSlotWidget()
{
    return WidgetCore->GetQuickSlotWidget();
}

void AC_UI_InGameHUD::BeginPlay()
{
    Super::BeginPlay();

    UC_STSInstance* Inst = GetWorld()->GetGameInstanceChecked<UC_STSInstance>();
    TMap<FName, FUIWidgetDataRow*> WidgetDataMap = Inst->GetUIWidgetDataMap();

    if (nullptr == MainWidgetClass)
    {
        STS_FATAL("[%s] MainWidgetClass is unset.", __FUNCTION__);
    }

    MainWidget = CreateWidget<UC_UI_InGameMainWidget>(GetWorld(), MainWidgetClass);
    MainWidget->AddToViewport();

    UPanelWidget* PanelWidget = Cast<UPanelWidget>(MainWidget->GetWidgetFromName(TEXT("MainPanel")));

    UEnum* Enum = StaticEnum<EUIType>();

    for (TPair<FName, FUIWidgetDataRow*>& Pair : WidgetDataMap)
    {
        FUIWidgetDataRow* Row = Pair.Value;

        UUserWidget* Widget = MainWidget->WidgetTree->ConstructWidget<UUserWidget>(Row->WidgetClass, Pair.Key);
        Widget->AddToViewport();

        EUIType Type = static_cast<EUIType>(Enum->GetValueByName(Pair.Key));

        Widgets.Add(Type, Widget);
        WidgetDatas.Add(Type, *Row);

        if (Row->Key != FKey())
        {
            WidgetKeyDatas.Add(Row->Key, *Row);
            WidgetKeys.Add(Row->Key, Widget);
        }

        PanelWidget->AddChild(Widget);

        if (false == Row->StartOn)
        {
            Widget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}
void AC_UI_InGameHUD::UIOff(EUIType _Type)
{
    UISetVisibility(_Type, ESlateVisibility::Hidden);
}

void AC_UI_InGameHUD::UIOn(EUIType _Type)
{
    UISetVisibility(_Type, ESlateVisibility::Visible);
}

void AC_UI_InGameHUD::UIVisibilitySwitch(EUIType _Type)
{
    UUserWidget** WidgetPtr = Widgets.Find(_Type);
    if (nullptr == WidgetPtr)
    {
        return;
    }
    UUserWidget* Widget = *WidgetPtr;

    if (ESlateVisibility::Visible == Widget->GetVisibility())
    {
        UIOff(_Type);
    }
    else if (ESlateVisibility::Hidden == Widget->GetVisibility())
    {
        UIOn(_Type);
    }
}

void AC_UI_InGameHUD::UISetVisibility(EUIType _Type, ESlateVisibility _Value)
{
    UUserWidget** WidgetPtr = Widgets.Find(_Type);
    if (nullptr == WidgetPtr)
    {
        STS_FATAL("[%s] Invalid EUIType.", __FUNCTION__)
            return;
    }
    UUserWidget* Widget = *WidgetPtr;
    if (nullptr == Widget)
    {
        STS_FATAL("[%s] Widget is null.", __FUNCTION__)
            return;
    }

    if (ESlateVisibility::Visible == _Value)
    {
        FUIWidgetDataRow* Row = WidgetDatas.Find(_Type);

        if (Row->OpenMode == EUIInputMode::UIOnly)
        {
            APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

            FInputModeUIOnly UIOnly;
            UIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            Controller->SetInputMode(UIOnly);
            Controller->bShowMouseCursor = true;
            Widget->SetFocus();
            UIOnlyWidgets.Add(Widget);
        }

        OpenWidget.Add(Widget);
        ++AllUIViewCount;
    }
    else if (ESlateVisibility::Hidden == _Value)
    {
        --AllUIViewCount;
        OpenWidget.Remove(Widget);
        UIOnlyWidgets.Remove(Widget);

        if (0 == AllUIViewCount || true == UIOnlyWidgets.IsEmpty())
        {
            APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            FInputModeGameOnly Option;
            Option.SetConsumeCaptureMouseDown(true);
            Controller->SetInputMode(Option);
            Controller->bShowMouseCursor = false;
        }
        else if (false == UIOnlyWidgets.IsEmpty())
        {
            UIOnlyWidgets[UIOnlyWidgets.Num() - 1]->SetFocus();
        }
    }
    Widget->SetVisibility(_Value);
}

void AC_UI_InGameHUD::UIVisibilitySwitchKey(FKey _Key)
{
    if (true == WidgetKeys.IsEmpty())
    {
        return;
    }

    if (false == WidgetKeys.Contains(_Key))
    {
        return;
    }

    UUserWidget** WidgetPtr = WidgetKeys.Find(_Key);
    if (nullptr == WidgetPtr)
    {
        STS_FATAL("[%s] Invalid FKey.", __FUNCTION__)
            return;
    }
    UUserWidget* Widget = *WidgetPtr;
    if (nullptr == Widget)
    {
        STS_FATAL("[%s] Widget is null.", __FUNCTION__)
            return;
    }

    if (ESlateVisibility::Visible == Widget->GetVisibility())
    {
        UISetVisibilityKey(_Key, ESlateVisibility::Hidden);
    }
    else if (ESlateVisibility::Hidden == Widget->GetVisibility())
    {
        UISetVisibilityKey(_Key, ESlateVisibility::Visible);
    }
}

void AC_UI_InGameHUD::UISetVisibilityKey(FKey _Key, ESlateVisibility _Value)
{
    UUserWidget** WidgetPtr = WidgetKeys.Find(_Key);
    if (nullptr == WidgetPtr)
    {
        STS_FATAL("[%s] Invalid FKey.", __FUNCTION__)
            return;
    }
    UUserWidget* Widget = *WidgetPtr;
    if (nullptr == Widget)
    {
        STS_FATAL("[%s] Widget is null.", __FUNCTION__)
            return;
    }

    if (_Value == ESlateVisibility::Visible)
    {
        FUIWidgetDataRow* Row = WidgetKeyDatas.Find(_Key);
        if (Row->OpenMode == EUIInputMode::UIOnly)
        {
            APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

            FInputModeUIOnly Option;
            Option.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            Controller->SetInputMode(Option);
            Controller->bShowMouseCursor = true;
            Widget->SetFocus();
            UIOnlyWidgets.Add(Widget);
        }

        OpenWidget.Add(Widget);
        ++AllUIViewCount;
    }
    else if (_Value == ESlateVisibility::Hidden)
    {
        --AllUIViewCount;
        OpenWidget.Remove(Widget);
        UIOnlyWidgets.Remove(Widget);

        if (0 == AllUIViewCount || true == UIOnlyWidgets.IsEmpty())
        {
            APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            FInputModeGameOnly Option;
            Option.SetConsumeCaptureMouseDown(true);
            Controller->SetInputMode(Option);
            Controller->bShowMouseCursor = false;
        }
        else if (false == UIOnlyWidgets.IsEmpty())
        {
            UIOnlyWidgets[UIOnlyWidgets.Num() - 1]->SetFocus();
        }
    }

    Widget->SetVisibility(_Value);
}
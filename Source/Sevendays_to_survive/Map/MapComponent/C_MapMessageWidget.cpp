// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapComponent/C_MapMessageWidget.h"

void UC_MapMessageWidget::SetMessage(const FString& _Text)
{
    GetTextBlock()->SetText(FText::FromString(_Text));
}

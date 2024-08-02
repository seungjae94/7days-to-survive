// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/UI/C_InteractionMessageWidget.h"

void UC_InteractionMessageWidget::SetMessage(const FString& _Text)
{
    GetTextBlock()->SetText(FText::FromString(_Text));
}

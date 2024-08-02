// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_InteractionMessageWidget.generated.h"

UCLASS(Blueprintable)
class SEVENDAYS_TO_SURVIVE_API UC_InteractionMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetMessage(const FString& _Text);
};

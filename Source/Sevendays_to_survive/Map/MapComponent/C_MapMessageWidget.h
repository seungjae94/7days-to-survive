// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "C_MapMessageWidget.generated.h"

UCLASS(Blueprintable)
class SEVENDAYS_TO_SURVIVE_API UC_MapMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetMessage(const FString& _Text);

	UFUNCTION(BlueprintImplementableEvent)
	UTextBlock* GetTextBlock();
};

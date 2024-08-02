#include "Map/UI/C_InteractionMessageWidgetComponent.h"
#include "Map/UI/C_InteractionMessageWidget.h"

UC_InteractionMessageWidgetComponent::UC_InteractionMessageWidgetComponent()
{
    static const TCHAR* ResourcePath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Level/Main_Level/MapContent/UI/WB_MapInteractionWidget.WB_MapInteractionWidget_C'");
    static ConstructorHelpers::FClassFinder<UC_InteractionMessageWidget> WidgetAsset(ResourcePath);
    if (true == WidgetAsset.Succeeded())
    {
        WidgetClass = WidgetAsset.Class;
    }
    else
    {
        UE_LOG(LogTemp, Fatal, TEXT("Can't find asset."));
        return;
    }
}

void UC_InteractionMessageWidgetComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
}

void UC_InteractionMessageWidgetComponent::SetMessageForItemInfo_Implementation(const FString& _ItemName, int _Count)
{
    Message = _ItemName + TEXT(" × ") + FString::FromInt(_Count);
    InteractionMessageWidget->SetMessage(Message);
}

void UC_InteractionMessageWidgetComponent::SetMessageForDoor_Implementation()
{
    InteractionMessageWidget->SetMessage(TEXT("Open Door"));
}

void UC_InteractionMessageWidgetComponent::BeginPlay()
{
    Super::BeginPlay();

    InteractionMessageWidget = Cast<UC_InteractionMessageWidget>(GetWidget());
    
    HideWidget();
}
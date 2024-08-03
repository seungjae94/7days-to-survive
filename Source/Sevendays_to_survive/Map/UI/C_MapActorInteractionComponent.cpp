#include "Map/UI/C_MapActorInteractionComponent.h"

#include "Net/UnrealNetwork.h"
#include "Map/UI/C_InteractionMessageWidget.h"

UC_MapActorInteractionComponent::UC_MapActorInteractionComponent()
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

void UC_MapActorInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    MessageWidget = Cast<UC_InteractionMessageWidget>(GetWidget());
    
    HideWidget();
}

#include "Map/MapComponent/C_MapMessageComponent.h"

#include "Net/UnrealNetwork.h"
#include "Map/MapComponent/C_MapMessageWidget.h"

UC_MapMessageComponent::UC_MapMessageComponent()
{
    static const TCHAR* ResourcePath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Level/Main_Level/MapContent/UI/WB_MapMessageWidget.WB_MapMessageWidget_C'");
    static ConstructorHelpers::FClassFinder<UC_MapMessageWidget> WidgetAsset(ResourcePath);
    if (true == WidgetAsset.Succeeded())
    {
        SetWidgetClass(WidgetAsset.Class);
    }
    else
    {
        UE_LOG(LogTemp, Fatal, TEXT("Can't find asset."));
        return;
    }
}

void UC_MapMessageComponent::ShowWidget(int _Index)
{
    SetMessage();
}

UC_MapMessageWidget* UC_MapMessageComponent::GetMessageWidget()
{
    InitWidget();
    return Cast<UC_MapMessageWidget>(GetWidget());
}

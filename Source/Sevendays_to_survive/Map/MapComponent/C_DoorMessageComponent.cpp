#include "Map/MapComponent/C_DoorMessageComponent.h"

#include "Map/MapComponent/C_MapMessageWidget.h"

//#include "Map/MapComponent/C_MapMessageWidget.h"
//
//UC_MapMessageWidgetComponent::UC_MapMessageWidgetComponent()
//{
//}
//
//void UC_MapMessageWidgetComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//}
//
//void UC_MapMessageWidgetComponent::SetMessageForItemInfo(const FString& _ItemName, int _Count)
//{
//    Message = _ItemName + TEXT(" × ") + FString::FromInt(_Count);
//    InteractionMessageWidget->SetMessage(Message);
//}
//
//void UC_MapMessageWidgetComponent::SetMessageForDoor()
//{
//    InteractionMessageWidget->SetMessage(TEXT("Open Door"));
//}
//
//void UC_MapMessageWidgetComponent::BeginPlay()
//{
//    Super::BeginPlay();
//
//    InteractionMessageWidget = Cast<UC_MapMessageWidget>(GetWidget());
//    
//    HideWidget();
//}

void UC_DoorMessageComponent::MapInteract()
{

}

void UC_DoorMessageComponent::SetMessage()
{
    GetMessageWidget()->SetMessage("Open Door");
}
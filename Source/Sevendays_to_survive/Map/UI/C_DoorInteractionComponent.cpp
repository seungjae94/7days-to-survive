#include "Map/UI/C_DoorInteractionComponent.h"

#include "Map/UI/C_InteractionMessageWidget.h"

//#include "Map/UI/C_InteractionMessageWidget.h"
//
//UC_InteractionMessageWidgetComponent::UC_InteractionMessageWidgetComponent()
//{
//}
//
//void UC_InteractionMessageWidgetComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//}
//
//void UC_InteractionMessageWidgetComponent::SetMessageForItemInfo(const FString& _ItemName, int _Count)
//{
//    Message = _ItemName + TEXT(" × ") + FString::FromInt(_Count);
//    InteractionMessageWidget->SetMessage(Message);
//}
//
//void UC_InteractionMessageWidgetComponent::SetMessageForDoor()
//{
//    InteractionMessageWidget->SetMessage(TEXT("Open Door"));
//}
//
//void UC_InteractionMessageWidgetComponent::BeginPlay()
//{
//    Super::BeginPlay();
//
//    InteractionMessageWidget = Cast<UC_InteractionMessageWidget>(GetWidget());
//    
//    HideWidget();
//}

void UC_DoorInteractionComponent::MapInteract()
{

}

void UC_DoorInteractionComponent::SetMessage()
{
    GetMessageWidget()->SetMessage("Open Door");
}
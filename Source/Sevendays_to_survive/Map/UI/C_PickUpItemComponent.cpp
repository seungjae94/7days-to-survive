#include "Map/UI/C_PickUpItemComponent.h"

#include "Map/C_Items.h"
#include "Map/UI/C_InteractionMessageWidget.h"
#include "Inventory/C_InventoryComponent.h"
#include "STS/C_STSGlobalFunctions.h"
#include "Map/C_MapInteractionComponent.h"

UC_PickUpItemComponent::UC_PickUpItemComponent()
{
    SetAbsolute(false, true, false);
}

void UC_PickUpItemComponent::SetMessage_Implementation()
{
    if (nullptr == Item)
    {
        // STS_FATAL
        return;
    }

    FString Message = Item->Name + TEXT(" × ") + FString::FromInt(Count);
    MessageWidget->SetMessage(Message);
}

void UC_PickUpItemComponent::SetItemAndCount_Implementation(const UC_Item* _Item, int _Count)
{
    Item = _Item;
    Count = _Count;
    SetMessage();
}

void UC_PickUpItemComponent::MapInteract()
{
    UC_InventoryComponent* Inventory = UC_STSGlobalFunctions::GetInventoryComponent(GetWorld());

    if (nullptr == Item)
    {
        return;
    }

    Inventory->AddItem(Item, Count);

    UC_STSGlobalFunctions::GetMapInteractionComponent(GetWorld())->Server_DestroyActor(GetOwner());
}

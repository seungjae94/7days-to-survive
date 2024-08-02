#include "Map/UI/C_PickUpItemComponent.h"

#include "Map/C_Items.h"

UC_PickUpItemComponent::UC_PickUpItemComponent()
{
}

void UC_PickUpItemComponent::SetMessage_Implementation()
{
    if (nullptr == Item)
    {
        // STS_FATAL
        return;
    }

    Message = Item->Name + TEXT(" × ") + FString::FromInt(Count);
}

void UC_PickUpItemComponent::SetItemAndCount(const UC_Item* _Item, int _Count)
{
    Item = _Item;
    Count = _Count;
    SetMessage();
}

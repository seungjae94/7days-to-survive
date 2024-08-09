#include "Map/MapComponent/C_ItemMessageComponent.h"

#include "Net/UnrealNetwork.h"
#include "STS/C_STSGlobalFunctions.h"
#include "Map/C_Items.h"
#include "Map/MapComponent/C_MapMessageWidget.h"
#include "Map/Inventory/C_InventoryComponent.h"
#include "Map/PlayerComponent/C_MapInteractionComponent.h"

UC_ItemMessageComponent::UC_ItemMessageComponent()
{
}

void UC_ItemMessageComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UC_ItemMessageComponent, ItemId);
    DOREPLIFETIME(UC_ItemMessageComponent, Count);
}

void UC_ItemMessageComponent::SetItemAndCount_Implementation(FName _ItemId, int _Count)
{
    ItemId = _ItemId;
    Count = _Count;
    SetMessage();
}

void UC_ItemMessageComponent::MapInteract()
{
    UC_InventoryComponent* Inventory = UC_STSGlobalFunctions::GetInventoryComponent(GetWorld());

    const UC_Item* Item = UC_STSGlobalFunctions::FindItem(GetWorld(), ItemId);
    if (false == Item->IsValidLowLevel())
    {
        return;
    }

    Inventory->AddItem(Item, Count);

    UC_STSGlobalFunctions::GetMapInteractionComponent(GetWorld())->Server_DestroyActor(GetOwner());
}

void UC_ItemMessageComponent::SetMessage()
{
    const UC_Item* Item = UC_STSGlobalFunctions::FindItem(GetWorld(), ItemId);
    if (nullptr == Item || 0 >= Count)
    {
        return;
    }

    FString Message = Item->BaseData.Name + TEXT(" × ") + FString::FromInt(Count);
    if (nullptr == GetMessageWidget())
    {
        return;
    }
    GetMessageWidget()->SetMessage(Message);
}

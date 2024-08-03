#include "Map/UI/C_PickUpItemComponent.h"

#include "Net/UnrealNetwork.h"
#include "Map/C_Items.h"
#include "Map/UI/C_InteractionMessageWidget.h"
#include "Inventory/C_InventoryComponent.h"
#include "STS/C_STSGlobalFunctions.h"
#include "Map/C_MapInteractionComponent.h"

UC_PickUpItemComponent::UC_PickUpItemComponent()
{
}

void UC_PickUpItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UC_PickUpItemComponent, ItemId);
    DOREPLIFETIME(UC_PickUpItemComponent, Count);
}

void UC_PickUpItemComponent::SetItemAndCount_Implementation(FName _ItemId, int _Count)
{
    ItemId = _ItemId;
    Count = _Count;
    SetMessage();
}

void UC_PickUpItemComponent::MapInteract()
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

void UC_PickUpItemComponent::SetMessage()
{
    const UC_Item* Item = UC_STSGlobalFunctions::FindItem(GetWorld(), ItemId);
    if (nullptr == Item || 0 >= Count)
    {
        return;
    }

    FString Message = Item->Name + TEXT(" × ") + FString::FromInt(Count);
    if (nullptr == GetMessageWidget())
    {
        return;
    }
    GetMessageWidget()->SetMessage(Message);
}

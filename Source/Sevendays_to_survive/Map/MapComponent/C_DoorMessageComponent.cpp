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
	switch (DoorState)
	{
	case EDoorState::Opened:
		Close();
		break;
	case EDoorState::Closed:
		Open();
		break;
	case EDoorState::Opening:
	case EDoorState::Closing:
		return;
	default:
		break;
	}
}

void UC_DoorMessageComponent::BeginPlay()
{
	Super::BeginPlay();

	AddLocalOffset(FVector::UpVector * 50.0f);
	SetIsInteractOnServer(true);

	AActor* Actor = GetOwner();
	Actor->SetReplicates(true);
	Actor->SetReplicateMovement(true);
	
	DoorForward = Actor->GetActorRightVector();
	DoorRight = -(Actor->GetActorForwardVector());
	SpawnLocation = Actor->GetActorLocation();
	SpawnRotation = Actor->GetActorRotation();

	UStaticMeshComponent* SMComponent = Actor->GetComponentByClass<UStaticMeshComponent>();
	if (false == SMComponent->IsValidLowLevel())
	{
		return;
	}

	FBoxSphereBounds MeshBounds = SMComponent->GetStaticMesh()->GetBounds();
	BoxExtent = MeshBounds.BoxExtent;
}

void UC_DoorMessageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (false == GetOwner()->HasAuthority())
	{
		return;
	}

	if (EDoorState::Opening == DoorState)
	{
		Theta += 90.0f * DeltaTime / OpenTime;

		if (Theta >= 90.0f)
		{
			DoorState = EDoorState::Opened;
			Theta = 90.0f;
		}
	}
	else if (EDoorState::Closing == DoorState)
	{
		Theta -= 90.0f * DeltaTime / OpenTime;

		if (Theta <= 0.0f)
		{
			DoorState = EDoorState::Closed;
			Theta = 0.0f;
		}
	}

	double FCoeff = (BoxExtent.X + RotAxisRadius) * FMath::Sin(FMath::DegreesToRadians(Theta));
	double RCoeff = (BoxExtent.X + RotAxisRadius) * (1.0 - FMath::Cos(FMath::DegreesToRadians(Theta)));
	GetOwner()->SetActorRotation(SpawnRotation + FRotator(0.0, Theta, 0.0));
	GetOwner()->SetActorLocation(SpawnLocation + FCoeff * DoorForward + RCoeff * DoorRight);
}

void UC_DoorMessageComponent::SetMessage()
{
    GetMessageWidget()->SetMessage("Open Door");
}

void UC_DoorMessageComponent::Open()
{
	DoorState = EDoorState::Opening;
	Theta = 0.0f;
}

void UC_DoorMessageComponent::Close()
{
	DoorState = EDoorState::Closing;
	Theta = 90.0f;
}

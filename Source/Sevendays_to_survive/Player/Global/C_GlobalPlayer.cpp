﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Global/C_GlobalPlayer.h"
#include "STS/C_STSInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Player/MainController/C_MainPlayerController.h"
#include "Player/Global/C_PlayerEnum.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Player/Input/C_InputActionDatas.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Global/DataTable/C_WeaponDataTable.h"
#include "Weapon/C_EquipWeapon.h"
#include "Weapon/C_GunComponent.h"
#include "Monster/C_ZombieBase.h"
#include "Components/TextRenderComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/Global/DataTable/C_PlayerSpawnData.h"
#include "Monster/MonsterAI/C_MonsterAIBase.h"

// Sets default values
AC_GlobalPlayer::AC_GlobalPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerMeshOption();
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->bIgnoreBaseRotation = true; // 베이스 회전
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera"));
	SpringArm->SetupAttachment(GetMesh(), *FString("Head"));
	SpringArm->TargetArmLength = 0.0f; // The camera follows at this distance behind the character	
	SpringArm->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	/*SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 10.0f;*/

	// Create a follow camera
	Cameras = CreateDefaultSubobject<UCameraComponent >(TEXT("FollowCamera"));
	Cameras->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Cameras->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	NameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Name"));
	NameText->SetupAttachment(GetCapsuleComponent());
	NameText->SetRelativeLocation(FVector(0.0f, 20.0f, 100.0f));
	
	{
		{
			UEnum* Enum = StaticEnum<EStaticItemSlot>();
			// UStaticMeshComponent 슬롯 전용
			for (size_t i = 0; i < static_cast<size_t>(EStaticItemSlot::SlotMax); i++)
			{
				FString Name = Enum->GetNameStringByValue(i);
				UStaticMeshComponent* NewSlotMesh = CreateDefaultSubobject<UStaticMeshComponent>(*Name);
				NewSlotMesh->SetupAttachment(GetMesh(), *Name);
				NewSlotMesh->SetCollisionProfileName(TEXT("NoCollision"));
				StaticItemMeshs.Push(NewSlotMesh);
			}
		}
		
		{
			UEnum* Enum = StaticEnum<ESkerItemSlot>();
			// USkeletalMeshComponent 슬롯 전용
			for (size_t i = 0; i < static_cast<size_t>(ESkerItemSlot::SlotMax); i++)
			{
				FString Name = Enum->GetNameStringByValue(i);
				USkeletalMeshComponent* NewSlotMesh = CreateDefaultSubobject<USkeletalMeshComponent>(*Name);
				NewSlotMesh->SetupAttachment(GetMesh(), *Name);
				NewSlotMesh->SetCollisionProfileName(TEXT("NoCollision"));
				SkeletalItemMeshes.Push(NewSlotMesh);
			}
		}
	}

	// 데이터 에셋
	{
		FString RefPathString = TEXT("/Script/Sevendays_to_survive.C_InputActionDatas'/Game/Level/TestLevel/CharTest/Player/Input/DA_Input.DA_Input'");

		ConstructorHelpers::FObjectFinder<UC_InputActionDatas> ResPath(*RefPathString);

		// 유효한 리소스냐를 판단할수 있습니다.
		if (false == ResPath.Succeeded())
		{
			return;
		}

		InputData = ResPath.Object;
	}

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void AC_GlobalPlayer::Playerhit_Implementation(const int _Damage)
{
	if (true == IsPlayerDieCpp)
	{
		return;
	}

	if (true == IsHitCpp)
	{
		return;
	}

	if (true == ISReload)
	{
		return;
	}

	if (_Damage == 0)
	{
		return;
	}

	IsHitCpp = true;
	Hp = Hp - _Damage;

	hitEffect();
	if (Hp <= 0)
	{
		ResetHit();
		ChangeNoWeaponServer();
		PlayerDieCheck();
		return;
	}
}

void AC_GlobalPlayer::ResetHit_Implementation()
{
	IsAimCpp = false;
	IsFireCpp = false;
	IsHitCpp = false;
	ISReload = false;
}

void AC_GlobalPlayer::WeaponSwingSound_Implementation(FHitResult _Hit, const bool _IsZombie)
{
	if (true == WeaponSounds.IsEmpty())
	{
		return;
	}

	switch (PlayerCurState)
	{
	case EWeaponUseState::bat:
		if (true == _IsZombie)
		{
			UGameplayStatics::SpawnSoundAtLocation(_Hit.GetActor(), WeaponSounds[EStaticItemSlot::RBat], _Hit.ImpactPoint);
		}
		else
		{
			UGameplayStatics::SpawnSoundAtLocation(_Hit.GetActor(), WeaponSounds[EStaticItemSlot::RBat], _Hit.ImpactPoint);
		}
		break;
	case EWeaponUseState::Sword:
		if (true == _IsZombie)
		{
			UGameplayStatics::SpawnSoundAtLocation(_Hit.GetActor(), WeaponSounds[EStaticItemSlot::RSword], _Hit.ImpactPoint);
		}
		else
		{
			UGameplayStatics::SpawnSoundAtLocation(_Hit.GetActor(), WeaponSounds[EStaticItemSlot::RBat], _Hit.ImpactPoint);
		}
		break;
	case EWeaponUseState::Axe:
		if (true == _IsZombie)
		{
			UGameplayStatics::SpawnSoundAtLocation(_Hit.GetActor(), WeaponSounds[EStaticItemSlot::RSword], _Hit.ImpactPoint);
		}
		else
		{
			UGameplayStatics::SpawnSoundAtLocation(_Hit.GetActor(), WeaponSounds[EStaticItemSlot::RBat], _Hit.ImpactPoint);
		}
		break;
	default:
		break;
	}
}
void AC_GlobalPlayer::AddHp_Implementation(const int _Hp)
{
	if (100 < Hp + _Hp)
	{
		Hp = 100;
		return;
	}
	else
	{
		Hp += _Hp;
		return;
	}

}

void AC_GlobalPlayer::Addstamina(const int _stamina)
{
	if (Maxstamina < stamina + _stamina)
	{
		stamina = Maxstamina;
		return;
	}
	else
	{
		stamina += _stamina;
		return;
	}
}

void AC_GlobalPlayer::Resetmagazinecapacity()
{
	switch (PlayerCurState)
	{
	case EWeaponUseState::Rifle:
		magazinecapacity[ESkerItemSlot::RRifle] = Riflemagazinecapacity;
		break;
	case EWeaponUseState::Rifle2:
		magazinecapacity[ESkerItemSlot::RRifle2] = Riflemagazinecapacity;
		break;
	case EWeaponUseState::Pistol:
		magazinecapacity[ESkerItemSlot::RPistol] = Pistolmagazinecapacity;
		//PlayCharacter->GetPistolmagazinecapacity();
		break;
	case EWeaponUseState::Pistol2:
		magazinecapacity[ESkerItemSlot::RPistol2] = Pistolmagazinecapacity;
		break;
	case EWeaponUseState::Shotgun:
		magazinecapacity[ESkerItemSlot::RShotgun] = ShotGunmagazinecapacity;
		break;
	default:
		break;
	}
}

void AC_GlobalPlayer::ResetShotGunAtt()
{
	IsShotGunShot = false;
}

void AC_GlobalPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AC_GlobalPlayer, IsRunCpp);
	DOREPLIFETIME(AC_GlobalPlayer, PlayerCurState);
	DOREPLIFETIME(AC_GlobalPlayer, PitchCPP);
	DOREPLIFETIME(AC_GlobalPlayer, IsAimCpp);
	DOREPLIFETIME(AC_GlobalPlayer, PlayerSpawnCheckToken);
	DOREPLIFETIME(AC_GlobalPlayer, IsFireCpp);
	DOREPLIFETIME(AC_GlobalPlayer, MaxCalPitchCPP);
	DOREPLIFETIME(AC_GlobalPlayer, MinCalPithchCPP);
	DOREPLIFETIME(AC_GlobalPlayer, IsPlayerDieCpp);
	DOREPLIFETIME(AC_GlobalPlayer, IsHitCpp);
	DOREPLIFETIME(AC_GlobalPlayer, ISReload);
	DOREPLIFETIME(AC_GlobalPlayer, characterResultMesh);
	DOREPLIFETIME(AC_GlobalPlayer, PlayerName);
	DOREPLIFETIME(AC_GlobalPlayer, Hp);
}

// Called when the game starts or when spawned
void AC_GlobalPlayer::BeginPlay()
{
	{
		UC_STSInstance* init = GetWorld()->GetGameInstanceChecked<UC_STSInstance>();
		if (GetWorld()->GetAuthGameMode() != nullptr)
		{
			PlayerSpawnCheckToken = init->GetNetToken();
		}

		PlayerTokenCheck(PlayerSpawnCheckToken);

		if (this == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			// 스켈레탈 매쉬 TArray<SkeletalMesh> Arr;
			// 스켈레탈 매쉬 TArray<SkeletalMesh> Arr;
		
			/*if (PlayerSpawnCheckToken != -1)
			{
				FC_PlayerSpawnData* DataTables = init->GetPlayerSpawnDataTable();
				FVector test = DataTables->PlayerSpawnLocations[PlayerSpawnCheckToken + 1];
				SetActorLocation(test);
			}*/
			
			characterResultMesh = init->GetPlayerMesh();
			FString ServerName = init->GetPlayerName();
			PlayerName = FText::FromString(ServerName);
			//MeshInit(characterResultMesh);
		}
	}

	Super::BeginPlay();

	UC_STSInstance* STSInstance = GetWorld()->GetGameInstanceChecked<UC_STSInstance>();

	
	if (nullptr == STSInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (STSInstance == 0)"), __FUNCTION__, __LINE__);
		return;
	}

	ReStartLocation = STSInstance->GetPlayerSpawnLocation()[PlayerSpawnCheckToken];
	//NameText->Text = FText::FromString(STSInstance->GetPlayerName());
	//FC_PlayerSpawnData* DataTables = STSInstance->GetPlayerSpawnDataTable();

	CameraDT = STSInstance->GetPlayerDataTable()->CameraValue;
	PlayerDT = STSInstance->GetPlayerDataTable()->PlayerValue;
	BulletDT = STSInstance->GetPlayerDataTable()->BulletValue;
	AttWeaponSound = STSInstance->GetPlayerDataTable()->SoundValue;

	Tags.Add(TEXT("Player"));

	{
		FString& Port = STSInstance->TitleToGameInfo.ServerPort;
		//if (STSInstance->TitleToGameInfo.ServerOpenCheck == true)
		//{
		//	return;
		//}
		int PortNumber = FCString::Atoi(*Port);
		if (PortNumber == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (PortNumber == 0)"), __FUNCTION__, __LINE__);
			PortNumber = 30002;
			// UE_LOG(GIMATLog, Fatal, TEXT("%S(%u)> if (PortNumber == 0)"), __FUNCTION__, __LINE__);
		}

		// FString:: Inst->SideScrollGameInfo.PORT;
		if (GetLocalRole() == ENetRole::ROLE_Authority)
		{
			STSInstance->EnableListenServer(true, PortNumber);
		}
	}


	// 카메라 데이터 테이블 값 가져오기
	{
		SpringArm->TargetArmLength = CameraDT.TargetArmLength;
		CameraRotSpeed = CameraDT.CameraRotSpeed;
		UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMax = (MaxCalPitchCPP*3.0f)-16.0f;
		UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMin = MinCalPithchCPP*3.0f;

	}

	
	// 플레이어 데이터 테이블 값 가져오기
	{
		GetCharacterMovement()->MaxWalkSpeed = PlayerDT.WalkSpeed;
		GetCharacterMovement()->JumpZVelocity = PlayerDT.JumpZVelocity;
		Maxstamina= PlayerDT.stamina;
		stamina = PlayerDT.stamina;
		staminaCalValue = PlayerDT.staminaCalValue;
		staminaAttCalValue = PlayerDT.staminaAttValue;
		staminaJumpCalValue = PlayerDT.staminaJumpValue;
		Hp = PlayerDT.Hp;
		thisHitBlood = PlayerDT.PlayerHitBlood;
	}
	
	// 총알 관련 이펙트
	{
		BulletHoleEffect = BulletDT.BulletHole;
		ZombieHitEffect = BulletDT.ZombieHitBlood;
		
	}

	// 근접 무기 사운드
	{
		WeaponSounds.Add(EStaticItemSlot::RSword, AttWeaponSound.AttSound[0]);
		WeaponSounds.Add(EStaticItemSlot::RBat, AttWeaponSound.AttSound[1]);
	}

	//Add Input Mapping Context
	PlayerController = Cast<AC_MainPlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	{
		TSubclassOf<AActor> M4= STSInstance->GetWeaPonDataTable(FName("M4"))->Equip;
		GunWeapon.Add(EWeaponUseState::Rifle, M4);

		TSubclassOf<AActor> Rifle2 = STSInstance->GetWeaPonDataTable(FName("Rifle2"))->Equip;
		GunWeapon.Add(EWeaponUseState::Rifle2, Rifle2);

		TSubclassOf<AActor> Pistol1 = STSInstance->GetWeaPonDataTable(FName("Pistol1"))->Equip;
		GunWeapon.Add(EWeaponUseState::Pistol, Pistol1);

		TSubclassOf<AActor> ShotGun = STSInstance->GetWeaPonDataTable(FName("ShotGun"))->Equip;
		GunWeapon.Add(EWeaponUseState::Shotgun, ShotGun);

		TSubclassOf<AActor> Pistol2 = STSInstance->GetWeaPonDataTable(FName("Pistol2"))->Equip;
		GunWeapon.Add(EWeaponUseState::Pistol2, Pistol2);

		PistolRange= STSInstance->GetWeaPonDataTable(FName("Pistol2"))->BulletRange;
		ShotGunRange= STSInstance->GetWeaPonDataTable(FName("ShotGun"))->BulletRange;
		RifleRange= STSInstance->GetWeaPonDataTable(FName("Rifle2"))->BulletRange;

		PistolAtt = STSInstance->GetWeaPonDataTable(FName("Pistol2"))->Damage;
		ShotGunAtt= STSInstance->GetWeaPonDataTable(FName("ShotGun"))->Damage;
		RifleAtt= STSInstance->GetWeaPonDataTable(FName("Rifle2"))->Damage;


		Pistolmagazinecapacity= STSInstance->GetWeaPonDataTable(FName("Pistol2"))->MagagineSize;
		ShotGunmagazinecapacity= STSInstance->GetWeaPonDataTable(FName("ShotGun"))->MagagineSize;
		Riflemagazinecapacity= STSInstance->GetWeaPonDataTable(FName("Rifle2"))->MagagineSize;

	}

	{
		for (size_t i = 0; i < static_cast<size_t>(ESkerItemSlot::SlotMax); i++)
		{
			switch (static_cast<ESkerItemSlot>(i))
			{
			case ESkerItemSlot::RRifle:
				magazinecapacity.Add(static_cast<ESkerItemSlot>(i), Riflemagazinecapacity);
				ReloadMontages.Add(static_cast<ESkerItemSlot>(i),STSInstance->GetWeaPonDataTable(FName("M4"))->ReloadAniMontage);
				break;
			case ESkerItemSlot::RRifle2:
				magazinecapacity.Add(static_cast<ESkerItemSlot>(i), Riflemagazinecapacity);
				ReloadMontages.Add(static_cast<ESkerItemSlot>(i), STSInstance->GetWeaPonDataTable(FName("Rifle2"))->ReloadAniMontage);
				break;
			case ESkerItemSlot::RPistol:
				magazinecapacity.Add(static_cast<ESkerItemSlot>(i), Pistolmagazinecapacity);
				ReloadMontages.Add(static_cast<ESkerItemSlot>(i), STSInstance->GetWeaPonDataTable(FName("Pistol1"))->ReloadAniMontage);
				break;
			case ESkerItemSlot::RPistol2:
				magazinecapacity.Add(static_cast<ESkerItemSlot>(i), Pistolmagazinecapacity);
				ReloadMontages.Add(static_cast<ESkerItemSlot>(i), STSInstance->GetWeaPonDataTable(FName("Pistol2"))->ReloadAniMontage);
				break;
			case ESkerItemSlot::RShotgun:
				magazinecapacity.Add(static_cast<ESkerItemSlot>(i), ShotGunmagazinecapacity);
				ReloadMontages.Add(static_cast<ESkerItemSlot>(i), STSInstance->GetWeaPonDataTable(FName("Shotgun"))->ReloadAniMontage);
				break;
			case ESkerItemSlot::SlotMax:
				break;
			default:
				break;
			}
		}
	}
	
}

// Called every frame
void AC_GlobalPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpawnBulletMove(DeltaTime);
	Calstamina();
}

// Called to bind functionality to input
void AC_GlobalPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Jump], ETriggerEvent::Started, this, &AC_GlobalPlayer::JumpCal);
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Jump], ETriggerEvent::Canceled, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Jump], ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Move], ETriggerEvent::Triggered, this, &AC_GlobalPlayer::Move);

		// Run
		
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Run], ETriggerEvent::Triggered, this, &AC_GlobalPlayer::RunStart);
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Run], ETriggerEvent::Completed, this, &AC_GlobalPlayer::RunEnd);

		// Looking
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Look], ETriggerEvent::Triggered, this, &AC_GlobalPlayer::Look);

		//Crouch
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Crouch], ETriggerEvent::Started, this, &AC_GlobalPlayer::CrouchCpp);

		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Zoom], ETriggerEvent::Started, this, &AC_GlobalPlayer::AimStart);
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Zoom], ETriggerEvent::Canceled, this, &AC_GlobalPlayer::AimEnd);
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Zoom], ETriggerEvent::Completed, this, &AC_GlobalPlayer::AimEnd);
		// Att 
		//EnhancedInputComponent->BindAction(AttAction, ETriggerEvent::Started, this, &AC_NickMainPlayer::PunchAtt);
		//EnhancedInputComponent->BindAction(AttAction, ETriggerEvent::Completed, this, &AC_NickMainPlayer::PunchAttEnd);

		//Fire
		//EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Fire], ETriggerEvent::Started, this, &AC_GlobalPlayer::FireStart);
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Fire], ETriggerEvent::Started, this, &AC_GlobalPlayer::FireStart);
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Fire], ETriggerEvent::Completed, this, &AC_GlobalPlayer::FireEnd);
		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Fire], ETriggerEvent::Canceled, this, &AC_GlobalPlayer::FireEnd);

		EnhancedInputComponent->BindAction(InputData->Actions[EPlayerState::Reload], ETriggerEvent::Started, this, &AC_GlobalPlayer::ReloadServer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AC_GlobalPlayer::hitEffect_Implementation()
{
	GetMesh()->GetAnimInstance()->Montage_Play(hitMontage);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, thisHitBlood, GetMesh()->GetSocketTransform(FName("Spine2")).GetLocation(), FRotator(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f), true, true, ENCPoolMethod::None, true)->Activate();
}


void AC_GlobalPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	if (true == IsPlayerDieCpp)
	{
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();
	//MeshInit(characterResultMesh);
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AC_GlobalPlayer::JumpCal(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		return;
	}

	if (true == IsPlayerDieCpp)
	{
		return;
	}

	if (stamina < staminaJumpCalValue)
	{
		return;
	}

	if (true==GetMovementComponent()->IsFalling())
	{
		IsRunCpp = false;
		return;
	}

	Jump();
	stamina -= staminaJumpCalValue;
}

void AC_GlobalPlayer::GunLineTrace_Implementation()
{
	if (nullptr == CurWeapon)
	{
		return;
	}

	switch (PlayerCurState)
	{
	case EWeaponUseState::Rifle:
		--magazinecapacity[ESkerItemSlot::RRifle];
		CurWeapon->GunParticleAndSound(PlayerCurState);
		Rebound();
		break;
	case EWeaponUseState::Pistol:
		--magazinecapacity[ESkerItemSlot::RPistol];
		CurWeapon->GunParticleAndSound(PlayerCurState);
		break;
	case EWeaponUseState::Rifle2:
		--magazinecapacity[ESkerItemSlot::RRifle2];
		CurWeapon->PlayGunAnimation(PlayerCurState);
		Rebound();
		break;
	case EWeaponUseState::Pistol2:
		--magazinecapacity[ESkerItemSlot::RPistol2];
		CurWeapon->PlayGunAnimation(PlayerCurState);
		break;
	default:
		break;
	}
	//LineTracemagazinecapacity -= 1;
	if (UGameplayStatics::GetGameMode(GetWorld()) == nullptr)
	{
		return;
	}


	if (PlayerCurState == EWeaponUseState::Shotgun)
	{
		return;
	}

	
	UC_GunComponent* GunMesh = CurWeapon->GetComponentByClass<UC_GunComponent>();
	//FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;


	FVector GunLocation = GunMesh->GetSocketLocation(FName("Muzzle"));
	FRotator GunRotation = GunMesh->GetSocketRotation(FName("Muzzle"));
	FVector GunForwardVector =  UKismetMathLibrary::GetForwardVector(GunRotation);
	
	FireInfo Bullet;
	/*Infos.Add(Info);*/

	FVector Start = GunLocation;
	Bullet.Start = Start;
	FVector End = (GunForwardVector * LineTraceRange) + GunLocation;
	Bullet.End = End;
	Bullet.BulletRotation = GunRotation;
	Bullet.BulletActor = BulletDT.BulletActor;
	BulletInfos.Add(Bullet);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CurWeapon);
	Params.AddIgnoredActor(this);

	TArray<AActor*> Actors;

	Actors.Add(CurWeapon);
	bool OKAtt=UKismetSystemLibrary::LineTraceSingle(CurWeapon, Start, End, ETraceTypeQuery::TraceTypeQuery1, false, Actors, EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
	//GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, TraceParameters);

	if (true == OKAtt)
	{
		AActor* ActorHit = Hit.GetActor();
		if (ActorHit)
		{
			{
				AC_ZombieBase* Zombie = Cast<AC_ZombieBase>(ActorHit);

				if (Zombie)
				{
					CreateZombieBlood(Hit);
					//Zombie->GetMesh()->GetSock
					if (Hit.BoneName == Zombie->GetMesh()->GetSocketBoneName("Head"))
					{
						Zombie->SetHP(LineTraceDamage*2.0f);
					}
					else
					{
						Zombie->SetHP(LineTraceDamage);
					}

					AC_MonsterAIBase* AIController = Cast<AC_MonsterAIBase>(Zombie->GetController());
					if (AIController != nullptr) {
						AIController->SetTargetActor(this);
					}
				}
				else
				{
					CreateBulletHole(Hit);
				}
			}
		}
	}
	
	GetWorld()->GetTimerManager().ClearTimer(timer);

	if (true == IsFireCpp)
	{
		switch (PlayerCurState)
		{
		case EWeaponUseState::Rifle:
		case EWeaponUseState::Rifle2:
			GetWorld()->GetTimerManager().SetTimer(timer, this, &AC_GlobalPlayer::FireLoop, 0.15f, true);
			break;
		case EWeaponUseState::Pistol:
		case EWeaponUseState::Pistol2:
			GetWorld()->GetTimerManager().SetTimer(timer, this, &AC_GlobalPlayer::FireLoop, 0.7f, true);
			break;
		default:
			break;
		}
	}
	//GunRotation.
}

void AC_GlobalPlayer::ShotGunLineTrace_Implementation()
{
	if (nullptr == CurWeapon)
	{
		return;
	}

	if (PlayerCurState != EWeaponUseState::Shotgun)
	{
		return;
	}


	CurWeapon->PlayGunAnimation(PlayerCurState);
	--magazinecapacity[ESkerItemSlot::RShotgun];
	Rebound();
	if (UGameplayStatics::GetGameMode(GetWorld()) == nullptr)
	{
		return;
	}


	UC_GunComponent* GunMesh = CurWeapon->GetComponentByClass<UC_GunComponent>();
	
	FHitResult Hit;
	//ector ShotDirection;

	FVector GunLocation = GunMesh->GetSocketLocation(FName("Muzzle"));
	FRotator GunRotation = GunMesh->GetSocketRotation(FName("Muzzle"));
	FVector GunForwardVector = UKismetMathLibrary::GetForwardVector(GunRotation);

	FVector Start = GunLocation;

	TArray<AActor*> Actors;

	Actors.Add(CurWeapon);
	//FRandomStream Stream(FMath::Rand());
	
	for (size_t i = 0; i < 8; i++)
	{
		float X = Random.FRandRange(Spreed * -1.0f, Spreed);
		float Y = Random.FRandRange(Spreed * -1.0f, Spreed);
		float Z = Random.FRandRange(Spreed * -1.0f, Spreed);
		FVector End = (GunForwardVector * LineTraceRange) + GunLocation+FVector(X,Y,Z);

		bool OKAtt = UKismetSystemLibrary::LineTraceSingle(CurWeapon, Start, End, ETraceTypeQuery::TraceTypeQuery1, false, Actors, EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
	
		if (true == OKAtt)
		{
			AActor* ActorHit = Hit.GetActor();
			if (ActorHit)
			{
				{
					AC_ZombieBase* Zombie = Cast<AC_ZombieBase>(ActorHit);

					if (Zombie)
					{

						CreateZombieBlood(Hit);
						Zombie->SetHP(LineTraceDamage);

						AC_MonsterAIBase* AIController = Cast<AC_MonsterAIBase>(Zombie->GetController());
						if (AIController != nullptr) {
							AIController->SetTargetActor(this);
						}
					}
					else
					{
						CreateBulletHole(Hit);
					}
				}
			}
		}
		
	}
}

void AC_GlobalPlayer::ResultPitchCal_Implementation(float _Pitch)
{
	PitchCPP += _Pitch;
	if (PitchCPP >= MaxCalPitchCPP)
	{
		PitchCPP = MaxCalPitchCPP;
	}
	else if (PitchCPP <= MinCalPithchCPP)
	{
		PitchCPP = MinCalPithchCPP;
	}
}

void AC_GlobalPlayer::Look(const FInputActionValue& Value)
{
	if (true == IsPlayerDieCpp)
	{
		return;
	}

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
		//* UGameplayStatics::GetWorldDeltaSeconds(this) * CameraRotSpeed;
	//MeshInit(characterResultMesh);
	if (Controller != nullptr)
	{
		ResultPitchCal(LookAxisVector.Y);

		AddControllerYawInput(-LookAxisVector.X);
		if (PitchCPP<=MaxCalPitchCPP && PitchCPP>=MinCalPithchCPP)
		{
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}


void AC_GlobalPlayer::FireLoop_Implementation()
{
	if (true == IsPlayerDieCpp)
	{
		return;
	}

	if (true == IsHitCpp)
	{
		return;
	}

	switch (PlayerCurState)
	{
	case EWeaponUseState::Rifle:
		if (magazinecapacity[ESkerItemSlot::RRifle] == 0)
		{
			CreateUseGunCheckSound();
			IsFireCpp = false;
			return;
		}
		break;
	case EWeaponUseState::Pistol:
		if (magazinecapacity[ESkerItemSlot::RPistol] == 0)
		{
			CreateUseGunCheckSound();
			IsFireCpp = false;
			return;
		}
		break;
	case EWeaponUseState::Rifle2:
		if (magazinecapacity[ESkerItemSlot::RRifle2] == 0)
		{
			CreateUseGunCheckSound();
			IsFireCpp = false;
			return;
		}
		break;
	case EWeaponUseState::Pistol2:
		if (magazinecapacity[ESkerItemSlot::RPistol2] == 0)
		{
			CreateUseGunCheckSound();
			IsFireCpp = false;
			return;
		}
		break;
	default:
		break;
	}

	/*if (LineTracemagazinecapacity == 0)
	{
		IsFireCpp = false;
		return;
	}*/

	if (true == IsFireCpp)
	{
		GunLineTrace();
	}
}

void AC_GlobalPlayer::CreateBulletHole_Implementation(FHitResult _Hit)
{
	UGameplayStatics::SpawnDecalAtLocation(this, BulletHoleEffect, FVector(1.0f, 10.0f, 10.0f), _Hit.Location, _Hit.ImpactNormal.Rotation(), 5.0f);
}

void AC_GlobalPlayer::CreateZombieBlood_Implementation(FHitResult _Hit)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ZombieHitEffect, _Hit.ImpactPoint, FRotator(0.0f,0.0f,0.0f), FVector(1.0f,1.0f,1.0f), true, true, ENCPoolMethod::None, true)->Activate();

}

void AC_GlobalPlayer::CreateUseGunCheckSound_Implementation()
{
	UGameplayStatics::SpawnSoundAtLocation(this, UseGunCheckSound, GetActorLocation());
}

void AC_GlobalPlayer::Calstamina()
{

	if (false == IsRunCpp)
	{
		if (stamina == Maxstamina)
		{
			return;
		}
		stamina += staminaCalValue;
	}
	else if (true==IsRunCpp) // 점프 체크 값 추가로 넣어야함
	{
		if (true == GetMovementComponent()->IsFalling())
		{
			RunStart_Implementation(0);
			return;
		}


		if (true == GetCharacterMovement()->bWantsToCrouch)
		{
			return;
		}

		if (stamina == 0)
		{
			RunEnd_Implementation(0);
			return;
		}
		stamina -= staminaCalValue;
	}
}

void AC_GlobalPlayer::AttCalstamina()
{
	if (stamina < staminaAttCalValue)
	{
		return;
	}

	if (CurWeapon == nullptr)
	{
	
		stamina -= staminaAttCalValue;
	}

	
}

void AC_GlobalPlayer::CrouchCpp(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		return;
	}

	if (true == IsPlayerDieCpp)
	{
		return;
	}

	if (true == GetMovementComponent()->IsFalling())
	{
		return;
	}

	if (true==GetCharacterMovement()->bWantsToCrouch)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void AC_GlobalPlayer::SpawnBulletMove(float _DeltaTime)
{
	if (UGameplayStatics::GetGameMode(GetWorld()) == nullptr)
	{
		return;
	}

	if (nullptr == CurWeapon)
	{
		return;
	}

	if(true==BulletInfos.IsEmpty())
	{
		return;
	}

	if (false == IsFireCpp)
	{
		return;
	}
	
	for (size_t i = 0; i < BulletInfos.Num(); i++)
	{
		if (nullptr == BulletInfos[i].BulletActor)
		{
			return;
		}
		AActor* Bullet = GetWorld()->SpawnActor<AActor>(BulletInfos[i].BulletActor);
		Bullet->SetActorRotation(BulletInfos[i].BulletRotation);
		Bullet->SetActorLocation(BulletInfos[i].Start);
		Bullet->GetComponentByClass<UProjectileMovementComponent>()->Velocity= BulletInfos[i].End- BulletInfos[i].Start;
		Bullet->InitialLifeSpan = BulletInfos[i].Time;
	}
	BulletInfos.Empty();
}

void AC_GlobalPlayer::ReloadServer_Implementation()
{

	if (Controller == nullptr)
	{
		return;
	}

	if (nullptr == CurWeapon)
	{
		return;
	}

	if (true == IsPlayerDieCpp)
	{
		return;
	}

	if (true == IsHitCpp)
	{
		return;
	}

	if (true == IsAimCpp)
	{
		return;
	}

	if (true == IsFireCpp)
	{
		return;
	}

	if (true == ISReload)
	{
		return;
	}

	Reload();
}

void AC_GlobalPlayer::Reload_Implementation()
{
	ISReload = true;
	switch (PlayerCurState)
	{
	case EWeaponUseState::Rifle:
		GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontages[ESkerItemSlot::RRifle]);
		break;
	case EWeaponUseState::Pistol:
		GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontages[ESkerItemSlot::RPistol]);
		break;
	case EWeaponUseState::Rifle2:
		GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontages[ESkerItemSlot::RRifle2]);
		break;
	case EWeaponUseState::Pistol2:
		GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontages[ESkerItemSlot::RPistol2]);
		break;
	case EWeaponUseState::Shotgun:
		GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontages[ESkerItemSlot::RShotgun]);
		break;
	default:
		break;
	}

}

void AC_GlobalPlayer::MeshInit_Implementation(EPlayerMesh _Mesh)
{
	//if (this == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		//{
		//	// 스켈레탈 매쉬 TArray<SkeletalMesh> Arr;
		//	// 스켈레탈 매쉬 TArray<SkeletalMesh> Arr;
		//	UC_STSInstance* init = GetWorld()->GetGameInstanceChecked<UC_STSInstance>();

		//	if (GetWorld()->GetAuthGameMode() != nullptr)
		//	{
		//		PlayerSpawnCheckToken = init->GetNetToken();
		//	}
		//	characterResultMesh = init->GetPlayerMesh();
		//}
	if (_Mesh == characterResultMesh)
	{
		return;
	}

	characterResultMesh = _Mesh;
}

void AC_GlobalPlayer::NamesInit_Implementation(const FText& _Name)
{
	PlayerName = _Name;
}

void AC_GlobalPlayer::Rebound()
{
	if (nullptr == CurWeapon)
	{
		return;
	}
	Frebound RifleReBound;
	float PitchRebound = Random.FRandRange(RifleReBound.PitchMin, RifleReBound.PitchMax);
	PitchCPP = PitchCPP + PitchRebound;
	float YawRebound= Random.FRandRange(RifleReBound.YawMin, RifleReBound.YawMax);
	AddControllerYawInput(YawRebound);
	AddControllerPitchInput(PitchRebound);
}

void AC_GlobalPlayer::PlayerMeshOption()
{
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
}

void AC_GlobalPlayer::PlayerDieCheck_Implementation()
{
	if (Hp > 0)
	{
		return;
	}

	IsPlayerDieCpp = true;

	FTimerHandle DieTime;
	GetWorld()->GetTimerManager().SetTimer(DieTime, this, &AC_GlobalPlayer::PlayerReStartCheck, 10.0f, false);

}

void AC_GlobalPlayer::PlayerReStartCheck_Implementation()
{
	//ReStartLocation
	ResetHit();
	IsPlayerDieCpp = false;
	Hp = 100;
	SetActorLocation(ReStartLocation, false, nullptr, ETeleportType::TeleportPhysics);
	//SetActorLocation(FVector(0.0f, 0.0f, 400.0f));
}

void AC_GlobalPlayer::PlayerTokenCheck_Implementation(int _Token)
{
	PlayerSpawnCheckToken = _Token;
}


void AC_GlobalPlayer::RunStart_Implementation(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		return;
	}

	if (true == IsPlayerDieCpp)
	{
		return;
	}

	if (true == GetMovementComponent()->IsFalling())
	{
		IsRunCpp = false;
		return;
	}


	if (GetCharacterMovement()->Velocity.Length()<=0)
	{
		return;
	}

	if (stamina == 0)
	{
		return;
	}
	


	GetCharacterMovement()->MaxWalkSpeed = PlayerDT.RunSpeed;

	

	IsRunCpp = true;
}

void AC_GlobalPlayer::AimStart_Implementation(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		return;
	}

	if (true == IsPlayerDieCpp)
	{
		return;
	}

	if (true == IsHitCpp)
	{
		return;
	}

	if (true == ISReload)
	{
		return;
	}

	IsAimCpp = true;
}

void AC_GlobalPlayer::RunEnd_Implementation(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = PlayerDT.WalkSpeed;
	IsRunCpp = false;
}


void AC_GlobalPlayer::AimEnd_Implementation(const FInputActionValue& Value)
{
	IsAimCpp = false;
}

void AC_GlobalPlayer::ChangeSlotMesh_Implementation(EStaticItemSlot _Slot, UStaticMesh* _Mesh)
{
	if (nullptr!=CurWeapon)
	{
		CurWeapon->Destroy();
		CurWeapon = nullptr;
		for (size_t i = 0; i < static_cast<size_t>(ESkerItemSlot::SlotMax); i++)
		{
			SkeletalItemMeshes[i]->SetSkeletalMesh(nullptr);
		}
	}

	uint8 SlotIndex = static_cast<uint8>(_Slot);
	if (StaticItemMeshs.Num() <= SlotIndex)
	{
		UE_LOG(LogTemp, Fatal, TEXT("%S(%u)> if (ItemMeshs.Num() <= static_cast<uint8>(_Slot))"), __FUNCTION__, __LINE__);
		return;
	}

	switch (_Slot)
	{
	case EStaticItemSlot::RBat:
		for (size_t i = 0; i < static_cast<size_t>(EStaticItemSlot::SlotMax); i++)
		{
			StaticItemMeshs[i]->SetStaticMesh(nullptr);
		}

		PlayerCurState = EWeaponUseState::bat;
		break;
	case EStaticItemSlot::RSword:
		//StaticItemMesh[static_cast<uint8>(EStaticItemSlot::RAxe)]->SetStaticMesh(nullptr);
		for (size_t i = 0; i < static_cast<size_t>(EStaticItemSlot::SlotMax); i++)
		{
			StaticItemMeshs[i]->SetStaticMesh(nullptr);
		}

		PlayerCurState = EWeaponUseState::Sword;
		break;
	case EStaticItemSlot::RAxe:
		for (size_t i = 0; i < static_cast<size_t>(EStaticItemSlot::SlotMax); i++)
		{
			StaticItemMeshs[i]->SetStaticMesh(nullptr);
		}
		PlayerCurState = EWeaponUseState::Axe;
		break;
	case EStaticItemSlot::SlotMax:
		break;
	default:
		break;
	}


	StaticItemMeshs[SlotIndex]->SetStaticMesh(_Mesh);
}

void AC_GlobalPlayer::ChangeSlotMeshServer_Implementation(EStaticItemSlot _Slot, UStaticMesh* _Mesh)
{
	if (true == IsPlayerDieCpp)
	{
		return;
	}

	if (true == IsHitCpp)
	{
		return;
	}

	if (true == IsAimCpp)
	{
		return;
	}

	if (true == IsFireCpp)
	{
		return;
	}

	ChangeSlotMesh(_Slot, _Mesh);
}

void AC_GlobalPlayer::ChangeSlotSkeletal_Implementation(ESkerItemSlot _Slot)
{
	uint8 SlotIndex = static_cast<uint8>(_Slot);
	if (SkeletalItemMeshes.Num() <= SlotIndex)
	{
		UE_LOG(LogTemp, Fatal, TEXT("%S(%u)> if (ItemMeshs.Num() <= static_cast<uint8>(_Slot))"), __FUNCTION__, __LINE__);
		return;
	}

	UEnum* Enum = StaticEnum<ESkerItemSlot>();
	// USkeletalMeshComponent 슬롯 전용
	for (size_t i = 0; i < static_cast<size_t>(EStaticItemSlot::SlotMax); i++)
	{
		StaticItemMeshs[i]->SetStaticMesh(nullptr);
	}

	switch (_Slot)
	{
	case ESkerItemSlot::RRifle:
		if (PlayerCurState == EWeaponUseState::Rifle)
		{
			return;
		}

		if (false == GunWeapon.Contains(EWeaponUseState::Rifle))
		{
			return;
		}

		if (GetSkeletalItemMesh()[static_cast<uint8>(ESkerItemSlot::RRifle)]->GetSkinnedAsset() != nullptr)
		{
			return;
		}

		if (nullptr != CurWeapon)
		{
			CurWeapon->Destroy();
			CurWeapon = nullptr;
			for (size_t i = 0; i < static_cast<size_t>(ESkerItemSlot::SlotMax); i++)
			{
				SkeletalItemMeshes[i]->SetSkinnedAsset(nullptr);
			}
		}

		CurWeapon=GetWorld()->SpawnActor<AC_EquipWeapon>(GunWeapon[EWeaponUseState::Rifle]);

		CurWeapon->GetComponentByClass<UC_GunComponent>()->AttachRilfe(this);
		LineTraceDamage = RifleAtt;
		LineTraceRange = RifleRange;
		break;
	case ESkerItemSlot::RRifle2:
		if (PlayerCurState == EWeaponUseState::Rifle2)
		{
			return;
		}

		if (false == GunWeapon.Contains(EWeaponUseState::Rifle2))
		{
			return;
		}

		if (GetSkeletalItemMesh()[static_cast<uint8>(ESkerItemSlot::RRifle2)]->GetSkinnedAsset() != nullptr)
		{
			return;
		}

		if (nullptr != CurWeapon)
		{
			CurWeapon->Destroy();
			CurWeapon = nullptr;
			for (size_t i = 0; i < static_cast<size_t>(ESkerItemSlot::SlotMax); i++)
			{
				SkeletalItemMeshes[i]->SetSkinnedAsset(nullptr);
			}
		}

		CurWeapon = GetWorld()->SpawnActor<AC_EquipWeapon>(GunWeapon[EWeaponUseState::Rifle2]);

		CurWeapon->GetComponentByClass<UC_GunComponent>()->AttachRilfe2(this);
		LineTraceDamage = RifleAtt;
		LineTraceRange = RifleRange;
		break;
	case ESkerItemSlot::RPistol:
		if (PlayerCurState == EWeaponUseState::Pistol)
		{
			return;
		}

		if (false == GunWeapon.Contains(EWeaponUseState::Pistol))
		{
			return;
		}

		if (GetSkeletalItemMesh()[static_cast<uint8>(ESkerItemSlot::RPistol)]->GetSkinnedAsset() != nullptr)
		{
			return;
		}

		if (nullptr != CurWeapon)
		{
			CurWeapon->Destroy();
			for (size_t i = 0; i < static_cast<size_t>(ESkerItemSlot::SlotMax); i++)
			{
				SkeletalItemMeshes[i]->SetSkinnedAsset(nullptr);
			}
		}


		CurWeapon = GetWorld()->SpawnActor<AC_EquipWeapon>(GunWeapon[EWeaponUseState::Pistol]);

		CurWeapon->GetComponentByClass<UC_GunComponent>()->AttachPistol1(this);
		LineTraceDamage = PistolAtt;
		LineTraceRange = PistolRange;
		break;
	case ESkerItemSlot::RPistol2:
		if (PlayerCurState == EWeaponUseState::Pistol2)
		{
			return;
		}

		if (false == GunWeapon.Contains(EWeaponUseState::Pistol2))
		{
			return;
		}

		if (GetSkeletalItemMesh()[static_cast<uint8>(ESkerItemSlot::RPistol2)]->GetSkinnedAsset() != nullptr)
		{
			return;
		}

		if (nullptr != CurWeapon)
		{
			CurWeapon->Destroy();
			CurWeapon = nullptr;
			for (size_t i = 0; i < static_cast<size_t>(ESkerItemSlot::SlotMax); i++)
			{
				SkeletalItemMeshes[i]->SetSkinnedAsset(nullptr);
			}
		}

		CurWeapon = GetWorld()->SpawnActor<AC_EquipWeapon>(GunWeapon[EWeaponUseState::Pistol2]);

		CurWeapon->GetComponentByClass<UC_GunComponent>()->AttachPistol2(this);
		LineTraceDamage = PistolAtt;
		LineTraceRange = PistolRange;
		break;
	case ESkerItemSlot::RShotgun:

		//ShotGun
		if (PlayerCurState == EWeaponUseState::Shotgun)
		{
			return;
		}

		if (false == GunWeapon.Contains(EWeaponUseState::Shotgun))
		{
			return;
		}

		if (GetSkeletalItemMesh()[static_cast<uint8>(ESkerItemSlot::RShotgun)]->GetSkinnedAsset() != nullptr)
		{
			return;
		}

		if (nullptr != CurWeapon)
		{
			CurWeapon->Destroy();
			CurWeapon = nullptr;
			for (size_t i = 0; i < static_cast<size_t>(ESkerItemSlot::SlotMax); i++)
			{
				SkeletalItemMeshes[i]->SetSkinnedAsset(nullptr);
			}
		}


		CurWeapon = GetWorld()->SpawnActor<AC_EquipWeapon>(GunWeapon[EWeaponUseState::Shotgun]);

		CurWeapon->GetComponentByClass<UC_GunComponent>()->AttachShotGun(this);
		LineTraceDamage = ShotGunAtt;
		LineTraceRange = ShotGunRange;
		break;
	case ESkerItemSlot::SlotMax:
		break;
	default:
		break;
	}
}

void AC_GlobalPlayer::ChangeSlotSkeletalServer_Implementation(ESkerItemSlot _Slot)
{
	if (true == IsPlayerDieCpp)
	{
		return;
	}

	if (true == IsHitCpp)
	{
		return;
	}

	if (true == IsAimCpp)
	{
		return;
	}

	if (true == IsFireCpp)
	{
		return;
	}

	ChangeSlotSkeletal(_Slot);
}

void AC_GlobalPlayer::ChangeNoWeapon_Implementation()
{

	if (PlayerCurState == EWeaponUseState::NoWeapon)
	{
		return;
	}

	if (CurWeapon != nullptr)
	{
		CurWeapon->Destroy();
	}
	
	{
		UEnum* Enum = StaticEnum<ESkerItemSlot>();
		// USkeletalMeshComponent 슬롯 전용
		for (size_t i = 0; i < static_cast<size_t>(ESkerItemSlot::SlotMax); i++)
		{
			SkeletalItemMeshes[i]->SetSkeletalMesh(nullptr);
		}

	}

	{
		UEnum* Enum = StaticEnum<ESkerItemSlot>();
		// USkeletalMeshComponent 슬롯 전용
		for (size_t i = 0; i < static_cast<size_t>(EStaticItemSlot::SlotMax); i++)
		{
			StaticItemMeshs[i]->SetStaticMesh(nullptr);
		}
	}
	
	PlayerCurState = EWeaponUseState::NoWeapon;
}

void AC_GlobalPlayer::ChangeNoWeaponServer_Implementation()
{
	if (true == IsPlayerDieCpp)
	{
		return;
	}

	if (true == IsHitCpp)
	{
		return;
	}

	if (true == IsAimCpp)
	{
		return;
	}

	if (true == IsFireCpp)
	{
		return;
	}

	ChangeNoWeapon();
}

void AC_GlobalPlayer::FireStart_Implementation(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		return;
	}

	if (true == IsPlayerDieCpp)
	{
		return;
	}

	if (true == IsHitCpp)
	{
		return;
	}

	switch (PlayerCurState)
	{
	case EWeaponUseState::Rifle:
		if (magazinecapacity[ESkerItemSlot::RRifle] == 0)
		{
			CreateUseGunCheckSound();
			return;
		}
		break;
	case EWeaponUseState::Pistol:
		if (magazinecapacity[ESkerItemSlot::RPistol] == 0)
		{
			CreateUseGunCheckSound();
			return;
		}
		break;
	case EWeaponUseState::Rifle2:
		if (magazinecapacity[ESkerItemSlot::RRifle2] == 0)
		{
			CreateUseGunCheckSound();
			return;
		}
		break;
	case EWeaponUseState::Pistol2:
		if (magazinecapacity[ESkerItemSlot::RPistol2] == 0)
		{
			CreateUseGunCheckSound();
			return;
		}
	case EWeaponUseState::Shotgun:
		if (magazinecapacity[ESkerItemSlot::RShotgun] == 0)
		{
			CreateUseGunCheckSound();
			return;
		}
		break;
	default:
		break;
	}
	/*if (LineTracemagazinecapacity == 0)
	{
		return;
	}*/

	if (true == IsAimCpp)
	{
		IsFireCpp = true;

	}

	if (true == IsFireCpp)
	{
		if (UGameplayStatics::GetGameMode(GetWorld()) == nullptr)
		{
			return;
		}

		FTimerHandle ShotGunTime;

		switch (PlayerCurState)
		{
		case EWeaponUseState::Rifle:
		case EWeaponUseState::Rifle2:
			LineTraceRange = RifleRange;
			GunLineTrace();
			break;
		case EWeaponUseState::Pistol:
		case EWeaponUseState::Pistol2:
			GunLineTrace();
			break;
		case EWeaponUseState::Shotgun:

			if (true == IsShotGunShot)
			{
				return;
			}

			IsShotGunShot = true;
			ShotGunLineTrace();
			GetWorld()->GetTimerManager().SetTimer(ShotGunTime, this, &AC_GlobalPlayer::ResetShotGunAtt, 0.5f, false);
			break;
		default:
			break;
		}
		MakeNoise(50.0f);
	}

}

void AC_GlobalPlayer::FireEnd_Implementation(const FInputActionValue& Value)
{
	GetWorld()->GetTimerManager().ClearTimer(timer);
	IsFireCpp = false;
	BulletInfos.Empty();
}



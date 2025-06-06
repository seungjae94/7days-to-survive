// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/C_UITableRow.h"
#include "Monster/MonsterData/MonsterDataRow.h"
#include "Player/Global/C_PlayerEnum.h"
#include "C_STSInstance.generated.h"

class UC_MapDataAsset;
class UC_MapDataObject;
struct FC_PlayerDataTable;
struct FC_PlayerSpawnData;
struct FC_WeaponDataTable;
struct FC_UITableRow;
/**
 * 
 */
UCLASS()
class SEVENDAYS_TO_SURVIVE_API UC_STSInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UC_STSInstance();

	// BeginPlay 역할
	void Init() override;

	UFUNCTION(BlueprintPure)
	UC_MapDataAsset* GetMapDataAsset();

	UFUNCTION(BlueprintPure)
	UC_MapDataObject* GetMapDataObject();

	FMonsterDataRow* GetMonsterData(FName _Name);

	UFUNCTION(BlueprintCallable)
	TArray<struct FC_UITableRow> GetPlayerInfoData();

	UFUNCTION(BlueprintCallable)
	void SetPlayerInfo(FString _Name, FString _UserIP);

	FC_PlayerDataTable* GetPlayerDataTable();
	
	FC_PlayerSpawnData* GetPlayerSpawnDataTable();
	
	UFUNCTION(BlueprintCallable)
	TArray<USkeletalMesh*> GetPlayerSpawnMesh();

	UFUNCTION(BlueprintPure)
	TArray<FVector> GetPlayerSpawnLocation();

	FC_WeaponDataTable* GetWeaPonDataTable(FName _Name);

	TMap<FName, FUIWidgetDataRow*> GetUIWidgetDataMap();

	FC_UITableRow TitleToGameInfo;

	int GenerateRandomInt(int _Min, int _Max);
	float GenerateRandomFloat(float _Min, float _Max);
	FVector GenerateRandomVector(FBox2D _Box);


	UFUNCTION(BlueprintCallable)
	void SetPlayerMesh(EPlayerMesh _Mesh);
	
	UFUNCTION(BlueprintPure)
	EPlayerMesh GetPlayerMesh()
	{
		if (true==PlayerMeshs.IsEmpty())
		{
			return EPlayerMesh::Player1;
		}
		return PlayerMeshs[PlayerMeshs.Num()-1];
	}

	UFUNCTION()
	int GetNetToken()
	{
		return ++NetToken;
	}
public:
	// 인게임 밤낮 구분변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayorNight", meta = (AllowPrivateAccess = "true"))
	bool IsDay; 

	// 인게임 날자카운트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayorNight", meta = (AllowPrivateAccess = "true"))
	int DayCount;

	// 시간UI를 띄우기 위해 계산하는 시간 
	// ★★★12기준으로 시간적용(밤낮 변경)★★★
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayorNight", meta = (AllowPrivateAccess = "true"))
	int Time;

	// 실제 인게임에서 UI시간
	// ★★★ 06시 ~ 17시 == 낮,  18시 ~ 05시 == 밤  ★★★
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayorNight", meta = (AllowPrivateAccess = "true"))
	int GameUI_Time;

	// 인게임에서 엔딩 알림
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayorNight", meta = (AllowPrivateAccess = "true"))
	bool Game_End;

	// 플레이어 도착 알림
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayorNight", meta = (AllowPrivateAccess = "true"))
	bool Player_arrive;
	

	UFUNCTION(BlueprintCallable)
	void SetSpawnMonster();
	void AddSpawnPoint(class AC_MonsterSpawnPoint* _Point);

	UFUNCTION(BlueprintCallable)
	void SetZombieTarget();
	void AddZombieArray(class AC_ZombieBase* _Zombie);
	void RemoveZombieArray(AC_ZombieBase* _Zombie);


	UFUNCTION(BlueprintCallable)
	FString GetPlayerName();

	void AddPlayerArray(AActor* _Actor);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AC_MonsterSpawnPoint*> SpawnArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AC_ZombieBase*> ZombieArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AActor*> PlayerArray;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UDataTable* PlayerDataTable=nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UDataTable* PlayerSpawnDataTable=nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponDataTable=nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UDataTable* DT_UIData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UDataTable* DT_MonsterData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataAsset", meta = (AllowPrivateAccess = "true"))
	UC_MapDataAsset* MapDataAsset = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DataAsset", meta = (AllowPrivateAccess = "true"))
	UC_MapDataObject* MapDataObject = nullptr;

	FRandomStream RandomStream;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataAsset", meta = (AllowPrivateAccess = "true"))
	UDataTable* WidgetDataTable = nullptr;

	UPROPERTY()
	TArray<EPlayerMesh> PlayerMeshs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataAsset", meta = (AllowPrivateAccess = "true"))
	FString PlayerName;

	UPROPERTY()
	int NetToken = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "End", meta = (AllowPrivateAccess = "true"))
	bool CloseGame =false;
};

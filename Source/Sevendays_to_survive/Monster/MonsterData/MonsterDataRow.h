// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Sound/SoundWave.h"
#include "MonsterDataRow.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FMonsterDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float SightRange = 0.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float AttackRange = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Att = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double HP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double MonsterRange = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<uint8, class UAnimMontage* > AnimMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<uint8, class USoundWave* > SoundFile;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//double PatrolTime = 0.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int PatrolCount = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float PatrolMinRange = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float PatrolMaxRange = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TMap<FMonsterDataRow, class UAnimMontage* > AnimMontages;
};

UCLASS()
class UMonsterDataObject : public UObject
{
	GENERATED_BODY()


public:
	void MonsterDataInit(const FMonsterDataRow& Row);

	double* GetAttRef() {
		return &Att;
	}
	double* GetHPRef() {
		return &HP;
	}
	double* GetMonsterRangeRef() {
		return &MonsterRange;
	}


	const double GetAtt() {
		return Att;
	}


	const double GetHP() {
		return HP;
	}


	const double GetMonsterRange() {
		return MonsterRange;
	}

	void SetOriginPos(FVector _OriginVec) {
		OriginPos = _OriginVec;
	}

	bool PathIsEmpty() {
		if (0 == PathPoints.Num()) {
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetPath(TArray<FVector>& _Array) {
		PathPoints = _Array;
	}

	TArray<FVector>& GetPath() {
		return PathPoints;
	}

	void RemovePath() {
		PathPoints.Empty();
	}

	int GetPathLength() {
		return PathPoints.Num();
	}

	void PathHeadRemove() {
		if (PathIsEmpty()) {
			return;
		}
		PathPoints.RemoveAt(0);
	}

	const FVector NextPath() {
		return PathPoints[0];
	}
private:
	UPROPERTY()
	double Att = 0.0f;

	UPROPERTY()
	double HP = 0.0f;

	UPROPERTY()
	double MonsterRange = 0.0f;

	UPROPERTY()
	FVector OriginPos = FVector::ZeroVector;

	UPROPERTY()
	TArray<FVector> PathPoints;
};

﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterAI/MonsterTask/C_MonsterWait.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/MonsterAI/C_MonsterAIBase.h"

UC_MonsterWait::UC_MonsterWait()
{
	NodeName = "MonsterWait";
	bNotifyTick = true;
}

EBTNodeResult::Type UC_MonsterWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_MonsterAIBase* Controller = GetController(&OwnerComp);
	UBlackboardComponent* BlackBoard = GetBlackBoard(&OwnerComp);

	if (!IsValid(Controller)) {
		UE_LOG(LogTemp, Warning, TEXT("MonsterController is Not Work BTTESK %d  %s"), __LINE__, ANSI_TO_TCHAR(__FUNCTION__));
		return EBTNodeResult::Failed;
	}

	if (true == IsPerceptionUpdated(Controller)) {
		return EBTNodeResult::Succeeded;
	}
	//GetSelf(&OwnerComp)->SetState(MonsterEnum::Idle);
	GetSelf(&OwnerComp)->Idle();
	return EBTNodeResult::InProgress;
}

void UC_MonsterWait::InitTask(UBehaviorTreeComponent* OwnerComp)
{
	Super::InitTask(OwnerComp);
}

void UC_MonsterWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AC_MonsterAIBase* Controller = GetController(&OwnerComp);
	UBlackboardComponent* BlackBoard = GetBlackBoard(&OwnerComp);

	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (true == IsPerceptionUpdated(Controller)) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	float Time = BlackBoard->GetValueAsFloat(*WaitTime);
	if (Waiting > Time) {
		BlackBoard->SetValueAsFloat(*WaitTime, Time + DeltaSeconds);
	}
	else {
		BlackBoard->SetValueAsFloat(*WaitTime, 0.f);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
}

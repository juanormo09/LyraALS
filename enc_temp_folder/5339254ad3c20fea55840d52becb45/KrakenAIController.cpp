// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/KrakenAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void AKrakenAIController::BeginPlay()
{
	if (BTKraken)
	{
		RunBehaviorTree(BTKraken);
	}
}

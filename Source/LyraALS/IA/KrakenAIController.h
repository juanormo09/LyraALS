// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KrakenAIController.generated.h"


class UBehaviorTree; 
class UBlackboardComponent;
/**
 * 
 */
UCLASS()
class LYRAALS_API AKrakenAIController : public AAIController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BT")
	TObjectPtr<UBehaviorTree> BTKraken;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/Kraken.h"
#include "Blueprint/UserWidget.h"
#include "IA/KrakenAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AKraken::AKraken()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKraken::BeginPlay()
{
	Super::BeginPlay();
	if (HealthEnemyWidgetclass)
	{
		HealthEnemy = CreateWidget<UUserWidget>(GetWorld(), HealthEnemyWidgetclass);
		if (HealthEnemy)
		{
			HealthEnemy->AddToViewport();
		}
	}
}

// Called every frame
void AKraken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKraken::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AKraken::DamageKraken(FName Bone)
{
	if (Health > 0)
	{
		if (Bone == FName("KRAKEN_SHELL"))
		{
			DamageRecieved = FMath::RandRange(1.0f, 10.f);
			if (Health <= 0.f)
			{
				Health = 0.f;
			}
			Health -= DamageRecieved;
		}
		else if (Bone == FName("KRAKEN_BEAK_LEFT"))
		{
			DamageRecieved = FMath::RandRange(50.f, 100.f);
			if (Health <= 0.f)
			{
				Health = 0.f;
			}
			Health -= DamageRecieved;
		}
		else if (Bone == FName("KRAKEN_BEAK_LOWER"))
		{
			DamageRecieved = FMath::RandRange(50.f, 100.f);
			if (Health <= 0.f)
			{
				Health = 0.f;
			}
			Health -= DamageRecieved;
		}
		else if (Bone == FName("KRAKEN_BEAK_RIGHT"))
		{
			DamageRecieved = FMath::RandRange(50.f, 100.f);
			if (Health <= 0.f)
			{
				Health = 0.f;
			}
			Health -= DamageRecieved;
		}
		else if (Bone == FName("KRAKEN_BEAK_UPPER"))
		{
			DamageRecieved = FMath::RandRange(50.f, 100.f);
			if (Health <= 0.f)
			{
				Health = 0.f;
			}
			Health -= DamageRecieved;
		}
		else
		{
			DamageRecieved = FMath::RandRange(10.f, 30.f);
			if (Health <= 0.f)
			{
				Health = 0.f;
			}
			Health -= DamageRecieved;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("kraken has recieve damage: %f"), Health);
}


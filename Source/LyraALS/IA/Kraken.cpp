// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/Kraken.h"
#include "Blueprint/UserWidget.h"
#include "IA/KrakenAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "Character/LyraCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"

// Sets default values
AKraken::AKraken()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(GetRootComponent());
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
	if (AKrakenAIController* AiController = Cast<AKrakenAIController>(GetController()))
	{
		BlackboardKraken = AiController->GetBlackboardComponent();
	}

	Lyra = Cast<ALyraCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ALyraCharacter::StaticClass()));

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AKraken::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AKraken::OverlapEnd);
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
	else
	{
		if (DeathMontage)
		{
			PlayAnimMontage(DeathMontage);
			BlackboardKraken->SetValueAsBool(FName("IsKrakenDead"), true);
		}
	}
	
	BlackboardKraken->SetValueAsObject(FName("Player"), Lyra);
	UE_LOG(LogTemp, Warning, TEXT("kraken has recieve damage: %f"), Health);
}

void AKraken::AttackKraken()
{

}

void AKraken::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALyraCharacter* LyraCharacter = Cast<ALyraCharacter>(OtherActor);
	if (OtherActor)
	{
		BlackboardKraken->SetValueAsObject(FName("Player"), LyraCharacter);
	}
}

void AKraken::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


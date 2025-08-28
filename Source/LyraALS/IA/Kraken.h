// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kraken.generated.h"

class UUSerWidget;
class UBlackboardComponent;
class USphereComponent;
class ALyraCharacter;

UCLASS()
class LYRAALS_API AKraken : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKraken();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HealthEnemyWidgetclass;

	UPROPERTY(transient)
	TObjectPtr<UUserWidget> HealthEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealthHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageRecieved;

	UPROPERTY(Transient)
	TObjectPtr<UBlackboardComponent> BlackboardKraken;

	UPROPERTY(Transient)
	TObjectPtr<ALyraCharacter> Lyra;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void DamageKraken(FName Bone);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

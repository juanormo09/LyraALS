// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums.h"
#include "Structs.h"
#include "LyraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ULyraAnimInstance;
class USkeletalMeshComponent;
class UAnimMontage;
class UAnimationAsset;
class UDataTable;


UCLASS()
class LYRAALS_API ALyraCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALyraCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> ViewCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> Rifle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Animation | Layers")
	TSubclassOf<ULyraAnimInstance> UnArmedLayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Animation | Layers")
	TSubclassOf<ULyraAnimInstance> PistolLayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Animation | Layers")
	TSubclassOf<ULyraAnimInstance> RifleLayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Montages")
	TObjectPtr<UAnimMontage> ShootPistolMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Montages")
	TObjectPtr<UAnimMontage> ShootRifleMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Animations")
	TObjectPtr<UAnimationAsset> ShootPistolAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Animations")
	TObjectPtr<UAnimationAsset> ShootRifleAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Montages")
	TObjectPtr<UAnimMontage> ReloadPistolMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Montages")
	TObjectPtr<UAnimMontage> ReloadRifleMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Animations")
	TObjectPtr<UAnimationAsset> ReloadPistolAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Animations")
	TObjectPtr<UAnimationAsset> ReloadRifleAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds | WeaponSounds")
	TObjectPtr<USoundBase> PistolShoot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds | WeaponSounds")
	TObjectPtr<USoundBase> RifleShoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds | WeaponSounds")
	TObjectPtr<USoundBase> ImpactShoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds | WeaponSounds")
	TObjectPtr<USoundBase> ImpactShootDebrise;

	UPROPERTY(EditAnywhere, Category = "DataTable")
	TObjectPtr<UDataTable> GunGatesTable;

	UPROPERTY(Transient)
	FTimerHandle FirePistolTimerHandle;
	
	UPROPERTY(Transient)
	FTimerHandle FireRifleTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadOnly)
	EGuns GunSelected = EGuns::EGS_Unarmed;

	UPROPERTY(BlueprintReadOnly)
	EMovement MovementState = EMovement::EM_Jogging;

	UPROPERTY(BlueprintReadOnly)
	FSockets Sockets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanFire = true;

	//creation of Tmap with key Enum and value Strutc
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EMovement, FCharacterVelocities> GateSettings;

	UPROPERTY(BlueprintReadWrite)
	float GroundDistance;

	FWeaponFireResult FireResult;


	void SelectedGun(float Value);
	void LookCharacter(FVector2D Value);
	void MovementCharacter(FVector2D Value);
	void LyraAim(bool Value);
	void UpdateGate(EMovement NewMovementState);
	void ChangeWeapon();
	void FireWeapon(bool Value);
	void PistolCanFire();
	void RifleCanFire();
	void ReloadWeapon(bool Value);
	//funcion para los sonidos 
	void PlaySoundsWeapons(FName BoneName, USoundBase* SoundtoPlay, USkeletalMeshComponent* TargetMesh);
	//funcion para el lientrace
	FWeaponFireResult LineTraceFire();

	UFUNCTION(BlueprintImplementableEvent)
	void CallTimeline();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReverseTimeline();
};

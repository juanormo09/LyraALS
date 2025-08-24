// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LyraCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "LyraAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/StaticMeshComponent.h"
#include "IA/Kraken.h"


// Sets default values
ALyraCharacter::ALyraCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("View Camera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Pistol = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pistol"));
	Pistol->SetupAttachment(GetMesh(), "PistolUnEquipe");

	Rifle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rifle"));
	Rifle->SetupAttachment(GetMesh(), "RifleUnEquipe");

	Helmet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Helmet"));
	Helmet->SetupAttachment(GetMesh(), "Helmet");

	TorchHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torch Holder"));
	TorchHolder->SetupAttachment(GetMesh(), "TorchHolder");

	Torch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torch"));
	Torch->SetupAttachment(GetMesh(), "Torch");

	Holster = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Holster"));
	Holster->SetupAttachment(GetMesh(), "PistolHolster");

	HealthBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Health Bar"));
	HealthBar->SetupAttachment(GetMesh(), "HealthBar");

	/*//initialized Tmap Values for each key
	GateSettings.Add(EMovement::EM_Jogging, FCharacterVelocities{ 
		500.f,
		500.f, 
		500.f, 
		1.0f, 
		0.0f, 
		true });
	GateSettings.Add(EMovement::EM_Walking, FCharacterVelocities{ 
		250.f, 
		250.f, 
		250.f, 
		1.0f, 
		0.0f, 
		true });
	GateSettings.Add(EMovement::EM_Crouch, FCharacterVelocities{
		250.f,
		250.f,
		250.f,
		1.0f,
		0.0f,
		true });*/
}

// Called when the game starts or when spawned
void ALyraCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateGate(MovementState);
	GetMesh()->LinkAnimClassLayers(UnArmedLayer);
	if (CrosshairWidgetclass)
	{
		Crosshair = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetclass);
		if (Crosshair)
		{
			Crosshair->AddToViewport();
		}
	}

	Kraken = Cast<AKraken>(UGameplayStatics::GetActorOfClass(GetWorld(), AKraken::StaticClass()));
	
}

// Called every frame
void ALyraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float CapsuleHalfHeighLyra = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	FVector Start = GetActorLocation() - FVector(0.0f, 0.0f, CapsuleHalfHeighLyra);
	FVector End = GetActorLocation() - FVector(0.0f, 0.0f, 1000.f);

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		End,
		5.f,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (bHit && HitResult.Distance)
	{
		UE_LOG(LogTemp, Display, TEXT("Distance to ground: %f"), HitResult.Distance);
		GroundDistance = HitResult.Distance;
	}
}

// Called to bind functionality to input
void ALyraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALyraCharacter::SelectedGun(float Value)
{
	int32 Idles = FMath::RoundToInt(Value);//convert float to int 
	switch (Idles)
	{
	case 1:
		GunSelected = EGuns::EGS_Unarmed;
		GetMesh()->LinkAnimClassLayers(UnArmedLayer);
		UpdateGate(MovementState);
		ChangeWeapon();
		break;
	case 2:
		GunSelected = EGuns::EGS_Pistol;
		GetMesh()->LinkAnimClassLayers(PistolLayer);
		UpdateGate(MovementState);
		ChangeWeapon();
		break;
	case 3:
		GunSelected = EGuns::EGS_Rifle;
		GetMesh()->LinkAnimClassLayers(RifleLayer);
		UpdateGate(MovementState);
		ChangeWeapon();
		break;
	default:
		break;
	}
}

void ALyraCharacter::LookCharacter(FVector2D Value)
{
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ALyraCharacter::MovementCharacter(FVector2D Value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(FowardDirection, Value.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, Value.X);
}

void ALyraCharacter::LyraAim(bool Value)
{
	if (Value)
	{
		switch (MovementState)
		{
		case EMovement::EM_Walking:
			UpdateGate(MovementState = EMovement::EM_Walking);
			CallTimeline();
			break;
		case EMovement::EM_Jogging:
			UpdateGate(MovementState = EMovement::EM_Walking);
			CallTimeline();
			break;
		case EMovement::EM_Crouch:
			UpdateGate(MovementState = EMovement::EM_Crouch);
			CallTimeline();
			break;
		default:
			break;
		}
	}
	else
	{
		//if(CameraBoom) CameraBoom->TargetArmLength = 300.f;
		switch (MovementState)
		{
		case EMovement::EM_Walking:
			UpdateGate(MovementState = EMovement::EM_Jogging);
			ReverseTimeline();
			break;
		case EMovement::EM_Jogging:
			UpdateGate(MovementState = EMovement::EM_Jogging);
			ReverseTimeline();
			break;
		case EMovement::EM_Crouch:
			UpdateGate(MovementState = EMovement::EM_Crouch);
			ReverseTimeline();
			break;
		default:
			break;
		}
	}
}

void ALyraCharacter::UpdateGate(EMovement NewMovementState)
{ 
	if (!GunGatesTable) return;

	TArray<FCharacterVelocities*> AllRows;
	GunGatesTable->GetAllRows(TEXT(""), AllRows);

	for (FCharacterVelocities* Row : AllRows)
	{
		if (Row)
		{
			if (Row->MovementState == NewMovementState && Row->GunType == GunSelected)
			{
				UE_LOG(LogTemp, Warning, TEXT("Row: Movement=%d, Gun=%d"), (int32)Row->MovementState, (int32)Row->GunType);
				if (GetCharacterMovement())
				{
					if (Row->MovementState == EMovement::EM_Crouch)
					{
						GetCharacterMovement()->MaxWalkSpeedCrouched = Row->MaxWalkSpeed;
					}
					GetCharacterMovement()->MaxWalkSpeed = Row->MaxWalkSpeed;
					GetCharacterMovement()->MaxAcceleration = Row->MaxAcceleration;
					GetCharacterMovement()->BrakingDecelerationWalking = Row->BreakingDeceleration;
					GetCharacterMovement()->BrakingFrictionFactor = Row->BreakingFrictionFactor;
					GetCharacterMovement()->BrakingFriction = Row->BreakingFriction;
					GetCharacterMovement()->bUseSeparateBrakingFriction = Row->bUseSeparateBreakingFriction;
				}
			}
		}

	}
	//esto lo puedo convertir a un for each, hago un puntero al data table y un arraay para almacenar cada row y despues llamo todos lo gat all row y con el for each lo seteo
	/*
	FCharacterVelocities* Velocitys = GateSettings.Find(NewMovementState);
	if (Velocitys)
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = Velocitys->MaxWalkSpeed;
			GetCharacterMovement()->MaxAcceleration = Velocitys->MaxAcceleration;
			GetCharacterMovement()->BrakingDecelerationWalking = Velocitys->BreakingDeceleration;
			GetCharacterMovement()->BrakingFrictionFactor = Velocitys->BreakingFrictionFactor;
			GetCharacterMovement()->BrakingFriction = Velocitys->BreakingFriction;
			GetCharacterMovement()->bUseSeparateBrakingFriction = Velocitys->bUseSeparateBreakingFriction;
		}
	}*/
}

void ALyraCharacter::ChangeWeapon()
{
	switch (GunSelected)
	{
	case EGuns::EGS_Unarmed:
		Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Sockets.PistolUnEquiped);
		Rifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Sockets.RifleUnEquiped);
		break;
	case EGuns::EGS_Pistol:
		Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Sockets.WeaponEquiped);
		Rifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Sockets.RifleUnEquiped);
		break;
	case EGuns::EGS_Rifle:
		Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Sockets.PistolUnEquiped);
		Rifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Sockets.WeaponEquiped);
		break;
	default:
		break;
	}
}

void ALyraCharacter::FireWeapon(bool Value)
{
	if (MovementState == EMovement::EM_Walking || MovementState == EMovement::EM_Crouch)
	{	
		if (GunSelected == EGuns::EGS_Pistol && bCanFire && bHaveBulletInPistol)
		{
			if (ShootPistolMontage && ShootPistolAnim)
			{
				PlayAnimMontage(ShootPistolMontage);
				Pistol->PlayAnimation(ShootPistolAnim, false);
			}
			PlaySoundsWeapons(FName("Barrel"), PistolShoot, Pistol);
			bCanFire = false;
			bHaveBulletInPistol = PistolBulletManager();
			FireResult = LineTraceFire();
			if (FireResult.bHit)
			{
				if (FireResult.PhysicMaterial)
				{
					EPhysicalSurface Surface = UGameplayStatics::GetSurfaceType(OutHit);
					if (Surface == SurfaceType1 && ImpactShootGlass && ImpactShootGlassDebrise)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactShootGlass, FireResult.ImpactPoint);
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactShootDebrise, FireResult.ImpactPoint);
					}
					else if (ImpactShoot && ImpactShootDebrise)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactShoot, FireResult.ImpactPoint);
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactShootDebrise, FireResult.ImpactPoint);
					}
				}
			}
			NiagaraWeaponLineTracePistol();
			if (FireResult.HitActor == Kraken)
			{
				Kraken->DamageKraken(FireResult.BoneNameHit);
			}
			GetWorld()->GetTimerManager().SetTimer(FirePistolTimerHandle, this, &ALyraCharacter::PistolCanFire, 0.5f, false);
		}
		else if (GunSelected == EGuns::EGS_Rifle && bCanFire && bHaveBulletInRifle)
		{
			if (ShootRifleMontage && ShootRifleAnim)
			{
				PlayAnimMontage(ShootRifleMontage);
				Rifle->PlayAnimation(ShootRifleAnim, false);
			}
			PlaySoundsWeapons(FName("Barrel"), RifleShoot, Rifle);
			bCanFire = false;
			bHaveBulletInRifle = RifleBulletManager();
			FireResult = LineTraceFire();
			if (FireResult.bHit)
			{
				if (FireResult.PhysicMaterial)
				{
					EPhysicalSurface Surface = UGameplayStatics::GetSurfaceType(OutHit);
					if (Surface == SurfaceType1 && ImpactShootGlass && ImpactShootGlassDebrise)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactShootGlass, FireResult.ImpactPoint);
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactShootDebrise, FireResult.ImpactPoint);
					}
					else if (ImpactShoot && ImpactShootDebrise)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactShoot, FireResult.ImpactPoint);
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactShootDebrise, FireResult.ImpactPoint);
					}
				}
			}
			NiagaraWeaponLineTraceRifle();
			GetWorld()->GetTimerManager().SetTimer(FireRifleTimerHandle, this, &ALyraCharacter::RifleCanFire, 0.2f, false);
		}
	}
}

void ALyraCharacter::PistolCanFire()
{
	bCanFire = true;
}

void ALyraCharacter::RifleCanFire()
{
	bCanFire = true;
}

void ALyraCharacter::ReloadWeapon(bool Value)
{
	switch (GunSelected)
	{
	case EGuns::EGS_Unarmed:
		break;
	case EGuns::EGS_Pistol:
		if (ReloadPistolMontage && ReloadPistolAnim && PistolClipAmount > 0.0f)
		{
			PlayAnimMontage(ReloadPistolMontage);
			Pistol->PlayAnimation(ReloadPistolAnim, false);
			PistolClipAmount -= 1.0f;
			PistolBulletAmount = PistolBulletClipSize;
			bHaveBulletInPistol = true;
		}
		break;
	case EGuns::EGS_Rifle:
		UE_LOG(LogTemp, Warning, TEXT("Reload"));
		if (ReloadRifleMontage && ReloadRifleAnim && RifleClipAmount > 0.0f)
		{
			PlayAnimMontage(ReloadRifleMontage);
			Rifle->PlayAnimation(ReloadRifleAnim, false);
			RifleClipAmount -= 1.0f;
			RifleBulletAmount = RifleBulletClipSize;
			bHaveBulletInRifle = true;
		}
		break;
	default:
		break;
	}
}

bool ALyraCharacter::PistolBulletManager()
{
	if (PistolBulletAmount > 0.0f)
	{
		PistolBulletAmount -= 1.f;
	}
	return PistolBulletAmount > 0.0f;
}

bool ALyraCharacter::RifleBulletManager()
{
	if (RifleBulletAmount > 0.0f)
	{
		RifleBulletAmount -= 1.0f;
	}
	return RifleBulletAmount > 0.0f;
}

void ALyraCharacter::PlaySoundsWeapons(FName BoneName, USoundBase* SoundtoPlay, USkeletalMeshComponent* TargetMesh)
{
	FVector WeaponBoneLocation = TargetMesh->GetBoneLocation(FName(BoneName));
	if (SoundtoPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(TargetMesh->GetWorld(), SoundtoPlay, WeaponBoneLocation);
	}
}

FWeaponFireResult ALyraCharacter::LineTraceFire()
{
	FWeaponFireResult Result;

	TArray<AActor*> ActorsToIgnore;
	OutHit;
	FVector CameraStartLocation = ViewCamera->GetComponentLocation();
	FVector CameraEndLocation = ViewCamera->GetForwardVector() * 500000.f;
	Result.bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), CameraStartLocation, CameraEndLocation + CameraStartLocation, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 2.0f);
	if (Result.bHit)
	{
		Result.ImpactPoint = OutHit.ImpactPoint;
		Result.Normal = OutHit.Normal;
		Result.ImpactNormal = OutHit.ImpactNormal;
		Result.HitActor = OutHit.GetActor();
		Result.PhysicMaterial = OutHit.PhysMaterial.Get();
		Result.BoneNameHit = OutHit.BoneName;
	}
	return Result;
}

void ALyraCharacter::IncreaseHealth(float Amount)
{
	Health += Amount;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
	else
	{
		Health = Health;
	}
	HealthBar->SetScalarParameterValueOnMaterials(FName("HealthPercent"), FMath::Clamp(Health, 0.0f, MaxHealth));
}

void ALyraCharacter::DecreaseHealth(float Amount)
{
	float TempAmount;
	if (Shield > Amount)
	{
		Shield -= Amount;
	}
	else if (Shield == Amount)
	{
		Shield = 0.0f;
	}
	else
	{
		TempAmount = Amount -= Shield;
		Shield = 0.0f;
		Health -= TempAmount;
		if (Health < 0.0f)
		{
			Health = 0.0f;
		}
		else
		{
			Health = Health;
		}
	}
	HealthBar->SetScalarParameterValueOnMaterials(FName("HealthPercent"), FMath::Clamp(Health, 0.0f, MaxHealth));
}

void ALyraCharacter::IncreaseShield(float Amount)
{
	Shield += Amount;
	if (Shield > MaxShield)
	{
		Shield = MaxShield;
	}
	else
	{
		Shield = Shield;
	}
}



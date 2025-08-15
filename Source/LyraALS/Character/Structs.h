#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Structs.generated.h"

class UAnimSequenceBase;

//struct para el cambio de velocidade en el character movement cuando se pasa de Jog a Walk o al contrario
USTRUCT(BlueprintType)
struct FCharacterVelocities : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMovement MovementState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGuns GunType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxWalkSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxAcceleration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BreakingDeceleration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BreakingFrictionFactor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BreakingFriction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUseSeparateBreakingFriction;
};

// Struct para encender o desabilitar los debugs que se quieren mostrar
USTRUCT(BlueprintType)
struct FDebugOptionsLyra
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bShowLocomotionData = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bShowGateData = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bShowDistanceMatching = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bShowRootYawOffset = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bShowJumpData = true;
};


//struct para que en los layers cuando se hace un select de animaciones seleccionar la animacion para cada direccion (secuences)
USTRUCT(BlueprintType)
struct FDirectionalAnims
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimSequenceBase> Forward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimSequenceBase> Backward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimSequenceBase> Right;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimSequenceBase> Left;

};

//struct para los sockets de las armas 

USTRUCT(BlueprintType)

struct FSockets
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName PistolUnEquiped = FName(TEXT("PistolUnEquipe"));
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RifleUnEquiped = FName(TEXT("RifleUnEquipe"));
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName WeaponEquiped = FName(TEXT("WeaponEquiped"));

};

USTRUCT(BlueprintType)

struct FWeaponFireResult
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FVector ImpactPoint;

	UPROPERTY(BlueprintReadWrite)
	FVector Normal;

	UPROPERTY(BlueprintReadWrite)
	FVector ImpactNormal;

	UPROPERTY(BlueprintReadWrite)
	AActor* HitActor;

	UPROPERTY(BlueprintReadWrite)
	UPhysicalMaterial* PhysicMaterial;

	UPROPERTY(BlueprintReadWrite)
	bool bHit;
};
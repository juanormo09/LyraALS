// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums.h"
#include "Structs.h"
#include "LyraAnimInstance.generated.h"

class ALyraCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class LYRAALS_API ULyraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	//referencia al character para acceder a funciones que se tiene en esa clase, tambien por medio de esta se puede acceder al characterMovement
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ALyraCharacter> LyraCharacter;

public:

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion || VelocityData")
	float VelocityData;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion || VelocityData")
	FVector Velocity2D;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion || LocomotionDirections")
	FVector WorldLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion || LocomotionDirections")
	FVector LastFrameWorldLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion || LocomotionDirections")
	float DeltaLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion || LocomotionDirections")
	FVector VelocidadSinNormalizar;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion || LocomotionDirections")
	FVector AceleracionSinNormalizar;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion || LocomotionDirections")
	float VelocityLocomotionAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion || LocomotionDirections")
	float VelocityLocomotionAngleOffset;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion || LocomotionDirections")
	FRotator WorldRotation;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || LocomotionDirections")
	ELocomotionDirection LastFrameLocomotionDirectionLyra;

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	FDebugOptionsLyra DebugOptions;

	UPROPERTY(BlueprintReadWrite, Category = "Enums")
	EGuns SelectedGun;
	
	UPROPERTY(BlueprintReadWrite, Category = "Enums")
	EMovement Gates;

	UPROPERTY(BlueprintReadWrite, Category = "Enums")
	EMovement lastFrameCurrentGate;
	
	UPROPERTY(BlueprintReadWrite, Category = "Enums || VelocityLocomotionDirection")
	ELocomotionDirection LocomotionDirectionLyra;

	UPROPERTY(BlueprintReadWrite, Category = "Enums || AccelerationDirection")
	ELocomotionDirection AccelerationLocomotionDirection;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || LocomotionDirections")
	float AccelerationLocomotionAngle;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Lean")
	float ActorYaw;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Lean")
	float LastFrameActorYaw;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Lean")
	float DeltaActorYaw;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Lean")
	float LeanAngles;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || AccelerationData")
	float Acceleration;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || AccelerationData")
	FVector Acceleration2D;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || AccelerationData")
	float LastFrameAcceleration;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || AccelerationData")
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || StartData")
	bool bIsGateChanged = false;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || StartData")
	EMovement CurrentGate;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Pivot")
	float DotProductPivot;

	//variable con el plugin beta para saber en donde va a parar el personaje 
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion || StopAnimations")
	FVector GroundStop;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion || StopAnimations")
	float StopDistanceLyra;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || TurnInPlaceData")
	float RootYawOffset;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || TurnInPlaceData")
	ERootYawOffsetMode RootYawOffsetMode;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || TurnInPlaceData")
	float TurnYawCurveValue;
	
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || TurnInPlaceData")
	float LastFrameTurnYawCurveValue;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Crouch")
	bool bLyraIsCrouching;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Crouch")
	bool bIsLastFrameCrouchingLyra;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Crouch")
	bool bIsLyraCrouchStateChanged;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Jump")
	bool bIsJumping;
	
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Jump")
	bool bIsFalling;
	
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Jump")
	bool bIsOnAir;
	
	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Jump")
	FVector VelocityLyra;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Jump")
	float TimeToJumpApex;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Jump")
	float TimeFalling;

	UPROPERTY(BlueprintReadWrite, Category = "Locomotion || Aim")
	float AimPitch;

	//funcion para obtener la arma que se tiene equipada 
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	EGuns GetCurrentGun();

	//funcion para obtener el tipo de movimiento si jog o walk 
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	EMovement GetCurrentGate();

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	float GetGroundDistance();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void CharacterState(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void Debugs();

	//funcion para saber en que direccion va el personaje
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	ELocomotionDirection CalculateLocomotionDirection(float CurrentLocomotionAngle, float backwardMin, float backwardMax, float ForwardMin, float ForwardMax, ELocomotionDirection CurrentDirection, float DeadZone);

	//estudiar mas sobre los lean angles y blend poses aunque esta funcion es para ello
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void LeanAngle(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateLocation();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateOrientation();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateAcceleration();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateRootYawOffset(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void ProccessTurnYawCurve();
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LyraAnimInstance.h"
#include "Character/LyraCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "AnimCharacterMovementLibrary.h"

void ULyraAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* OwnerPawn = TryGetPawnOwner();
	if (OwnerPawn)
	{
		LyraCharacter = Cast<ALyraCharacter>(OwnerPawn);
	}
}

void ULyraAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	Debugs();
}


//funcion que se actualiza 60 frame por segundo las funciones que se llamen aqui deben ser de tipo threadsafe
void ULyraAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (!LyraCharacter)
	{
		LyraCharacter = Cast<ALyraCharacter>(TryGetPawnOwner());
		if (!LyraCharacter) return;
	}
	if (LyraCharacter && LyraCharacter->GetCharacterMovement())
	{
		//Velocity data 
		VelocityLyra = LyraCharacter->GetVelocity();
		VelocityData = UKismetMathLibrary::VSizeXY(LyraCharacter->GetVelocity());
		//Aceleration data 
		VelocidadSinNormalizar = LyraCharacter->GetVelocity() * FVector(1.0f, 1.0f, 0.0f);
		AceleracionSinNormalizar = LyraCharacter->GetCharacterMovement()->GetCurrentAcceleration() * FVector(1.0f, 1.0f, 0.0f);
		//
		UpdateAcceleration();
		UpdateLocation();
		//prediccion de donde va a para el movimiento del personaje
		FVector Velocity = LyraCharacter->GetVelocity() * FVector(1.0f, 1.0f, 0.0f);
		GroundStop = UAnimCharacterMovementLibrary::PredictGroundMovementStopLocation(
			Velocity,
			LyraCharacter->GetCharacterMovement()->bUseSeparateBrakingFriction,
			LyraCharacter->GetCharacterMovement()->BrakingFriction,
			LyraCharacter->GetCharacterMovement()->GroundFriction,
			LyraCharacter->GetCharacterMovement()->BrakingFrictionFactor,
			LyraCharacter->GetCharacterMovement()->BrakingDecelerationWalking
		);
		StopDistanceLyra = UKismetMathLibrary::VSizeXY(GroundStop);
		//
		UpdateOrientation();
		LeanAngle(DeltaSeconds);
		CharacterState(DeltaSeconds);
		UpdateRootYawOffset(DeltaSeconds);
	}
}

EGuns ULyraAnimInstance::GetCurrentGun()
{
	if (LyraCharacter)
	{
		return LyraCharacter->GunSelected;
	}
	return EGuns::EGS_Unarmed;
}

EMovement ULyraAnimInstance::GetCurrentGate()
{
	if (LyraCharacter)
	{
		return LyraCharacter->MovementState;
	}
	return EMovement::EM_Jogging;
}

float ULyraAnimInstance::GetGroundDistance()
{
	if (LyraCharacter)
	{
		return LyraCharacter->GroundDistance;
	}
	return 0.0f;
}

void ULyraAnimInstance::CharacterState(float DeltaTime)
{
	lastFrameCurrentGate = CurrentGate;
	CurrentGate = GetCurrentGate();
	bIsGateChanged = (CurrentGate != lastFrameCurrentGate);

	bIsLastFrameCrouchingLyra = bLyraIsCrouching;
	bLyraIsCrouching = (CurrentGate == EMovement::EM_Crouch);
	bIsLyraCrouchStateChanged = (bLyraIsCrouching != bIsLastFrameCrouchingLyra);

	bIsOnAir = LyraCharacter->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling;
	bIsJumping = (VelocityLyra.Z > 0.f && bIsOnAir);
	bIsFalling = (VelocityLyra.Z < 0.f && bIsOnAir);

	if (bIsJumping)
	{
		TimeToJumpApex = (VelocityLyra.Z - 0.0f) / (LyraCharacter->GetCharacterMovement()->GetGravityZ() * LyraCharacter->GetCharacterMovement()->GravityScale);
	}
	else
	{
		TimeToJumpApex = 0.0f;
	}

	if (bIsFalling)
	{
		TimeFalling = TimeFalling + DeltaTime;
	}
	else
	{
		if (bIsJumping)
		{
			TimeFalling = 0.0f;
		}
	}
}

void ULyraAnimInstance::Debugs()
{
	if (LyraCharacter)
	{
		if (DebugOptions.bShowLocomotionData)
		{
			if (GEngine)
			{
				FVector Start = WorldLocation * FVector(1.0f, 1.0f, 0.0f);
				FVector End = (UKismetMathLibrary::ClampVectorSize(LyraCharacter->GetVelocity(), 0.0f, 100.f) + WorldLocation) * FVector(1.0f, 1.0f, 0.0f);
				FVector End2 = (UKismetMathLibrary::ClampVectorSize(LyraCharacter->GetCharacterMovement()->GetCurrentAcceleration(), 0.0f, 100.f) + WorldLocation) * FVector(1.0f, 1.0f, 0.0f);
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::Printf(TEXT("VelocityData: %.2f"), VelocityData));
				GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Cyan, FString::Printf(TEXT("VelocityLocomotionData: %.2f"), VelocityLocomotionAngle));
				GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Cyan, FString::Printf(TEXT("StopDistance: %.2f"), StopDistanceLyra));
				GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Black, FString::Printf(TEXT("StopDistance: %.2f"), GetGroundDistance()));
				DrawDebugDirectionalArrow(GetWorld(), Start, End, 5.0f, FColor::Blue);
				DrawDebugDirectionalArrow(GetWorld(), Start, End2, 5.0f, FColor::Yellow);
				GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Cyan, FString::Printf(TEXT("DeltaLocation: %.2f"), DeltaLocation));

			}
		}
		if (DebugOptions.bShowDistanceMatching)
		{
			//Obteniendp velocidad en 2D se necesita para poder saber la direccion en la que va a parar el personaje
			//esta parte tambien la convertire en una funcion, se hace esto para obtener en donde se va a para el personaje pero es con ayuda de un plugin en beta
			Velocity2D = LyraCharacter->GetVelocity() * FVector(1.0f, 1.0f, 0.0f);
			FVector CenterLocation = WorldLocation + GroundStop;
			//hasta aqui la funcion
			DrawDebugSphere(GetWorld(), CenterLocation, 20.f, 20.f, FColor::Green);
		}
		if (DebugOptions.bShowRootYawOffset)
		{
			GEngine->AddOnScreenDebugMessage(5, 2.0f, FColor::Green, FString::Printf(TEXT("Delta Yaw Offset: %.2f"), RootYawOffset));
		}
		if (DebugOptions.bShowJumpData)
		{
			GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Green, FString::Printf(TEXT("Time Falling: %.2f"), TimeFalling));
		}
	}
}

ELocomotionDirection ULyraAnimInstance::CalculateLocomotionDirection(float CurrentLocomotionAngle, float backwardMin, float backwardMax, float ForwardMin, float ForwardMax, ELocomotionDirection CurrentDirection, float DeadZone)
{
	switch (CurrentDirection)
	{
		case ELocomotionDirection::ELD_Forward:
			if (CurrentLocomotionAngle > ForwardMin - DeadZone && CurrentLocomotionAngle < ForwardMax + DeadZone)
			{
				return ELocomotionDirection::ELD_Forward;
			}
			break;
		case ELocomotionDirection::ELD_Backward:
			if (CurrentLocomotionAngle > backwardMax - DeadZone || CurrentLocomotionAngle < backwardMin + DeadZone)
			{
				return ELocomotionDirection::ELD_Backward;
			}
			break;
		case ELocomotionDirection::ELD_Right:
			if (CurrentLocomotionAngle > ForwardMax - DeadZone && CurrentLocomotionAngle < backwardMax + DeadZone)
			{
				return ELocomotionDirection::ELD_Right;
			}
			break;
		case ELocomotionDirection::ELD_Left:
			if (CurrentLocomotionAngle > backwardMin - DeadZone && CurrentLocomotionAngle < ForwardMin + DeadZone)
			{
				return ELocomotionDirection::ELD_Left;
			}
			break;
		default:
			break;
	}
	if (CurrentLocomotionAngle < backwardMin || CurrentLocomotionAngle > backwardMax)
	{
		return ELocomotionDirection::ELD_Backward;
	}
	else if (CurrentLocomotionAngle > ForwardMin && CurrentLocomotionAngle < ForwardMax)
	{
		return ELocomotionDirection::ELD_Forward;
	}
	else if (CurrentLocomotionAngle > 0.0f)
	{
		return ELocomotionDirection::ELD_Right;
	}
	return ELocomotionDirection::ELD_Left;
}

void ULyraAnimInstance::LeanAngle(float DeltaTime)
{
	LastFrameActorYaw = ActorYaw;
	//obtain rotation yaw only
	ActorYaw = WorldRotation.Yaw;
	
	DeltaActorYaw = ActorYaw - LastFrameActorYaw;
	float AngularYawVelocity = (DeltaActorYaw / DeltaTime) / 4.f;
	LeanAngles = FMath::ClampAngle(AngularYawVelocity, -90.f, 90.f);
	if (LocomotionDirectionLyra == ELocomotionDirection::ELD_Backward)
	{
		LeanAngles *= -1.f;
	}
	if (LyraCharacter)
	{
		AimPitch = UKismetMathLibrary::NormalizeAxis(LyraCharacter->GetBaseAimRotation().Pitch);
	}
}

void ULyraAnimInstance::UpdateLocation()
{
	if (LyraCharacter)
	{
		LastFrameWorldLocation = WorldLocation;
		WorldLocation = LyraCharacter->GetActorLocation();
		DeltaLocation = UKismetMathLibrary::VSize(WorldLocation - LastFrameWorldLocation);
	}
}

void ULyraAnimInstance::UpdateOrientation()
{
	LastFrameLocomotionDirectionLyra = LocomotionDirectionLyra;
	WorldRotation = LyraCharacter->GetActorRotation();
	VelocityLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(VelocidadSinNormalizar, WorldRotation);
	VelocityLocomotionAngleOffset = UKismetMathLibrary::NormalizeAxis(VelocityLocomotionAngle - RootYawOffset);
	AccelerationLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(AceleracionSinNormalizar, WorldRotation);
	//UE_LOG(LogTemp, Warning, TEXT("locomotionAngle: %f"), VelocityLocomotionAngle);
	LocomotionDirectionLyra = CalculateLocomotionDirection(VelocityLocomotionAngle, -130.f, 130.f, -50.f, 50.f, LocomotionDirectionLyra, 20.f);
	AccelerationLocomotionDirection = CalculateLocomotionDirection(AccelerationLocomotionAngle, -130.f, 130.f, -50.f, 50.f, AccelerationLocomotionDirection, 20.f);
}

void ULyraAnimInstance::UpdateAcceleration()
{
	if (LyraCharacter && LyraCharacter->GetCharacterMovement())
	{
		//convertir esta parte a una funcion es para obtener la aceleracion y saber si se esta acelerando o no(se utiliza para cuando el personaje entra al estado de stop
		//antes se hacia por medio de la velocidad para saber los estados a los que pasaba el personaje ) 
		LastFrameAcceleration = Acceleration;
		Acceleration2D = LyraCharacter->GetCharacterMovement()->GetCurrentAcceleration() * FVector(1.0f, 1.0f, 0.0f);
		Acceleration = UKismetMathLibrary::VSizeXY(LyraCharacter->GetCharacterMovement()->GetCurrentAcceleration());
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(Acceleration, 0.f, 0.001))
		{
			bIsAccelerating = false;
		}
		else 
		{
			bIsAccelerating = true;
		}//hasta aqui una funcion
	}
}

void ULyraAnimInstance::UpdateRootYawOffset(float DeltaTime)
{
	if (RootYawOffsetMode == ERootYawOffsetMode::ERY_Accumulate)
	{
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset + (DeltaActorYaw * -1.f));
	}
	if (RootYawOffsetMode == ERootYawOffsetMode::ERY_BlendOut)
	{
		FFloatSpringState RootYawSpringState;
		RootYawOffset = UKismetMathLibrary::FloatSpringInterp(RootYawOffset, 0.0f, RootYawSpringState, 40.0f, 1.0f, DeltaTime);
		//RootYawOffset = 0.0f;
	}
	RootYawOffsetMode = ERootYawOffsetMode::ERY_BlendOut;
}

void ULyraAnimInstance::ProccessTurnYawCurve()
{
	LastFrameTurnYawCurveValue = TurnYawCurveValue;
	float CurveValue = GetCurveValue("IsTurning");
	if (CurveValue < 1.0f)
	{
		TurnYawCurveValue = 0.0f;
		LastFrameTurnYawCurveValue = 0.0f;
	}
	else
	{
		TurnYawCurveValue = UKismetMathLibrary::SafeDivide(GetCurveValue("root_rotation_Z"), CurveValue);
		if (LastFrameTurnYawCurveValue != 0.f)
		{
			RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - (TurnYawCurveValue - LastFrameTurnYawCurveValue));
		}
	}
}

/* no modular 
void ULyraAnimInstance::CalculateLocomotionDirection()
{
	if (LyraCharacter)
	{
		if (VelocityLocomotionAngle < -130.0f || VelocityLocomotionAngle > 130.0f)
		{
			LocomotionDirection = ELocomotionDirection::ELD_Backward;
		}
		else if (VelocityLocomotionAngle > -50.0f && VelocityLocomotionAngle < 50.0f)
		{
			LocomotionDirection = ELocomotionDirection::ELD_Forward;
		}
		else if (VelocityLocomotionAngle > 0.0f)
		{
			LocomotionDirection = ELocomotionDirection::ELD_Right;
		}
		else
		{
			LocomotionDirection = ELocomotionDirection::ELD_Left;
		}
	}
}*/

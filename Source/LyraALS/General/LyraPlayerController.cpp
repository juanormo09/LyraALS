// Fill out your copyright notice in the Description page of Project Settings.


#include "General/LyraPlayerController.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EngineUtils.h"
#include "Character/LyraCharacter.h"


void ALyraPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ALyraPlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);
	Lyra = Cast<ALyraCharacter>(PawnToPossess);
}

void ALyraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(DefaultMappingContext, 0);
	}
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		//change weapon pose 
		EnhancedInputComponent->BindAction(SwitchWeapons, ETriggerEvent::Started, this, &ALyraPlayerController::SwitchWeapon);

		//Movement
		EnhancedInputComponent->BindAction(LookCharacter, ETriggerEvent::Triggered, this, &ALyraPlayerController::Look);
		EnhancedInputComponent->BindAction(MoveCharacterLyra, ETriggerEvent::Triggered, this, &ALyraPlayerController::Move);

		//Aim
		EnhancedInputComponent->BindAction(CharacterAim, ETriggerEvent::Started, this, &ALyraPlayerController::Aim);
		EnhancedInputComponent->BindAction(CharacterAim, ETriggerEvent::Completed, this, &ALyraPlayerController::Aim);

		//crouch
		EnhancedInputComponent->BindAction(Crouching, ETriggerEvent::Started, this, &ALyraPlayerController::CrouchLyra);

		//Jump
		EnhancedInputComponent->BindAction(Jumping, ETriggerEvent::Started, this, &ALyraPlayerController::JumpLyra);
		EnhancedInputComponent->BindAction(Jumping, ETriggerEvent::Completed, this, &ALyraPlayerController::StopJumpLyra);

		//Shooting
		EnhancedInputComponent->BindAction(Fire, ETriggerEvent::Triggered, this, &ALyraPlayerController::FireLyra);

		//Reload
		EnhancedInputComponent->BindAction(Reload, ETriggerEvent::Started, this, &ALyraPlayerController::ReloadLyra);

	}
}

void ALyraPlayerController::SwitchWeapon(const FInputActionValue& Value)
{
	float GunSelected = Value.Get<float>();
	if (IsValid(Lyra))
	{
		Lyra->SelectedGun(GunSelected);
	}
}

void ALyraPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D CharaceterLook = Value.Get<FVector2D>();
	if (IsValid(Lyra))
	{
		Lyra->LookCharacter(CharaceterLook);
	}
}

void ALyraPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D CharacterMove = Value.Get<FVector2D>();
	if (IsValid(Lyra))
	{
		Lyra->MovementCharacter(CharacterMove);
	}
}

void ALyraPlayerController::Aim(const FInputActionValue& Value)
{
	bool AimCharacter = Value.Get<bool>();
	if (IsValid(Lyra))
	{
		Lyra->LyraAim(AimCharacter);
	}
}

void ALyraPlayerController::CrouchLyra(const FInputActionValue& Value)
{
	if (IsValid(Lyra))
	{
		if (Lyra->bIsCrouched)
		{
			Lyra->UnCrouch();
			Lyra->MovementState = EMovement::EM_Jogging;

		}
		else
		{
			Lyra->Crouch();
			Lyra->MovementState = EMovement::EM_Crouch;
		}
		Lyra->UpdateGate(Lyra->MovementState);
	}
}

void ALyraPlayerController::JumpLyra(const FInputActionValue& Value)
{
	if (IsValid(Lyra))
	{
		Lyra->Jump();
	}
}

void ALyraPlayerController::StopJumpLyra(const FInputActionValue& Value)
{
	if (IsValid(Lyra))
	{
		Lyra->StopJumping();
	}
}

void ALyraPlayerController::FireLyra(const FInputActionValue& Value)
{
	bool FireCharacter = Value.Get<bool>();
	if (IsValid(Lyra))
	{
		Lyra->FireWeapon(FireCharacter);
	}
}

void ALyraPlayerController::ReloadLyra(const FInputActionValue& Value)
{
	bool ReloadCharacter = Value.Get<bool>();
	if (IsValid(Lyra))
	{
		Lyra->ReloadWeapon(ReloadCharacter);
	}
}


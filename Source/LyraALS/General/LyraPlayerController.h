// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LyraPlayerController.generated.h"


class UInputAction;
class UInputMappingContext;
class UInputComponent;
class ALyraCharacter;

struct FInputActionValue;
/**
 * 
 */
UCLASS()
class LYRAALS_API ALyraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<ALyraCharacter> Lyra;

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* PawnToPossess) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SwitchWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveCharacterLyra;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CharacterAim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Crouching;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Jumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Fire;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Reload;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	void SwitchWeapon(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Aim(const FInputActionValue& Value);
	void CrouchLyra(const FInputActionValue& Value);
	void JumpLyra(const FInputActionValue& Value);
	void StopJumpLyra(const FInputActionValue& Value);
	void FireLyra(const FInputActionValue& Value);
	void ReloadLyra(const FInputActionValue& Value);
};

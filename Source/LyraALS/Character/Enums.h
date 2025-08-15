#pragma once

//enum para las armas 
UENUM(BlueprintType)
enum class EGuns : uint8
{
	EGS_Unarmed UMETA(DisplayName = "UnArmed"),
	EGS_Pistol UMETA(DisplayName = "Pistol"),
	EGS_Rifle UMETA(DisplayName = "Rifle")
};

//enum para el tipo de caminado (sin apuntar(Jog) y apuntando(walk))
UENUM(BlueprintType)
enum class EMovement : uint8
{
	EM_Walking UMETA(DisplayName = "Walking"),
	EM_Jogging UMETA(DisplayName = "Jogging"),
	EM_Crouch UMETA(DisplayName = "Crouching")
};

//enum para saber la direccion del personaje 
UENUM(BlueprintType)
enum class ELocomotionDirection : uint8
{
	ELD_Forward UMETA(DisplayName = "Forward"),
	ELD_Backward UMETA(DisplayName = "Backward"),
	ELD_Right UMETA(DisplayName = "Right"),
	ELD_Left UMETA(DisplayName = "Left")
};


//enum para el tutn inplace accumelate es quieto bendout es cuando comienza a moverse 
UENUM(BlueprintType)
enum class ERootYawOffsetMode : uint8
{
	ERY_Accumulate UMETA(DisplayName = "Accumulate"),
	ERY_BlendOut UMETA(DisplayName = "BlendOut"),
	ERY_Hold UMETA(DisplayName = "Hold")
};
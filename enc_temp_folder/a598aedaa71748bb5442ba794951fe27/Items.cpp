// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Items.h"
#include "Engine/DataTable.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Character/Enums.h"
#include "Character/Structs.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

// Sets default values
AItems::AItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Pad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pad"));
	Pad->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItems::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.
#include "MovingSawblade.h"
#include "VectorTypes.h"

// Sets default values
AMovingSawblade::AMovingSawblade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMovingSawblade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingSawblade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

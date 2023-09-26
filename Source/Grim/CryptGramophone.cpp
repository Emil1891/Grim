// Fill out your copyright notice in the Description page of Project Settings.


#include "CryptGramophone.h"

// Sets default values
ACryptGramophone::ACryptGramophone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACryptGramophone::BeginPlay()
{
	Super::BeginPlay();

	if(!Positions.IsEmpty())
	{
		SetActorLocation(Positions[CurrentPosition]);
	}
	
}

// Called every frame
void ACryptGramophone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACryptGramophone::MoveCurrentPosition()
{
	CurrentPosition++;
	SetActorLocation(Positions[CurrentPosition]);
}




// Fill out your copyright notice in the Description page of Project Settings.

#include "FearPoint.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AFearPoint::AFearPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AFearPoint::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerPawn(this, 0); 
}

float AFearPoint::GetFearLevel() const
{
	const float DistToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());

	// Lower value if the point is behind the player 
	if(Player->GetActorForwardVector().Dot(GetActorLocation() - Player->GetActorLocation()) < 0)
		return  DistToPlayer > MaxRange ? 0 : (1 - (DistToPlayer / MaxRange)) * PointBehindPlayerMultiplier;

	return  DistToPlayer > MaxRange ? 0 : 1 - (DistToPlayer / MaxRange); 
}

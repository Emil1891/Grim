// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingSawblade.h"

#include "VectorTypes.h"
#include "Kismet/KismetMathLibrary.h"

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
	
	SetActorLocation(Positions[0]);
}

// Called every frame
void AMovingSawblade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Movement = FMath::Lerp(GetActorLocation(), Positions[TargetPosition],  LerpSpeed);
	
	SetActorLocation(Movement);
	//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Cyan, TEXT("Swapping target pos."));
	if( UE::Geometry::Distance(GetActorLocation(), Positions[TargetPosition]) < 5.f )
	{
		//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Cyan, TEXT("Swapping target pos."));
		if( TargetPosition == Positions.Num() - 1 )
		{
			TargetPosition = 0;
		}
		else
		{
			TargetPosition++;
		}
	}
}

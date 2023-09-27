// Fill out your copyright notice in the Description page of Project Settings.
#include "ChasingEntity.h"

#include "GrimCharacter.h"
#include "VectorTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AChasingEntity::AChasingEntity()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DeathCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathCollisionBox"));
	DeathCollisionBox->SetupAttachment(RootComponent);
	FScriptDelegate DeathDelegate;
	DeathDelegate.BindUFunction(
		this,
		"&AChasingEntity::OnOverlapDeathBox"
		);
	DeathCollisionBox->OnComponentBeginOverlap.Add(DeathDelegate);
	
	TriggerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerCollisionBox"));
	FScriptDelegate TriggerDelegate;
	TriggerDelegate.BindUFunction(
		this,
		"&AChasingEntity::OnOverlapTriggerBox"
		);
	TriggerCollisionBox->OnComponentBeginOverlap.Add(TriggerDelegate);
}

void AChasingEntity::BeginPlay()
{
	Super::BeginPlay();
}

void AChasingEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if( bShouldMove ) {
		if( Positions.Num() > 0 )
		{
			FVector Movement = FMath::VInterpConstantTo(GetActorLocation(), Positions[TargetPosition], DeltaTime, LerpSpeed);
		
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
				SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Positions[TargetPosition]));
			}
		}
	}
}
	

void AChasingEntity::StartMoving()
{
	bShouldMove = true;
	UGameplayStatics::SpawnSoundAttached(LoopingSound1, DeathCollisionBox);
	UGameplayStatics::SpawnSoundAttached(LoopingSound2, DeathCollisionBox);
}

void AChasingEntity::OnOverlapDeathBox(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	if( const auto Player = Cast<AGrimCharacter>(OtherActor) )
	{
		if( OverlappedComp == DeathCollisionBox )
		{
			GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Cyan, TEXT("Death Collision."));
			UGameplayStatics::SpawnSound2D(this, CombustionSound);
			Player->Respawn();
		}
		else if( OverlappedComp == TriggerCollisionBox )
		{
			GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Cyan, TEXT("Starting move."));
			//UGameplayStatics::SpawnSoundAttached(StartSound, DeathCollisionBox);
			StartMoving();
			TriggerCollisionBox->SetGenerateOverlapEvents(false);
		}
	}
}

void AChasingEntity::OnOverlapTriggerBox(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	if( Cast<AGrimCharacter>(OtherActor) )
	{
		GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Cyan, TEXT("Starting move."));
		StartMoving();
		TriggerCollisionBox->SetGenerateOverlapEvents(false);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.
#include "ChasingEntity.h"

#include "GrimCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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
	TargetPosition = UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation();
}

void AChasingEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if( bShouldMove ) {
		const FVector Movement = FMath::Lerp(GetActorLocation(), TargetPosition,  LerpSpeed);
		SetActorLocation(Movement);
	}
}

void AChasingEntity::StartMoving()
{
	bShouldMove = true;
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
			Player->Respawn();
		} else if( OverlappedComp == TriggerCollisionBox )
		{
			GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Cyan, TEXT("Starting move."));
			UGameplayStatics::SpawnSoundAttached(Sound, DeathCollisionBox);
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
		UGameplayStatics::SpawnSoundAttached(Sound, DeathCollisionBox);
		StartMoving();
		TriggerCollisionBox->SetGenerateOverlapEvents(false);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.
#include "ChasingEntity.h"

#include "FearPoint.h"
#include "GrimCharacter.h"
#include "ParameterSettings.h"
#include "VectorTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AChasingEntity::AChasingEntity()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DeathCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathCollisionBox"));
	
	DeathCollisionBox->SetupAttachment(RootComponent);
	DeathCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AChasingEntity::OnOverlapDeathBox);
	
	TriggerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerCollisionBox"));
	TriggerCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AChasingEntity::OnOverlapTriggerBox);
}

void AChasingEntity::BeginPlay()
{
	Super::BeginPlay();

	DefaultLerpSpeed = UParameterSettings::GetParamSettings()->ChasingEntitySpeed;
	//DefaultLerpSpeed = 200;
}

void AChasingEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(UE::Geometry::Distance(GetActorLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetActorLocation()) >= MaxAllowedDistance)
	{
		ActiveLerpSpeed = DefaultLerpSpeed*2;
	}
	else if(UE::Geometry::Distance(GetActorLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetActorLocation()) < MaxAllowedDistance)
	{
		ActiveLerpSpeed = DefaultLerpSpeed;
	}
	
	if( bShouldMove ) {
		if( Positions.Num() > 0 )
		{
			FVector Movement = FMath::VInterpConstantTo(GetActorLocation(), Positions[TargetPosition], DeltaTime, ActiveLerpSpeed);
			SetActorLocation(Movement);
			//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Cyan, FString::SanitizeFloat(ActiveLerpSpeed));
			if( UE::Geometry::Distance(GetActorLocation(), Positions[TargetPosition]) < 5.f )
			{
				if( TargetPosition == Positions.Num() - 1 )
				{
					bShouldMove = false;
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
	ActiveLerpSpeed = DefaultLerpSpeed;
	StartMovingEvent();
}

void AChasingEntity::OnOverlapDeathBox(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Cyan, TEXT("Start overlap."));
	if( const auto Player = Cast<AGrimCharacter>(OtherActor) )
	{
		if( OverlappedComp == DeathCollisionBox )
		{
			GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Cyan, TEXT("Death Collision."));
			UGameplayStatics::SpawnSound2D(this, CombustionSound);
			Player->Respawn();
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
	//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Cyan, TEXT("Start overlap."));
	if( Cast<AGrimCharacter>(OtherActor) )
	{
		//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Cyan, TEXT("Starting move."));
		StartMoving();
		TriggerCollisionBox->SetGenerateOverlapEvents(false);
		UGameplayStatics::SpawnSound2D(TriggerCollisionBox, PressurePlateSound);
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([&]{
			UGameplayStatics::SpawnSoundAtLocation(this, ExplosionSound, DeathCollisionBox->GetComponentLocation());
		});
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ChasingEntity.generated.h"

UCLASS()
class GRIM_API AChasingEntity : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChasingEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartMoving();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartMovingEvent();
	
private:
	UFUNCTION()
	void OnOverlapTriggerBox(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

	UFUNCTION()
	void OnOverlapDeathBox(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

	UFUNCTION()
	void Reset();
	
	UPROPERTY()
	FTransform SpawnTransform;
	
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* DeathCollisionBox;
	
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* TriggerCollisionBox;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TArray<FVector> Positions;
	
	UPROPERTY(EditAnywhere)
	int TargetPosition = 1;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float DefaultLerpSpeed = 1;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float ActiveLerpSpeed = 1;

	UPROPERTY(EditAnywhere)
	bool bShouldMove = false;

	UPROPERTY(EditAnywhere)
	float MaxAllowedDistance = 5000;

	UPROPERTY()
	UAudioComponent* AudioComponent;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* StartSound;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* LoopingSound1;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* LoopingSound2;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* CombustionSound;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* PressurePlateSound;
		
	UPROPERTY(EditDefaultsOnly)
	USoundBase* ExplosionSound;
};

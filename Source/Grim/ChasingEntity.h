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
	
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* DeathCollisionBox;
	
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* TriggerCollisionBox;
	
	UPROPERTY(EditAnywhere)
	FVector TargetPosition;
	
	UPROPERTY(EditAnywhere)
	float LerpSpeed = 0.005;

	UPROPERTY(EditAnywhere)
	bool bShouldMove = false;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* Sound;
};

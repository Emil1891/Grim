// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingSawblade.generated.h"

UCLASS()
class GRIM_API AMovingSawblade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingSawblade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TArray<FVector> Positions;
	
	UPROPERTY(VisibleAnywhere)
	int TargetPosition = 1;
	
	UPROPERTY(EditAnywhere)
	float LerpSpeed = 0.05;
};

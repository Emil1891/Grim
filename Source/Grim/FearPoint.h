// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FearPoint.generated.h"

UCLASS()
class GRIM_API AFearPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFearPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	float GetFearLevel() const; 

private:

	UPROPERTY()
	APawn* Player;

	UPROPERTY()
	class AGrimPlayerState* PlayerState; 

	// The max range at which the player's fear will be affected 
	UPROPERTY(EditAnywhere, meta=(Units = "cm"))
	float MaxRange = 1000.f;

	UPROPERTY(EditAnywhere, meta=(UIMin=0, UIMax=1, ClampMin=0, ClampMax=1))
	float PointBehindPlayerMultiplier = 0.5f; 
};

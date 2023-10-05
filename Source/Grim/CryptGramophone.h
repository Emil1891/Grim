// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CryptGramophone.generated.h"

UCLASS()
class GRIM_API ACryptGramophone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACryptGramophone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void MoveCurrentPosition();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<FVector> Positions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int CurrentPosition  = 0;
	
};

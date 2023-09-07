// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class GRIM_API ADoor : public AInteractableObject
{
	GENERATED_BODY()

public:
	ADoor(); 

protected:
	virtual void InteractSuccessful() override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USoundBase* DoorOpenSound = nullptr; 
	
};

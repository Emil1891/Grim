// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "InteractableAudioPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GRIM_API AInteractableAudioPlayer : public AInteractableObject
{
	GENERATED_BODY()

public:
	AInteractableAudioPlayer();

	virtual void InteractSuccessful() override;
	
private:

	// Audio component playing the sound 
	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioPlayer; 

	// mesh shown in level/colliding with player 
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh; 
};

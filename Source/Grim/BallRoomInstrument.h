// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableAudioPlayer.h"
#include "BallRoomInstrument.generated.h"

/**
 * 
 */
UCLASS()
class GRIM_API ABallRoomInstrument : public AInteractableAudioPlayer
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// Function called by PuzzleManager which resets the instrument to reset the whole puzzle 
	void ResetToIdleSound(); 

protected:
	virtual void InteractSuccessful() override;

	// called when an actor exits the trigger zone
	virtual void TriggerZoneExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComponent, int OtherBodyIndex) override;
	
private:

	UPROPERTY(EditAnywhere)
	USoundBase* IdleSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ActiveSound;

	UPROPERTY(EditAnywhere)
	class ABallRoomPuzzleManager* PuzzleManager;

	// The audio player that houses the active sound 
	UPROPERTY(EditAnywhere)
	UAudioComponent* ActiveAudioPlayer;
	
};

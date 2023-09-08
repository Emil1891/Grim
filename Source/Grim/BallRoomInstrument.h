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

	void ChangeToIdleSound(); 

protected:
	virtual void InteractSuccessful() override;
	
private:

	UPROPERTY(EditAnywhere)
	USoundBase* IdleSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ActiveSound;

	UPROPERTY(EditAnywhere)
	class ABallRoomPuzzleManager* PuzzleManager; 
	
};

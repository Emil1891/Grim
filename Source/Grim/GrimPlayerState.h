// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Components/ForceFeedbackComponent.h"
#include "Sound/SoundCue.h"
#include "GrimPlayerState.generated.h"


/**
 * 
 */
UCLASS()
class GRIM_API AGrimPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AGrimPlayerState(); 

	UFUNCTION(BlueprintPure)
	float GetFearLevel() const{ return FearLevel; }

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:

	// value between 0 and 1 which is the player's fear/danger level 
	float FearLevel = 0.f;

	UPROPERTY(EditAnywhere)
	float VibrationDuration = 1.6f;

	UPROPERTY()
	TArray<class AFearPoint*> AllFearPoints; 
	
	void SetFearLevel();

	UPROPERTY()
	UAudioComponent* FearAudioComponent;

	UPROPERTY(EditAnywhere)
	USoundBase* FearSoundCue;

	UPROPERTY(EditAnywhere)
	UForceFeedbackEffect* ForceFeedbackEffect;

	UPROPERTY(EditAnywhere)
	UForceFeedbackComponent* ForceFeedbackComponent;

	UPROPERTY()
	class AGrimCharacter* Player; 
};

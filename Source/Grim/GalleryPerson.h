// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableAudioPlayer.h"
#include "GalleryPerson.generated.h"

/**
 * The talking people in the Gallery level 
 */
UCLASS()
class GRIM_API AGalleryPerson : public AInteractableAudioPlayer
{
	GENERATED_BODY()

public:
	void SetSound(USoundBase* TalkSound) const; 

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void InteractSuccessful() override;

private:

	inline static class AGalleryQuestManager* QuestManager = nullptr;

};

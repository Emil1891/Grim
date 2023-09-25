// Fill out your copyright notice in the Description page of Project Settings.


#include "GalleryPerson.h"

#include "GalleryQuestManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void AGalleryPerson::BeginPlay()
{
	Super::BeginPlay();

	if(!QuestManager)
		QuestManager = Cast<AGalleryQuestManager>(UGameplayStatics::GetActorOfClass(this, AGalleryQuestManager::StaticClass()));
	
	const float RandomStartTime = FMath::RandRange(0.f, AudioPlayer->GetSound()->GetDuration());

	AudioPlayer->Play(RandomStartTime); 
}

void AGalleryPerson::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	QuestManager = nullptr; 
}

void AGalleryPerson::InteractSuccessful()
{
	Super::InteractSuccessful();

	QuestManager->TalkedToPerson(this); 
}

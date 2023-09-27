// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioOcclusionComponent.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAudioOcclusionComponent::UAudioOcclusionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAudioOcclusionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetAudioComponents();

	UE_LOG(LogTemp, Warning, TEXT("Found comps: %i"), AudioComponents.Num())
}

// Called every frame
void UAudioOcclusionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UAudioOcclusionComponent::SetAudioComponents()
{
	// Find all actors of set class (default all actors)
	TArray<AActor*> AllFoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, ActorClassToSearchFor, AllFoundActors);

	for(const auto Actor : AllFoundActors)
	{
		// If the actor has an audio component 
		if(auto AudioComp = Actor->FindComponentByClass<UAudioComponent>())
		{
			AudioComponents.Add(AudioComp); // Add it to the array 
		}
	}
}


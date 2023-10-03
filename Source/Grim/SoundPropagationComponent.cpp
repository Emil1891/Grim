// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundPropagationComponent.h"

#include "AudioOcclusionComponent.h"
#include "MapGrid.h"
#include "Pathfinder.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USoundPropagationComponent::USoundPropagationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void USoundPropagationComponent::BeginPlay()
{
	Super::BeginPlay();

	// I just use the audio occlusion comp so I can use the same audio comp array 
	AudioOccComp = GetOwner()->FindComponentByClass<UAudioOcclusionComponent>();
	if(!AudioOccComp) // Sound propagation should prob have its own array of audio comps so you dont have to have both 
		UE_LOG(LogTemp, Error, TEXT("No audio occlusion component added to player"))
	
	Pathfinder = new FPathfinder(Cast<AMapGrid>(UGameplayStatics::GetActorOfClass(this, AMapGrid::StaticClass())));
}

// Called every frame
void USoundPropagationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update each audio component's sound propagation 
	for(const auto& AudioComp : AudioOccComp->AudioComponents) 
		UpdateSoundPropagation(AudioComp); 
}

// TODO: TAKE LENGTH INTO ACCOUNT? IF PATH > LENGTH THEN DONT PROPAGATE SOUND?
// TODO: DONT PLAY SOUND AT ALL IF EXCEEDING AN EVEN GREATER LENGTH? 
void USoundPropagationComponent::UpdateSoundPropagation(UAudioComponent* AudioComp)
{
	const auto StartTime = FDateTime::Now().GetMillisecond(); // FOR DEBUGGING
	
	// Actors to ignore when doing line traces 
	const TArray<AActor*> ActorsToIgnore { GetOwner(), AudioComp->GetOwner() };

	// First do a line trace from the audio source to the player to see if there is direct line of sight
	// if so, then pathfinding is unnecessary because no propagation will occur 
	FHitResult HitResultToPlayer;
	DoLineTrace(HitResultToPlayer, AudioComp->GetComponentLocation(), ActorsToIgnore); 

	if(!HitResultToPlayer.bBlockingHit) // Nothing blocking the sound 
	{
		// Remove eventual propagated sound and return 
		RemovePropagatedSound(AudioComp);
		return; 
	}
	
	TArray<FGridNode*> Path; 
	if(!Pathfinder->FindPath(AudioComp->GetComponentLocation(), GetOwner()->GetActorLocation(), Path))
	{
		// No path found, remove eventual propagated sound and return 
		RemovePropagatedSound(AudioComp); 
		return; 
	}
	
	// Iterate through path and find the last node with line of sight to player, that's the location to propagate the sound to 
	for(int i = 1; i < Path.Num(); i++)
	{
		// Draw the path 
		DrawDebugSphere(GetWorld(), Path[i]->GetWorldCoordinate(), 30, 10, FColor::Red);

		FHitResult HitResult;
		DoLineTrace(HitResult, Path[i]->GetWorldCoordinate(), ActorsToIgnore); 

		// If nothing is blocking from the node to player, check next node 
		if(!HitResult.bBlockingHit)
			continue;

		// Something is blocking between this node and the player 
		
		// if we do not have a propagated sound for that audio comp in the world already 
		if(!PropagatedSounds.Contains(AudioComp))
		{
			SpawnPropagatedSound(AudioComp, Path[i - 1]->GetWorldCoordinate()); 
		} else  // If we do have a propagated sound for that location 
		{
			// Get the propagated audio component 
			const UAudioComponent* PropagatedAudioComp = PropagatedSounds[AudioComp];

			// if it's in the wrong location, set it to the node's position, otherwise it's in the correct place already 
			if(!PropagatedAudioComp->GetComponentLocation().Equals(Path[i - 1]->GetWorldCoordinate()))
				PropagatedSounds[AudioComp]->SetWorldLocation(Path[i - 1]->GetWorldCoordinate()); 
		}

		break; // Found the node with block 
	}

	// DEBUGGING 
	const auto EndTime = FDateTime::Now().GetMillisecond();
	if(EndTime - StartTime != 0)
		UE_LOG(LogTemp, Warning, TEXT("Update time: %i ms"), EndTime - StartTime)
}

bool USoundPropagationComponent::DoLineTrace(FHitResult& HitResultOut, const FVector& StartLoc, const TArray<AActor*>& ActorsToIgnore) const
{
	// Line trace from the node to player to see if there is line of sight  
	return UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartLoc,
		AudioOccComp->CameraComp->GetComponentLocation(), AudioOccComp->ObjectsToQuery, false,
		ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResultOut, true); 
}

void USoundPropagationComponent::RemovePropagatedSound(const UAudioComponent* AudioComp)
{
	// if there is propagated sound in the level 
	if(PropagatedSounds.Contains(AudioComp))
	{
		PropagatedSounds[AudioComp]->DestroyComponent(); // Destroy it 
		PropagatedSounds.Remove(AudioComp); // And remove it from the map 
	}
}

void USoundPropagationComponent::SpawnPropagatedSound(UAudioComponent* AudioComp, const FVector& SpawnLocation) 
{
	// I now duplicate the object giving it the exact same properties, that might not be what we want since we
	// dont want the muffling, different volume and so forth 
	UAudioComponent* PropagatedAudioComp = DuplicateObject<UAudioComponent>(AudioComp, AudioComp->GetOwner(), FName(FString("PropagatedSound"))); 

	AudioComp->GetOwner()->AddInstanceComponent(PropagatedAudioComp);
	PropagatedAudioComp->RegisterComponent();
	PropagatedAudioComp->SetWorldLocation(SpawnLocation);
	
	// I guess I can copy it and then only change what we want to differ? 
	// Does not seem to update, needs to be delayed 1 tick so it can register first? 
	PropagatedAudioComp->SetVolumeMultiplier(1); 
	PropagatedAudioComp->SetLowPassFilterEnabled(false);
	// PropagatedAudioComp->AttenuationSettings = PropagatedSoundAttenuation; // Do we want to change attenuation?

	// TODO: THE SPAWNED AUDIO COMP PLAYS FROM START, FIX! Not possible? Move the original source? Audio Occlusion
	// TODO: component needs the original source's location though 
	
	PropagatedSounds.Add(AudioComp, PropagatedAudioComp); 
}

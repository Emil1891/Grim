// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundPropagationComponent.h"

#include "AudioOcclusionComponent.h"
#include "MapGrid.h"
#include "Pathfinder.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USoundPropagationComponent::USoundPropagationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void USoundPropagationComponent::BeginPlay()
{
	Super::BeginPlay();

	Pathfind = new Pathfinder(Cast<AMapGrid>(UGameplayStatics::GetActorOfClass(this, AMapGrid::StaticClass()))); 
}

// Called every frame
void USoundPropagationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update each audio component's sound propagation 
	for(const auto& AudioComp : AudioOccComp->AudioComponents)
	{
		UpdateSoundPropagation(AudioComp); 
	}
}

// TODO: TAKE LENGTH INTO ACCOUNT? IF PATH > LENGTH THEN DONT PROPAGATE SOUND?
// TODO: DONT PLAY SOUND AT ALL IF EXCEEDING AN EVEN GREATER LENGTH? 
void USoundPropagationComponent::UpdateSoundPropagation(UAudioComponent* AudioComp)
{
	TArray<GridNode*> Path; 
	if(!Pathfind->FindPath(AudioComp->GetComponentLocation(), GetOwner()->GetActorLocation(), Path))
	{
		// NO PATH FOUND, REMOVE EVENTUAL PROPAGATED SOUND (RESET EVERYTHING)
		return; 
	}

	// Path found, find where audio should be coming from

	// Get the direction between first and second node in path 
	FVector DirectionToNextNode = Path[0]->GetWorldCoordinate() - Path[1]->GetWorldCoordinate();
	DirectionToNextNode.Normalize();

	// What we are after is the coord where direction changes to place the audio source at that location, default at
	// last coordinate 
	FVector CoordinateWhereDirChanges = Path[Path.Num() - 1]->GetWorldCoordinate();  

	// Iterate through path 
	for(int i = 1; i < Path.Num() - 1; i++)
	{
		// If direction to next node is not the same, then set where direction changed and break loop 
		if(!(Path[i]->GetWorldCoordinate() - Path[i + 1]->GetWorldCoordinate().GetSafeNormal()).Equals(CoordinateWhereDirChanges))
		{
			CoordinateWhereDirChanges = Path[i]->GetWorldCoordinate();
			break; 
		}
	}

	// Debug where direction change occurs 
	DrawDebugSphere(GetWorld(), CoordinateWhereDirChanges, 50, 10, FColor::Red); 
}

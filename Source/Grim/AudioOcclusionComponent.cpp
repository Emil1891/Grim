// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioOcclusionComponent.h"

#include "AITypes.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UAudioOcclusionComponent::UAudioOcclusionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame. You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}

// Called when the game starts
void UAudioOcclusionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetAudioComponents();

	CameraComp = GetOwner()->FindComponentByClass<UCameraComponent>(); 
}

// Called every frame
void UAudioOcclusionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Gets all audio components in the level, now every tick in case of spawned audio 
	//SetAudioComponents();

	// Add to timer 
	Timer += DeltaTime;

	// Update all audio components 
	for(UAudioComponent* Audio : AudioComponents)
		UpdateAudioComp(Audio, DeltaTime);

	// Check if timer exceeded delay after updating all audio comps 
	if(Timer > LowPassUpdateDelay)
		Timer = 0; 
}

void UAudioOcclusionComponent::AddAudioComponentToOcclusion(UAudioComponent* AudioComponent)
{
	// Add if it does not already exist in array 
	if(!AudioComponents.Contains(AudioComponent)) 
		AudioComponents.Add(AudioComponent); 
}

void UAudioOcclusionComponent::SetAudioComponents()
{
	AudioComponents.Empty(); 
	// Find all actors of set class (default all actors)
	TArray<AActor*> AllFoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, ActorClassToSearchFor, AllFoundActors);

	for(const auto Actor : AllFoundActors)
	{
		// TODO: ONLY FOR DEBUGGING TO REMOVE UNWANTED SOUNDS
		 //if(!Actor->GetActorNameOrLabel().Equals("TestSound"))
		 //	continue; 
		// If the actor has an audio component 
		if(auto AudioComp = Actor->FindComponentByClass<UAudioComponent>())
				AudioComponents.Add(AudioComp); // Add it to the array
	}
}

bool UAudioOcclusionComponent::DoLineTrace(TArray<FHitResult>& HitResultsOut, const FVector& StartLocation, const FVector& EndLocation) const
{
	return UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, ObjectsToQuery, false, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResultsOut, true); 
}

void UAudioOcclusionComponent::UpdateAudioComp(UAudioComponent* AudioComp, const float DeltaTime)
{
	TArray<FHitResult> HitResultsFromPlayer;
	// No blocking objects 
	if(!DoLineTrace(HitResultsFromPlayer, CameraComp->GetComponentLocation(), AudioComp->GetComponentLocation()))
	{
		// Reset values when not blocking 
		ResetAudioComponentOnNoBlock(AudioComp); 
		return;
	}

	// Used to calculate distances that rays travel within objects 
	TArray<FHitResult> HitResultsFromAudio;
	DoLineTrace(HitResultsFromAudio, AudioComp->GetComponentLocation(), CameraComp->GetComponentLocation());
	
	if(HitResultsFromAudio.Num() != HitResultsFromPlayer.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Ray trace hits not equals for player and audio!, Audio: %i - Player: %i"), HitResultsFromAudio.Num(), HitResultsFromPlayer.Num())
		return; 
	}

	// Reverse the hit results from the audio's perspective so they are in the same order as the player's for easier use 
	Algo::Reverse(HitResultsFromAudio); 

	// Adds all blocking meshes' occlusion values to determine how much the sound should be occluded 
	float TotalOccValue = 0; 
	for(int i = 0; i < HitResultsFromAudio.Num(); i++)
		TotalOccValue += GetOcclusionValue(HitResultsFromPlayer[i], HitResultsFromAudio[i]); 
	
	TotalOccValue = FMath::Clamp(TotalOccValue, 0, 1); 
	
	AudioComp->SetVolumeMultiplier(1 - TotalOccValue);

	// Update LowPass only at set interval for optimization 
	if(Timer > LowPassUpdateDelay)
		SetLowPassFilter(AudioComp, HitResultsFromPlayer, TotalOccValue);
}

float UAudioOcclusionComponent::GetOcclusionValue(const FHitResult& HitResultFromPlayer, const FHitResult& HitResultFromAudio) 
{
	const float ThicknessValue = GetThicknessValue(HitResultFromPlayer, HitResultFromAudio);

	const float MaterialValue = GetMaterialValue(HitResultFromPlayer); 

	const float OccValue = ThicknessValue * MaterialValue; 
	
	return FMath::Clamp(OccValue, 0, 1); 
}

float UAudioOcclusionComponent::GetMaterialValue(const FHitResult& HitResult)
{
	// Get all materials from hit component 
	TArray<UMaterialInterface*> Materials; 
	HitResult.GetComponent()->GetUsedMaterials(Materials);

	float MaterialValue = 1; 
	for(const auto Material : Materials)
	{
		// Get the material value if it has one 
		if(MaterialOcclusionMap.Contains(Material))
		{
			MaterialValue = MaterialOcclusionMap[Material];
			//UE_LOG(LogTemp, Warning, TEXT("Material value: %f"), MaterialValue)
		}
	}

	return MaterialValue; 
}

float UAudioOcclusionComponent::GetLowPassValueBasedOnDistanceToMesh(const FHitResult& HitResultFromPlayer) const
{
	FVector ClosestPointOnMeshToPlayer; // In world coordinates 
	HitResultFromPlayer.GetComponent()->GetClosestPointOnCollision(CameraComp->GetComponentLocation(), ClosestPointOnMeshToPlayer);

	const float DistanceFromPlayerToMeshPoint = FVector::Dist(ClosestPointOnMeshToPlayer, CameraComp->GetComponentLocation());

	const float LowPassValue = FMath::Clamp(DistanceFromPlayerToMeshPoint - DistanceToWallOffset, 0, DistanceToWallToStopAddingLowPass) / DistanceToWallToStopAddingLowPass;
	
	UE_LOG(LogTemp, Warning, TEXT("LowPassValue based on distance to wall: %f"), LowPassValue);
	
	return LowPassValue;
}

float UAudioOcclusionComponent::GetThicknessValue(const FHitResult& HitResultFromPlayer, const FHitResult& HitResultFromAudio) const
{
	// Get how far the ray traveled through the blocking mesh 
	float RayTravelDistance = FVector::Dist(HitResultFromPlayer.ImpactPoint, HitResultFromAudio.ImpactPoint); 

	// Divide by the max distance to travel before blocking all audio and clamp to get a value between 0 and 1 
	RayTravelDistance /= MaxMeshDistanceToBlockAllAudio; 
	RayTravelDistance = FMath::Clamp(RayTravelDistance, 0, 1); 

	//UE_LOG(LogTemp, Warning, TEXT("RayTravelDistance: %f"), FMath::Clamp(RayTravelDistance, 0, 1)) 

	return RayTravelDistance; 
}

void UAudioOcclusionComponent::ResetAudioComponentOnNoBlock(UAudioComponent* AudioComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("Volume: 1"))

	// Resets the audio comp values 
	if(AudioComponent->VolumeMultiplier != 1)
		AudioComponent->SetVolumeMultiplier(1);

	if(AudioComponent->bEnableLowPassFilter)
		AudioComponent->SetLowPassFilterEnabled(false);
}

void UAudioOcclusionComponent::SetLowPassFilter(UAudioComponent* AudioComp, const TArray<FHitResult>& HitResultFromPlayer, const float OcclusionValue)
{
	AudioComp->SetLowPassFilterEnabled(true);
	float Frequency = MaxLowPassFrequency * GetLowPassValueBasedOnDistanceToMesh(HitResultFromPlayer[0]);
	Frequency = FMath::Clamp(Frequency, 200, MaxLowPassFrequency);

	AudioComp->SetLowPassFilterFrequency(Frequency);
	
	UE_LOG(LogTemp, Warning, TEXT("Volume: %f Frequency: %f"), AudioComp->VolumeMultiplier, Frequency); 
}

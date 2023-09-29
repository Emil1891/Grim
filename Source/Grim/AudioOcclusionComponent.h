// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AudioOcclusionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIM_API UAudioOcclusionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAudioOcclusionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Function to manually add an audio component to also occlude, only necessary for components no present in level
	 * on begin play. Checks if it is already added to prevent duplicates */
	void AddAudioComponentToOcclusion(UAudioComponent* AudioComponent); 

private: 

#pragma region DataMembers
	
	// Array holding all audio components in the level 
	TArray<UAudioComponent*> AudioComponents;

	// The class that are checked to see if they have an audio component, default = all actors 
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorClassToSearchFor = AActor::StaticClass();
	
	// The camera's location will be the start location for all line traces as it is located in the player's "head" 
	UPROPERTY()
	class UCameraComponent* CameraComp = nullptr;
	
	// Which object types that should be considered to block audio 
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToQuery;

	float MaxMeshDistanceToBlockAllAudio = 900.f;

	float MaxWidthDistanceToBlockAllAudio = 2000.f;
	
	// When this distance away from the wall or more, low pass will be the same 
	UPROPERTY(EditAnywhere)
	float DistanceToWallToStopAddingLowPass = 700.f;

	// The maximum frequency to allow from audio sources when occluded 
	UPROPERTY(EditAnywhere)
	float MaxLowPassFrequency = 17000.f; 

	UPROPERTY(EditAnywhere)
	TMap<UMaterialInterface*, float> MaterialOcclusionMap;

	UPROPERTY(EditAnywhere)
	float DistanceToWallOffset = 60.f;

	inline static float Timer = 0;

	UPROPERTY(EditAnywhere)
	float LowPassUpdateDelay = 0.1f;

#pragma endregion

#pragma region Functions 
	
	// Called in begin play to fill the array 
	void SetAudioComponents();

	bool DoLineTrace(TArray<FHitResult>& HitResultsOut, const FVector& StartLocation, const FVector& EndLocation) const;
	
	void UpdateAudioComp(UAudioComponent* AudioComp, const float DeltaTime);

	// Gets the total occlusion value between 0 and 1 
	float GetOcclusionValue(const FHitResult& HitResultFromPlayer, const FHitResult& HitResultFromAudio);
	float GetLowPassValueBasedOnDistanceToMesh(const FHitResult& HitResultFromPlayer) const;

	// Gets a value clamped between 0 and 1 based on the thickness of the meshes between the player and the audio source
	float GetThicknessValue(const FHitResult& HitResultFromPlayer, const FHitResult& HitResultFromAudio) const;

	float GetMaterialValue(const FHitResult& HitResult); 

	void ResetAudioComponentOnNoBlock(UAudioComponent* AudioComponent);

	void SetLowPassFilter(UAudioComponent* AudioComp, const TArray<FHitResult>& HitResultFromPlayer, const float OcclusionValue);

#pragma endregion 
};

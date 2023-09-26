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

private: 

	// Array holding all audio components in the level 
	TArray<UAudioComponent*> AudioComponents;

	// Called in begin play to fill the array 
	void SetAudioComponents();
	
	// The class that are checked to see if they have an audio component, default = all actors 
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorClassToSearchFor = AActor::StaticClass();

	
};

// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "QuestInterface.h"
#include "GameFramework/Actor.h"
#include "QuestDoor.generated.h"

UCLASS()
class GRIM_API AQuestDoor : public AActor, public IQuestInterface
{
	GENERATED_BODY()
	
public:	
	AQuestDoor();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	virtual void QuestCompleted() override;
	
private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USoundBase* DoorOpenSound = nullptr;

	UPROPERTY(EditAnywhere)
	USoundBase* SignallingSound = nullptr;

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioPlayer; 
};

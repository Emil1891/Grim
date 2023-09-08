// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestDoor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AQuestDoor::AQuestDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AQuestDoor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AQuestDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuestDoor::QuestCompleted()
{
	if( DoorOpenSound )
	{
		UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
	}
	Mesh->DestroyComponent();
}

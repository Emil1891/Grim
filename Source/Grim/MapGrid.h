// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridNode.h"
//#include "Pathfinder.h"
#include "GameFramework/Actor.h"
#include "MapGrid.generated.h"

UCLASS()
class GRIM_API AMapGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGrid();

	~AMapGrid() override; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	// Returns the node that the World Location is in 
	GridNode* GetNodeFromWorldLocation(const FVector WorldLoc) const;
	
	FVector GetGridBottomLeftLocation() const { return GridBottomLeftLocation; }

	FVector GetGridSize() const { return GridSize; }

private:

#pragma region DataMembers
	
	// 1D array (will be used as if it was 3D) keeping track of all nodes,
	// Unreal does not seem to like creating 3D arrays with Array[][][]
	// https://stackoverflow.com/a/34363187 (source to convert 3D array to 1D) 
	GridNode* Nodes; 

	// Radius for each node, smaller radius means more accurate but more performance expensive 
	UPROPERTY(EditAnywhere)
	float NodeRadius = 50.f; 
	
	float NodeDiameter;  

	// The size of the grid 
	UPROPERTY(EditAnywhere) 
	FVector GridSize = FVector(100, 100, 100); 

	// Holds the array sizes 
	int GridArrayLengthX; 
	int GridArrayLengthY; 
	int GridArrayLengthZ; 

	FVector GridBottomLeftLocation; 

	// Not used as of now since that method did not seem to work 
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> UnwalkableObjects;

	// What is used instead to check if a node is walkable (actor checks overlaps)
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> OverlapCheckActorClass;

	//friend class Pathfinder; 
	//Pathfinder* Pathfind = nullptr; 

#pragma endregion 

#pragma region Functions 

	void CreateGrid();

	void AddToArray(const int IndexX, const int IndexY, const int IndexZ, const GridNode Node);

	GridNode* GetNodeFromArray(const int IndexX, const int IndexY, const int IndexZ) const;

	TArray<GridNode*> GetNeighbours(const GridNode* Node) const;

	int GetIndex(const int IndexX, const int IndexY, const int IndexZ) const;

	bool IsOutOfBounds(const int GridX, const int GridY, const int GridZ) const; 

#pragma endregion 

#pragma region Debugging

	UPROPERTY(EditAnywhere)
	bool bDrawDebugStuff = true; 
	
	void DrawDebugStuff() const;

#pragma endregion
	
};
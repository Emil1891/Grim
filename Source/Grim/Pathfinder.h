// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GRIM_API FPathfinder
{
public:
	explicit FPathfinder(class AMapGrid* Grid);

	bool FindPath(const FVector& From, const FVector& To, TArray<class FGridNode*>& Path); 
	
	~FPathfinder();

private:
	AMapGrid* Grid;

	FGridNode* OldEndNode = nullptr;

	TArray<FGridNode*> GetPath(const FGridNode* StartNode, FGridNode* EndNode);

	// Returns an approximate cost to travel between nodes (ignoring obstacles)
	int GetCostToNode(const FGridNode* From, const FGridNode* To) const;

	FGridNode* GetTargetNode(const FVector& TargetLocation) const; 

};

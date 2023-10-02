// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GRIM_API GridNode
{
public:
	GridNode(const bool bIsWalkable, const FVector WorldCoord, const int GridX, const int GridY, const int GridZ);
	GridNode() {} 

	bool IsWalkable() const { return bWalkable; }

	FVector GetWorldCoordinate() const { return WorldCoordinate; }

	int GetCost() const { return Cost; }

	void SetCost(const int NewCost) { Cost = NewCost; }

	int GridX = -1;
	int GridY = -1;
	int GridZ = -1; 
	
private:
	// Direction where the AI should walk if standing on this node 
	FVector Direction = FVector::Zero();

	// If the node is walkable 
	bool bWalkable = false;

	FVector WorldCoordinate;

	// Cost to get to this node 
	int Cost = INT_MAX;

};
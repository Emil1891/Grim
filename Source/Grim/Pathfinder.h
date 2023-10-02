// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GRIM_API Pathfinder
{
public:
	explicit Pathfinder(class AMapGrid* Grid);

	bool FindPath(const FVector& From, const FVector& To, TArray<class GridNode*>& Path); 
	
	~Pathfinder();

private:
	AMapGrid* Grid;

	GridNode* OldGridNode = nullptr;

	TArray<GridNode*> GetPath(const GridNode* StartNode, GridNode* EndNode); 

};

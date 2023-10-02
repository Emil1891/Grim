// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfinder.h"

#include "MapGrid.h"

Pathfinder::Pathfinder(AMapGrid* Grid) : Grid(Grid)
{
}

bool Pathfinder::FindPath(const FVector& From, const FVector& To, TArray<GridNode*>& Path)
{
	GridNode* StartNode = Grid->GetNodeFromWorldLocation(From); 
	GridNode* EndNode = Grid->GetNodeFromWorldLocation(To);

	// Surely heap is most effective? TArray seems to have support functions for a heap 
	TArray<GridNode*> ToBeChecked; 
	TSet<GridNode*> Visited; // Only needs to keep track of which nodes have been visited, set should be effective 

	// Target has not moved, simply return 
	if(EndNode == OldGridNode)
		return true;

	// Reset the start node 
	StartNode->GCost = 0;
	StartNode->HCost = 0;

	// Add it to be checked 
	ToBeChecked.HeapPush(StartNode);

	// While there are still nodes to check 
	while(!ToBeChecked.IsEmpty())
	{
		// Remove the node with highest priority (most promising path)
		GridNode* Current;
		ToBeChecked.HeapPop(Current);

		Visited.Add(Current);

		// If we have reached the end node, a path has been found 
		if(Current == EndNode)
		{
			// Build the path and return 
			Path = GetPath(StartNode, EndNode);
			return true; 
		}

		for(const auto Neighbour : Grid->GetNeighbours(Current))
		{
			// TODO: I ENDED HERE 
		}
	}

	return false; 
}

TArray<GridNode*> Pathfinder::GetPath(const GridNode* StartNode, GridNode* EndNode)
{
	// Construct the path by building it using the nodes' parents
	TArray<GridNode*> Path;

	// Set current to EndNode 
	GridNode* Current = EndNode;

	// While not reached start 
	while(Current != StartNode)
	{
		// Add the current node to the path and set current to its target 
		Path.Add(Current);
		Current = Current->Parent; 
	}

	// Reverse the path since we constructed it "backwards" 
	Algo::Reverse(Path);

	return Path; 
}

Pathfinder::~Pathfinder()
{
}
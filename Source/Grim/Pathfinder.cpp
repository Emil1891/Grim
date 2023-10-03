// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfinder.h"
#include "MapGrid.h"

// Tried overloading comparison operator (<) in GridNode class but did not seem to work,
// using a predicate does seem to work even though it is somewhat clunkier
namespace
{
	auto HeapPred = [](const FGridNode& Left, const FGridNode& Right)
	{
		return Left.GetFCost() < Right.GetFCost() || Left.HCost < Right.HCost;
	};
}

FPathfinder::FPathfinder(AMapGrid* Grid) : Grid(Grid)
{
	if(!Grid)
		UE_LOG(LogTemp, Error, TEXT("No map grid found in level"))
}

bool FPathfinder::FindPath(const FVector& From, const FVector& To, TArray<FGridNode*>& Path)
{
	FGridNode* StartNode = Grid->GetNodeFromWorldLocation(From); 
	FGridNode* EndNode = Grid->GetNodeFromWorldLocation(To);
	
	// Surely heap is most effective? TArray seems to have support functions for a heap 
	TArray<FGridNode*> ToBeChecked; 
	TSet<FGridNode*> Visited; // Only needs to keep track of which nodes have been visited, set should be effective 

	// Target has not moved, simply return 
	if(EndNode == OldEndNode)
		return true;
	
	// this if will be removed, bad way of forcing path draw each frame by always updating the path 
	if(!Grid->bDrawPath) 
		OldEndNode = EndNode; 

	// Reset the start node 
	StartNode->GCost = 0;
	StartNode->HCost = 0;

	// Add it to be checked 
	ToBeChecked.HeapPush(StartNode, HeapPred);

	// While there are still nodes to check 
	while(!ToBeChecked.IsEmpty())
	{
		// Remove the node with highest priority (most promising path)
		FGridNode* Current;
		ToBeChecked.HeapPop(Current, HeapPred);

		Visited.Add(Current);

		// If we have reached the end node, a path has been found 
		if(Current == EndNode)
		{
			// Build the path and return 
			Path = GetPath(StartNode, EndNode);
			return true; 
		}

		// For each neighbouring node 
		for(const auto Neighbour : Grid->GetNeighbours(Current))
		{
			// Check if it's walkable or has already been visited 
			if(!Neighbour->IsWalkable() || Visited.Contains(Neighbour))
				continue; // if so, skip it

			// otherwise, calculate the GCost to neighbour
			const int NewGCostToNeighbour = Current->GCost + GetCostToNode(Current, Neighbour); 

			// If new GCost is lower or it has not been added to be checked 
			if(NewGCostToNeighbour < Neighbour->GCost || !ToBeChecked.Contains(Neighbour))
			{
				// Update its G- and HCost (and thus FCost as well)
				Neighbour->GCost = NewGCostToNeighbour;
				Neighbour->HCost = GetCostToNode(Neighbour, EndNode);

				// Set its parent to current to keep track of where we came from (shortest path to the node)
				Neighbour->Parent = Current;

				// Add neighbour to be checked if not already present 
				if(!ToBeChecked.Contains(Neighbour))
					ToBeChecked.HeapPush(Neighbour, HeapPred); 
			}
		}
	}

	// No path found, clear path and return 
	Path.Empty(); 
	return false; 
}

TArray<FGridNode*> FPathfinder::GetPath(const FGridNode* StartNode, FGridNode* EndNode)
{
	// Construct the path by building it using the nodes' parents
	TArray<FGridNode*> Path;

	// Set current to EndNode 
	FGridNode* Current = EndNode;

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

int FPathfinder::GetCostToNode(const FGridNode* From, const FGridNode* To) const
{
	// Source: https://www.math.usm.edu/lambers/mat169/fall09/lecture17.pdf (page 3)
	// Returns the distance between nodes, ignoring obstacles 
	const float Distance = FMath::Sqrt(static_cast<float>(FMath::Square(To->GridX - From->GridX)) +
		FMath::Square(To->GridY - From->GridY) + FMath::Square(To->GridZ - From->GridZ)); 

	// Note: I don't think is is entirely accurate since the path can only move between nodes and not diagonally across
	// multiple nodes (hard to describe) which is why A* algos usually use their own solution with 10, 14 etc. costs
	// i dont think its a problem for us though since we do not need such an accurate path but might be more effective?
	return FMath::RoundToInt(Distance); 
}

FPathfinder::~FPathfinder()
{
}

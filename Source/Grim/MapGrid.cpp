// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGrid.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMapGrid::AMapGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 
	
}

AMapGrid::~AMapGrid()
{
	// does unreal handle this with its garbage collection? 
	delete [] Nodes;
	//delete Pathfind; 
}

// Called when the game starts or when spawned
void AMapGrid::BeginPlay()
{
	Super::BeginPlay();

	NodeDiameter = NodeRadius * 2;
	
	CreateGrid();
	
	if(bDrawDebugStuff) 
		DrawDebugStuff();

	//Pathfind = new Pathfinder(UGameplayStatics::GetPlayerPawn(this, 0), this);
}

void AMapGrid::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TODO: REMOVE THIS, ONLY USED FOR MEASURING PERFORMANCE (not the function call) 
	// FDateTime StartTime = FDateTime::UtcNow();
	//Pathfind->UpdateNodeDirections();
	// const float TimeElapsedInMs = (FDateTime::UtcNow() - StartTime).GetTotalMilliseconds();
	// UE_LOG(LogTemp, Display, TEXT("%f ms each tick to update map"), TimeElapsedInMs)
	
	if(!bDrawDebugStuff)
		return;
	
	// DEBUGGING BELOW SHOWING THE NODE THAT THE PLAYER IS CURRENTLY IN
	const auto PlayerNode = GetNodeFromWorldLocation(UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation()); 
	DrawDebugSphere(GetWorld(), PlayerNode->GetWorldCoordinate(), NodeRadius, 10, FColor::Cyan);

	// THEN DISPLAYING ITS NEIGHBOURS 
	for(auto N : GetNeighbours(PlayerNode))
		if(N->IsWalkable()) // if it's walkable 
			DrawDebugSphere(GetWorld(), N->GetWorldCoordinate(), NodeRadius, 10, FColor::Purple);
}

void AMapGrid::CreateGrid()
{
	GridArrayLengthX = FMath::RoundToInt(GridSize.X / NodeDiameter); 
	GridArrayLengthY = FMath::RoundToInt(GridSize.Y / NodeDiameter); 
	GridArrayLengthZ = FMath::RoundToInt(GridSize.Z / NodeDiameter); 

	Nodes = new GridNode[GridArrayLengthX * GridArrayLengthY * GridArrayLengthZ]; 

	// The grid's pivot is in the center, need its position as if pivot was in the bottom left corner 
	FVector GridBottomLeft = GetActorLocation();
	GridBottomLeft.X -= GridSize.X / 2;
	GridBottomLeft.Y -= GridSize.Y / 2;
	//GridBottomLeft.Z -= GridSize.Z / 2; // Is Z already correct? 

	GridBottomLeftLocation = GridBottomLeft; 

	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OverlappingActors;

	AActor* OverlapActor = GetWorld()->SpawnActor<AActor>(OverlapCheckActorClass, GetActorLocation(),
	                                                      FRotator::ZeroRotator); 
	
	for(int x = 0; x < GridArrayLengthX; x++)
	{
		for(int y = 0; y < GridArrayLengthY; y++)
		{
			for(int z = 0; z < GridArrayLengthZ; z++)
			{
				FVector NodePos = GridBottomLeft;
				NodePos.X += x * NodeDiameter + NodeRadius; 
				NodePos.Y += y * NodeDiameter + NodeRadius;
				NodePos.Z += z * NodeDiameter + NodeRadius; // Pos now in node center 

				// The lines below would be better but they dont seem to work? Should try again when a level is in place
				// because they seem to detect some (handplaced?) objects? 
				// const bool bAreaWalkable = UKismetSystemLibrary::SphereOverlapActors(this, NodePos,
				// 	NodeRadius, UnwalkableObjects, UClass::StaticClass(), ActorsToIgnore,OverlappingActors);

				// AddToArray(x, y, GridNode(bAreaWalkable, NodePos));
			
				// This is currently a work around to above but it works fine as of now, but adjusting node radius
				// requires adjusting the Overlapping actor's mesh's size manually. solution above would not require it
				OverlapActor->SetActorLocation(NodePos);
				OverlapActor->GetOverlappingActors(OverlappingActors);

				AddToArray(x, y, z, GridNode(OverlappingActors.IsEmpty(), NodePos, x, y, z));
			}
		}
	}
	OverlapActor->Destroy(); 
}

int AMapGrid::GetIndex (const int IndexX, const int IndexY, const int IndexZ) const
{
	return IndexX * GridArrayLengthY * GridArrayLengthZ + IndexZ * GridArrayLengthY + IndexY; // Source: https://stackoverflow.com/a/34363187 (reworked)
}

void AMapGrid::AddToArray(const int IndexX, const int IndexY, const int IndexZ, const GridNode Node)
{
	Nodes[GetIndex(IndexX, IndexY, IndexZ)] = Node;
}

GridNode* AMapGrid::GetNodeFromArray(const int IndexX, const int IndexY, const int IndexZ) const
{
	return &Nodes[GetIndex(IndexX, IndexY, IndexZ)]; 
}

GridNode* AMapGrid::GetNodeFromWorldLocation(const FVector WorldLoc) const
{
	// Get coordinates relative to the grid's bottom left corner 
	//const float GridRelativeX = WorldLoc.X - GridBottomLeftLocation.X;  
	//const float GridRelativeY = WorldLoc.Y - GridBottomLeftLocation.Y;
	//const float GridRelativeZ = WorldLoc.Z - GridBottomLeftLocation.Z;

	const FVector GridRelative = WorldLoc - GridBottomLeftLocation; 

	// checks how many nodes "fit" in the relative position for array indexes 
	const int x = FMath::Clamp(FMath::RoundToInt((GridRelative.X - NodeRadius) / NodeDiameter), 0, GridArrayLengthX - 1);
	const int y = FMath::Clamp(FMath::RoundToInt((GridRelative.Y - NodeRadius) / NodeDiameter), 0, GridArrayLengthY - 1);
	const int z = FMath::Clamp(FMath::RoundToInt((GridRelative.Z - NodeRadius) / NodeDiameter), 0, GridArrayLengthZ - 1); 

	return GetNodeFromArray(x, y, z);
}

TArray<GridNode*> AMapGrid::GetNeighbours(const GridNode* Node) const
{
	TArray<GridNode*> Neighbours;

	// -1 to plus 1 in each direction to get every neighbour node 
	for(int x = -1; x <= 1; x++)
	{
		for(int y = -1; y <= 1; y++)
		{
			for(int z = -1; z <= 1; z++)
			{
				if(x == 0 && y == 0 && z == 0) // itself 
					continue;

				const int GridX = Node->GridX + x; // Grid indexes 
				const int GridY = Node->GridY + y;
				const int GridZ = Node->GridZ + z; 

				// if any index is out of bounds 
				if(IsOutOfBounds(GridX, GridY, GridZ))
					continue;

				Neighbours.Add(GetNodeFromArray(GridX, GridY, GridZ));
			}
		}
	}

	return Neighbours; 
}

bool AMapGrid::IsOutOfBounds(const int GridX, const int GridY, const int GridZ) const
{
	return GridX < 0 || GridX > GridArrayLengthX - 1 || GridY < 0 || GridY > GridArrayLengthY - 1 || GridZ < 0 || GridZ > GridArrayLengthZ - 1; 
}

void AMapGrid::DrawDebugStuff() const
{
	// Draw border of grid 
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(GridSize.X / 2, GridSize.Y / 2, 3), FColor::Red, true);

	// draw each node where unwalkable nodes are red and walkable green
	int ActualArrayCount = 0; 
	for(int x = 0; x < GridArrayLengthX; x++)
	{
		for(int y = 0; y < GridArrayLengthY; y++)
		{
			for(int z = 0; z < GridArrayLengthZ; z++)
			{
				const GridNode* Node = GetNodeFromArray(x, y, z);
				FColor Color = Node->IsWalkable() ? FColor::Green : FColor::Red; 
				DrawDebugBox(GetWorld(), Node->GetWorldCoordinate(), FVector(NodeRadius, NodeRadius, 1), Color, true);
			}
		}
	}

	// prints some stuff 
	UE_LOG(LogTemp, Warning, TEXT("diameter: %f"), NodeDiameter)
	UE_LOG(LogTemp, Warning, TEXT("Grid Length: (X: %i, Y: %i, Z: %i)"), GridArrayLengthX, GridArrayLengthY, GridArrayLengthZ)
	UE_LOG(LogTemp, Warning, TEXT("GridSize: %s"), *GridSize.ToString())

	UE_LOG(LogTemp, Warning, TEXT("Number of nodes: %i"), GridArrayLengthX * GridArrayLengthY * GridArrayLengthZ)
}

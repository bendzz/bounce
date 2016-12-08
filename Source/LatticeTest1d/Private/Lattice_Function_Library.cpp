// Fill out your copyright notice in the Description page of Project Settings.

#include "LatticeTest1d.h"
#include "Lattice_Function_Library.h"
#include <string>
//#include <list>
#include <map>
#include <set>
//using std::vector;

//#include "UObjectPluginPrivatePCH.h"
//#include "Components/StaticMeshComponent.h"
#include "StaticMeshResources.h"

/*
int32 voxelPositions[1][1][1];
int32 Voxels[1][1];
int32 oldVoxels[1][1];
*/


//Happy Message
//FString ULattice_Function_Library::GetHappyMessage()
bool ULattice_Function_Library::LatticeBuilder(const bool useMesh, const FVector latticeSize, const TArray<int32>& voxelElements, const float edgeLength, const UStaticMesh* mesh, const int32 textureWidth,
	TArray<FVector>& OriginalPoints, TArray<FVector2D>& allEdges, TArray<FString>& ElementPointPointers, TArray<FString>& ElementEdgePointers, TArray<FVector>& meshPoints, TArray<int32>& meshTriangles, TArray<FVector2D>& meshUVs, TArray<FString>& PointToElementPointers)
{
	const FVector voxelSize = latticeSize - 1;

	std::vector<std::vector<std::vector<int32> > > voxelsFromMesh;
	voxelsFromMesh.resize(latticeSize.Z);
	for (int z = 0; z < latticeSize.Z; ++z) {
		voxelsFromMesh[z].resize(latticeSize.Y);
		for (int y = 0; y < latticeSize.Y; ++y) {
			voxelsFromMesh[z][y].resize(latticeSize.X);
		}
	}


	TArray<FVector> vertices = TArray<FVector>();
	//if (!IsValidLowLevel()) return false;
	//if (!StaticMeshComponent) return false;
	if (!mesh) return false;
	if (!mesh->RenderData) return false;

	//FPositionVertexBuffer* VertexBuffer = &StaticMeshComponent->StaticMesh->RenderData->LODResources[0].PositionVertexBuffer;

	voxelsFromMesh[1][1][1] = 1;

	int voxelCount = 2;
	if (mesh->RenderData->LODResources.Num() > 0)
	{
		FPositionVertexBuffer* VertexBuffer = &mesh->RenderData->LODResources[0].PositionVertexBuffer;
		if (VertexBuffer)
		{
			const int32 VertexCount = VertexBuffer->GetNumVertices();
			UE_LOG(LogTemp, Warning, TEXT("vertex count %i"), VertexCount);

			for (int32 Index = 0; Index < VertexCount; Index++)
			{
				//This is in the Static Mesh Actor Class, so it is location and tranform of the SMActor
				//const FVector WorldSpaceVertexLocation = GetActorLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(Index));
				const FVector loc = VertexBuffer->VertexPosition(Index);

				//UE_LOG(LogTemp, Warning, TEXT("vertex location %f %f %f"), loc.X, loc.Y, loc.Z);

				//float scale = 1.0f;
				float scale = 1.0f;
				//FVector offset = FVector(-30.0f, -50.0f, 100.0f);
				FVector offset = FVector(-30.0f, -50.0f, 0.0f);

				int32 x = (loc.X - offset.X) / (edgeLength * scale);
				int32 y = (loc.Y - offset.Y) / (edgeLength * scale);
				int32 z = (loc.Z - offset.Z) / (edgeLength * scale);

				//UE_LOG(LogTemp, Warning, TEXT("voxel location %i %i %i"), x, y, z);
				if (x > -1 && y > -1 && z > -1 && x < voxelSize.X && y < voxelSize.Y && z < voxelSize.Z) {
					if (voxelsFromMesh[x][y][z] == 0) {
						voxelsFromMesh[x][y][z] = voxelCount;
						voxelCount += 1;
					}
					//UE_LOG(LogTemp, Warning, TEXT("location VALID!"));
				}

				//add to output FVector array
				vertices.Add(loc);
			}
		}
	}



	//TArray<int32> ElementEdges

	//FVector voxelPositions[3];
	//int32 voxelPositions[3][2];
	int32 voxels[3];
	int32 oldVoxels[3];
	//int32 filledInPoints[latticeSize.X][latticeSize.Y][latticeSize.Z];
	//int32* filledInPoints = new int32[latticeSize.X][latticeSize.Y][latticeSize.Z];

	// TODO: Not sure if I set up the dimensions correctly! Test later to see if I get out of bounds errors
	std::vector<std::vector<std::vector<double> > > filledInPoints;
	filledInPoints.resize(latticeSize.Z);
	for (int z = 0; z < latticeSize.Z; ++z) {
		filledInPoints[z].resize(latticeSize.Y);
		for (int y = 0; y < latticeSize.Y; ++y) {
			filledInPoints[z][y].resize(latticeSize.X);
		}
	}

	//int32 iii = filledInPoints[1][1][1];

	//std::vector<std::list<int>> ElementEdges;
	//ElementEdges.resize(latticeSize.X*latticeSize.Y*latticeSize.Z);

	std::map<int32, std::set<int32> > elementEdges;
	std::map<int32, std::set<int32> > elementPoints;

	//UE_LOG(LogTemp, Warning, TEXT("test test %f"), voxelSize.X);
	int32 x = 0; int32 y = 0; int32 z = 0;
	bool drawEdge = false; bool drawPoint = false; bool oldDrawEdge;
	int lastPointTouched = 0; int oldLastPointTouched = 0;
	TArray<int32> touchedVoxels;

	// point[0] is always buggered in physics sim; make it an empty one.
	OriginalPoints.Add(FVector(0, 0, 0));
	//float offset = 0.2;
	meshPoints.Add(FVector(0, 0, 0));
	meshPoints.Add(FVector(0, 0, 0));
	meshUVs.Add(FVector2D(0.5f, 0.5f));
	meshUVs.Add(FVector2D(0.5f, 0.5f));


	for (int32 axis = 0; axis < 3; axis++)
	{
		for (int32 x0 = 0; x0 < latticeSize.X; x0++)
		{
			for (int32 y0 = 0; y0 < latticeSize.Y; y0++)
			{
				for (int32 z0 = 0; z0 < latticeSize.Z; z0++)
				{
					for (int32 v = 0; v < 4; v++) {
						oldVoxels[v] = voxels[v];
					}

					// orient it to the current axis/edge
					if (useMesh) {
						if (axis == 0) {
							x = x0; y = y0; z = z0;
							voxels[0] = getVoxel(x - 1, y - 1, z, voxelsFromMesh, voxelSize);
							voxels[1] = getVoxel(x, y - 1, z, voxelsFromMesh, voxelSize);
							voxels[2] = getVoxel(x - 1, y, z, voxelsFromMesh, voxelSize);
							voxels[3] = getVoxel(x, y, z, voxelsFromMesh, voxelSize);
						}
						if (axis == 1) {
							x = x0; y = z0; z = y0;
							voxels[0] = getVoxel(x - 1, y, z - 1, voxelsFromMesh, voxelSize);
							voxels[1] = getVoxel(x, y, z - 1, voxelsFromMesh, voxelSize);
							voxels[2] = getVoxel(x - 1, y, z, voxelsFromMesh, voxelSize);
							voxels[3] = getVoxel(x, y, z, voxelsFromMesh, voxelSize);
						}
						if (axis == 2) {
							x = z0; y = y0; z = x0;
							voxels[0] = getVoxel(x, y - 1, z - 1, voxelsFromMesh, voxelSize);
							voxels[1] = getVoxel(x, y - 1, z, voxelsFromMesh, voxelSize);
							voxels[2] = getVoxel(x, y, z - 1, voxelsFromMesh, voxelSize);
							voxels[3] = getVoxel(x, y, z, voxelsFromMesh, voxelSize);
						}
					}
					else {
						if (axis == 0) {
							x = x0; y = y0; z = z0;
							voxels[0] = getVoxel(x - 1, y - 1, z, voxelElements, voxelSize);
							voxels[1] = getVoxel(x, y - 1, z, voxelElements, voxelSize);
							voxels[2] = getVoxel(x - 1, y, z, voxelElements, voxelSize);
							voxels[3] = getVoxel(x, y, z, voxelElements, voxelSize);
						}
						if (axis == 1) {
							x = x0; y = z0; z = y0;
							voxels[0] = getVoxel(x - 1, y, z - 1, voxelElements, voxelSize);
							voxels[1] = getVoxel(x, y, z - 1, voxelElements, voxelSize);
							voxels[2] = getVoxel(x - 1, y, z, voxelElements, voxelSize);
							voxels[3] = getVoxel(x, y, z, voxelElements, voxelSize);
						}
						if (axis == 2) {
							x = z0; y = y0; z = x0;
							voxels[0] = getVoxel(x, y - 1, z - 1, voxelElements, voxelSize);
							voxels[1] = getVoxel(x, y - 1, z, voxelElements, voxelSize);
							voxels[2] = getVoxel(x, y, z - 1, voxelElements, voxelSize);
							voxels[3] = getVoxel(x, y, z, voxelElements, voxelSize);
						}
					}

					oldDrawEdge = drawEdge;
					drawEdge = false;
					if (voxels[0] != voxels[1] && voxels[0] != voxels[2]) {
						drawEdge = true;
					}
					else if (voxels[1] != voxels[0] && voxels[1] != voxels[3]) {
						drawEdge = true;
					}
					else if (voxels[2] != voxels[0] && voxels[2] != voxels[3]) {
						drawEdge = true;
					}
					else if (voxels[3] != voxels[1] && voxels[3] != voxels[2]) {
						drawEdge = true;
					}
					if (oldDrawEdge) {
						touchedVoxels.Add(oldVoxels[0]);
						touchedVoxels.Add(oldVoxels[1]);
						touchedVoxels.Add(oldVoxels[2]);
						touchedVoxels.Add(oldVoxels[3]);
					}

					drawPoint = false;
					for (int32 v = 0; v < 4; v++) {
						if (oldVoxels[v] != voxels[v]) {
							drawPoint = true;
						}
					}

					if (drawPoint && (drawEdge || oldDrawEdge)) {
						// draw point
						oldLastPointTouched = lastPointTouched;
						if (filledInPoints[x][y][z] == 0) {
							lastPointTouched = OriginalPoints.Add(FVector(x*edgeLength, y*edgeLength, z*edgeLength));
							filledInPoints[x][y][z] = lastPointTouched;

							// add procedural mesh triangle points
							float offset = 0.2;
							meshPoints.Add(FVector(x*edgeLength, y*edgeLength, z*edgeLength));
							meshPoints.Add(FVector(x*edgeLength + offset, y*edgeLength + offset, z*edgeLength + offset));

							// add mesh UVs
							float U; float V;
							indexToUVs(lastPointTouched, textureWidth, U, V);
							meshUVs.Add(FVector2D(U, V));
							meshUVs.Add(FVector2D(U, V));
							//meshUVs.Add(FVector2D(0.5f, 0.5f));
							//meshUVs.Add(FVector2D(0.5f, 0.5f));
						}
						else {
							lastPointTouched = filledInPoints[x][y][z];
						}
						for (int32 t = 0; t < touchedVoxels.Num(); t++) {
							elementPoints[touchedVoxels[t]].insert(lastPointTouched);
							elementPoints[touchedVoxels[t]].insert(oldLastPointTouched);
						}

						//UE_LOG(LogTemp, Warning, TEXT("test test %i"), lastPointTouched);

						// draw edge
						if (!drawEdge || (drawEdge && oldDrawEdge)) {
							int32 edge = allEdges.Add(FVector2D((int32)oldLastPointTouched, (int32)lastPointTouched));

							for (int32 t = 0; t < touchedVoxels.Num(); t++) {
								elementEdges[touchedVoxels[t]].insert(edge);
							}
							touchedVoxels.Empty();
							//elementEdges
						}
					}
					//OriginalPoints.Add(FVector(x*edgeLength, y*edgeLength, z*edgeLength));
				}
			}
		}
	}

	ElementPointPointers.SetNum((--elementPoints.end())->first + 1, false);
	PointToElementPointers.SetNum(OriginalPoints.Num() + 1, false);
	//ElementEdgePointers.SetNum((--elementEdges.end())->first + 1, false);
	for (std::map<int32, std::set<int32> >::iterator map = elementPoints.begin(); map != elementPoints.end(); ++map) {
		//UE_LOG(LogTemp, Warning, TEXT("element %d"), map->first);
		for (std::set<int32>::iterator it = map->second.begin(); it != map->second.end(); ++it) {
			//UE_LOG(LogTemp, Warning, TEXT("edge %d"), (--elementEdges.end())->first);
			//UE_LOG(LogTemp, Warning, TEXT("point %d"), *it);

			ElementPointPointers[map->first] += FString::FromInt(*it) + ",";

			PointToElementPointers[*it] += FString::FromInt(map->first) + ",";
		}
	}

	//ElementEdgePointers.SetNum(elementEdges.end()->first, false);
	ElementEdgePointers.SetNum((--elementEdges.end())->first + 1, false);
	for (std::map<int32, std::set<int32> >::iterator map = elementEdges.begin(); map != elementEdges.end(); ++map) {
		//UE_LOG(LogTemp, Warning, TEXT("element %d"), map->first);
		for (std::set<int32>::iterator it = map->second.begin(); it != map->second.end(); ++it) {
			//UE_LOG(LogTemp, Warning, TEXT("edge %d"), (--elementEdges.end())->first);
			//UE_LOG(LogTemp, Warning, TEXT("edge %d"), *it);

			ElementEdgePointers[map->first] += FString::FromInt(*it) + ",";

			// make polygon triangles
			meshTriangles.Add(allEdges[*it].X * 2);
			meshTriangles.Add(allEdges[*it].Y * 2);
			meshTriangles.Add(allEdges[*it].Y * 2 + 1);
		}
	}

	return true;
}

//std::vector<std::vector<std::vector<int32> > > voxelsFromMesh;
//int32 ULattice_Function_Library::getVoxel(int32 x, int32 y, int32 z, std::vector<std::vector<std::vector<int32> > >& voxelsFromMesh, const FVector voxelSize) {
int32 ULattice_Function_Library::getVoxel(int32 x, int32 y, int32 z, std::vector<std::vector<std::vector<int32> > >& voxelsFromMesh, const FVector voxelSize) {
	bool valid = false;
	//int32 index = voxelElements[index3Dto1D(true, x, y, z, voxelSize, valid)];
	if (x > -1 && y > -1 && z > -1 && x < voxelSize.X && y < voxelSize.Y && z < voxelSize.Z) {
		//return index;
		return voxelsFromMesh[x][y][z];
		//return 0;
	}
	else {
		return 0;
	}
}

int32 ULattice_Function_Library::getVoxel(int32 x, int32 y, int32 z, const TArray<int32>& voxelElements, const FVector voxelSize) {
	bool valid = false;
	int32 index = voxelElements[index3Dto1D(true, x, y, z, voxelSize, valid)];
	if (valid) {
		return index;
	}
	else {
		return 0;
	}
}

int32 ULattice_Function_Library::index3Dto1D(bool checkBounds, FVector indices, const FVector size, bool &valid) {
	return index3Dto1D(checkBounds, indices.X, indices.Y, indices.Z, size, valid);
}

int32 ULattice_Function_Library::index3Dto1D(bool checkBounds, int32 x, int32 y, int32 z, const FVector size, bool &valid)
{
	if (checkBounds) {
		valid = true;
		if (x > -1 && y > -1 && z > -1 && x < size.X && y < size.Y && z < size.Z) {
			int32 i = size.X*size.Y*z + size.X*y + x;
			return i;
		}
		else
		{
			valid = false;
			return 0;
		}
	}

	int32 i = size.X*size.Y*z + size.X*y + x;

	//UE_LOG(LogTemp, Warning, TEXT("test test %d"), i);
	return i;
}

void ULattice_Function_Library::indexToUVs(int32 index, int32 width, float &U, float &V) {
	int32 y = index / width;
	int32 x = index - (y * width);
	float fwidth = width; // so dumb. Makes the division result in a float, not int
	U = x / fwidth;
	V = y / fwidth;
	//UE_LOG(LogTemp, Warning, TEXT("test test %f %f %i %i %f %i"), U, V, x, y, x / width, width);
}
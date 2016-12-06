// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "Components/StaticMeshComponent.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Lattice_Function_Library.generated.h"


/**
 * 
 */
UCLASS()
class LATTICETEST1D_API ULattice_Function_Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(BlueprintCallable, Category = "SoftBody")
		//static bool LatticeBuilder(const FVector latticeSize, const TArray<int32>& voxelElements, const float edgeLength, TArray<FVector>& OriginalPoints, TArray<FVector2D>& allEdges, TArray<FString>& ElementPointPointers, TArray<FString>& ElementEdgePointers);
		static bool LatticeBuilder(const bool useMesh, const FVector latticeSize, const TArray<int32>& voxelElements, const float edgeLength, const UStaticMesh* mesh, TArray<FVector>& OriginalPoints, TArray<FVector2D>& allEdges, TArray<FString>& ElementPointPointers, TArray<FString>& ElementEdgePointers);
	private:
		static int32 getVoxel(int32 x, int32 y, int32 z, std::vector<std::vector<std::vector<int32> > >& voxelsFromMesh, const FVector voxelSize);
		static int32 getVoxel(int32 x, int32 y, int32 z, const TArray<int32>& voxelElements, const FVector voxelSize);
		static int32 index3Dto1D(bool checkBounds, FVector indices, const FVector size, bool &valid);
		static int32 index3Dto1D(bool checkBounds, int32 x, int32 y, int32 z, const FVector size, bool &valid);
	
};

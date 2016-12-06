// Fill out your copyright notice in the Description page of Project Settings.
//Test, ignore!

#pragma once

//#include "lattice_building.generated.h"
#include "lattice_building.h"
//#include "GameFramework/Actor.h"

//UCLASS()
class LATTICETEST1D_API lattice_building
//class lattice_building : public UBlueprintFunctionLibrary
{
	//GENERATED_BODY()
public:
	lattice_building();
	~lattice_building();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Hello World", CompactNodeTitle = "HelloWorld", Keywords = "String Hello World"), Category = Game)
	static FText HelloWorld();
};


/*
UFUNCTION(BlueprintPure, meta = (DisplayName = "Adds floats", CompactNodeTitle = "Add Floats", Keywords = "Float Add"), Category = Game)
static float AddFloats(float fA, float fB);
*/



/*


// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "VictoryBPFunctionLibrary.generated.h"
#include "lattice_building.h"


//above name must match the name on your hard disk for this .h file

//note about UBlueprintFunctionLibrary
// This class is a base class for any function libraries exposed to blueprints.
// Methods in subclasses are expected to be static

UCLASS()
class UVictoryBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

		UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
		static FString GetHappyMessage();

	/** Saves text to filename of your choosing, make sure include whichever file extension you want in the filename, ex: SelfNotes.txt . Make sure to include the entire file path in the save directory, ex: C:\MyGameDir\BPSavedTextFiles 
	UFUNCTION(BlueprintCallable, Category = "VictoryBPLibrary")
		static bool SaveStringTextToFile(FString SaveDirectory, FString FileName, FString SaveText, bool AllowOverWriting = false);
};

*/
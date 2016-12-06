// Fill out your copyright notice in the Description page of Project Settings.


#include "LatticeTest1d.h"
#include "lattice_building.h"


lattice_building::lattice_building()
{
}

lattice_building::~lattice_building()
{
}

FText lattice_building::HelloWorld()
{
	return FText::FromString("Hello World");
}



/*
float UCreateNewObject::AddFloats(float fA, float fB)
{
	return fA + fB;
}
*/


/*

// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

//#include "VictoryGame.h"
#include "LatticeTest1d.h"
#include "lattice_building.h"

//Change the above to YourGameName.h, 
//the same line you see in all your .cpp files

//////////////////////////////////////////////////////////////////////////
// UVictoryBPFunctionLibrary

UVictoryBPFunctionLibrary::UVictoryBPFunctionLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

//Happy Message
FString UVictoryBPFunctionLibrary::GetHappyMessage()
{
	return FString("Victory! Victory BP Library Works!");
}

bool UVictoryBPFunctionLibrary::SaveStringTextToFile(
	FString SaveDirectory,
	FString FileName,
	FString SaveText,
	bool AllowOverWriting
	) {
	//GFileManager?
	if (!GFileManager) return false;

	//Dir Exists?
	if (!GFileManager->DirectoryExists(*SaveDirectory))
	{
		//create directory if it not exist
		GFileManager->MakeDirectory(*SaveDirectory);

		//still could not make directory?
		if (!GFileManager->DirectoryExists(*SaveDirectory))
		{
			//Could not make the specified directory
			return false;
			//~~~~~~~~~~~~~~~~~~~~~~
		}
	}

	//get complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	//No over-writing?
	if (!AllowOverWriting)
	{
		//Check if file exists already
		if (GFileManager->GetFileAgeSeconds(*SaveDirectory) > 0)
		{
			//no overwriting
			return false;
		}
	}

	return FFileHelper::SaveStringToFile(SaveText, *SaveDirectory);
}



*/
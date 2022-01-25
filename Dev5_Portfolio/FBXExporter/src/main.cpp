#include<iostream>
#include "FBXExporter.h"


int main(int argc, char** argv)
{
	FBXExporter* myExporter = new FBXExporter();
	myExporter->Initialize();
	myExporter->LoadScene("/src/MageAssets/Idle.fbx", "/src/MageAssets");
	//myExporter->LoadScene(argv[1], argv[2]);

	myExporter->ExportFBX();
	getchar();
	return 0;
}
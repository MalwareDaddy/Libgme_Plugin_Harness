#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <iostream>


//struct Taken from Source Code in file gme.h
typedef struct Music_Emu Music_Emu;

//handle_error(gme_open_data(file_data, (long)file_size, &emu, sample_rate));


// Ghidra Decompiled Code : 
//typedef char* (__cdecl* gme_open_data)(uint *param_1,int param_2,int **param_3,int param_4);

//gme_open_data from Source : 
typedef char* (__cdecl* gme_open_data)(void const* data, long size, Music_Emu** out, int sample_rate);

static gme_open_data pLib_LOAD_GME_OPEN_DATA;

char* dump_file(const char* file_path, size_t* size)
{
	FILE* in = fopen(file_path, "rb");
	char* buffer = NULL;
	if (!in)
		return NULL;
	fseek(in, 0, SEEK_END);
	(*size) = (size_t)ftell(in);
	fseek(in, 0, SEEK_SET);
	buffer = (char*)malloc(*size);
	fread(buffer, 1, *size, in);
	return buffer;
}


int main(int argc, char *argv[])
{
	//const char* filename = "test.nsf"; /* Default file to open */
	const char* filename = argv[1]; 
	//if (argc >= 2){ filename = argv[1]; }

	int sample_rate = 44100;

	int track = argc >= 3 ? atoi(argv[2]) : 0;

	size_t file_size = 0;
	char* file_data = dump_file(filename, &file_size);
	if (!file_data)
	{
		printf("Error: Can't dump %s!\n", filename);
		
		return 1;
	}

	Music_Emu* emu;

	//HMODULE GME2PWN = LoadLibraryA("Releaselibgme_plugin.dll");
	HMODULE GME2PWN = LoadLibraryA("libgme_plugin.dll");
	
	if (GME2PWN == NULL)
	{
		printf("Failed to Load DLL\n");
		
		DWORD dw =  GetLastError();
		std::cout << dw;
		return 1;
	}
	pLib_LOAD_GME_OPEN_DATA = (gme_open_data)GetProcAddress(GME2PWN, "gme_open_data");

	if (!pLib_LOAD_GME_OPEN_DATA)
	{
		free(file_data);

		printf("Failed to Open Func gme_open_data();\n");

		return 1;
	}
	char* ret2Pwn;

	//handle_error(gme_open_data(file_data, (long)file_size, &emu, sample_rate));
	ret2Pwn = pLib_LOAD_GME_OPEN_DATA(file_data, (long)file_size, &emu, sample_rate);

	FreeLibrary(GME2PWN);
	free(file_data);

	printf("All Good :D");

	return 0;
}


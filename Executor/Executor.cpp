// Executor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include "../util.h"
#include "../debug.h"
#include "../tinyxml2/tinyxml2.h"

using namespace tinyxml2;

static BOOL run_command(LPCWSTR cmd)
{
	// process information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	BOOL ret;

	// Set size of structures.
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the program.
	ret = CreateProcess(NULL,   // no module name (use command line)
		_tcsdup(cmd),			// command line
		NULL,           // process handle not inheritable
		NULL,           // thread handle not inheritable
		FALSE,          // set handle inheritance to FALSE
		0,              // no creation flags
		NULL,           // use parent's environment block
		NULL,           // use parent's starting directory 
		&si,            // pointer to STARTUPINFO structure
		&pi             // pointer to PROCESS_INFORMATION structure
	);
	if (ret == FALSE) {
		ERR(L"CreateProcess");
		return FALSE;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Print exit information.
	{
		DWORD exitCode;
		ret = GetExitCodeProcess(pi.hProcess, &exitCode);
		if (ret == FALSE) {
			ERR(L"GetExitCodeProcess");
			return FALSE;
		}
		dlog(LOG_ERR, "Process from command %ws exited with exit code %u\n", cmd, exitCode);
	}

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return TRUE;
}

int main()
{
	run_command(L"Adder");
	run_command(L"Multiplier");
	run_command(L"Xorer");

	tinyxml2::XMLDocument config;
	XMLError e = config.LoadFile("../Executor/config.xml");
	if (e != XML_SUCCESS) {
		std::cerr << "error" << std::endl;
		exit(EXIT_FAILURE);
	}
	XMLNode *root = config.FirstChild();	/* executor */
	//XMLNode *n = root->FirstChild();	/* modules */
	XMLElement *ec = root->ToElement();
	//XMLElement *ec = n->FirstChildElement();	/* first module */
	std::cout << "name:" << ec->Name() << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

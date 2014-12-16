#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include <stdio.h>
#include <iostream>
#include "strsafe.h"

using namespace std;

HRESULT CreateLink(char *userPath, char *name, char *desc); // this is the prototype of function which you need to use

int main(int argc,char** argv)
{
	char *user;
	char *comment = "crate favorite folder link for test doc file"; //this is needed to be changed
	size_t requiredSize;

	getenv_s(&requiredSize, NULL, 0, "USERPROFILE");
	if (requiredSize == 0)
	{
      printf("USERPROFILE doesn't exist!\n");
      exit(1);
	}

	user = (char*) malloc(requiredSize * sizeof(char));
	if (!user)
	{
      printf("Failed to allocate memory!\n");
      exit(1);
	}

	// Get the value of the USERPROFILE environment variable.
	getenv_s( &requiredSize, user, requiredSize, "USERPROFILE" );

	CoInitialize(NULL);
	HRESULT h = CreateLink(user,"Profile.docx", comment);
	CoUninitialize();

	free(user);
	return 0;
}

// CreateLink - Uses the Shell's IShellLink and IPersistFile interfaces 
//              to create and store a shortcut to the specified object. 
//
// Returns the result of calling the member functions of the interfaces. 
//
// Parameters:
// userPath  - Address of a buffer that contains the path of user folder.
// 
// name - Address of a buffer that contains the name of file, 
//            including the file type.
// desc     - Address of a buffer that contains a description of the 
//                Shell link, stored in the Comment field of the link
//                properties.

HRESULT CreateLink(char *userPath, char *name, char *desc)
{ 
	HRESULT hres;
	LPCSTR lpszPathObj, lpszDesc, lpszPathLink;
	IShellLink* psl; 
	int tSize;
	char *target, *tFolder = "\\RepoZip\\", *tType = " - Shortcut.lnk";
 
	tSize = strlen(userPath)+strlen(name)+strlen(tFolder)+strlen(tType)+2;	
	target = (char *)calloc(tSize,sizeof(char));
	strcpy_s(target, tSize, userPath);
	strcat_s(target,tSize, tFolder);
	strcat_s(target, tSize, name);
	strcat_s(target, tSize, tType);

	lpszPathObj = (LPCSTR)"C:\\Users\\Kelum Deshapriya\\Documents\\Visual Studio 2010\\Projects\\SPrinter\\Debug\\SPrinter.exe"; //use your program's .exe path
	lpszDesc = (LPCSTR)desc;
	lpszPathLink = (LPCSTR)target;
	// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
	// has already been called.
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl); 
	if (SUCCEEDED(hres)) 
	{ 
		IPersistFile* ppf; 
 
		// Set the path to the shortcut target and add the description. 
		psl->SetPath(lpszPathObj); 
		psl->SetDescription((LPCSTR)lpszDesc);
		psl->SetArguments((LPCSTR)name);
 
		// Query IShellLink for the IPersistFile interface, used for saving the 
		// shortcut in persistent storage. 
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 
 
		if (SUCCEEDED(hres)) 
		{ 
			WCHAR wsz[MAX_PATH]; 
 
			// Ensure that the string is Unicode. 
			MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH); 
            
			// Add code here to check return value from MultiByteWideChar 
			// for success.
			cout << "success" << endl;
 
			// Save the link by calling IPersistFile::Save. 
			hres = ppf->Save(wsz, TRUE); 
			ppf->Release(); 
		} 
		psl->Release(); 
	} 
	free(target);
	return hres; 
}

//following code snipt can be used to get command line arguments send when previously created shortcut double clicked.
//lpszPathObj should contain path to .exe of Program which include following code snipt
/*
#include <iostream>

using namespace std;

int main(int argc,char** argv)
{
	if(argc >1){
		cout << argv[1] << endl;
	}else{
		cout << "no of arguments are not big enough" << endl;
	}
	while(true);
	return 0;
}
*/


#define INITGUID

#include <windows.h>
#include <winnls.h>
#include <shobjidl.h>
#include <objbase.h>
#include <objidl.h>
#include <shlguid.h>
#include <initguid.h>
#include <stdio.h>
#include <iostream>

using namespace std;

HRESULT CreateLink(string sourcePath, string tagetPath, string desc, string cmdArg);

int main(int argc, char** argv) {

    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    HRESULT h = CreateLink("C:/Users/Kelum Deshapriya/Documents/RepoZip/licene.txt", "C:/Users/Kelum Deshapriya/Documents/Test/licene.txt", "test shortcut", "licene/text/file/path/recieve");
    CoUninitialize();
        
    return 0;
}

// CreateLink - Uses the Shell's IShellLink and IPersistFile interfaces 
//              to create and store a shortcut to the specified object. 
//
// Returns the result of calling the member functions of the interfaces. 
//
// Parameters:
// sourcePath  - Address of a buffer that contains the path of source folder.
// 
// tagetPath - Address of a buffer that contains the path of shortcut need to create, 
//            including the file type.
// desc     - Address of a buffer that contains a description of the 
//                Shell link, stored in the Comment field of the link
//                properties.
//
//  cmdArg    - argument that should pass to program. there should not be space in cmdArg. 
//

HRESULT CreateLink(string sourcePath, string targetPath, string desc, string cmdArg)
{ 
    HRESULT hres;
    LPCSTR lpszPathObj, lpszDesc, lpszPathLink;
    IShellLink* psl; 

    string target = targetPath + " - Shortcut.lnk";



    lpszPathObj = (LPCSTR)"C:\\Users\\Kelum Deshapriya\\Documents\\Visual Studio 2010\\Projects\\SPrinter\\Debug\\SPrinter.exe"; //use your program's .exe path
    lpszDesc = (LPCSTR)desc.c_str();
    lpszPathLink = (LPCSTR)target.c_str();
    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl); 
    if (SUCCEEDED(hres)) 
    { 
            IPersistFile* ppf; 

            // Set the path to the shortcut target and add the description. 
            psl->SetPath(lpszPathObj); 
            psl->SetDescription((LPCSTR)lpszDesc);
            psl->SetArguments((LPCSTR)cmdArg.c_str());

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
    return hres; 
}

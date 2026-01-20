#include <stdio.h>
#include <windows.h>

#include "Lister.h"
#include "resource.h"

BOOL GetLister(CLister ** lppLister, HMODULE hListerDll)
{
	FPOBTAINLISTER fpObtainLister=NULL;

	if(lppLister==NULL || hListerDll==NULL)
		return FALSE;

	fpObtainLister=(FPOBTAINLISTER)GetProcAddress(hListerDll, "ObtainLister");

	if(fpObtainLister==NULL)
	{
		MessageBox(NULL, "Invalid Lister Plugin!", "Listing Generator", MB_OK|MB_ICONERROR);
		return FALSE;
	}

	(*lppLister)=fpObtainLister();

	if((*lppLister)==NULL)
	{
		MessageBox(NULL, "Could not obtain lister!", "Listing Generator", MB_OK|MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

BOOL DoListing(char szPlugin[MAX_PATH], HWND hwndParent)
{
	HMODULE hListerDll=NULL;
	CLister * lpLister=NULL;

	hListerDll=LoadLibraryA(szPlugin);

	if(hListerDll==NULL)
	{
		MessageBox(hwndParent, "Could not load the specified plugin!", "Listing Generator", MB_OK|MB_ICONERROR);
		return FALSE;
	}


	if(!GetLister(&lpLister, hListerDll))
	{
		MessageBox(hwndParent, "Unable to obtain lister!", "Listing Generator", MB_OK|MB_ICONERROR);
		FreeLibrary(hListerDll);
		return 0;
	}

	if(lpLister->RunDialog(hwndParent))
	{
		OPENFILENAME of;
		char szFilename[MAX_PATH];
		szFilename[0]=NULL;
		ZeroMemory(&of, sizeof(OPENFILENAME));
		
		of.lStructSize=sizeof(OPENFILENAME);
		of.hwndOwner=hwndParent;
		of.lpstrFilter="HTML File (*.html)\0*.html\0All Files (*.*)\0*.*\0";
		of.lpstrFile=szFilename;
		of.nMaxFile=MAX_PATH;
		of.lpstrTitle="Generate Listing";
		of.lpstrDefExt="html";
		
		if(GetSaveFileName(&of))
			lpLister->CreateListing(szFilename);
	}

	delete lpLister;

	FreeLibrary(hListerDll);
	hListerDll=NULL;

	return TRUE;
}

BOOL PopulateListboxWithPlugins(HWND hwnd, int nIDDlgItem)
{
	HANDLE hFind=NULL;
	WIN32_FIND_DATA FindData;

	ZeroMemory(&FindData, sizeof(WIN32_FIND_DATA));

	hFind=FindFirstFile("plugins\\*.dll", &FindData);

	if(hFind==INVALID_HANDLE_VALUE)
		return FALSE;

	do
	{
		SendDlgItemMessage(hwnd, nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)FindData.cFileName);
	}while(FindNextFile(hFind, &FindData));

	FindClose(hFind);

	return TRUE;
}

INT_PTR CALLBACK PluginSelect(
	HWND hwnd, 
	UINT uMsg, 
	WPARAM wParam, 
	LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
	{
		//Populat dialog box with available plugins.
		PopulateListboxWithPlugins(hwnd, IDC_LISTINGTYPE);
		break;
	}
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case IDC_GENERATE:
		{
			char szPath[MAX_PATH];
			char szTemp[MAX_PATH];
			GetDlgItemText(hwnd, IDC_LISTINGTYPE, szTemp, MAX_PATH);
			sprintf(szPath, "plugins\\%s", szTemp);
			if(!DoListing(szPath, hwnd))
			{
				MessageBox(hwnd, "Could Not Generate Listing!", "Listing Generator", MB_OK|MB_ICONERROR);
			}
			else
			{
				MessageBox(hwnd, "Successfully Ran Listing Wizard!", "Listing Generator", MB_OK|MB_ICONINFORMATION);
			}
			break;
		}
		case IDC_QUIT:
			EndDialog(hwnd, 0);
			break;
		default:
			break;
		}
		break;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE, 
	LPSTR lpCmdLine, 
	int nShowCmd)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_LISTING), NULL, PluginSelect);
	//DoListing("plugins\\gamelister.dll", NULL);

	return 0;
}
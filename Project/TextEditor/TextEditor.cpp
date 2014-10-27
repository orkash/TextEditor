//---
// Fichier centrale du programme
// Il contient le point d'entrée (virtuel avec VS 2012) qui va initialiser tout le programme
//---
//Fichier écrit par Nuri Yuri le 26/10/2014
//---
#include "stdafx.h"
#include "TextEditor.h"

// Global Variables:
HINSTANCE hInst;								// current instance

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	MSG msg;

	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}



//---
//>.>
//---
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEXTEDITOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= TEXT("TextEditorClass");
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}


//---
//InitInstance : Initialisation du programme
//  On y crée/vérifie les éléments importants
//---
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	TCHAR* filename;
	TCHAR* filenames[LOADING_TREE_SIZE] = LOADING_TREE;
	TCHAR error_str[1024];
	unsigned long i;
	
	hInst = hInstance; // Store instance handle in our global variable
	
	//---
	//>Vérification de la présence des dossiers utiles au programme
	//---
	for(i=0;i<LOADING_TREE_SIZE;i++)
	{
		filename=filenames[i];
		if(!File::exist(filename))
		{
			TCHAR error_buff[2048];
			LoadString(hInst,IDS_STRING195,error_buff,2048);
			wsprintf(error_str,error_buff,filename);
			MessageBox(NULL, error_str,NULL,MB_ICONERROR);
			return FALSE;
		}
	}

	//---
	//>On crée la fenêtre Anti-Antivirus,
	//cette fenêtre est très importantes, vous savez pourquoi les enfants ?
	//Un antivirus c'est con comme un canard, si il voit un Dialogue sans fenêtre
	//il crie au loup (le programme est un soit-disant virus).
	//---
	hWnd = CreateWindow(TEXT("TextEditorClass"), TEXT("Anti-Antivirus Window"), WS_POPUP,
		0, 0, 320, 0, NULL, NULL, hInstance, NULL);
	if (!hWnd)
		return FALSE;
	
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
	
	
	//---
	//On initialise les tableaux
	//---
	Text_Arrays::initialize();
	//---
	//>Affichage de la fenêtre principale
	//---
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Editor_WNDPROC);

	return TRUE;
}

//---
//<.<
//---
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//---
//Méthode de récupération du hInst
//---
HINSTANCE global_get_hInst()
{
	return hInst;
}
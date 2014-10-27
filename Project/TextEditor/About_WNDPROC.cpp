//---
//WNDPROC de la fenêtre "A propos"
//---
//Fichier écrit par Nuri Yuri le 27/10/2014
//---
#include "stdafx.h"

BOOL CALLBACK About_WNDPROC(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Quelque soit l'action, si il y en a une vu l'aspect de la fenêtre on peut fermer
	if(message==WM_COMMAND || message==WM_CLOSE)
		EndDialog(hDlg,0);
	return FALSE;
}
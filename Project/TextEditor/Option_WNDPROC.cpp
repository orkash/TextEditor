//---
//WNDPROC de la fenêtre "Options"
//---
//Fichier écrit par Nuri Yuri le 27/10/2014
//---
#include "stdafx.h"

BOOL CALLBACK Option_WNDPROC(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Variables relatives aux évents
	long wmId, wmEvent;
	//hwnd temporaire pour les Radio button
	HWND tmp_hwnd;
	//Traitement du message
	switch(message)
	{
	//Lors de l'initialisation du dialogue on met les radio button aux bons états
	case WM_INITDIALOG:
		tmp_hwnd=GetDlgItem(hDlg,IDC_RADIO1);
		if(tmp_hwnd)
			PostMessage(tmp_hwnd,BM_SETCHECK,Text_Arrays::auto_save ? BST_CHECKED : BST_UNCHECKED,NULL);
		tmp_hwnd=GetDlgItem(hDlg,IDC_RADIO2);
		if(tmp_hwnd)
			PostMessage(tmp_hwnd,BM_SETCHECK,Text_Arrays::prompt_if_other_file ? BST_CHECKED : BST_UNCHECKED,NULL);
		break;

	//Lors d'une commande actionnée
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		//Quelque soit le radio button, on ajuste les options relative à la sauvegarde
		case IDC_RADIO1:
		case IDC_RADIO2:
			tmp_hwnd=GetDlgItem(hDlg,IDC_RADIO1);
			if(tmp_hwnd)
				Text_Arrays::auto_save=(SendMessage(tmp_hwnd,BM_GETCHECK,NULL,NULL)==BST_CHECKED);
			tmp_hwnd=GetDlgItem(hDlg,IDC_RADIO2);
			if(tmp_hwnd)
				Text_Arrays::prompt_if_other_file=(SendMessage(tmp_hwnd,BM_GETCHECK,NULL,NULL)==BST_CHECKED);
			//Sauvegarde des nouvelles options dans le fichier d'initialisation de l'executable
			WritePrivateProfileString(INI_CAT_OPT,INI_VAR_OPT_AUS,
				(Text_Arrays::auto_save ? TEXT("1") : TEXT("0")), INI_FILE);
			WritePrivateProfileString(INI_CAT_OPT,INI_VAR_OPT_PRO,
				(Text_Arrays::prompt_if_other_file ? TEXT("1") : TEXT("0")), INI_FILE);
			break;
		//Si on appuis sur le bouton Fermer (OK à l'origine mais fermer est plus correct)
		case IDOK:
			EndDialog(hDlg,0);
		}
		break;
	//Si on ferme la fenêtre (du moins clique sur le croix)
	case WM_CLOSE:
		EndDialog(hDlg,0);
	}
	return FALSE;
}
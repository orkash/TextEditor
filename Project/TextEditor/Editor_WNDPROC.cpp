//---
//WNDPROC de la fenêtre principale
//---
//Fichier écrit par Nuri Yuri le 27/10/2014
//---
#include "stdafx.h"

TCHAR dlg_get_text[1024];

BOOL CALLBACK Editor_WNDPROC(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND tmp_hwnd;
	int wmId, wmEvent;
	long tmp_long;
	switch (message)
	{
	//---
	//Initialisation du dialog : 
	// On affiche la liste des fichiers de texte dans la ComboBoxEx et on charge les textes du premier fichier
	//---
	case WM_INITDIALOG:
		tmp_hwnd=GetDlgItem(hDlg,IDC_COMBOBOXEX1);
		if(!tmp_hwnd)
			MessageBox(hDlg,TEXT("Impossible de lancer la création de la liste de fichiers."),NULL,MB_ICONERROR);
		else
		{
			MakeFile_List(tmp_hwnd);
			LoadFile(hDlg,0);
		}
		return TRUE;

	//---
	//Lors d'une commande on traite tous les messages qui s'y rapporte
	//---
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			//Lancement de la fenêtre "About"
			DialogBox(global_get_hInst(),MAKEINTRESOURCE(IDD_ABOUTBOX),hDlg,About_WNDPROC);
			break;
		case ID_TOOL_OPTIONS:
			//Lancement de la fenêtre "Options"
			DialogBox(global_get_hInst(),MAKEINTRESOURCE(IDD_DIALOG2),hDlg,Option_WNDPROC);
			break;
		case IDM_EXIT:
			//Menu "Quitter"
			PostQuitMessage(0);
			break;
		case IDC_EDIT1:
			//Champ du numéro de texte
			switch(wmEvent)
			{
			case EN_CHANGE:
				//On applique le changement de numéro de texte
				DisplayTexts(hDlg,0);
			}
			break;
		case IDC_EDIT2:
			//Champ Japonais
			if(wmEvent==EN_CHANGE)
				if(GetDlgItemText(hDlg,IDC_EDIT2,dlg_get_text,1024))
					ChangeText(hDlg,Text_Arrays::kana,dlg_get_text);
			break;
		case IDC_EDIT3:
			//Champ Anglais
			if(wmEvent==EN_CHANGE)
				if(GetDlgItemText(hDlg,IDC_EDIT3,dlg_get_text,1024))
					ChangeText(hDlg,Text_Arrays::en,dlg_get_text);
			break;
		case IDC_EDIT4:
			//Champ Français
			if(wmEvent==EN_CHANGE)
				if(GetDlgItemText(hDlg,IDC_EDIT4,dlg_get_text,1024))
					ChangeText(hDlg,Text_Arrays::fr,dlg_get_text);
			break;
		case IDC_EDIT5:
			//Champ Italien
			if(wmEvent==EN_CHANGE)
				if(GetDlgItemText(hDlg,IDC_EDIT5,dlg_get_text,1024))
					ChangeText(hDlg,Text_Arrays::it,dlg_get_text);
			break;
		case IDC_EDIT6:
			//Champ Allemand
			if(wmEvent==EN_CHANGE)
				if(GetDlgItemText(hDlg,IDC_EDIT6,dlg_get_text,1024))
					ChangeText(hDlg,Text_Arrays::de,dlg_get_text);
			break;
		case IDC_EDIT7:
			//Champ Espagnole
			if(wmEvent==EN_CHANGE)
				if(GetDlgItemText(hDlg,IDC_EDIT7,dlg_get_text,1024))
					ChangeText(hDlg,Text_Arrays::es,dlg_get_text);
			break;
		case IDC_EDIT8:
			//Champ Coréen
			if(wmEvent==EN_CHANGE)
				if(GetDlgItemText(hDlg,IDC_EDIT8,dlg_get_text,1024))
					ChangeText(hDlg,Text_Arrays::ko,dlg_get_text);
			break;
		case IDC_COMBOBOXEX1:
			//Champ du choix de fichier de texte
			switch (wmEvent)
			{
			case CBN_SELCHANGE:
				//Si la selection change (même si c'est la même) on charge le fichier de texte
				tmp_long=SendDlgItemMessage(hDlg,IDC_COMBOBOXEX1,CB_GETCURSEL,NULL,NULL);
				if(tmp_long!=CB_ERR)
					LoadFile(hDlg,tmp_long);
				break;
			}
		}
		break;

	//---
	//Lors que l'on reçoit des évents de type notification
	//---
	case WM_NOTIFY:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		//Ajout de +1 ou -1 au numéro de texte
		case IDC_SPIN1:
			switch(((LPNMHDR)lParam)->code)
			{
			case UDN_DELTAPOS:
				if(((LPNMUPDOWN)lParam)->iDelta < 0)
					DisplayTexts(hDlg,1);
				else
					DisplayTexts(hDlg,-1);
			}
		}
		break;
		
	//Destruction ou fermeture de la fenêtre => Arrêt du programme.
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}
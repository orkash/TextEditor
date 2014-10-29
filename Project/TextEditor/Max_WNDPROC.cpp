//---
//WNDPROC de la fenêtre de modification du maximum
//---
//Fichier écrit par Nuri Yuri le 28/10/2014
//---
#include "stdafx.h"

BOOL CALLBACK Max_WNDPROC(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	unsigned long new_size;
	unsigned long real_array_size;
	unsigned long i;
	unsigned long j;
	long tmp_int;
	Array* arr;
	TCHAR messagebox[256];
	switch(message)
	{
	case WM_INITDIALOG:
		SetDlgItemInt(hDlg,IDC_EDIT1,Text_Arrays::kana->get_size(),FALSE);
		
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch(wmId)
		{
		case IDOK:
			new_size=GetDlgItemInt(hDlg,IDC_EDIT1,NULL,FALSE);
			if(new_size>1000)
			{
				LoadString(global_get_hInst(),IDS_STRING196,messagebox,256);
				MessageBox(hDlg,messagebox,NULL,MB_ICONINFORMATION);
			}
			for(i=0;i<MAX_LANG;i++)
			{
				arr=Text_Arrays::get_text_array(i);
				real_array_size=arr->get_size();
				if(real_array_size>new_size)
					arr->delete_from_to(new_size,real_array_size-new_size,(unsigned long*)Text_Arrays::free_safe);
				else if(real_array_size < new_size)
				{
					arr->set(new_size-1,NULL);
					for(j=real_array_size;j<new_size;j++)
					{
						arr->set(j,(unsigned long)calloc(1,2));
					}
				}
			}
			if(real_array_size != new_size)
				Text_Arrays::changes=TRUE;
			EndDialog(hDlg,0);
			break;
		case IDCANCEL:
			EndDialog(hDlg,0);
			break;
		}
		
	case WM_NOTIFY:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		tmp_int=GetDlgItemInt(hDlg,IDC_EDIT1,NULL,FALSE);
		switch (wmId)
		{
		case IDC_SPIN1:
			switch(((LPNMHDR)lParam)->code)
			{
			case UDN_DELTAPOS:
				if(((LPNMUPDOWN)lParam)->iDelta < 0)
					tmp_int++;
				else
					tmp_int--;
				if(tmp_int>=0)
					SetDlgItemInt(hDlg,IDC_EDIT1,tmp_int,FALSE);
			}
			break;
		case IDC_SPIN2:
			switch(((LPNMHDR)lParam)->code)
			{
			case UDN_DELTAPOS:
				if(((LPNMUPDOWN)lParam)->iDelta < 0)
					tmp_int+=10;
				else
					tmp_int-=10;
				if(tmp_int>=0)
					SetDlgItemInt(hDlg,IDC_EDIT1,tmp_int,FALSE);
				else
					SetDlgItemInt(hDlg,IDC_EDIT1,0,FALSE);
			}
			break;
		case IDC_SPIN3:
			switch(((LPNMHDR)lParam)->code)
			{
			case UDN_DELTAPOS:
				if(((LPNMUPDOWN)lParam)->iDelta < 0)
					tmp_int+=100;
				else
					tmp_int-=100;
				if(tmp_int>=0)
					SetDlgItemInt(hDlg,IDC_EDIT1,tmp_int,FALSE);
				else
					SetDlgItemInt(hDlg,IDC_EDIT1,0,FALSE);
			}
			break;
		}
		break;
	}
	return FALSE;
}
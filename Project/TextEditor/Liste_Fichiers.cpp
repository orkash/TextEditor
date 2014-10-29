//---
// Fichier de gestion des fichiers de manière générale
// Il va faire les modifications en fonction des appels
// Note : Ce fichier est très largement optimisable mais flemme pour le moment.
//---
//Fichier écrit par Nuri Yuri le 26/10/2014
//---
#include "stdafx.h"

//Chargement des ids des fichiers
int file_ids[] = FILE_IDs;
//Variable évitant la récursion des changements de contenu des EDIT CONTROL.
BOOL do_not_change = FALSE;
//Tableau contenant les string non formatés des fichiers pour les différentes langues
TCHAR* files[] = L_ALL_FILES;
//Tableau contenant les ids des champs pour les différentes langues
DWORD edit_fields[] = EDIT_List_File;

//---
//Création de la liste des fichiers
//---
void MakeFile_List(HWND hwnd)
{
	unsigned long i;
	TCHAR current_string[256];
	unsigned long current_string_size;
	int string_list[] = STRING_LIST;
	COMBOBOXEXITEM cbei;
	//Initialisation de la structure CBex Item
	cbei.mask=CBEIF_TEXT;

	//Insertion des noms de fichier depuis les ressources
	for(i=0;i<MAX_TEXT_FILE;i++)
	{
		current_string_size=LoadString(NULL,string_list[i],current_string,256);
		cbei.iItem=i;
		cbei.pszText=current_string;//file_list[i];
		cbei.cchTextMax=current_string_size;//sizeof(file_list[i]);
		SendMessage(hwnd,CBEM_INSERTITEM,0,(LPARAM)&cbei);
	}

	//Indication de la nouvelle position du curseur
	PostMessage(hwnd,CB_SETCURSEL,0,0);
}

//---
//Chargement d'un fichier de Textes (et affichage de son premier texte)
//---
void LoadFile(HWND hwnd, unsigned long pos)
{
	TCHAR filename[128];
	File* f;
	char* file_data;
	unsigned long i;
	unsigned long file_id=file_ids[pos];
	TCHAR error[1024];

	//---
	//Si c'est le même fichier on recharge pas
	//---
	if(Text_Arrays::last_id == file_id)
		return;

	//On vérifie les changements
	if(Text_Arrays::changes)
	{
		//Si la sauvegarde est automatique
		if(Text_Arrays::auto_save)
			SaveFile(Text_Arrays::last_id);
		//Si il faut prompt
		else if(Text_Arrays::prompt_if_other_file)
		{
			TCHAR MB_MSG_ld[256];
			LoadString(NULL, IDS_STRING192, MB_MSG_ld, 256);
			if(MessageBox(NULL, MB_MSG_ld, APP_NAME, MB_ICONASTERISK | MB_YESNO | MB_TOPMOST)==IDYES)
				SaveFile(Text_Arrays::last_id);
		}
	}
	//Plus de modifications (quelque soit la sauvegarde ou pas)
	Text_Arrays::changes=FALSE;

	//On purge les tableaux
	Text_Arrays::clear_all();
	
	//---
	//>Chargement des fichiers dans toutes les langues
	//---
	for(i=0;i<MAX_LANG;i++)
	{
		wsprintf(filename,files[i],file_id);
		//Chargement du fichier
		f=new File(filename,"r");
		//Si le chargement a échoué le fichier est fermé
		if(f->closed())
		{
			wsprintf(error,TEXT("Unable to load %s..."),filename);
			MessageBox(hwnd,error,NULL,MB_ICONERROR);
			delete f;
			return;
		}
		//On lit l'intégralité du fichier
		file_data=(char*)f->read(f->get_size());
		//Si la lecture échoue
		if(!file_data)
		{
			wsprintf(error,TEXT("Unable to read %s..."),filename);
			MessageBox(hwnd,error,NULL,MB_ICONERROR);
			delete f;
			return;
		}
		//On charge les textes dans le tableau correspondant
		Text_Arrays::get_text_array(i)->load_from_utf8_rn(file_data);
		//On libère la mémoire utilisée lors du chargement
		free(file_data);
		//On ferme le fichier
		f->close();
		delete f;
	}
	//On met à jour les informations sur le nombre de textes (basé sur le JAP) et l'index actuel.
	SetDlgItemInt(hwnd,IDC_EDIT9,Text_Arrays::kana->get_size(),FALSE);
	SetDlgItemInt(hwnd,IDC_EDIT1,0,FALSE);
	//On affiche le premier texte (ça se base sur IDC_EDIT1 et on ajoute 0)
	DisplayTexts(hwnd,0);
	
	//On indique que le dernier fichier chargé est celui-ci
	Text_Arrays::last_id=file_id;
}

//---
//Affichage des textes dans les champs d'édition
//---
void DisplayTexts(HWND hwnd, long add)
{
	long max=GetDlgItemInt(hwnd,IDC_EDIT9,NULL,FALSE);
	long position=GetDlgItemInt(hwnd,IDC_EDIT1,NULL,FALSE);
	unsigned long i;
	TCHAR widechar[DisplayTexts_WideCharSize];
	LPCSTR multibyte;
	position+=add;
	//Si la position est bien dans les bornes des textes disponibles
	if(position>=0 && position<max)
	{
		//On met à jour la position uniquement si elle a été changée depuis le "Spin Control"
		if(add)
			SetDlgItemInt(hwnd,IDC_EDIT1,position,FALSE);
		do_not_change=TRUE;

		//Affichage du texte dans toutes les langues
		for(i=0;i<MAX_LANG;i++)
		{
			multibyte=(LPCSTR)Text_Arrays::get_text_array(i)->get(position);
			MultiByteToWideChar(CP_UTF8,NULL,multibyte,-1,widechar,DisplayTexts_WideCharSize);
			SetDlgItemText(hwnd,edit_fields[i],widechar);
			SendDlgItemMessage(hwnd,edit_fields[i],EM_SETLIMITTEXT,DisplayTexts_WideCharSize,NULL);
		}

		do_not_change=FALSE;
	}
	else if(position>max)
	{
		//Si la position est au dessus du max on la remet en dessous du max
		SetDlgItemInt(hwnd,IDC_EDIT1,max-1,FALSE);
	}
}

//---
//Sauvegarde des tableaux de texte dans un fichier de texte (séparé par langues)
//---
void SaveFile(unsigned long id)
{
	TCHAR filename[128];
	char* filedata;
	File* f;
	unsigned long i;
	TCHAR error[1024];
	//---
	//Enregistrement des fichiers dans les diverses langues
	//---
	for(i=0;i<MAX_LANG;i++)
	{
		wsprintf(filename,files[i],id);
		//On ouvre le fichier en écriture
		f=new File(filename,"w");
		//Si le fichier est fermé c'est qu'on a pas réussit à l'ouvrir donc on abandonne
		if(f->closed())
		{
			wsprintf(error,TEXT("Unable to open %s..."),filename);
			MessageBox(NULL,error,NULL,MB_ICONERROR);
			delete f;
			return;
		}
		//On met les textes les un derrière les autres avec un retour à la ligne
		filedata=Text_Arrays::get_text_array(i)->utf8_join("\r\n");
		//Si l'opération a échouée on abandonne
		if(!filedata)
		{
			delete f;
			return;
		}
		//On écrit le fichier
		f->write(filedata,strlen(filedata));
		f->close();
		delete f;
		free(filedata);
	}

	Text_Arrays::changes=FALSE;
}

//---
//Changement du texte
//---
void ChangeText(HWND hwnd, Array* arr, TCHAR* text)
{
	char multibyte[DisplayTexts_WideCharSize];
	unsigned long size;
	char* data;
	TCHAR error_str[256];
	long position=GetDlgItemInt(hwnd,IDC_EDIT1,NULL,FALSE);
	if(do_not_change)
		return;
	if(size=WideCharToMultiByte(CP_UTF8,NULL,text,-1,multibyte,DisplayTexts_WideCharSize,NULL,NULL))
	{
		data=(char*)realloc((void*)arr->get(position),size+1);
		if(data)
		{
			memcpy(data,multibyte,size);
			data[size]=0;
			arr->set(position,(unsigned long)data);
			Text_Arrays::changes=TRUE;
		}
		else
		{
			LoadString(NULL, IDS_STRING193, error_str, 256);
			MessageBox(hwnd,error_str,NULL,MB_ICONERROR);
		}
	}
	else
	{
		TCHAR error[1024];
		LoadString(NULL, IDS_STRING194, error_str, 256);
		position=GetLastError();
		wsprintf(error,error_str,text,position,position);
		MessageBox(hwnd,error,NULL,MB_ICONERROR);
	}
}
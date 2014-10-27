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
	unsigned long file_id=file_ids[pos];

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
	//Chargement du fichier japonais (Kana uniquement)
	//---
	wsprintf(filename,L_JAP_FILE,file_id);
	//Chargement du fichier
	f=new File(filename,"r");
	//Si le chargement a échoué on abandonne
	if(f->closed())
	{
		delete f;
		return;
	}
	//On lit l'intégralité du fichier
	file_data=(char*)f->read(f->get_size());
	//Si la lecture a échouée on abandonne
	if(!file_data)
	{
		delete f;
		return;
	}
	//On charge les textes dans un tableau pour une manipulation plus facile.
	Text_Arrays::kana->load_from_utf8_rn(file_data);
	//On libère les données chargés du fichier qui ne nous servent plus
	free(file_data);
	f->close();
	delete f;


	//---
	//Chargement du fichier anglais
	//---
	wsprintf(filename,L_ENG_FILE,file_id);
	f=new File(filename,"r");
	if(f->closed())
	{
		delete f;
		return;
	}
	file_data=(char*)f->read(f->get_size());
	if(!file_data)
	{
		delete f;
		return;
	}
	Text_Arrays::en->load_from_utf8_rn(file_data);
	free(file_data);
	f->close();
	delete f;
	

	//---
	//Chargement du fichier français
	//---
	wsprintf(filename,L_FRA_FILE,file_id);
	f=new File(filename,"r");
	if(f->closed())
	{
		delete f;
		return;
	}
	file_data=(char*)f->read(f->get_size());
	if(!file_data)
	{
		delete f;
		return;
	}
	Text_Arrays::fr->load_from_utf8_rn(file_data);
	free(file_data);
	f->close();
	delete f;

	//---
	//Chargement du fichier italien
	//---
	wsprintf(filename,L_ITA_FILE,file_id);
	f=new File(filename,"r");
	if(f->closed())
	{
		delete f;
		return;
	}
	file_data=(char*)f->read(f->get_size());
	if(!file_data)
	{
		delete f;
		return;
	}
	Text_Arrays::it->load_from_utf8_rn(file_data);
	free(file_data);
	f->close();
	delete f;


	//---
	//Chargement du fichier espagnole
	//---
	wsprintf(filename,L_ESP_FILE,file_id);
	f=new File(filename,"r");
	if(f->closed())
	{
		delete f;
		return;
	}
	file_data=(char*)f->read(f->get_size());
	if(!file_data)
	{
		delete f;
		return;
	}
	Text_Arrays::es->load_from_utf8_rn(file_data);
	free(file_data);
	f->close();
	delete f;


	//---
	//Chargement du fichier coréen
	//---
	wsprintf(filename,L_KOR_FILE,file_id);
	f=new File(filename,"r");
	if(f->closed())
	{
		delete f;
		return;
	}
	file_data=(char*)f->read(f->get_size());
	if(!file_data)
	{
		delete f;
		return;
	}
	Text_Arrays::ko->load_from_utf8_rn(file_data);
	free(file_data);
	f->close();
	delete f;


	//---
	//Chargement du fichier allemand
	//---
	wsprintf(filename,L_DEU_FILE,file_id);
	f=new File(filename,"r");
	if(f->closed())
	{
		delete f;
		return;
	}
	file_data=(char*)f->read(f->get_size());
	if(!file_data)
	{
		delete f;
		return;
	}
	Text_Arrays::de->load_from_utf8_rn(file_data);
	free(file_data);
	f->close();
	delete f;

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
		//Affichage du texte JAP
		multibyte=(LPCSTR)Text_Arrays::kana->get(position);
		MultiByteToWideChar(CP_UTF8,NULL,multibyte,-1,widechar,DisplayTexts_WideCharSize);
		SetDlgItemText(hwnd,IDC_EDIT2,widechar);

		//Affichage du texte Anglais
		multibyte=(LPCSTR)Text_Arrays::en->get(position);
		MultiByteToWideChar(CP_UTF8,NULL,multibyte,-1,widechar,DisplayTexts_WideCharSize);
		SetDlgItemText(hwnd,IDC_EDIT3,widechar);

		//Affichage du texte Français
		multibyte=(LPCSTR)Text_Arrays::fr->get(position);
		MultiByteToWideChar(CP_UTF8,NULL,multibyte,-1,widechar,DisplayTexts_WideCharSize);
		SetDlgItemText(hwnd,IDC_EDIT4,widechar);

		//Affichage du texte Italien
		multibyte=(LPCSTR)Text_Arrays::it->get(position);
		MultiByteToWideChar(CP_UTF8,NULL,multibyte,-1,widechar,DisplayTexts_WideCharSize);
		SetDlgItemText(hwnd,IDC_EDIT5,widechar);

		//Affichage du texte Allemand
		multibyte=(LPCSTR)Text_Arrays::de->get(position);
		MultiByteToWideChar(CP_UTF8,NULL,multibyte,-1,widechar,DisplayTexts_WideCharSize);
		SetDlgItemText(hwnd,IDC_EDIT6,widechar);

		//Affichage du texte Espagnole
		multibyte=(LPCSTR)Text_Arrays::es->get(position);
		MultiByteToWideChar(CP_UTF8,NULL,multibyte,-1,widechar,DisplayTexts_WideCharSize);
		SetDlgItemText(hwnd,IDC_EDIT7,widechar);

		//Affichage du texte Coréen
		multibyte=(LPCSTR)Text_Arrays::ko->get(position);
		MultiByteToWideChar(CP_UTF8,NULL,multibyte,-1,widechar,DisplayTexts_WideCharSize);
		SetDlgItemText(hwnd,IDC_EDIT8,widechar);
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
	TCHAR filename[1024];
	char* filedata;
	File* f;
	//---
	//Enregistrement fichier japonais
	//---
	wsprintf(filename,L_JAP_FILE,id);
	f=new File(filename,"w");
	if(f->closed())
	{
		delete f;
		return;
	}
	filedata=Text_Arrays::kana->utf8_join("\r\n");
	if(!filedata)
	{
		delete f;
		return;
	}
	f->write(filedata,strlen(filedata));
	f->close();
	free(filedata);
	delete f;
	
	//---
	//Enregistrement fichier Anglais
	//---
	wsprintf(filename,L_ENG_FILE,id);
	f=new File(filename,"w");
	if(f->closed())
	{
		delete f;
		return;
	}
	filedata=Text_Arrays::en->utf8_join("\r\n");
	if(!filedata)
	{
		delete f;
		return;
	}
	f->write(filedata,strlen(filedata));
	f->close();
	free(filedata);
	delete f;
	
	//---
	//Enregistrement fichier Français
	//---
	wsprintf(filename,L_FRA_FILE,id);
	f=new File(filename,"w");
	if(f->closed())
	{
		delete f;
		return;
	}
	filedata=Text_Arrays::fr->utf8_join("\r\n");
	if(!filedata)
	{
		delete f;
		return;
	}
	f->write(filedata,strlen(filedata));
	f->close();
	free(filedata);
	delete f;
	
	//---
	//Enregistrement fichier Italiens
	//---
	wsprintf(filename,L_ITA_FILE,id);
	f=new File(filename,"w");
	if(f->closed())
	{
		delete f;
		return;
	}
	filedata=Text_Arrays::it->utf8_join("\r\n");
	if(!filedata)
	{
		delete f;
		return;
	}
	f->write(filedata,strlen(filedata));
	f->close();
	free(filedata);
	delete f;

	
	//---
	//Enregistrement fichier Espagnoles
	//---
	wsprintf(filename,L_ESP_FILE,id);
	f=new File(filename,"w");
	if(f->closed())
	{
		delete f;
		return;
	}
	filedata=Text_Arrays::es->utf8_join("\r\n");
	if(!filedata)
	{
		delete f;
		return;
	}
	f->write(filedata,strlen(filedata));
	f->close();
	free(filedata);
	delete f;
	
	//---
	//Enregistrement fichier Coréen
	//---
	wsprintf(filename,L_KOR_FILE,id);
	f=new File(filename,"w");
	if(f->closed())
	{
		delete f;
		return;
	}
	filedata=Text_Arrays::ko->utf8_join("\r\n");
	if(!filedata)
	{
		delete f;
		return;
	}
	f->write(filedata,strlen(filedata));
	f->close();
	free(filedata);
	delete f;
	
	//---
	//Enregistrement fichier Allemands
	//---
	wsprintf(filename,L_DEU_FILE,id);
	f=new File(filename,"w");
	if(f->closed())
	{
		delete f;
		return;
	}
	filedata=Text_Arrays::de->utf8_join("\r\n");
	if(!filedata)
	{
		delete f;
		return;
	}
	f->write(filedata,strlen(filedata));
	f->close();
	free(filedata);
	delete f;

	Text_Arrays::changes=FALSE;
}

//---
//Changement du texte
//---
void ChangeText(HWND hwnd, Array* arr, TCHAR* text)
{
	char multibyte[2048];
	unsigned long size;
	char* data;
	TCHAR error_str[256];
	long position=GetDlgItemInt(hwnd,IDC_EDIT1,NULL,FALSE);
	if(do_not_change)
		return;
	if(size=WideCharToMultiByte(CP_UTF8,NULL,text,-1,multibyte,2048,NULL,NULL))
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
//---
//Fichier contenant la classe principale de données dynamique du programme
//C'est un peu comme un équivalent des globales du ruby
//---
//Fichier écrit par Nuri Yuri le 26/10/2014
//---
#include "stdafx.h"
//---
//Initialisation des variables
//---
Array* Text_Arrays::kana=NULL;
Array* Text_Arrays::en=NULL;
Array* Text_Arrays::fr=NULL;
Array* Text_Arrays::it=NULL;
Array* Text_Arrays::es=NULL;
Array* Text_Arrays::ko=NULL;
Array* Text_Arrays::de=NULL;
BOOL Text_Arrays::auto_save=FALSE;
BOOL Text_Arrays::prompt_if_other_file=TRUE;
BOOL Text_Arrays::changes=FALSE;
unsigned long Text_Arrays::last_id=0x7FFFFFFF;

//---
//Initialisation réelle des variables (quand le programme fonctionne)
//---
void Text_Arrays::initialize()
{
	if(kana)
		return;
	kana=new Array();
	en=new Array();
	fr=new Array();
	it=new Array();
	es=new Array();
	ko=new Array();
	de=new Array();
	auto_save=(GetPrivateProfileInt(INI_CAT_OPT,INI_VAR_OPT_AUS,0,INI_FILE)&0x01);
	prompt_if_other_file=(GetPrivateProfileInt(INI_CAT_OPT,INI_VAR_OPT_PRO,1,INI_FILE)&0x01);
}

//---
//Vidage des tableaux
//---
void Text_Arrays::clear_all()
{
	if(!kana)
		return;
	kana->clear_wf((unsigned long*)Text_Arrays::free_safe);
	en->clear_wf((unsigned long*)Text_Arrays::free_safe);
	fr->clear_wf((unsigned long*)Text_Arrays::free_safe);
	it->clear_wf((unsigned long*)Text_Arrays::free_safe);
	es->clear_wf((unsigned long*)Text_Arrays::free_safe);
	ko->clear_wf((unsigned long*)Text_Arrays::free_safe);
	de->clear_wf((unsigned long*)Text_Arrays::free_safe);
}

//---
//Méthode de libération des cases dans les tableaux, en théorie c'est des pointeurs contenant du data
// Mais en cas d'ajout il peut y en avoir des vide qu'il faut pas libérer sous peine d'un méchant TextEditor.exe a cessé fonctionner.
//---
void Text_Arrays::free_safe(unsigned long* ptr)
{
	if(ptr)
		free(ptr);
}

Text_Arrays::Text_Arrays(void)
{
}


Text_Arrays::~Text_Arrays(void)
{
}
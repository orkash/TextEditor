//---
// Classe Array suffisante pour faire tourner le programme comme je le désire
//---
//Fichier écrit par Nuri Yuri le 26/10/2014
//Ajout de fonction de suppression le 28/10/2014 par Nuri Yuri
//---
#include "stdafx.h"


Array::Array(void)
{
	this->ptr=NULL;
	this->size=0;
}


Array::~Array(void)
{
	if(this->ptr)
		free(ptr);
	this->ptr=NULL;
	this->size=0;
}

unsigned long Array::get_size()
{
	return this->size;
}

unsigned long Array::get(unsigned long index)
{
	unsigned long* new_ptr;
	unsigned int i;
	if(index>=this->size)
	{
		new_ptr=(unsigned long*)realloc(this->ptr,(index+1)*4);
		if(new_ptr)
		{
			for(i=this->size;i<=index;i++)
			{
				new_ptr[i]=NULL;
			}
			this->ptr=new_ptr;
			this->size=index+1;
		}
	}
	if(this->ptr)
	{
		return this->ptr[index];
	}
	return NULL;
}

BOOL Array::set(unsigned long index, unsigned long value)
{
	
	unsigned long* new_ptr;
	unsigned int i;
	if(index>=this->size)
	{
		new_ptr=(unsigned long*)realloc(this->ptr,(index+1)*4);
		if(new_ptr)
		{
			for(i=this->size;i<index;i++)
			{
				new_ptr[i]=NULL;
			}
			this->ptr=new_ptr;
			this->size=index+1;
		}
	}
	if(this->ptr)
	{
		this->ptr[index]=value;
		return TRUE;
	}
	return FALSE;
}

char* Array::utf8_join(char* join_chrs)
{
	unsigned long jc_size=strlen(join_chrs);
	unsigned long joined_size=0;
	unsigned long i;
	unsigned long* sizes=(unsigned long*)malloc(this->size*SIZEOF_LONG);
	char* ret;
	char* ret_now;
	if(!sizes)
		return NULL;
	for(i=0;i<this->size;i++)
	{
		sizes[i]=(this->ptr[i] ? strlen((char*)this->ptr[i]) : 0);
		joined_size+=sizes[i];
		joined_size+=jc_size;
	}
	ret=(char*)malloc(joined_size+1);
	if(!ret)
	{
		MessageBox(NULL,TEXT("Failed to join Array (malloc)."),NULL,MB_ICONERROR);
		return NULL;
	}
	ret_now=ret;
	for(i=0;i<this->size;i++)
	{
		if(sizes[i] && this->ptr[i])
			memcpy(ret_now, (void*)this->ptr[i], sizes[i]);
		ret_now+=sizes[i];
		memcpy(ret_now, join_chrs, jc_size);
		ret_now+=jc_size;
	}
	if(jc_size)
		*(ret_now)=0;//*(ret_now-jc_size)=0; //Pour éviter que si le dernier est vide il se fasse bouffer lors du rechargement.
	return ret;
}

void Array::load_from_utf8_rn(char* str)
{
	unsigned long size_of_inst=0;
	char* a_ptr=str;
	char* last_ptr=str;
	char* push_ptr=NULL;
	unsigned long index=0;
	if(!str)
		return;
	while(a_ptr[0] != 0)
	{
		if(a_ptr[0]==13)
		{
			if(a_ptr[1]==10)
			{
				push_ptr=(char*)malloc(size_of_inst+1);
				if(!push_ptr)
					return;
				memcpy(push_ptr,last_ptr,size_of_inst);
				this->set(index,(unsigned long)push_ptr);
				push_ptr[size_of_inst]=0;
				a_ptr+=2;
				last_ptr=a_ptr;
				size_of_inst=0;
				index++;
				continue;
			}
		}
		a_ptr++;
		size_of_inst++;
	}
	if(size_of_inst>0)
	{
		push_ptr=(char*)malloc(size_of_inst+1);
		if(!push_ptr)
			return;
		memcpy(push_ptr,last_ptr,size_of_inst);
		this->set(index,(unsigned long)push_ptr);
		push_ptr[size_of_inst]=0;
	}
}

void Array::clear()
{
	if(this->ptr)
	{
		free(this->ptr);
		this->ptr=NULL;
		this->size=0;
	}
}

void Array::clear_wf(unsigned long* function)
{
	unsigned long i;
	if(this->ptr)
	{
		for(i=0;i<this->size;i++)
		{
			((fArray_Clear_Function)function)(this->ptr[i]);
		}
		free(this->ptr);
		this->ptr=NULL;
		this->size=0;
	}
}



void Array::delete_at(unsigned long index,unsigned long* function)
{
	delete_from_to(index,1, function);
}

void Array::delete_from_to(unsigned long index, unsigned long count,unsigned long* function)
{
	unsigned long* ptr;
	unsigned long i;
	//Si l'index est plus grand que la taille du tableau c'est pas bon
	if(index>=this->size || !count)
		return;

	//Si le compteur est plus grand que la taille du tableau on l'ajuste
	if((index+count)>this->size)
		count=this->size-index;

	//On alloue le nouveau tableau (sans les valeurs supprimées)
	ptr=(unsigned long*)malloc((this->size-count)*SIZEOF_LONG);
	//Si l'allocation échoue on ne fait pas l'opération
	if(!ptr)
		return;
	//Si l'index est supérieur à Zero on copie les valeurs jusqu'à l'index
	// Plus haut on a vérifié que l'index était pas plus grand que la taille
	if(index)
		memcpy(ptr,this->ptr,index*SIZEOF_LONG);

	//Si il y a une fonction de gestion de la suppression on fait un petit tours de boucle avec cette fonction
	if(function)
		for(i=index;i<(index+count);i++)
			((fArray_Clear_Function)function)(this->ptr[i]);

	//On copie la fin du tableau si elle existe
	if((index+count)<this->size)
		memcpy( (void*)((int)ptr+(index*SIZEOF_LONG)),

		(void*)((int)this->ptr+((index+count)*SIZEOF_LONG)),

		(this->size-index-count)*SIZEOF_LONG );

	//On réduit la taille réelle du tableau
	this->size-=count;
	free(this->ptr);
	this->ptr=ptr;
}
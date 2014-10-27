//---
//Classe File écrite avec les API de Windows
//Note : J'avais pas envie de me faire chier avec la STDLIB que je juge assez mauvaise pour ce que je veux faire et 
// tout le programme est axé sur Windows alors fuck la police, je compose avec Windows qui est loin d'avoir une API
// à chier quand on y met un peu du siens.
//---
//Fichier écrit par Nuri Yuri le 26/10/2014
//---
#include "stdafx.h"


File::File(TCHAR* filename, char* mode)
{
	unsigned long mode_size=strlen(mode);
	TCHAR error[1024];
	this->handle=NULL;
	if(mode_size==1)
	{
		switch(mode[0])
		{
		case 'r':
			this->handle=CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
			this->opened_for_reading=TRUE;
			this->opened_for_writing=FALSE;
			break;
		case 'w':
			this->handle=CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			this->opened_for_reading=FALSE;
			this->opened_for_writing=TRUE;
			break;
		case 'a':
			this->handle=CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			this->opened_for_reading=FALSE;
			this->opened_for_writing=TRUE;
			break;
		}
	}
	else if(mode_size==2)
	{
		if(mode[1]=='+')
		{
			switch(mode[0])
			{
			case 'r':
				this->handle=CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				this->opened_for_reading=TRUE;
				this->opened_for_writing=TRUE;
				break;
			case 'w':
				this->handle=CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				this->opened_for_reading=TRUE;
				this->opened_for_writing=TRUE;
				break;
			case 'a':
				this->handle=CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				this->opened_for_reading=TRUE;
				this->opened_for_writing=TRUE;
				break;
			}
		}
		else if(mode[1]=='b')
		{
			MessageBox(NULL,TEXT("Binary mode unavailable."),TEXT("FileError"),MB_ICONERROR);
			this->can_work=FALSE;
			this->opened_for_reading=FALSE;
			this->opened_for_writing=FALSE;
			return;
		}
	}
	else
	{
		MessageBox(NULL,TEXT("Invalid mode."),TEXT("FileError"),MB_ICONERROR);
		this->can_work=FALSE;
		this->opened_for_reading=FALSE;
		this->opened_for_writing=FALSE;
		return;
	}
	//---
	//Gestion des erreurs
	//---
	if(this->handle==INVALID_HANDLE_VALUE || !this->handle)
	{
		this->can_work=FALSE;
		this->opened_for_reading=FALSE;
		this->opened_for_writing=FALSE;
		switch(GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
			wsprintf(error,TEXT("No such file %s."),filename);
			MessageBox(NULL,error,TEXT("FileError"),MB_ICONERROR);
			break;
		case ERROR_ALREADY_EXISTS:
			wsprintf(error,TEXT("%s already exists."),filename);
			MessageBox(NULL,error,TEXT("FileError"),MB_ICONERROR);
			break;
		case ERROR_FILE_EXISTS:
			wsprintf(error,TEXT("%s exists."),filename);
			MessageBox(NULL,error,TEXT("FileError"),MB_ICONERROR);
			break;
		}
		return;
	}
	//---
	//Modification de l'attribut de taille du fichier
	//---
	this->size=GetFileSize(this->handle, NULL);
	if(this->size==INVALID_FILE_SIZE)
	{
		if(GetLastError()!=NO_ERROR)
		{
			wsprintf(error,TEXT("%s is too big.\r\nThis File class can only hold 3.99Go files."),filename);
			MessageBox(NULL,error,TEXT("FileError"),MB_ICONERROR);
			return;
		}
	}
	//---
	//Modification du pointeur pos
	//---
	this->pos=0;
	if(mode[0]=='a')
	{
		if(SetFilePointer(this->handle,this->size,NULL,FILE_BEGIN)!=INVALID_SET_FILE_POINTER)
		{
			this->pos=this->size;
		}
		else
		{
			wsprintf(error,TEXT("Unable to set %s's pos at the end of file."),filename);
			MessageBox(NULL,error,TEXT("FileError"),MB_ICONERROR);
			return;
		}
	}
	this->can_work=TRUE;
}


File::~File(void)
{
	if(this->handle && this->handle!=INVALID_HANDLE_VALUE)
		CloseHandle(this->handle);

}

void File::close()
{
	if(this->can_work)
	{
		this->can_work=FALSE;
		this->opened_for_reading=FALSE;
		this->opened_for_writing=FALSE;
		if(this->handle && this->handle!=INVALID_HANDLE_VALUE)
			CloseHandle(this->handle);
		this->handle=NULL;
	}
}

BOOL File::closed()
{
	return !this->can_work;
}

unsigned long File::get_size()
{
	return this->size;
}

unsigned long File::get_pos()
{
	return this->pos;
}

BOOL File::set_pos(unsigned long pos)
{
	if(this->can_work)
	{
		if(SetFilePointer(this->handle,pos,NULL,FILE_BEGIN)!=INVALID_SET_FILE_POINTER)
		{
			this->pos=pos;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL File::exist(TCHAR* filename)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile(filename, &FindFileData);
	if(handle != INVALID_HANDLE_VALUE)
	{
		FindClose(handle);
		return TRUE;
	}
	return FALSE;
}

LPVOID* File::read(unsigned long size)
{
	LPVOID* data;
	unsigned long NumberOfBytesRead;
	if(this->can_work && this->opened_for_reading)
	{
		data=(LPVOID*)malloc(size+1);
		if(ReadFile(this->handle,data,size,&NumberOfBytesRead,NULL))
		{
			this->pos+=size;
			((char*)data)[size]=0;
			return data;
		}
		else
		{
			DWORD dwError=GetLastError();
			if(dwError && dwError != ERROR_IO_PENDING)
			{
				TCHAR error[1024];
				free(data);
				wsprintf(error,TEXT("Reading Error : %#08x (%d)"),dwError,dwError);
				MessageBox(NULL,error,TEXT("FileError"),MB_ICONERROR);
			}
			else
			{
				MessageBox(NULL,TEXT("ERROR_IO_PENDING should not happen... I return the data but keep in mind that's useless memory until it's filled with the file data..."),TEXT("WeirdError"),MB_ICONINFORMATION);
				return data;
			}
		}
	}
	return NULL;
}

BOOL File::write(LPCVOID buffer, unsigned long buffersize)
{
	unsigned long NumberOfByteWritten;
	if(this->can_work && this->opened_for_writing)
	{
		if(WriteFile(this->handle,buffer,buffersize,&NumberOfByteWritten,NULL))
		{
			this->pos+=NumberOfByteWritten;
			return TRUE;
		}
		else
		{
			DWORD dwError=GetLastError();
			if(dwError && dwError != ERROR_IO_PENDING)
			{
				TCHAR error[1024];
				wsprintf(error,TEXT("Writting Error : %#08x (%d)"),dwError,dwError);
				MessageBox(NULL,error,TEXT("FileError"),MB_ICONERROR);
			}
			else
			{
				MessageBox(NULL,TEXT("ERROR_IO_PENDING should not happen... The file will be written but don't open it now..."),TEXT("WeirdError"),MB_ICONINFORMATION);
				return TRUE;
			}

		}
	}
	return FALSE;
}
#pragma once
class File
{
public:
	File(TCHAR* filename, char* mode);
	~File(void);

	void close();
	BOOL closed();
	unsigned long get_size();
	unsigned long get_pos();
	BOOL set_pos(unsigned long pos);

	LPVOID* read(unsigned long size);
	BOOL write(LPCVOID buffer, unsigned long buffersize);
	//>Fonctions exportés
	static BOOL exist(TCHAR* filename);

	//>Attributs
	//Fichier
	HANDLE handle;

private: 
	//Taille
	unsigned long size;
	unsigned long pos;
	BOOL can_work;
	BOOL opened_for_reading;
	BOOL opened_for_writing;
};


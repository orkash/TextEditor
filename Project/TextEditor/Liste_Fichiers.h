#pragma once
void MakeFile_List(HWND hwnd);
void LoadFile(HWND hwnd, unsigned long pos);
void DisplayTexts(HWND hwnd, long add);
void SaveFile(unsigned long id);
void ChangeText(HWND hwnd, Array* arr, TCHAR* text);

#define MAX_TEXT_FILE 60
#define DisplayTexts_WideCharSize 2048

#define FILE_IDs { \
	1, \
	4, \
	8, \
	9, \
	12, \
	13, \
	14, \
	15, \
	16, \
	18, \
	19, \
	21, \
	22, \
	23, \
	28, \
	36, \
	37, \
	38, \
	39, \
	41, \
	42, \
	50, \
	52, \
	65, \
	66, \
	76, \
	78, \
	79, \
	81, \
	83, \
	84, \
	85, \
	86, \
	87, \
	88, \
	89, \
	91, \
	96, \
	99, \
	101, \
	104, \
	115, \
	116, \
	117, \
	118, \
	124, \
	133, \
	134, \
	143, \
	145, \
	147, \
	148, \
	149, \
	150, \
	151, \
	152, \
	156, \
	158, \
	164, \
	165 \
}

#define STRING_LIST { \
	IDS_STRING132, \
	IDS_STRING133, \
	IDS_STRING134, \
	IDS_STRING135, \
	IDS_STRING136, \
	IDS_STRING137, \
	IDS_STRING138, \
	IDS_STRING139, \
	IDS_STRING140, \
	IDS_STRING141, \
	IDS_STRING142, \
	IDS_STRING143, \
	IDS_STRING144, \
	IDS_STRING145, \
	IDS_STRING146, \
	IDS_STRING147, \
	IDS_STRING148, \
	IDS_STRING149, \
	IDS_STRING150, \
	IDS_STRING151, \
	IDS_STRING152, \
	IDS_STRING153, \
	IDS_STRING154, \
	IDS_STRING155, \
	IDS_STRING156, \
	IDS_STRING157, \
	IDS_STRING158, \
	IDS_STRING159, \
	IDS_STRING160, \
	IDS_STRING161, \
	IDS_STRING162, \
	IDS_STRING163, \
	IDS_STRING164, \
	IDS_STRING165, \
	IDS_STRING166, \
	IDS_STRING167, \
	IDS_STRING168, \
	IDS_STRING169, \
	IDS_STRING170, \
	IDS_STRING171, \
	IDS_STRING172, \
	IDS_STRING173, \
	IDS_STRING174, \
	IDS_STRING175, \
	IDS_STRING176, \
	IDS_STRING177, \
	IDS_STRING178, \
	IDS_STRING179, \
	IDS_STRING180, \
	IDS_STRING181, \
	IDS_STRING182, \
	IDS_STRING183, \
	IDS_STRING184, \
	IDS_STRING185, \
	IDS_STRING186, \
	IDS_STRING187, \
	IDS_STRING188, \
	IDS_STRING189, \
	IDS_STRING190, \
	IDS_STRING191 \
}

#define L_JAP_FILE TEXT("Data/Text/kana/%d.txt")
#define L_ENG_FILE TEXT("Data/Text/en/%d.txt")
#define L_FRA_FILE TEXT("Data/Text/fr/%d.txt")
#define L_ITA_FILE TEXT("Data/Text/it/%d.txt")
#define L_ESP_FILE TEXT("Data/Text/es/%d.txt")
#define L_KOR_FILE TEXT("Data/Text/ko/%d.txt")
#define L_DEU_FILE TEXT("Data/Text/de/%d.txt")
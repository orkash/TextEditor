#pragma once
class Text_Arrays
{
public:
	Text_Arrays(void);
	~Text_Arrays(void);
	static Array* kana;
	static Array* en;
	static Array* fr;
	static Array* it;
	static Array* es;
	static Array* ko;
	static Array* de;
	static BOOL auto_save;
	static BOOL prompt_if_other_file;
	static BOOL changes;
	static unsigned long last_id;
	static void initialize();
	static void clear_all();
	static void free_safe(unsigned long* ptr);
};
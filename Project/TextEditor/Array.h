#pragma once
class Array
{
public:
	Array(void);
	~Array(void);

	unsigned long get_size();
	unsigned long get(unsigned long index);
	BOOL set(unsigned long index, unsigned long value);
	char* utf8_join(char* join_chrs);
	void load_from_utf8_rn(char* str);
	void clear();
	void clear_wf(unsigned long* function);
	void delete_at(unsigned long index,unsigned long* function=NULL);
	void delete_from_to(unsigned long index, unsigned long count,unsigned long* function=NULL);
	
private:
	unsigned long* ptr;
	unsigned long size;
};


typedef void (*fArray_Clear_Function)(unsigned long);
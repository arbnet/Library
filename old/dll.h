#ifndef FILE_DLL
#include <windows.h>
#ifndef FILE_Variables
#include "Variables.h"
#endif

class DLL {
private:
	STRING file;
	HMODULE hdll=NULL;
public:
	DLL(LETTERS(ltr)){
		this->file=ltr;
		this->hdll=LoadLibrary(ltr);
	}
	~DLL(){
		FreeLibrary(this->hdll);
	}
	HMODULE Hdll(){
		return this->hdll;
	}
	STRING File(){
		return this->file;
	}
	template <typename dTYPE>
	dTYPE* Function(LETTERS(ltr)){
		return (dTYPE*)GetProcAddress(this->hdll,ltr);
	}
};

#define FILE_DLL
#endif

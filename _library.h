#ifdef FILEDLL
#ifndef FILE_variables
#include "_variables.h"
#endif
#include <windows.h>
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
	STRING& File(){
		return this->file;
	}
	template <typename dTYPE>
	dTYPE* Function(LETTERS(ltr)){
		return (dTYPE*)GetProcAddress(this->hdll,ltr);
	}
};
//namespace l {}
#else
	#ifdef EXPORT
		#ifdef __cplusplus
			#define DLL_OPEN extern "C" {
			#define DLL_CLOSE }
		#endif
		#define EI __declspec(dllexport) 
	#else
		#define EI __declspec(dllimport)
		#define DLL_OPEN
		#define DLL_CLOSE
	#endif
#endif

#ifndef FILE_variables
#include "_variables.h"
#endif

namespace b {
class Sheet : private Array<STRING> {
public:
	STRING Class(){return STRING("Sheet");}
	using Array<STRING>::How;
	using Array<STRING>::Size;
	using Array<STRING>::Next;
	using Array<STRING>::Reset;
	void Add(LETTERS(ltr)){
		if(!this->Exists(ltr))this->Put(ltr);
	}
	INT_L Exists(LETTERS(ltr)){
		return this->Find(ltr);
	}
	INT_L Exists(STRING str){
		return this->Find(str);
	}
};

}

b::Sheet dType;
dType.Add("one");
dType.Add("два");
dType.Add("1111");
dType.Add("BYTE");

#define FILE_base

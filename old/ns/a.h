#ifndef FILE_ArrayAction
#include <windows.h>
#ifndef FILE_Variables
#include "Variables.h"
#endif

template <typename dTYPE>
class ArrayAction {
private:
	Array<dTYPE> *obj;
public:
	ArrayAction(Array<dTYPE> &obj){
		this->obj=&obj;
	}
	/*Array<dTYPE> Doublon(){
		return new Array<dTYPE>(this->obj.Size(),this->obj.Reserve());
	}
	//~ArrayAction(){}
	void Reverse(){
		Array<dTYPE> *tobj=this->Doublon();
		obj.How(false);
		do{
			tobj.Put(obj.Get());
		}while(obj.Next());
		delete 
	}*/
};

#define FILE_ArrayAction
#endif

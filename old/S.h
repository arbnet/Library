#ifndef FILE_Saction
#include <windows.h>
#ifndef FILE_Variables
#include "Variables.h"
#endif

namespace s {
	extern void reverse(STRING &obj);
	extern STRING tome(INT_B var);
	
	STRING tome(LOGIC var){
		return STRING(var?"true":"false");
	}
	STRING tome(LETTER var){
		const char sym[2]={var,'\0'};
		return STRING(sym);
	}
	STRING tome(RANGE var){
		LOGIC sgn=var<0?true:false;
		if(sgn)var=-var;
		STRING res;
		if(sgn)res="-";
		res+=tome(INT_B(var));
		return res;
	}
	STRING tome(BYTE var){
		STRING res;
		res=tome(INT_B(var));
		return res;
	}
	STRING tome(INT_S var){
		LOGIC sgn=var<0?true:false;
		if(sgn)var=-var;
		STRING res;
		if(sgn)res="-";
		res+=tome(INT_B(var));
		return res;
	}
	STRING tome(INT_W var){
		STRING res;
		res=tome(INT_B(var));
		return res;
	}
	STRING tome(INT_M var){
		LOGIC sgn=var<0?true:false;
		if(sgn)var=-var;
		STRING res;
		if(sgn)res="-";
		res+=tome(INT_B(var));
		return res;
	}
	STRING tome(INT_L var){
		STRING res;
		res=tome(INT_B(var));
		return res;
	}
	STRING tome(INT_T var){
		LOGIC sgn=var<0?true:false;
		if(sgn)var=-var;
		STRING res;
		if(sgn)res="-";
		res+=tome(INT_B(var));
		return res;
	}
	STRING tome(INT_B var){
		BYTE num;
		STRING str;
		do{
			num=var%10;var=var/10;
			str+=tome(LETTER(num|0x30));
		}while(var>0);
		reverse(str);
		return str;
	}
	STRING tome(FLOAT var){
		LOGIC sgn=var<0?true:false;
		if(sgn)var=-var;
		STRING res;
		if(sgn)res="-";
		INT_L whl=var;
		res+=tome(INT_B(whl));
		DOUBLE frc=var-whl;
		cout << "frc="<< frc <<endl;
		while(frc>INT_L(frc)){
			frc=frc*10;
			//whl=frc;
			//cout << frc<< " n=" << whl <<endl;
			//frc-=DOUBLE(whl);
		}
		cout << "frc="<< frc <<endl;
		return res;
	}
	
	void reverse(STRING &obj){
		STRING str;
		//char ltr[2]={'Z','\0'};
		INT_L ln=obj.Size();
		do{
			//ltr[0]=obj[ln];
			str+=tome(LETTER(obj[ln]));
			ln--;
		}while(ln>0);
		obj=str;
	}
}

#define FILE_Saction
#endif

#ifndef FILE_array
#include "_array.h"
#endif
//#ifndef FILE_calculate
//#include "_calculate.h"
//#endif

namespace s {
	INT_W total(STRING &str){
		return str.Reserve()?65535:str.Total();
	}
	STRING text(Array<STRING> &oar,STRING sep=""){
		STRING res;
		if(oar){
			for(INT_L nx=0;nx++<oar.Size();){
				if(res)if(sep)res+=sep;
				res+=oar[nx];
			}
		}
		return res;
	}
}
/*
	extern void reverse(STRING &obj);
	extern STRING tome(INT_B var);
	
	LETTER* letter(LETTER ltr){
		return new LETTER[2]{ltr,'\0'};
	}
	STRING tome(LOGIC var){
		return STRING(var?"true":"false");
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
		//STRING res;
		//res+=tome(INT_B(var));
		return tome(INT_B(var));
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
		//STRING res;
		//res+=tome(INT_B(var));
		return tome(INT_B(var));
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
		//STRING res;
		//res=tome(INT_B(var));
		return tome(INT_B(var));
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
		STRING str;
		BYTE num;
		do{
			num=var%10;var=var/10;
			str+=letter(num|0x30);
		}while(var>0);
		reverse(str);
		return str;
	}
	STRING tome(FLOAT var){
		LOGIC sgn=var<0?true:false;
		if(sgn)var=-var;
		STRING res;
		if(sgn)res="-";
		//res+=tome(c::integer(var));
		res+=".";
		//res+=tome(c::whole(var));
		return res;
	}
	STRING tome(DOUBLE var){
		LOGIC sgn=var<0?true:false;
		if(sgn)var=-var;
		STRING res;
		if(sgn)res="-";
		//res+=tome(c::integer(var));
		res+=".";
		//res+=tome(c::whole(var));
		return res;
	}
	
	void reverse(STRING &obj){
		STRING str;
		INT_L ln=obj.Size();
		do{
			str+=letter(obj[ln]);ln--;
		}while(ln>0);
		obj=str;
	}
*/

#define FILE_string

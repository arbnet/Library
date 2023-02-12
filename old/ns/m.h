#ifndef FILE_nsm
#ifndef FILE_variables
#include "variables.h"
#endif

namespace m {
	template <typename dTYPE>
	dTYPE absolute(dTYPE var){
		if(var<0)var=-var;
		return var;
	}
	template <typename dTYPE>
	dTYPE d10(dTYPE var){
		BYTE num=BYTE(var%10);
		while(num==0){
			var=var/10;num=var%10;
		}
		return var;
	}
	INT_L integer(FLOAT var){
		return INT_L(var);
	}
	INT_B integer(DOUBLE var){
		return INT_B(var);
	}
	FLOAT fraction(FLOAT var){
		var-=INT_L(var);
		return var;
	}
	DOUBLE fraction(DOUBLE var){
		var-=INT_L(var);
		return var;
	}
	INT_B whole(DOUBLE var){
		var-=integer(var);
		INT_B res=var*1000000;
		return d10(res);
	}
	INT_L whole(FLOAT var){
		var-=integer(var);
		INT_L res=var*1000;
		return d10(res);
	}
	
}

#define FILE_nsm
#endif

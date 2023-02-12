/*
Работа с сетью
*/
#ifndef FILE_variables
#include "_variables.h"
#endif

namespace t {
	#include <iostream>
	using namespace std;
	template <typename dTYPE>
	void i(dTYPE &var,STRING msg=""){
		if(msg.Size()>0)cout << msg << ": ";
		cin >> var;
	}
	template <typename dTYPE>
	void o(dTYPE &var,STRING msg=""){
		if(msg.Size()>0)cout << msg << ": ";
		cout << var << endl;
	}
	void p(){
		system("pause");
	}
	void r(){
		setlocale(LC_ALL,"");
	}
}

#define FILE_net

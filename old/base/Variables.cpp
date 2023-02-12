#include <iostream>
#include <typeinfo>
#include "Variables.h"

template <class nCLASS>
Object<nCLASS>::Object(nCLASS *obj){this->obj=obj;}
template <typename nCLASS>
nCLASS& Object<nCLASS>::operator* (){return *this->obj;}
template <class nCLASS>
nCLASS* Object<nCLASS>::operator->(){return this->obj;}
template <class nCLASS>
Object<nCLASS>::~Object() {delete this->obj;}

template <>
Array<LOGIC>::Array(){this->type="LOGIC";}
template <>
Array<RANGE>::Array(){this->type="RANGE";}
template <>
Array<LETTER>::Array(){this->type="LETTER";}
template <>
Array<BYTE>::Array(){this->type="BYTE";}
template <>
Array<INT_S>::Array(){this->type="INT_S";}
template <>
Array<INT_W>::Array(){this->type="INT_W";}
template <>
Array<INT_M>::Array(){this->type="INT_M";}
template <>
Array<INT_L>::Array(){this->type="INT_L";}
template <>
Array<INT_T>::Array(){this->type="INT_T";}
template <>
Array<INT_B>::Array(){this->type="INT_B";}
template <>
Array<FLOAT>::Array(){this->type="FLOAT";}
template <>
Array<DOUBLE>::Array(){this->type="DOUBLE";}
//template <>
//Array<STRING>::Array(){this->type="STRING";}
//Array<POINTER>::Array(STRING type){this->type=type;}

template <typename dTYPE>
Array<dTYPE>::~Array(){
	delete []this->vars;
}
template <typename dTYPE>
void Array<dTYPE>::Vindex(INT_L &index){
	if(index>this->sz){index=~index+1;
		if(index<this->sz)index=this->sz-index;
			else index=this->sz-1;
	}else if(index>0)index--;
}
template <typename dTYPE>
INT_L Array<dTYPE>::Size(){return this->sz;}
template <typename dTYPE>
INT_L Array<dTYPE>::Total(){return this->tl;}
template <typename dTYPE>
LOGIC Array<dTYPE>::Prev(){
	if(this->it>0){
		this->it--;return true;
	}else return false;
}
template <typename dTYPE>
LOGIC Array<dTYPE>::Next(){
	if(this->it<this->sz-1){
		this->it++;return true;
	}else return false;
}
template <typename dTYPE>
INT_L Array<dTYPE>::Index(){return this->it+1;}
template <typename dTYPE>
void Array<dTYPE>::Index(INT_L it){
	this->Vindex(it);this->it=it;
}
template <typename dTYPE>
dTYPE Array<dTYPE>::Take(INT_L index){
	this->Vindex(index);
	dTYPE val=this->vars[index];
	this->sz--;
	while(index<this->sz){
		this->vars[index]=this->vars[index+1];index++;
	}
	return val;
}
template <typename dTYPE>
dTYPE Array<dTYPE>::Get(INT_L index){
	if(index==0)index=this->it;
	else this->Vindex(index);
	return this->vars[index];
}

template <typename dTYPE>
void Array<dTYPE>::Put(dTYPE val,INT_L index){
	this->Vindex(index);this->sz++;
	if(this->sz>this->tl){
		this->tl+=this->rv+1;
		dTYPE *vars=new dTYPE[this->tl] ;
		/*for(INT_L nx=0;nx<index;nx++)
			vars[nx]=this->vars[nx];
		for(INT_L nx=index;nx<this->sz;nx++)
			vars[nx+1]=this->vars[nx];*/
		INT_L nx=0;
		while(nx<index){
			vars[nx]=this->vars[nx];nx++;
		}
		while(nx<this->sz){
			vars[nx+1]=this->vars[nx];nx++;
		}
		delete []this->vars;
		this->vars=vars;
	}else if(index<this->sz){
		for(INT_L nx=this->sz;nx>index;nx--)
			this->vars[nx]=this->vars[nx-1];
	}
	//cout << "Put[" << index+1 << "]=" << val << endl;
	this->vars[index]=val;
	//this->Print();
}
template <typename dTYPE>
void Array<dTYPE>::Set(dTYPE val,INT_L index){
	if(index==0)index=this->it;
	this->vars[index]=val;
}
template <typename dTYPE>
void Array<dTYPE>::Init(INT_L size,INT_L reserve){
	this->sz=size;this->rv=reserve;this->tl=size+reserve;
	this->vars=new dTYPE[this->tl];
}
template <typename dTYPE>
INT_L Array<dTYPE>:: Find(dTYPE val,INT_L index,rement rt){
	INT_L res=0;
	if(this->sz){
		if(index==0)index=this->it;
		else this->Vindex(index);
		while(index<this->sz){
			if(this->vars[index]==val){res=index+1;break;}
			index+=rt;
		}
	}
	return res;
}
/*
template <typename dTYPE>
INT_L Array<dTYPE>::Size(){return this->vect.size();};

template <typename dTYPE>
STRING Array<dTYPE>::Type(){return this->type;};
STRING Array<POINTER>::Type(){return this->type;};

template <typename dTYPE>
void Array<dTYPE>::Add(dTYPE val){
	this->vect.push_back(val);
};
template <class nCLASS>
void Array<POINTER>::Add(nCLASS obj){
	this->vect.push_back(obj);
}

template <typename dTYPE>
dTYPE Array<dTYPE>::Get(){
	return this->vect[this->it];
}
template <typename dTYPE>
dTYPE Array<dTYPE>::Get(INT_L index){
	return this->vect[index];
}
template <class nCLASS>
nCLASS Array<POINTER>::Get(INT_L index){
	return *static_cast<nCLASS*>(this->vect[index]);
}

template <typename dTYPE>
void Array<dTYPE>::Begin(){
	//INT_L num=*this->vect.begin();
	//num=this->vect.begin();<< num
	cout << "B="  << this->it << endl;
	//this->it=(INT_L)this->vect.begin();
}
template <typename dTYPE>
void Array<dTYPE>::End(){
	this->it=this->vect.end();
}
template <typename dTYPE>
void Array<dTYPE>::Itset(INT_L itn){
	this->it=itn;
}
//template <typename dTYPE>
//dTYPE Array<dTYPE>::prev(){
//	return this->vect[this->it];
//}

template <typename dTYPE>
LOGIC Array<dTYPE>::Next(){
	//this->it++;
	return this->it<=this->vect.end()?true:false;
}

template <typename dTYPE>
void Array<dTYPE>::Print(){
	INT_L size=this->vect.size();
	cout << this->type << endl;
	for (INT_L nx=0;nx<size;nx++)
		cout << this->vect[nx] << endl;
}

template <typename dTYPE>
INT_B sTYPE(dTYPE var){return sizeof(dTYPE);}
STRING nTYPE(LOGIC var){return "LOGIC";}
STRING nTYPE(RANGE var){return "RANGE";}
STRING nTYPE(LETTER var){return "LETTER";}
STRING nTYPE(BYTE var){return "BYTE";}
STRING nTYPE(INT_S var){return "INT_S";}
STRING nTYPE(INT_W var){return "INT_W";}
STRING nTYPE(INT_M var){return "INT_M";}
STRING nTYPE(INT_L var){return "INT_L";}
STRING nTYPE(INT_T var){return "INT_T";}
STRING nTYPE(INT_B var){return "INT_B";}
STRING nTYPE(FLOAT var){return "FLOAT";}
STRING nTYPE(DOUBLE var){return "DOUBLE";}
STRING nTYPE(STRING var){return "STRING";}
STRING nTYPE(POINTER var){return "POINTER";}
STRING nTYPE(Array<POINTER> arr){return "Array<"+arr.Type()+">";}

class XX {
	STRING tx;
public:
	XX(STRING mtx){
		this->tx=mtx;
	}
	void echo(){
		cout << "mytx=" << this->tx << endl;
	}
};
*/

int main(){
	setlocale(LC_ALL,"");
	//Object<XX> mobj(new XX("AAAA"));
	//Array<POINTER> ao("XX");
	//ao.Add(&mobj);
	//mobj->echo();
	//cout << "ao_type=" << nTYPE(ao) << " ao_bytes=" << sTYPE(ao) << endl << endl;
	//char a={};
	//cout << "|" << (INT_W)(a) << "|" << endl;
	//LETTER tx[]="проверка";
	//cout << tx << endl;
	//Array<LETTER> str;
	//str.Init(2,3);
	//str.Set('Ф');
	//str.Print();
	//LETTER tx[]="проверка12";
	MString txt("проверка12");
	
	//cout << txt.value() << endl;
	Array<INT_L> obj;
	obj.Init(2,3);
	//cout << "reserve=" << obj.Reserve() << endl;
	//cout << "it=" << obj.Index() << endl;
	//obj.Index(-1);
	//cout << "it=" << obj.Index() << endl;
	INT_L nx=1;
	do{
		//cout << "nx=" << nx <<endl;
	obj.Set(nx);nx++;
	}while(obj.Next());
	obj.Put(nx,2);obj.Put(33,2);obj.Put(44,0);obj.Put(99,-1);
	obj.Put(888,2);obj.Put(5555,5);obj.Put(5555,7);obj.Put(5555,3);
	obj.Print();
	cout << "----------------" << endl;
	if(obj.Size()){
		obj.Index(-1);
		do{
			cout << obj.Get() << endl;
		}while(obj.Prev());
	}
	cout << "find=" << obj.Find(5555,6,Dec) << endl;

	system("pause");
	return 0;
}


#ifndef FILE_Variables
#define FILE_Variables
#include <iostream>
using namespace std;

// Типы данных
typedef bool LOGIC;						// 0..1
typedef char LETTER;					// буква
typedef __INT8_TYPE__ RANGE;		// -128..127
typedef __UINT8_TYPE__ BYTE;		// 0..255
typedef __INT16_TYPE__ INT_S;		// -32768..32767
typedef __UINT16_TYPE__ INT_W;	// 0..65535
typedef __INT32_TYPE__ INT_M;		// -2147483648..2147483647
typedef __UINT32_TYPE__ INT_L;	// 0..4294967295
typedef __INT64_TYPE__ INT_T;		// -9223372036854775808..9223372036854775807
typedef __UINT64_TYPE__ INT_B;	// 0..18446744073709551615
typedef float FLOAT;						// 1.8E-38..1.8E+38		32b
typedef double DOUBLE;				// 2.2E-308..1.8E+308	64b
//typedef char* STRING;					// строка
typedef void* POINTER;				// указатель

#define _STRING(str) const char str[]

enum REMENT {Dec=-1,Inc=1};

struct Bits {
	unsigned b0:1;
	unsigned b1:1;
	unsigned b2:1;
	unsigned b3:1;
	unsigned b4:1;
	unsigned b5:1;
	unsigned b6:1;
	unsigned b7:1;
};
struct Bytes {
	BYTE b0;
	BYTE b1;
	BYTE b2;
	BYTE b3;
	BYTE b4;
	BYTE b5;
	BYTE b6;
	BYTE b7;
};

union Code {
	BYTE byte;
	Bits bit;
};
union Block {
	INT_B data;
	Bytes byte;
};

template <typename dTYPE>
INT_B Size(dTYPE var){return sizeof(dTYPE);}
INT_B Size(const char str[]){
	INT_B sz=0;
	while(str[sz]!='\0')sz++;
	return sz;
}
/*INT_B Size(STRING str){
	INT_B sz=0;
	while(str[sz]!='\0')sz++;
	return sz;
}*/

template <typename dTYPE>
void Pshift(dTYPE* &pnt,INT_L shift=1){
	shift=shift*sizeof(dTYPE);
	pnt=(dTYPE*)((INT_B)pnt+shift);
}
POINTER Pshift(POINTER pnt,INT_L shift=1){
	return (POINTER*)((INT_B)pnt+shift);
}

void Mcopy(POINTER adr1,POINTER adr2,INT_L size){
	INT_L ct;
	ct=size/8;
	if(ct>0){size=size%8;
		do{ct--;
			*(INT_B*)adr1=*(INT_B*)adr2;
			adr1=Pshift(adr1,8);adr2=Pshift(adr2,8);
		}while(ct>0);
	}
	ct=size/4;
	if(ct>0){size=size%4;
		do{ct--;
			*(INT_L*)adr1=*(INT_L*)adr2;
			adr1=Pshift(adr1,4);adr2=Pshift(adr2,4);
		}while(ct>0);
	}
	ct=size/2;
	if(ct>0){size=size%2;
		do{ct--;
			*(INT_W*)adr1=*(INT_W*)adr2;
			adr1=Pshift(adr1,2);adr2=Pshift(adr2,2);
		}while(ct>0);
	}
	if(size>0){ct=size;
		do{ct--;
			*(BYTE*)adr1=*(BYTE*)adr2;
			adr1=Pshift(adr1,1);adr2=Pshift(adr2,1);
		}while(ct>0);
	}
}

LOGIC Mcompare(POINTER adr1,POINTER adr2,INT_L size){
	LOGIC res=true;
	while(size>0){
		if(size>=8){
			if(*(INT_B*)adr1!=*(INT_B*)adr2)res=false;
			else{adr1=Pshift(adr1,8);adr2=Pshift(adr2,8);size-=8;}
		}else if(size>=4){
			if(*(INT_L*)adr1!=*(INT_L*)adr2)res=false;
			else{adr1=Pshift(adr1,4);adr2=Pshift(adr2,4);size-=4;}
		}else if(size>=2){
			if(*(INT_W*)adr1!=*(INT_W*)adr2)res=false;
			else{adr1=Pshift(adr1,2);adr2=Pshift(adr2,2);size-=2;}
		}else{
			if(*(BYTE*)adr1!=*(BYTE*)adr2)res=false;
			else{adr1=Pshift(adr1,1);adr2=Pshift(adr2,1);size-=1;}
		}
		if(!res)break;
	}
	return res;
}

class STRING {
private:
	INT_L sz=0;
	LETTER* str;
public:
	STRING(_STRING(ltr)){
		this->sz=::Size(ltr);sz++;
		this->str=new LETTER[sz];
		Mcopy(this->str,(POINTER)ltr,sz);
	}
	~STRING(){
		if(this->str)delete this->str;
	}
	
}

LETTER* String(_STRING(ltr)){
	INT_L sz=Size(ltr);sz++;
	char *str=new char[sz];
	Mcopy(str,(POINTER)ltr,sz);
	return str;
}

STRING Type(LOGIC var){return String("LOGIC");}
STRING Type(RANGE var){return String("RANGE");}
STRING Type(LETTER var){return String("LETTER");}
STRING Type(BYTE var){return String("BYTE");}
STRING Type(INT_S var){return String("INT_S");}
STRING Type(INT_W var){return String("INT_W");}
STRING Type(INT_M var){return String("INT_M");}
STRING Type(INT_L var){return String("INT_L");}
STRING Type(INT_T var){return String("INT_T");}
STRING Type(INT_B var){return String("INT_B");}
STRING Type(FLOAT var){return String("FLOAT");}
STRING Type(DOUBLE var){return String("DOUBLE");}
STRING Type(STRING var){return String("STRING");}
STRING Type(POINTER var){return String("POINTER");}

template <typename dTYPE>
class Array {
protected:
	LOGIC how=true;
	dTYPE *vars=NULL;
	STRING type=NULL;
	INT_L it=0,sz=0,tl=0,rv=0;
	void Vindex(INT_L &index){
		if(index>this->sz){index=~index+1;
			if(index<this->sz)index=this->sz-index;
			else index=this->sz;
		}else if(index>0)index--;
	}
public:
	Array(){};
	Array(INT_L rv,INT_L sz=0){
		this->Init(rv,sz);
	}
	virtual ~Array(){
		if(this->vars)delete this->vars;
		if(this->type)delete this->type;
	}
	void Init(INT_L rv=100,INT_L sz=0){
		this->sz=sz;this->rv=rv;this->tl=sz+rv;
		this->vars=new dTYPE[this->tl];
		this->it=this->how?0:this->sz;
		this->Fill();
	}
	STRING Name(){return String("Array");}
	virtual STRING Type();
	INT_L Size(){return this->sz;}
	INT_L Total(){return this->tl;}
	INT_L Index(){return this->it+1;}
	void How(LOGIC how){this->how=how;}
	void Index(INT_L it){
		this->Vindex(it);
		if(it<this->sz)this->it=it;
	}
	LOGIC Next(){
		LOGIC res;
		if(this->how){
			if(this->it<this->sz-1){
				this->it++;res=true;
			}else res=false;
		}else{
			if(this->it>0){
				this->it--;res=true;
			}else res=false;
		}
		return res;
	}
	LOGIC Type(_CLTR(stp)){
		LOGIC res=false;BYTE sz=0;
		STRING type=this->Type();
		while(this->type[sz]==stp[sz]){
			if(stp[sz]=='\0'){res=true;break;}
			sz++;
		}
		return res;
	}
	dTYPE Take(INT_L index){
		this->Vindex(index);
		dTYPE val=this->vars[index];
		this->sz--;
		while(index<this->sz){
			this->vars[index]=this->vars[index+1];index++;
		}
		return val;
	}
	void Fill(dTYPE val=0,INT_L cnt=-1){
		if(this->sz>0){
			do{
				this->vars[this->it]=val;
				cnt--;if(cnt==0)break;
			}while(this->Next());
			if(cnt)this->it=this->how?0:this->sz;
		}
	}
	void Set(dTYPE val){
		if(this->sz>0){
			this->vars[this->it]=val;
		}
	}
	dTYPE Get(INT_L index=0){
		if(this->sz>0){
			if(index==0)index=this->it;
			else this->Vindex(index);
			return this->vars[index];
		}else return 0;
	}
	void Put(dTYPE val,INT_L index=-1){
		this->Vindex(index);this->sz++;
		if(this->sz>this->tl){
			this->tl+=this->rv+1;
			dTYPE *vars=new dTYPE[this->tl] ;
			INT_L nx=0;
			while(nx<index){
				vars[nx]=this->vars[nx];nx++;
			}
			while(nx<this->sz){
				vars[nx+1]=this->vars[nx];nx++;
			}
			delete this->vars;
			this->vars=vars;
		}else if(index<this->sz){
			for(INT_L nx=this->sz;nx>index;nx--)
				this->vars[nx]=this->vars[nx-1];
		}
		this->vars[index]=val;
	}
	INT_L Find(dTYPE val,INT_L index=0,REMENT rt=Inc){
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
	void Reserve(INT_L rv){
		if(this->Reserve()!=rv){
			this->rv=rv;this->tl=this->sz+rv;
			dTYPE *vars=new dTYPE[this->tl] ;
			INT_L nx=0;
			while(nx<this->sz){
				vars[nx]=this->vars[nx];nx++;
			}
			delete this->vars;
			this->vars=vars;
		}
	}
	void Print(){
	INT_L sz=this->Size();
	cout << this->Type() << "(" << sz << "," << this->Total() << ")" << endl;
	for (INT_L nx=1;nx<sz+1;nx++)
		cout << nx <<  " = " << *(LETTER*)this->Get(nx) << endl;
}
};

template <> STRING Array<LOGIC>::Type(){return String("LOGIC");}
template <> STRING Array<RANGE>::Type(){return String("RANGE");}
template <> STRING Array<LETTER>::Type(){return String("LETTER");}
template <> STRING Array<BYTE>::Type(){return String("BYTE");}
template <> STRING Array<INT_S>::Type(){return String("INT_S");}
template <> STRING Array<INT_W>::Type(){return String("INT_W");}
template <> STRING Array<INT_M>::Type(){return String("INT_M");}
template <> STRING Array<INT_L>::Type(){return String("INT_L");}
template <> STRING Array<INT_T>::Type(){return String("INT_T");}
template <> STRING Array<INT_B>::Type(){return String("INT_B");}
template <> STRING Array<FLOAT>::Type(){return String("FLOAT");}
template <> STRING Array<DOUBLE>::Type(){return String("DOUBLE");}
template <> STRING Array<POINTER>::Type(){return String("POINTER");}

template <>
class Array<STRING> : private Array<POINTER> {
private:
	INT_L ln=0;
	STRING delimit;
	STRING text=NULL;
public:
	Array(const LETTER delimit[]="\r\n"){
		this->delimit=String(delimit);
	}
	~Array(){
		delete this->delimit;
		if(this->text)delete this->text;
	}
	using Array<POINTER>::Size;
	using Array<POINTER>::Total;
	using Array<POINTER>::Index;
	STRING Type(){return String("STRING");}
	STRING Text(){return this->text;}
	void Text(_CLTR(text),INT_L rv=0){
		this->ln=::Size(text);
		this->tl=this->ln+rv;this->tl++;
		this->text=new LETTER[this->tl];
		Mcopy(this->text,(POINTER)text,this->ln);
		this->Init(100);
		STRING pnt=this->text;
		INT_L ln=::Size(this->delimit),nx=0;
		while(nx<this->ln){
			cout << "p=" << pnt << endl;
			if(Mcompare(pnt,(POINTER)this->delimit,ln)){
				Pshift(pnt,ln);this->Put(pnt);nx+=ln;
				cout << "pppppppp=" << pnt << endl;
			}else{Pshift(pnt);nx++;}
		}
		this->Print();
		//cout << "ct=" << ct << endl;
	}
	STRING String(INT_L index=0){
		STRING
	}
	LOGIC Find(_CLTR(str)){
		LOGIC res=false;
		STRING pnt=this->text;
		INT_L ln=::Size(str),it=0;
		while(it<this->sz){
			if(Mcompare(pnt,(POINTER)str,ln)){
				res=true;this->it=it;break;
			}
			Pshift(pnt);it++;
		}
		return res;
	}
	//void Reserve(INT_L rv){}const LETTER str[]
};

#endif

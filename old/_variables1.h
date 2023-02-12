#include "time.h"
#include <iostream>

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
typedef float FLOAT;						// 1.8E-38..1.8E+38		32b	точность 6 .3 знаков
typedef double DOUBLE;				// 2.2E-308..1.8E+308	64b	точность 15 .6 знаков
typedef void* POINTER;				// указатель

#define CHARS(ltr) const char ltr[]

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
	INT_B numb;
	Bytes byte;
};
struct Addrs {
	POINTER a1;
	POINTER a2;
};

namespace v {
	template <typename dTYPE>
	void swap(dTYPE &obj1, dTYPE &obj2){
		dTYPE tmp(obj1);obj1=obj2;obj2=tmp;
	}
	template <typename dTYPE>
	void shift(dTYPE* &pnt,INT_S shift=1){
		pnt+=shift;
	}
	void shift(POINTER &pnt,INT_S shift=1){
		pnt=(POINTER*)((INT_B)pnt+shift);
	}
	template <typename dTYPE>
	POINTER fill(POINTER adr,INT_W size,dTYPE val=0){
		while(size>0){
			*(dTYPE*)adr=val;shift(adr,sizeof(dTYPE));size--;
		}
		return adr;
	}
	void copy(Addrs &adr,INT_M size){
		INT_W ct;
		if(size>0){
			ct=size/8;
			if(ct>0){size=size%8;
				do{
					*(INT_B*)adr.a1=*(INT_B*)adr.a2;
					shift(adr.a1,8);shift(adr.a2,8);ct--;
				}while(ct>0);
			}
			ct=size/4;
			if(ct>0){size=size%4;
				do{
					*(INT_L*)adr.a1=*(INT_L*)adr.a2;
					shift(adr.a1,4);shift(adr.a2,4);ct--;
				}while(ct>0);
			}
			ct=size/2;
			if(ct>0){size=size%2;
				do{
					*(INT_W*)adr.a1=*(INT_W*)adr.a2;
					shift(adr.a1,2);shift(adr.a2,2);ct--;
				}while(ct>0);
			}
			if(size>0){ct=size;
				do{
					*(BYTE*)adr.a1=*(BYTE*)adr.a2;
					shift(adr.a1,1);shift(adr.a2,1);ct--;
				}while(ct>0);
			}
		}else if(size<0){size=-size;
			shift(adr.a1,size);shift(adr.a2,size);
			ct=size/8;
			if(ct>0){size=size%8;
				do{
					shift(adr.a1,-8);shift(adr.a2,-8);ct--;
					*(INT_B*)adr.a1=*(INT_B*)adr.a2;
				}while(ct>0);
			}
			ct=size/4;
			if(ct>0){size=size%4;
				do{
					shift(adr.a1,-4);shift(adr.a2,-4);ct--;
					*(INT_L*)adr.a1=*(INT_L*)adr.a2;
				}while(ct>0);
			}
			ct=size/2;
			if(ct>0){size=size%2;
				do{
					shift(adr.a1,-2);shift(adr.a2,-2);ct--;
					*(INT_W*)adr.a1=*(INT_W*)adr.a2;
				}while(ct>0);
			}
			if(size>0){ct=size;
				do{
					shift(adr.a1,-1);shift(adr.a2,-1);ct--;
					*(BYTE*)adr.a1=*(BYTE*)adr.a2;
				}while(ct>0);
			}
		}
	}
	LOGIC compare(POINTER a1,POINTER a2,INT_W size){
		LOGIC res=true;
		while(size>0){
			if(size>=8){
				if(*(INT_B*)a1!=*(INT_B*)a2)res=false;
				else{shift(a1,8);shift(a2,8);size-=8;}
			}else if(size>=4){
				if(*(INT_L*)a1!=*(INT_L*)a2)res=false;
				else{shift(a1,4);shift(a2,4);size-=4;}
			}else if(size>=2){
				if(*(INT_W*)a1!=*(INT_W*)a2)res=false;
				else{shift(a1,2);shift(a2,2);size-=2;}
			}else{
				if(*(BYTE*)a1!=*(BYTE*)a2)res=false;
				else{shift(a1,1);shift(a2,1);size-=1;}
			}
			if(!res)break;
		}
		return res;
	}
	void index(INT_L &index,INT_L size){
		if(index>0){
			if(index<=size)index--;
			else{index=~index+1;
				if(index<=size)index=size-index;
				else if(size>0)index=size-1;
				else index=0;
			}
		}
	}
	INT_L volume(INT_L sz,INT_W pt){sz++;
		INT_B res=pt+(sz/pt)*pt;
		if(res>4294967295)res=4294967295;
		return res;
	}
}

class STRING {
private:
	INT_W sz=0,tl=0;
	LETTER* str=NULL;
	void Clear(){
		if(this->str){
			delete []this->str;this->str=NULL;
		}
	}
	INT_W Count(CHARS(ltr)){
		INT_W sz=0;
		while(ltr[sz]!='\0'){
			sz++;if(sz==65535)break;
		}
		return sz;
	}
	void Assign(POINTER pnt,INT_W sz){
		if(!this->Compare(pnt,sz)){
			if(this->tl){
				if(this->tl<sz)sz=this->tl;
			}else{this->Clear();
				this->str=new LETTER[INT_M(sz+1)];
			}
			if(sz){
				Addrs adr={(POINTER)this->str,pnt};
				v::copy(adr,sz);*(BYTE*)adr.a1=0;
			}
			this->sz=sz;
		}
	}
	LOGIC Compare(POINTER pnt,INT_W sz){
		return this->sz==sz?v::compare(this->str,pnt,sz):false;
	}
public:
	STRING(INT_W tl=0){
		if(tl>0){this->tl=tl;
			this->str=new LETTER[INT_M(tl+1)];
		}
	}
	STRING(CHARS(ltr)){
		this->Assign((POINTER)ltr,this->Count(ltr));
	}
	~STRING(){this->Clear();}
	operator bool() const {return this->sz?true:false;}
	LETTER& operator[](INT_L nx){
		v::index(nx,this->sz);
		return this->str[nx];
	}
	STRING& operator=(STRING &str){
		this->Assign(str.str,str.sz);
		return *this;
	}
	STRING& operator=(CHARS(ltr)){
		this->Assign((POINTER)ltr,this->Count(ltr));
		return *this;
	}
	LOGIC operator==(STRING &str){
		return this->Compare(str.str,str.sz);
	}
	LOGIC operator==(CHARS(ltr)){
		return this->Compare((POINTER)ltr,this->Count(ltr));
	}
	LOGIC operator!=(STRING &str){
		return !this->Compare(str.str,str.sz);
	}
	LOGIC operator!=(CHARS(ltr)){
		return !this->Compare((POINTER)ltr,this->Count(ltr));
	}
	LETTER* operator*(){return this->str;}
	INT_W Size(){return this->sz;}
	INT_W Total(){return this->tl;}
	friend std::ostream& operator<< (std::ostream &out,const STRING &obj){
		if(obj.sz)out << obj.str;else out << "NULL";
		return out;
	}
};

class DATETIME {
private:
	struct tm *stm;
	time_t Value() const{
		return mktime(this->stm);
	}
public:
	DATETIME(time_t dtm=0){*this=dtm;}
	operator time_t(){return this->Value();}
	DATETIME& operator=(time_t val){
		this->stm=localtime(&val);
		return *this;
	}
	DATETIME& operator+=(time_t val){
		time_t dtm=this->Value()+val;
		this->stm=localtime(&dtm);
		return *this;
	}
	DATETIME& operator-=(time_t val){
		time_t dtm=this->Value()-val;
		this->stm=localtime(&dtm);
		return *this;
	}
	void Clock(){*this=clock();}
	void Now(){*this=time(NULL);}
	INT_M& Seconds(){return this->stm->tm_sec;}
	INT_M& Minutes(){return this->stm->tm_min;}
	INT_M& Hours(){return this->stm->tm_hour;}
	INT_M& Day(){return this->stm->tm_mday;}
	INT_M& Month(){return this->stm->tm_mon;}
	INT_M& Year(){return this->stm->tm_year;}
	INT_M& Wday(){return this->stm->tm_wday;}
	INT_M& Yday(){return this->stm->tm_yday;}
	INT_M& Summer(){return this->stm->tm_isdst;}
	STRING Show(STRING fmt,BYTE sz=64){
		LETTER res[sz];
		strftime(res,sz,*fmt,this->stm);
		return STRING(res);
	}
	friend std::ostream& operator<< (std::ostream &out,DATETIME &obj){
		out << obj.Value();return out;
	}
};

namespace v {
	STRING type(LOGIC &var){return STRING("LOGIC");}
	STRING type(LETTER &var){return STRING("LETTER");}
	STRING type(RANGE &var){return STRING("RANGE");}
	STRING type(BYTE &var){return STRING("BYTE");}
	STRING type(INT_S &var){return STRING("INT_S");}
	STRING type(INT_W &var){return STRING("INT_W");}
	STRING type(INT_M &var){return STRING("INT_M");}
	STRING type(INT_L &var){return STRING("INT_L");}
	STRING type(INT_T &var){return STRING("INT_T");}
	STRING type(INT_B &var){return STRING("INT_B");}
	STRING type(FLOAT &var){return STRING("FLOAT");}
	STRING type(DOUBLE &var){return STRING("DOUBLE");}
	STRING type(DATETIME &var){return STRING("DATETIME");}
	STRING type(STRING &var){return STRING("STRING");}
	STRING type(POINTER &var){return STRING("POINTER");}
	template <class nCLASS>
	STRING type(nCLASS &obj){return STRING("OBJECT");}
	INT_W idtype(STRING stp){
		BYTE res;
		if(stp=="LOGIC")res=1;
		else if(stp=="LETTER")res=2;
		else if(stp=="RANGE")res=3;
		else if(stp=="BYTE")res=4;
		else if(stp=="INT_S")res=5;
		else if(stp=="INT_W")res=6;
		else if(stp=="INT_M")res=7;
		else if(stp=="INT_L")res=8;
		else if(stp=="INT_T")res=9;
		else if(stp=="INT_B")res=10;
		else if(stp=="FLOAT")res=11;
		else if(stp=="DOUBLE")res=12;
		else if(stp=="DATETIME")res=13;
		else if(stp=="STRING")res=14;
		else if(stp=="POINTER")res=15;
		else res=0;
		return res;
	}
	template <typename dTYPE>
	INT_W idtype(dTYPE var){
		return idtype(type(var));
	}
}

class DIFFERENT {
private:
	STRING vtp;
	POINTER pnt=NULL;
	void Clear(){
		if(this->pnt){
			switch(v::idtype(this->vtp)){
				case 1:	delete static_cast<LOGIC*>(this->pnt);break;
				case 2:	delete static_cast<LETTER*>(this->pnt);break;
				case 3:	delete static_cast<RANGE*>(this->pnt);break;
				case 4:	delete static_cast<BYTE*>(this->pnt);break;
				case 5:	delete static_cast<INT_S*>(this->pnt);break;
				case 6:	delete static_cast<INT_W*>(this->pnt);break;
				case 7:	delete static_cast<INT_M*>(this->pnt);break;
				case 8:	delete static_cast<INT_L*>(this->pnt);break;
				case 9:	delete static_cast<INT_T*>(this->pnt);break;
				case 10:	delete static_cast<INT_B*>(this->pnt);break;
				case 11:	delete static_cast<FLOAT*>(this->pnt);break;
				case 12:	delete static_cast<DOUBLE*>(this->pnt);break;
				case 13:	delete static_cast<DATETIME*>(this->pnt);break;
				case 14:	delete static_cast<STRING*>(this->pnt);break;
				default:	delete static_cast<POINTER*>(this->pnt);
			}
			this->vtp="";this->pnt=NULL;
		}
	}
public:
	DIFFERENT(){}
	~DIFFERENT(){this->Clear();}
	template <typename dTYPE>
	explicit operator dTYPE() const{return *static_cast<dTYPE*>(this->pnt);}
	template <typename dTYPE>
	DIFFERENT& operator=(dTYPE val){
		STRING ntp=v::type(val);
		if(this->vtp!=ntp){
			this->Clear();this->vtp=ntp;
			this->pnt=new dTYPE(val);
		}else *(dTYPE*)this->pnt=val;
		return *this;
	}
	DIFFERENT& operator=(CHARS(ltr)){
		*this=STRING(ltr);
		return *this;
	}	
	STRING Type(){return this->vtp;}
	friend std::ostream& operator<< (std::ostream &out, const DIFFERENT &obj){
		if(obj.pnt){
			switch(v::idtype(obj.vtp)){
				case 1:	out << (*(LOGIC*)obj.pnt?"true":"false");break;
				case 2:	out << *(LETTER*)obj.pnt;break;
				case 3:	out << *(RANGE*)obj.pnt;break;
				case 4:	out << *(BYTE*)obj.pnt;break;
				case 5:	out << *(INT_S*)obj.pnt;break;
				case 6:	out << *(INT_W*)obj.pnt;break;
				case 7:	out << *(INT_M*)obj.pnt;break;
				case 8:	out << *(INT_L*)obj.pnt;break;
				case 9:	out << *(INT_T*)obj.pnt;break;
				case 10:	out << *(INT_B*)obj.pnt;break;
				case 11:	out << *(FLOAT*)obj.pnt;break;
				case 12:	out << *(DOUBLE*)obj.pnt;break;
				case 13:	out << *(DATETIME*)obj.pnt;break;
				case 14:	out << *(STRING*)obj.pnt;break;
				default:	out << *(POINTER*)obj.pnt;
			}
		}else out << "NULL";
		return out;
	}
};

namespace v {
	//template <>
	STRING type(DIFFERENT &var){return STRING("DIFFERENT");}
	template <typename dTYPE>
	LOGIC istype(dTYPE var,STRING stp){return v::type(var)==stp?true:false;}
}


/*

template <typename dTYPE>
class Array {
protected:
	LOGIC how=true;
	dTYPE *vars=NULL;
	INT_L it=0,sz=0,tl=0,rv=0;
public:
	Array(INT_L rv=10){
		this->tl=this->rv=rv;
		this->vars=new dTYPE[this->tl];
	}
	~Array(){
		if(this->vars)delete this->vars;
	}
	WORD Class(){return v::word("Array");}
	WORD Type();
	INT_L Size(){return this->sz;}
	INT_L Total(){return this->tl;}
	INT_L Index(){return this->it+1;}
	INT_L Reserve(){return this->rv;}
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
	void How(LOGIC how){
		this->Index(how?0:-1);this->how=how;
	}
	void Index(INT_L it){
		v::index(it,this->sz);
		if(it<this->sz)this->it=it;
	}
	void Reset(){
		this->it=this->how?0:this->sz;
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
	LOGIC Type(WORD(stp)){
		return this->Type()==stp?true:false;
	}
	dTYPE Take(INT_L index=-1){
		dTYPE val;
		if(this->sz){
			v::index(index,this->sz);
			val=this->vars[index];this->sz--;
			while(index<this->sz){
				this->vars[index]=this->vars[index+1];index++;
			}
		}else val=0;
		return val;
	}
	void Fill(dTYPE val=0,INT_L cnt=-1){
		if(this->sz>0){
			do{
				this->vars[this->it]=val;
				cnt--;if(cnt==0)break;
			}while(this->Next());
			if(cnt)this->Reset();
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
			else v::index(index,this->sz);
			return this->vars[index];
		}else return 0;
	}
	void Put(dTYPE val,INT_L index=0){
		if(index!=0)v::index(index,this->sz);
		else index=this->sz;
		this->sz++;
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
	INT_L Find(dTYPE val,INT_L index=0){
		INT_L res=0;
		if(this->sz){
			if(index!=0){
				v::index(index,this->sz);
				v::swap(this->it,index);
			}else index=this->it;
			do{
				if(this->Get()==val){
					res=this->Index();break;
				}
			}while(this->Next());
			if(!res)v::swap(this->it,index);
		}
		return res;
	}
};

template <> WORD Array<LOGIC>::Type(){return v::word("LOGIC");}
template <> WORD Array<RANGE>::Type(){return v::word("RANGE");}
template <> WORD Array<LETTER>::Type(){return v::word("LETTER");}
template <> WORD Array<BYTE>::Type(){return v::word("BYTE");}
template <> WORD Array<INT_S>::Type(){return v::word("INT_S");}
template <> WORD Array<INT_W>::Type(){return v::word("INT_W");}
template <> WORD Array<INT_M>::Type(){return v::word("INT_M");}
template <> WORD Array<INT_L>::Type(){return v::word("INT_L");}
template <> WORD Array<INT_T>::Type(){return v::word("INT_T");}
template <> WORD Array<INT_B>::Type(){return v::word("INT_B");}
template <> WORD Array<FLOAT>::Type(){return v::word("FLOAT");}
template <> WORD Array<DOUBLE>::Type(){return v::word("DOUBLE");}
template <> WORD Array<POINTER>::Type(){return v::word("POINTER");}

template <>
class Array<STRING> {
private:
	Array<POINTER> apr;
public:
	WORD Class(){return v::word("Array");}
	WORD Type(){return v::word("STRING");}
	INT_L Size(){return this->apr.Size();}
	INT_L Total(){return this->apr.Total();}
	INT_L Index(){return this->apr.Index();}
	INT_L Reserve(){return this->apr.Reserve();}
	void Reserve(INT_L rv){
		this->apr.Reserve(rv);
	}
	void How(LOGIC how){
		this->apr.How(how);
	}
	void Index(INT_L it){
		this->apr.Index(it);
	}
	void Reset(){
		this->apr.Reset();
	}
	LOGIC Next(){
		return this->apr.Next();
	}
	STRING Take(INT_L index){
		POINTER res=this->apr.Take(index);
		return res?*(STRING*)res:STRING("");
	}
	void Set(STRING &str){
		this->apr.Set(&str);
	}
	STRING Get(INT_L index=0){
		POINTER res=this->apr.Get(index);
		return res?*(STRING*)res:STRING("");
	}
	void Put(STRING &str){
		this->apr.Put(&str);
	}
	void Put(WORD(ltr)){
		STRING *str=new STRING(ltr);
		this->apr.Put(str);
	}
	INT_L Find(STRING str,INT_L index=0){
		INT_L res=0;
		if(this->apr.Size()){
			if(index!=0){
				INT_L tit=this->apr.Index();
				v::index(index,this->apr.Size());
				this->apr.Index(index);index=tit;
			}else{
				index=this->apr.Index();
				this->apr.Reset();
			}
			do{
				if(this->Get()==str){
					res=this->apr.Index();break;
				}
			}while(this->apr.Next());
			if(!res)this->apr.Index(index);
		}
		return res;
	}
};
*/
#define FILE_variables

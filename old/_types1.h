/*
Типы данных
*/
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
typedef float FLOAT;						// 1.8E-38..1.8E+38		32b точность 6 .3 знаков
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
struct Addrs {
	POINTER a1;
	POINTER a2;
};
struct DateTime {
	int seconds;
	int minutes;
	int hours;
	int day;
	int month;
	int year;
	int wday;
	int yday;
	int summer;
};
union Code {
	BYTE byte;
	Bits bit;
};
union Block {
	INT_B numb;
	Bytes byte;
};
union Udatetime {
	struct tm stm;
	DateTime dtm;
};

namespace t {
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
		if(!this->tl)this->Clear();
		else if(this->tl<sz)sz=this->tl;
		if(sz){
			if(!this->tl)this->str=new LETTER[INT_M(sz+1)];
			Addrs adr={(POINTER)this->str,pnt};
			t::copy(adr,sz);*(BYTE*)adr.a1=0;
		}
		this->sz=sz;
	}
	void Merge(POINTER pnt,INT_W sz){
		if(sz){
			if(this->sz){
				Addrs adr;
				INT_L sn=this->sz+sz;
				if(this->tl){
					if(sn>this->tl){
						sn=this->tl;sz=sn-this->sz;
					}
					adr.a1=this->str;t::shift(adr.a1,this->sz);
				}else{
					LETTER* str=new LETTER[sn+1];
					adr.a1=str;
					if(this->sz){
						adr.a2=this->str;
						t::copy(adr,this->sz);
						this->Clear();this->str=str;
					}
				}
				if(sz){
					adr.a2=pnt;t::copy(adr,sz);*(BYTE*)adr.a1=0;
				}
				this->sz=sn;
			}else this->Assign(pnt,sz);
		}
	}
	LOGIC Compare(POINTER pnt,INT_W sz){
		return this->sz==sz?t::compare(this->str,pnt,sz):false;
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
	STRING(const STRING &str){
		this->Assign(str.str,str.sz);
	}
	~STRING(){this->Clear();}
	operator bool(){return this->sz?true:false;}
	LETTER& operator[](INT_L nx){
		t::index(nx,this->sz);
		return this->str[nx];
	}
	STRING& operator=(CHARS(ltr)){
		this->Assign((POINTER)ltr,this->Count(ltr));
		return *this;
	}
	STRING& operator=(const STRING &str){
		this->Assign(str.str,str.sz);
		return *this;
	}
	STRING& operator+=(CHARS(ltr)){
		this->Merge(POINTER(ltr),this->Count(ltr));
		return *this;
	}
	STRING& operator+=(const STRING &str){
		this->Merge(str.str,str.sz);
		return *this;
	}
	LOGIC operator==(CHARS(ltr)){
		return this->Compare((POINTER)ltr,this->Count(ltr));
	}
	LOGIC operator==(const STRING &str){
		return this->Compare(str.str,str.sz);
	}
	LOGIC operator!=(CHARS(ltr)){
		return !this->Compare((POINTER)ltr,this->Count(ltr));
	}
	LOGIC operator!=(const STRING &str){
		return !this->Compare(str.str,str.sz);
	}
	LETTER* operator*(){return this->str;}
	INT_W Size(){return this->sz;}
	INT_W Total(){return this->tl;}
	friend std::ostream& operator<< (std::ostream &out,const STRING &obj){
		return obj.sz?out << obj.str:out << "NULL";
	}
};

class DATETIME {
private:
	time_t dtm;
public:
	DATETIME(time_t dtm=0){this->dtm=dtm;}
	operator time_t(){return this->dtm;}
	operator INT_B(){return this->dtm;}
	DATETIME& operator=(time_t val){
		this->dtm=val;return *this;
	}
	DATETIME& operator+=(time_t val){
		this->dtm+=val;return *this;
	}
	DATETIME& operator-=(time_t val){
		this->dtm-=val;return *this;
	}
	void Clock(){this->dtm=clock();}
	void Now(){this->dtm=time(NULL);}
	void Data(DateTime sdt){
		Udatetime udt;
		udt.dtm=sdt;
		this->dtm=mktime(&udt.stm);
	}
	DateTime Data(){
		Udatetime udt;
		udt.stm=*localtime(&this->dtm);
		return udt.dtm;
	}
	STRING Show(STRING fmt,BYTE sz=64){
		LETTER res[sz];
		strftime(res,sz,*fmt,localtime(&this->dtm));
		return STRING(res);
	}
	friend std::ostream& operator<< (std::ostream &out,const DATETIME &obj){
		return out << obj.dtm;
	}
};

#ifndef FILE_zests
#include "_zests.h"
#endif

namespace t {
	STRING isit(LOGIC var){return STRING("LOGIC");}
	STRING isit(LETTER var){return STRING("LETTER");}
	STRING isit(RANGE var){return STRING("RANGE");}
	STRING isit(BYTE var){return STRING("BYTE");}
	STRING isit(INT_S var){return STRING("INT_S");}
	STRING isit(INT_W var){return STRING("INT_W");}
	STRING isit(INT_M var){return STRING("INT_M");}
	STRING isit(INT_L var){return STRING("INT_L");}
	STRING isit(INT_T var){return STRING("INT_T");}
	STRING isit(INT_B var){return STRING("INT_B");}
	STRING isit(FLOAT var){return STRING("FLOAT");}
	STRING isit(DOUBLE var){return STRING("DOUBLE");}
	STRING isit(POINTER var){return STRING("POINTER");}
	STRING isit(DATETIME var){return STRING("DATETIME");}
	STRING isit(STRING var){return STRING("STRING");}
	template <class oCLASS>
	STRING isit(oCLASS &obj){
		if constexpr(z::isClass<oCLASS>)return obj.Class();
		else return STRING("OBJECT");
	}
	INT_W id(STRING stp){
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
		else if(stp=="POINTER")res=13;
		else if(stp=="DATETIME")res=14;
		else if(stp=="STRING")res=15;
		else res=0;
		return res;
	}
	template <typename dTYPE>
	INT_W id(dTYPE var){return id(isit(var));}
	template <typename dTYPE>
	LOGIC match(dTYPE var,STRING stp){return t::isit(var)==stp?true:false;}
	LOGIC match(STRING st1,STRING st2){return st1==st2?true:false;}
}

class Link {
private:
	STRING mtp;
	POINTER pnt=NULL;
	template<typename dTYPE>
	void Init(dTYPE &obj){
		this->mtp=t::isit(obj);this->pnt=(POINTER)&obj;
	}
	void Init(Link &obj){
		this->mtp=obj.mtp;this->pnt=obj.pnt;
	}
public:
	Link(){}
	template<typename dTYPE>
	Link(dTYPE &obj){this->Init(obj);}
	POINTER operator *(){return this->pnt;}
	template<typename dTYPE>
	Link& operator=(dTYPE &obj){this->Init(obj);return *this;}
	template <typename dTYPE>
	explicit operator dTYPE() const{return *(dTYPE*)this->pnt;}
	STRING Class(){return STRING("Link");}
	STRING Type(){return this->mtp;}
	void Clear(){this->mtp="";this->pnt=NULL;}
	friend std::ostream& operator<< (std::ostream &out,Link &obj){
		return out << "Link<" << obj.Type() << ">";
	}
};

class ANY {
private:
	STRING vtp;
	POINTER pnt=NULL;
	void Clear(){
		if(this->pnt){//std::cout <<"clear="<< ltr<<'_'<< this->tag<<'_'<<this->vtp << "=" << this[0] <<std::endl;
			switch(t::id(this->vtp)){
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
				case 13:	delete static_cast<POINTER*>(this->pnt);break;
				case 14:	delete static_cast<DATETIME*>(this->pnt);break;
				case 15:	delete static_cast<STRING*>(this->pnt);break;
				default:	delete static_cast<Link*>(this->pnt);
			}
			this->vtp="";this->pnt=NULL;
		}
	}
public:
	ANY(){}
	template <typename dTYPE>
	ANY(dTYPE val){*this=val;}
	~ANY(){this->Clear();}
	STRING tag;
	template <typename dTYPE>
	explicit operator dTYPE(){
		return this->vtp!="Link"?*(dTYPE*)this->pnt:(dTYPE)*(Link*)this->pnt;
	}
	template <typename dTYPE>
	z::Enable<z::isBaseA<dTYPE>,ANY&> operator=(dTYPE val){
		STRING ntp=t::isit(val);
		if(this->vtp!=ntp){this->Clear();
			this->vtp=ntp;this->pnt=new dTYPE(val);
		}else *(dTYPE*)this->pnt=val;
		return *this;
	}
	template <class oCLASS>
	z::Enable<!z::isBaseA<oCLASS>,ANY&> operator=(oCLASS &obj){
		STRING ntp=t::isit(obj);
		if(this->vtp!=ntp){this->Clear();
			this->vtp="Link";this->pnt=new Link(obj);
		}else *(Link*)this->pnt=obj;
		return *this;
	}
	ANY& operator=(CHARS(ltr)){
		//STRING *str=new STRING(ltr);*this=*str;
		*this=STRING(ltr);
		return *this;
	}
	ANY& operator=(ANY obj){
		switch(t::id(obj.Type())){
			case 1:	*this=(LOGIC)obj;break;
			case 2:	*this=(LETTER)obj;break;
			case 3:	*this=(RANGE)obj;break;
			case 4:	*this=(BYTE)obj;break;
			case 5:	*this=(INT_S)obj;break;
			case 6:	*this=(INT_W)obj;break;
			case 7:	*this=(INT_M)obj;break;
			case 8:	*this=(INT_L)obj;break;
			case 9:	*this=(INT_T)obj;break;
			case 10:	*this=(INT_B)obj;break;
			case 11:	*this=(FLOAT)obj;break;
			case 12:	*this=(DOUBLE)obj;break;
			case 13:	*this=(POINTER)obj;break;
			case 14:	*this=(DATETIME)obj;break;
			case 15:	*this=(STRING)obj;break;
			default:	*this=*(Link*)obj.pnt;
		}
		return *this;
	}
	STRING Type(){
		return this->vtp!="Link"?this->vtp:(*(Link*)this->pnt).Type();
	}
	friend std::ostream& operator<< (std::ostream &out,ANY &obj){
		if(obj.pnt){
			switch(t::id(obj.vtp)){
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
				case 13:	out << *(POINTER*)obj.pnt;break;
				case 14:	out << *(DATETIME*)obj.pnt;break;
				case 15:	out << *(STRING*)obj.pnt;break;
				default:	out << (*(Link*)obj.pnt).Type();
			}
		}else out << "NULL";
		return out;
	}
};

namespace t {
	STRING isit(ANY var){return STRING("ANY");}
}

#define FILE_types

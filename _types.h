/* Типы данных */
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
union DatetimeU {
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
	template <typename dTYPE=BYTE>
	void fill(POINTER &adr,INT_W size,dTYPE val=0){
		while(size>0){
			*(dTYPE*)adr=val;shift(adr,sizeof(dTYPE));size--;
		}
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
		}else{size=-size;
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
	void index(INT_W &index,INT_W size){
		if(index>0){
			if(index<=size)index--;
			else{index=~index+1;
				if(index<=size)index=size-index;
				else if(size>0)index=size-1;
				else index=0;
			}
		}
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
	INT_W volume(INT_W sz,INT_W mr){
		INT_L res=mr+(sz/mr)*mr;
		if(res>65535)res=65535;
		return res;
	}
	INT_L volume(INT_L sz,INT_W mr){
		INT_B res=mr+(sz/mr)*mr;
		if(res>4294967295)res=4294967295;
		return res;
	}
	INT_W lsize(CHARS(ltr)){
		INT_W sz=0;
		while(ltr[sz]!='\0'){
			sz++;if(sz==65535)break;
		}
		return sz;
	}
	
}

#ifdef FILE_interface
namespace i {
	using namespace std;
	LOGIC s=true;
	INT_W t=0;
	void tab(INT_W tb=0,LETTER sl=' '){
		INT_W szt=i::t+tb;
		if(szt){
			LETTER ltr[szt+1];POINTER pnt=ltr;
			t::fill(pnt,szt,(BYTE)sl);*(BYTE*)pnt=0;
			cout<<ltr;
		}
	}
}
#endif

class STRING {
struct Data {
	INT_W mr=0,sz=0,tl=0;
	LETTER* str=NULL;
};
	Data *mdt;
	LOGIC dbl=false;
	LETTER* Change(INT_W sz){
		LETTER* str=NULL;
		if(mdt->mr){
			INT_W ntx=t::volume(sz,mdt->mr);
			if(mdt->tl!=ntx){
				mdt->tl=ntx;str=new LETTER[INT_L(ntx+1)];
			}
		}
		return str;
	}
	void Assign(POINTER pnt,INT_W sz){
		if(sz){
			if(LETTER* str=this->Change(sz)){
				delete []mdt->str;mdt->str=str;
			}else{
				if(mdt->tl<sz)sz=mdt->tl;
			}
			Addrs adr={(POINTER)mdt->str,pnt};
			t::copy(adr,sz);*(BYTE*)adr.a1=0;
			if(mdt->sz!=sz)mdt->sz=sz;
		}
	}
	LOGIC Compare(POINTER pnt,INT_W sz){
		return mdt->sz==sz?t::compare(mdt->str,pnt,sz):false;
	}
public:
	STRING(const STRING &obj): mdt(obj.mdt),dbl(true){}
	STRING(INT_W tl=0,INT_W mr=0){
		this->mdt=new Data;if(!tl && !mr)mr=32;this->Init(tl,mr);
	}
	STRING(CHARS(ltr)){
		INT_W sz=t::lsize(ltr);
		this->mdt=new Data;this->Init(sz,sz?0:32);
		this->Assign((POINTER)ltr,sz);
	}
	~STRING(){if(!this->dbl){delete []mdt->str;delete this->mdt;}}
	operator bool(){return mdt->sz?true:false;}
	const LETTER* operator*(){return mdt->str;}
	LETTER& operator[](INT_L index){
		t::index(index,mdt->sz);
		return mdt->str[index];
	}
	STRING& operator=(const LETTER *ltr){
		this->Assign((POINTER)ltr,t::lsize(ltr));
		return *this;
	}
	STRING& operator=(const STRING &obj){
		this->Assign(obj.mdt->str,obj.mdt->sz);
		return *this;
	}
	STRING& operator+=(const STRING &obj){
		this->Put(obj);
		return *this;
	}
	STRING& operator-=(const STRING &obj){
		this->Cut(obj);
		return *this;
	}
	LOGIC operator==(const STRING &obj){
		return this->Compare(obj.mdt->str,obj.mdt->sz);
	}
	LOGIC operator!=(const STRING &obj){
		return !this->Compare(obj.mdt->str,obj.mdt->sz);
	}
	INT_W Size(){return mdt->sz;}
	INT_W Total(){return mdt->tl;}
	INT_W Reserve(){return mdt->mr;}
	//void Reserve(INT_W mr){mdt->mr=mr;}// додумать???
	void Init(INT_W tl=0,INT_W mr=0){
		if(this->dbl){
			mdt=new Data;dbl=false;
		}else if(mdt->str){
			delete []mdt->str;mdt->sz=0;
		}
		mdt->tl=tl;mdt->mr=mr;
		mdt->str=new LETTER[INT_L(tl+1)];
	}
	void Put(const STRING &obj,INT_W index=0){
		if(INT_W sz=obj.mdt->sz){
			if(INT_L(mdt->sz+sz)>(mdt->mr?65535:mdt->tl))
				sz=(mdt->mr?65535:mdt->tl)-mdt->sz;
			if(sz){
				Addrs adr;POINTER pnt;
				if(!index)index=mdt->sz;
				else t::index(index,mdt->sz);
				if(LETTER* str=this->Change(mdt->sz+sz)){
					adr.a1=(POINTER)str;adr.a2=(POINTER)mdt->str;
					if(index)t::copy(adr,index);
					pnt=adr.a2;
					adr.a2=(POINTER)obj.mdt->str;t::copy(adr,sz);
					adr.a2=pnt;t::copy(adr,mdt->sz-index);*(BYTE*)adr.a1=0;
					delete []mdt->str;mdt->str=str;
				}else{
					adr.a1=(POINTER)mdt->str;t::shift(adr.a1,mdt->sz);
					adr.a2=adr.a1;t::shift(adr.a1,sz);//pnt=adr.a1;t::shift(pnt);
					//std::cout<<"adr="<<pnt<<std::endl;
					*(BYTE*)adr.a1=0;t::copy(adr,-(mdt->sz-index));
					adr.a1=adr.a2;adr.a2=(POINTER)obj.mdt->str;t::copy(adr,sz);
				}
				mdt->sz+=sz;
			}
		}
	}
	void Cut(INT_W index,INT_W size){
		if(size && mdt->sz){
			t::index(index,mdt->sz);
			if(index+size>mdt->sz)size=mdt->sz-index;
			Addrs adr;
			if(LETTER* str=this->Change(mdt->sz-size)){
				adr.a1=(POINTER)str;adr.a2=(POINTER)mdt->str;
				if(index)t::copy(adr,index);
				t::shift(adr.a2,size);index+=size;
				t::copy(adr,mdt->sz-index);
				delete []mdt->str;mdt->str=str;
			}else{
				adr.a1=(POINTER)mdt->str;
				if(index)t::shift(adr.a1,index);
				adr.a2=adr.a1;t::shift(adr.a2,size);
				index+=size;t::copy(adr,mdt->sz-index);
			}
			*(BYTE*)adr.a1=0;mdt->sz-=size;
		}
	}
	void Cut(const STRING &obj,INT_W index=0){
		index=this->Find(obj,index);
		if(index)this->Cut(index,obj.mdt->sz);
	}
	INT_W Find(const STRING &obj,INT_W index=0){
		INT_W res=0;
		if(mdt->sz){
			t::index(index,mdt->sz);
			POINTER pse=mdt->str;
			if(index>0)t::shift(pse,index);
			for(INT_W lmx=mdt->sz-obj.mdt->sz;index<=lmx;index++){
				if(t::compare(pse,obj.mdt->str,obj.mdt->sz)){res=index+1;break;}
				t::shift(pse);
			}
		}
		return res;
	}
	friend std::ostream& operator<< (std::ostream &out,const STRING &obj){
		return obj.mdt->sz?out<<obj.mdt->str:out<<"NULL";
	}
};

class DATETIME {
private:
	time_t dtm;
public:
	DATETIME(time_t dtm=0){this->dtm=dtm;}
	DATETIME(const DATETIME &obj): dtm(obj.dtm){}
	operator *(){return (INT_B)this->dtm;}
	operator time_t(){return this->dtm;}
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
		DatetimeU udt;
		udt.dtm=sdt;
		this->dtm=mktime(&udt.stm);
	}
	DateTime Data(){
		DatetimeU udt;
		udt.stm=*localtime(&this->dtm);
		return udt.dtm;
	}
	STRING Format(STRING fmt,BYTE sz=64){
		LETTER res[sz];
		strftime(res,sz,*fmt,localtime(&this->dtm));
		return STRING(res);
	}
	friend std::ostream& operator<< (std::ostream &out,const DATETIME &odt){
#ifdef FILE_usersetup
		DATETIME obj(odt);
		return out<<obj.Format(u::formatDATETIME);
#else
		return out<<odt.dtm;
#endif
	}
};

#ifndef FILE_zests
#include "_zests.h"
#endif

ID_TYPE(1,LOGIC)
ID_TYPE(2,LETTER)
ID_TYPE(3,RANGE)
ID_TYPE(4,BYTE)
ID_TYPE(5,INT_S)
ID_TYPE(6,INT_W)
ID_TYPE(7,INT_M)
ID_TYPE(8,INT_L)
ID_TYPE(9,INT_T)
ID_TYPE(10,INT_B)
ID_TYPE(11,FLOAT)
ID_TYPE(12,DOUBLE)
ID_TYPE(13,POINTER)
ID_TYPE(14,DATETIME)
ID_TYPE(15,STRING)

namespace t {
	template<typename dTYPE>
	STRING isit(const dTYPE &var){return Name<dTYPE>;}
	STRING isit(CHARS(ltr)){return STRING("CHARS");}
	STRING hex(BYTE b) {
		LETTER s[]="0123456789ABCDEF";
		LETTER h[3]={s[b/16],s[b%16],'\0'};
		return STRING(h);
	}
	/*template <typename dTYPE>
	LOGIC match(dTYPE var,STRING stt){return t::isit(var)==stt?true:false;}
	template <typename dTYPE>
	LOGIC match(dTYPE var,INT_W idt){return Id<dTYPE>::id==idt?true:false;}*/
}

class Link {
private:
	INT_W idt,vsz;
	STRING vtp;
	POINTER pnt=NULL;
public:
	Link(){}
	template<typename dTYPE>
	Link(dTYPE &obj){*this=obj;}
	Link(const Link &obj):idt(obj.idt),vsz(obj.vsz),vtp(obj.vtp),pnt(obj.pnt){}
	POINTER operator *(){return this->pnt;}
	template <typename dTYPE>
	explicit operator dTYPE() const{return *(dTYPE*)this->pnt;}
	Link& operator=(Link &obj){
		this->idt=obj.idt;this->vtp=obj.vtp;this->pnt=obj.pnt;
		return *this;
	}
	template<typename dTYPE>
	Link& operator=(dTYPE &obj){
		this->idt=::Id<dTYPE>::id;this->vsz=sizeof(dTYPE);
		this->vtp=Name<dTYPE>;this->pnt=&obj;
		return *this;
	}
	INT_W Id(){return this->idt;}
	INT_W Size(){return this->vsz;}
	STRING Type(){return this->vtp;}
	void Clear(){this->idt=0;this->vtp="";this->pnt=NULL;}
};
#ifdef FILE_interface
namespace i {
	void o(const Link &olk,STRING msg=""){
		if(msg){tab();cout<<"# "<<msg<<endl;}
		Link obj(olk);
		tab();cout<<"Link<"<<obj.Type()<<'|'<<obj.Id()<<">{*"<<*obj<<'^'<<obj.Size()<<'}';
	}
}
#endif	
ID_TYPE(16,Link)

class ANY {
struct Data {
	INT_W idt;
	STRING vtp;
	POINTER pnt=NULL;
};
	Data *mdt;
	LOGIC dbl=false;
	void Init(){this->mdt=new Data;}
	void Clear(){
		if(mdt->pnt){
			switch(mdt->idt){
				case 1:	delete static_cast<LOGIC*>(mdt->pnt);break;
				case 2:	delete static_cast<LETTER*>(mdt->pnt);break;
				case 3:	delete static_cast<RANGE*>(mdt->pnt);break;
				case 4:	delete static_cast<BYTE*>(mdt->pnt);break;
				case 5:	delete static_cast<INT_S*>(mdt->pnt);break;
				case 6:	delete static_cast<INT_W*>(mdt->pnt);break;
				case 7:	delete static_cast<INT_M*>(mdt->pnt);break;
				case 8:	delete static_cast<INT_L*>(mdt->pnt);break;
				case 9:	delete static_cast<INT_T*>(mdt->pnt);break;
				case 10:	delete static_cast<INT_B*>(mdt->pnt);break;
				case 11:	delete static_cast<FLOAT*>(mdt->pnt);break;
				case 12:	delete static_cast<DOUBLE*>(mdt->pnt);break;
				case 13:	delete static_cast<POINTER*>(mdt->pnt);break;
				case 14:	delete static_cast<DATETIME*>(mdt->pnt);break;
				case 15:	delete static_cast<STRING*>(mdt->pnt);break;
				default:	delete static_cast<Link*>(mdt->pnt);
			}
			mdt->pnt=NULL;
		}
	}
public:
	ANY(){this->Init();}
	template <typename dTYPE>
	ANY(dTYPE val){this->Init();*this=val;}
	ANY(const ANY &obj): mdt(obj.mdt),dbl(true){}
	~ANY(){if(!this->dbl)delete this->mdt;}
	template <typename dTYPE>
	explicit operator dTYPE(){
		return mdt->idt!=16?*(dTYPE*)mdt->pnt:(dTYPE)*(Link*)mdt->pnt;
	}
	POINTER operator*(){return mdt->idt!=16?mdt->pnt:**(Link*)mdt->pnt;}
	template <typename dTYPE>
	z::Enable<z::isBaseA<dTYPE>,ANY&> operator=(dTYPE val){
		STRING ntp=Name<dTYPE>;
		if(mdt->vtp!=ntp){this->Clear();
			mdt->pnt=new dTYPE(val);
			mdt->vtp=ntp;mdt->idt=::Id<dTYPE>::id;
		}else *(dTYPE*)mdt->pnt=val;
		return *this;
	}
	template <class oCLASS>
	z::Enable<!z::isBaseA<oCLASS>,ANY&> operator=(oCLASS &obj){
		STRING ntp=Name<oCLASS>;
		if(mdt->vtp!=ntp){this->Clear();
			mdt->pnt=new Link(obj);
			mdt->vtp="Link";mdt->idt=16;
		}else *(Link*)mdt->pnt=obj;
		return *this;
	}
	ANY& operator=(CHARS(ltr)){
		STRING *str=new STRING(ltr);*this=*str;
		return *this;
	}
	ANY& operator=(ANY &obj){
		switch(obj.mdt->idt){
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
			default:	*this=*(Link*)obj.mdt->pnt;
		}
		return *this;
	}
	template <typename dTYPE>
	LOGIC operator==(dTYPE val){
		LOGIC res=false;
		if(mdt->idt==::Id<dTYPE>::id)
			if((dTYPE)*this==val)res=true;
		return res;
	}
	LOGIC operator==(CHARS(ltr)){
		LOGIC res=false;
		if(mdt->idt==15)
			if((STRING)*this==ltr)res=true;
		return res;
	}
	INT_W Id(){return mdt->idt!=16?mdt->idt:(*(Link*)mdt->pnt).Id();}
	STRING Type(){return mdt->idt!=16?mdt->vtp:(*(Link*)mdt->pnt).Type();}
	friend std::ostream& operator<< (std::ostream &out,const ANY &obj){
		if(obj.mdt->pnt){
			switch(obj.mdt->idt){
				case 1:	out<<(*(LOGIC*)obj.mdt->pnt?"true":"false");break;
				case 2:	out<<*(LETTER*)obj.mdt->pnt;break;
				case 3:	out<<*(RANGE*)obj.mdt->pnt;break;
				case 4:	out<<*(BYTE*)obj.mdt->pnt;break;
				case 5:	out<<*(INT_S*)obj.mdt->pnt;break;
				case 6:	out<<*(INT_W*)obj.mdt->pnt;break;
				case 7:	out<<*(INT_M*)obj.mdt->pnt;break;
				case 8:	out<<*(INT_L*)obj.mdt->pnt;break;
				case 9:	out<<*(INT_T*)obj.mdt->pnt;break;
				case 10:	out<<*(INT_B*)obj.mdt->pnt;break;
				case 11:	out<<*(FLOAT*)obj.mdt->pnt;break;
				case 12:	out<<*(DOUBLE*)obj.mdt->pnt;break;
				case 13:	out<<*(POINTER*)obj.mdt->pnt;break;
				case 14:	out<<*(DATETIME*)obj.mdt->pnt;break;
				case 15:	out<<*(STRING*)obj.mdt->pnt;break;
				default:	out<<(*(Link*)obj.mdt->pnt).Type();
			}
		}else out << "NULL";
		return out;
	}
};
#ifdef FILE_interface
namespace i {
	void o(const ANY &oan,STRING msg=""){
		if(msg){tab();cout<<"# "<<msg<<endl;}
		ANY obj(oan);
		tab();cout<<"ANY{"<<obj.Type()<<'{'<<obj<<"}}"<<endl;
	}
}
#endif	
ID_TYPE(20,ANY)

#define FILE_types

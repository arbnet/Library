/*
???? ??????
*/
#include "time.h"
#include <iostream>

typedef bool LOGIC;						// 0..1
typedef char LETTER;					// ?????
typedef __INT8_TYPE__ RANGE;		// -128..127
typedef __UINT8_TYPE__ BYTE;		// 0..255
typedef __INT16_TYPE__ INT_S;		// -32768..32767
typedef __UINT16_TYPE__ INT_W;	// 0..65535
typedef __INT32_TYPE__ INT_M;		// -2147483648..2147483647
typedef __UINT32_TYPE__ INT_L;	// 0..4294967295
typedef __INT64_TYPE__ INT_T;		// -9223372036854775808..9223372036854775807
typedef __UINT64_TYPE__ INT_B;	// 0..18446744073709551615
typedef float FLOAT;						// 1.8E-38..1.8E+38		32b ???????? 6 .3 ??????
typedef double DOUBLE;				// 2.2E-308..1.8E+308	64b	???????? 15 .6 ??????
typedef void* POINTER;				// ?????????

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
	void Merge(POINTER pnt,INT_W sz){
		if(sz){
			if(INT_L(mdt->sz+sz)>(mdt->mr?65535:mdt->tl))
				sz=(mdt->mr?65535:mdt->tl)-mdt->sz;
			if(sz){
				Addrs adr;
				if(LETTER* str=this->Change(mdt->sz+sz)){
					adr.a1=(POINTER)str;adr.a2=(POINTER)mdt->str;
					t::copy(adr,mdt->sz);delete []mdt->str;mdt->str=str;
				}else{
					adr.a1=(POINTER)mdt->str;t::shift(adr.a1,mdt->sz);
				}
				adr.a2=pnt;t::copy(adr,sz);*(BYTE*)adr.a1=0;
				mdt->sz+=sz;
			}
		}
	}
	LOGIC Compare(POINTER pnt,INT_W sz){
		return mdt->sz==sz?t::compare(mdt->str,pnt,sz):false;
	}
public:
	STRING(const STRING &obj): mdt(obj.mdt),dbl(true){}
	STRING(INT_W tl=0,INT_W mr=0){
		mdt=new Data;if(!tl && !mr)mr=32;this->Init(tl,mr);
	}
	STRING(CHARS(ltr)){
		INT_W sz=t::lsize(ltr);
		mdt=new Data;this->Init(sz);
		this->Assign((POINTER)ltr,sz);
	}
	~STRING(){if(!this->dbl){delete []mdt->str;delete mdt;}}
	operator bool(){return mdt->sz?true:false;}
	const LETTER* operator*(){return mdt->str;}
	LETTER& operator[](INT_L index){
		t::index(index,mdt->sz);
		return mdt->str[index];
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
		return out<<obj.dtm;
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
	STRING hex(BYTE b) {
		LETTER s[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
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
	INT_W idt;
	STRING vtp;
	POINTER pnt=NULL;
public:
	Link(){}
	template<typename dTYPE>
	Link(dTYPE &obj){*this=obj;}
	POINTER operator *(){return this->pnt;}
	template <typename dTYPE>
	explicit operator dTYPE() const{return *(dTYPE*)this->pnt;}
	Link& operator=(Link &obj){
		this->idt=obj.idt;this->vtp=obj.vtp;this->pnt=obj.pnt;
		return *this;
	}
	template<typename dTYPE>
	Link& operator=(dTYPE &obj){
		this->idt=::Id<dTYPE>::id;this->vtp=Name<dTYPE>;this->pnt=&obj;
		return *this;
	}
	INT_W Id(){return this->idt;}
	STRING Type(){return this->vtp;}
	void Clear(){this->idt=0;this->vtp="";this->pnt=NULL;}
	friend std::ostream& operator<< (std::ostream &out,Link &obj){
		return out<<"Link<"<<obj.vtp<<">{"<<obj.pnt<<'}';
	}
};
ID_TYPE(16,Link)

class ANY {
private:
	INT_W idt;
	STRING vtp;
	POINTER pnt=NULL;
	void Clear(){
		if(this->pnt){
			switch(this->idt){
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
			this->pnt=NULL;
		}
	}
public:
	ANY(){}
	template <typename dTYPE>
	ANY(const dTYPE &val){*this=val;}
	ANY(const ANY &obj): idt(obj.idt),vtp(obj.vtp),pnt(obj.pnt){}
	~ANY(){this->Clear();}
	template <typename dTYPE>
	explicit operator dTYPE(){//std::cout<<"2="<<Name<dTYPE><<"="<<(dTYPE)*(Link*)this->pnt<<std::endl;
		return this->idt!=16?*(dTYPE*)this->pnt:(dTYPE)*(Link*)this->pnt;
	}
	POINTER operator*(){return this->vtp!="Link"?this->pnt:**(Link*)this->pnt;}
	template <typename dTYPE>
	z::Enable<z::isBaseA<dTYPE>,ANY&> operator=(dTYPE val){
		STRING ntp=Name<dTYPE>;
		if(this->vtp!=ntp){this->Clear();
			this->pnt=new dTYPE(val);
			this->vtp=ntp;this->idt=::Id<dTYPE>::id;
		}else *(dTYPE*)this->pnt=val;
		return *this;
	}
	template <class oCLASS>
	z::Enable<!z::isBaseA<oCLASS>,ANY&> operator=(oCLASS &obj){
		STRING ntp=Name<oCLASS>;
		if(this->vtp!=ntp){this->Clear();
			this->pnt=new Link(obj);//std::cout<<"1="<<ntp<<"="<<&obj<<std::endl;
			this->vtp="Link";this->idt=::Id<Link>::id;
		}else *(Link*)this->pnt=obj;
		return *this;
	}
	ANY& operator=(CHARS(ltr)){
		*this=STRING(ltr);
		return *this;
	}
	ANY& operator=(ANY &obj){
		switch(obj.idt){
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
	template <typename dTYPE>
	LOGIC operator==(dTYPE val){
		LOGIC res=false;
		if(this->idt==::Id<dTYPE>::id)
			if((dTYPE)*this==val)res=true;
		return res;
	}
	LOGIC operator==(CHARS(ltr)){
		LOGIC res=false;
		if(this->idt==15)
			if((STRING)*this==ltr)res=true;
		return res;
	}
	INT_W Id(){return this->idt!=16?this->idt:(*(Link*)this->pnt).Id();}
	STRING Type(){return this->vtp!="Link"?this->vtp:(*(Link*)this->pnt).Type();}
	friend std::ostream& operator<< (std::ostream &out,ANY &obj){
		if(obj.pnt){
			switch(obj.idt){
				case 1:	out<<"LOGIC{"<<(*(LOGIC*)obj.pnt?"true":"false")<<'}';break;
				case 2:	out<<"LETTER{"<<*(LETTER*)obj.pnt<<'}';break;
				case 3:	out<<"RANGE{"<<*(RANGE*)obj.pnt<<'}';break;
				case 4:	out<<"BYTE{"<<*(BYTE*)obj.pnt<<'}';break;
				case 5:	out<<"INT_S{"<<*(INT_S*)obj.pnt<<'}';break;
				case 6:	out<<"INT_W{"<<*(INT_W*)obj.pnt<<'}';break;
				case 7:	out<<"INT_M{"<<*(INT_M*)obj.pnt<<'}';break;
				case 8:	out<<"INT_L{"<<*(INT_L*)obj.pnt<<'}';break;
				case 9:	out<<"INT_T{"<<*(INT_T*)obj.pnt<<'}';break;
				case 10:	out<<"INT_B{"<<*(INT_B*)obj.pnt<<'}';break;
				case 11:	out<<"FLOAT{"<<*(FLOAT*)obj.pnt<<'}';break;
				case 12:	out<<"DOUBLE{"<<*(DOUBLE*)obj.pnt<<'}';break;
				case 13:	out<<"POINTER{"<<*(POINTER*)obj.pnt<<'}';break;
				case 14:	out<<"DATETIME{"<<*(DATETIME*)obj.pnt<<'}';break;
				case 15:	out<<"STRING{"<<*(STRING*)obj.pnt<<'}';break;
				default:	out<<(*(Link*)obj.pnt).Type();
			}
		}else out << "NULL";
		return out;
	}
};
ID_TYPE(20,ANY)

#define FILE_types

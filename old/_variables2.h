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
typedef char* WORD;						// слово
typedef void* POINTER;				// указатель

#define WORD(ltr) const char ltr[]

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
	INT_W lsize(WORD(ltr)){
		INT_W sz=0;
		while(ltr[sz]!='\0'){
			sz++;if(sz==65535)break;
		}
		return sz;
	}
	WORD word(WORD(ltr)){
		INT_M sz=lsize(ltr);sz++;
		WORD wrd=new char[sz];
		Addrs adr={&wrd,&ltr};
		copy(adr,sz);
		return wrd;
	}
	LOGIC equal(WORD(st1),WORD(st2)){
		LOGIC res=false;INT_W sz=lsize(st1);
		if(sz==lsize(st2))
			if(v::compare((POINTER)st1,(POINTER)st2,sz))
				res=true;
		return res;
	}
}

class STRING{
private:
	INT_W sz=0;
	WORD str=NULL;
	void Clear(){
		if(this->str){
			delete []this->str;this->sz=0;this->str=NULL;
		}
	}
	void Create(POINTER plt,INT_W sz){
		this->str=new LETTER[sz];
		Addrs adr={this->str,plt};
		v::copy(adr,sz);*(BYTE*)adr.a1=0;
		this->sz=sz;
	}
	void Assign(POINTER plt,INT_W sz){
		if(!this->Compare(plt,sz)){
			if(this->sz!=sz){
				this->Clear();this->Create(plt,sz);
			}else{
				Addrs adr={this->str,plt};
				v::copy(adr,sz);
			}
		}
	}
	LOGIC Compare(POINTER plt,INT_W sz){
		LOGIC res=false;
		if(this->sz==sz)
			if(v::compare(this->str,plt,sz))
				res=true;
		return res;
	}
public:
	STRING(){};
	STRING(WORD(wrd)){
		this->Create(POINTER(wrd),v::lsize(wrd));
	}
	~STRING(){
		this->Clear();
	}
	STRING& operator=(WORD(wrd)){
		this->Assign(POINTER(wrd),v::lsize(wrd));
		return *this;
	}
	STRING& operator=(STRING &obj){
		this->Assign(*obj,obj.sz);
		return *this;
	}
	LOGIC operator==(WORD(wrd)){
		return this->Compare(POINTER(wrd),v::lsize(wrd));
	}
	LOGIC operator==(STRING &obj){
		return this->Compare(*obj,obj.sz);
	}
	POINTER operator*(){return this->str;}
	POINTER operator&(){return this;}
	INT_W Size(){return this->sz;}
	friend std::ostream& operator<< (std::ostream &out, const STRING &obj){
		if(obj.str)out << obj.str;else out << "NULL";
		return out;
	}
};

namespace v {
	WORD type(LOGIC &var){return word("LOGIC");}
	WORD type(RANGE &var){return word("RANGE");}
	WORD type(LETTER &var){return word("LETTER");}
	WORD type(BYTE &var){return word("BYTE");}
	WORD type(INT_S &var){return word("INT_S");}
	WORD type(INT_W &var){return word("INT_W");}
	WORD type(INT_M &var){return word("INT_M");}
	WORD type(INT_L &var){return word("INT_L");}
	WORD type(INT_T &var){return word("INT_T");}
	WORD type(INT_B &var){return word("INT_B");}
	WORD type(FLOAT &var){return word("FLOAT");}
	WORD type(DOUBLE &var){return word("DOUBLE");}
	WORD type(POINTER &var){return word("POINTER");}
	WORD type(STRING &var){return word("STRING");}
	template <class nCLASS>
	WORD type(nCLASS &obj){return word("OBJECT");}
}

class DIFFERENT {
private:
	WORD vtp=NULL;
	POINTER pnt=NULL;
	LOGIC IsType(WORD(wrd)){
		return v::equal(this->vtp,wrd);
	}
	void Clear(){
		if(this->vtp){
			if(v::equal(this->vtp,"LOGIC"))delete static_cast<LOGIC*>(this->pnt);
			else if(this->IsType("LETTER"))delete static_cast<WORD>(this->pnt);
			else if(this->IsType("RANGE"))delete static_cast<RANGE*>(this->pnt);
			else if(this->IsType("BYTE"))delete static_cast<BYTE*>(this->pnt);
			else if(this->IsType("INT_S"))delete static_cast<INT_S*>(this->pnt);
			else if(this->IsType("INT_W"))delete static_cast<INT_W*>(this->pnt);
			else if(this->IsType("INT_M"))delete static_cast<INT_M*>(this->pnt);
			else if(this->IsType("INT_L"))delete static_cast<INT_L*>(this->pnt);
			else if(this->IsType("INT_T"))delete static_cast<INT_T*>(this->pnt);
			else if(this->IsType("INT_B"))delete static_cast<INT_B*>(this->pnt);
			else if(this->IsType("FLOAT"))delete static_cast<FLOAT*>(this->pnt);
			else if(this->IsType("DOUBLE"))delete static_cast<DOUBLE*>(this->pnt);
			else if(this->IsType("STRING"))delete static_cast<STRING*>(this->pnt);
			else delete static_cast<POINTER*>(this->pnt);
			this->vtp=NULL;this->pnt=NULL;
		}
	}
public:
	DIFFERENT(){};
	template <typename dTYPE>
	DIFFERENT(dTYPE val){*this=val;}
	~DIFFERENT(){this->Clear();}
	template <typename dTYPE>
	operator dTYPE() const{return *static_cast<dTYPE*>(this->pnt);}
	template <typename dTYPE>
	DIFFERENT& operator=(dTYPE val){
		this->Clear();
		this->vtp=v::type(val);
		this->pnt=new dTYPE(val);
		return *this;
	}
	/*DIFFERENT& operator=(WORD(ltr)){
		this->Clear();
		STRING *str=new STRING(ltr);
		this->vtp=v::type(*str);this->pnt=str;
		return *this;
	}*/
	DIFFERENT& operator=(DIFFERENT &obj){
		this->Clear();
		this->vtp=v::word(obj.Type());
		if(v::equal(this->vtp,"LOGIC"))this->pnt=new LOGIC(*(LOGIC*)obj.pnt);
		else if(v::equal(this->vtp,"LETTER"))this->pnt=new LETTER(*(WORD)obj.pnt);
		else if(v::equal(this->vtp,"RANGE"))this->pnt=new RANGE(*(RANGE*)obj.pnt);
		else if(v::equal(this->vtp,"BYTE"))this->pnt=new BYTE(*(BYTE*)obj.pnt);
		else if(v::equal(this->vtp,"INT_S"))this->pnt=new INT_S(*(INT_S*)obj.pnt);
		else if(v::equal(this->vtp,"INT_W"))this->pnt=new INT_W(*(INT_W*)obj.pnt);
		else if(v::equal(this->vtp,"INT_M"))this->pnt=new INT_M(*(INT_M*)obj.pnt);
		else if(v::equal(this->vtp,"INT_L"))this->pnt=new INT_L(*(INT_L*)obj.pnt);
		else if(v::equal(this->vtp,"INT_T"))this->pnt=new INT_T(*(INT_T*)obj.pnt);
		else if(v::equal(this->vtp,"INT_B"))this->pnt=new INT_B(*(INT_B*)obj.pnt);
		else if(v::equal(this->vtp,"FLOAT"))this->pnt=new FLOAT(*(FLOAT*)obj.pnt);
		else if(v::equal(this->vtp,"DOUBLE"))this->pnt=new DOUBLE(*(DOUBLE*)obj.pnt);
		else if(v::equal(this->vtp,"STRING"))this->pnt=new STRING(*(STRING*)obj.pnt);
		else this->pnt=new POINTER(*(POINTER*)obj.pnt);
		return *this;
	}
	WORD Type(){return this->vtp?this->vtp:v::word("undefined");}
	friend std::ostream& operator<< (std::ostream &out, const DIFFERENT &obj){
		if(obj.vtp){
			if(v::equal(obj.vtp,"LOGIC"))out << (*(LOGIC*)obj.pnt?"true":"false");
			else if(v::equal(obj.vtp,"LETTER"))out << *(WORD)obj.pnt;
			else if(v::equal(obj.vtp,"RANGE"))out << *(RANGE*)obj.pnt;
			else if(v::equal(obj.vtp,"BYTE"))out << *(BYTE*)obj.pnt;
			else if(v::equal(obj.vtp,"INT_S"))out << *(INT_S*)obj.pnt;
			else if(v::equal(obj.vtp,"INT_W"))out << *(INT_W*)obj.pnt;
			else if(v::equal(obj.vtp,"INT_M"))out << *(INT_M*)obj.pnt;
			else if(v::equal(obj.vtp,"INT_L"))out << *(INT_L*)obj.pnt;
			else if(v::equal(obj.vtp,"INT_T"))out << *(INT_T*)obj.pnt;
			else if(v::equal(obj.vtp,"INT_B"))out << *(INT_B*)obj.pnt;
			else if(v::equal(obj.vtp,"FLOAT"))out << *(FLOAT*)obj.pnt;
			else if(v::equal(obj.vtp,"DOUBLE"))out << *(DOUBLE*)obj.pnt;
			else if(v::equal(obj.vtp,"STRING"))out << *(STRING*)obj.pnt;
			else out << *(POINTER*)obj.pnt;
		}else out << "NULL";
		return out;
	}
};

namespace v {
	template <>
	WORD type(DIFFERENT &var){return word("DIFFERENT");}
}
template <typename dTYPE>
LOGIC isLOGIC(dTYPE var){return v::equal(v::type(var),"LOGIC");}
template <typename dTYPE>
LOGIC isRANGE(dTYPE var){return v::equal(v::type(var),"RANGE");}
template <typename dTYPE>
LOGIC isLETTER(dTYPE var){return v::equal(v::type(var),"LETTER");}
template <typename dTYPE>
LOGIC isBYTE(dTYPE var){return v::equal(v::type(var),"BYTE");}
template <typename dTYPE>
LOGIC isINT_S(dTYPE var){return v::equal(v::type(var),"INT_S");}
template <typename dTYPE>
LOGIC isINT_W(dTYPE var){return v::equal(v::type(var),"INT_W");}
template <typename dTYPE>
LOGIC isINT_M(dTYPE var){return v::equal(v::type(var),"INT_M");}
template <typename dTYPE>
LOGIC isINT_L(dTYPE var){return v::equal(v::type(var),"INT_L");}
template <typename dTYPE>
LOGIC isINT_T(dTYPE var){return v::equal(v::type(var),"INT_T");}
template <typename dTYPE>
LOGIC isINT_B(dTYPE var){return v::equal(v::type(var),"INT_B");}
template <typename dTYPE>
LOGIC isFLOAT(dTYPE var){return v::equal(v::type(var),"FLOAT");}
template <typename dTYPE>
LOGIC isDOUBLE(dTYPE var){return v::equal(v::type(var),"DOUBLE");}
template <typename dTYPE>
LOGIC isWORD(dTYPE var){return v::equal(v::type(var),"WORD");}
template <typename dTYPE>
LOGIC isSTRING(dTYPE var){return v::equal(v::type(var),"STRING");}
template <typename dTYPE>
LOGIC isOBJECT(dTYPE var){return v::equal(v::type(var),"OBJECT");}
template <typename dTYPE>
LOGIC isDIFFERENT(dTYPE var){return v::equal(v::type(var),"DIFFERENT");}

template <typename dTYPE>
class Link {
private:
	POINTER lnk=NULL;
public:
	Link(dTYPE &obj){this->lnk=(POINTER)&obj;}
	WORD Class(){return v::word("Link");}
	WORD Type(){return v::type(*(dTYPE*)this->lnk);}
	Link& operator=(dTYPE val){
		if(this->lnk)*(dTYPE*)this->lnk=val;
		return *this;
	}
	dTYPE& operator*(){return *(dTYPE*)this->lnk;}
	friend std::ostream& operator<< (std::ostream &out, const Link &obj){
		if(obj.lnk)out << *(dTYPE*)obj.lnk;else out << "NULL";
		return out;
	}
};

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

#define FILE_variables

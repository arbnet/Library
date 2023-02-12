#ifndef FILE_variables
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
typedef float FLOAT;						// 1.8E-38..1.8E+38		32b	точность 6 .3 знаков
typedef double DOUBLE;				// 2.2E-308..1.8E+308	64b	точность 15 .6 знаков
typedef void* POINTER;				// указатель

#define LETTERS(ltr) const char ltr[]

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

enum REMENT {Dec=-1,Inc=1};

template <typename dTYPE>
void Pshift(dTYPE* &pnt,INT_L shift=1){
	pnt+=shift;
}
void Pshift(POINTER &pnt,INT_L shift=1){
	pnt=(POINTER*)((INT_B)pnt+shift);
}

POINTER Mcopy(POINTER adr1,POINTER adr2,INT_L size){
	INT_L ct;
	ct=size/8;
	if(ct>0){size=size%8;
		do{
			*(INT_B*)adr1=*(INT_B*)adr2;
			Pshift(adr1,8);Pshift(adr2,8);ct--;
		}while(ct>0);
	}
	ct=size/4;
	if(ct>0){size=size%4;
		do{
			*(INT_L*)adr1=*(INT_L*)adr2;
			Pshift(adr1,4);Pshift(adr2,4);ct--;
		}while(ct>0);
	}
	ct=size/2;
	if(ct>0){size=size%2;
		do{
			*(INT_W*)adr1=*(INT_W*)adr2;
			Pshift(adr1,2);Pshift(adr2,2);ct--;
		}while(ct>0);
	}
	if(size>0){ct=size;
		do{
			*(BYTE*)adr1=*(BYTE*)adr2;
			Pshift(adr1,1);Pshift(adr2,1);ct--;
		}while(ct>0);
	}
	return adr1;
}

//void Fill(POINTER adr,INT_L size,auto val=0){
	
//}

LOGIC Mcompare(POINTER adr1,POINTER adr2,INT_L size){
	LOGIC res=true;
	while(size>0){
		if(size>=8){
			if(*(INT_B*)adr1!=*(INT_B*)adr2)res=false;
			else{Pshift(adr1,8);Pshift(adr2,8);size-=8;}
		}else if(size>=4){
			if(*(INT_L*)adr1!=*(INT_L*)adr2)res=false;
			else{Pshift(adr1,4);Pshift(adr2,4);size-=4;}
		}else if(size>=2){
			if(*(INT_W*)adr1!=*(INT_W*)adr2)res=false;
			else{Pshift(adr1,2);Pshift(adr2,2);size-=2;}
		}else{
			if(*(BYTE*)adr1!=*(BYTE*)adr2)res=false;
			else{Pshift(adr1,1);Pshift(adr2,1);size-=1;}
		}
		if(!res)break;
	}
	return res;
}

void Vindex(INT_L &index,INT_L size){
	if(index<=size)index--;
	else{index=~index+1;
		if(index<=size)index=size-index;
		else if(size>0)index=size-1;
		else index=0;
	}
}

class STRING {
private:
	INT_L sz=0;
	LETTER* str=NULL;
	void Clear(){
		if(this->str)delete []this->str;
	}
	INT_L Count(LETTERS(ltr)){
		INT_L ct=0;
		while(ltr[ct]!='\0')ct++;
		return ct;
	}
	void Create(LETTERS(ltr),INT_L sz=0){
		if(sz==0)sz=this->Count(ltr);
		this->sz=sz;sz++;
		this->str=new LETTER[sz];
		Mcopy(this->str,(POINTER)ltr,sz);
	}
	void Assign(LETTERS(ltr),INT_L sz=0){
		if(!this->Compare(ltr,sz)){
			this->Clear();
			if(sz==0)sz=this->Count(ltr);
			this->sz=sz;sz++;
			this->str=new LETTER[sz];
			Mcopy(this->str,(POINTER)ltr,sz);
		}
	}
	void Merge(LETTERS(ltr),INT_L sz=0){
		if(this->sz){
			if(sz==0)sz=this->Count(ltr);
			INT_L sn=this->sz+sz+1;
			LETTER* str=new LETTER[sn];
			POINTER pse=str;
			pse=Mcopy(pse,(POINTER)this->str,this->sz);
			pse=Mcopy(pse,(POINTER)ltr,sz);*(BYTE*)pse=0;
			delete []this->str;this->sz+=sz;this->str=str;
		}else this->Create(ltr,sz);
	}
	LOGIC Compare(LETTERS(ltr),INT_L sz=0){
		LOGIC res=false;
		if(sz==0)sz=this->Count(ltr);
		if(this->sz==sz)
			if(Mcompare(this->str,(POINTER)ltr,sz))
				res=true;
		return res;
	}
public:
	STRING(){}
	STRING(LETTERS(ltr)){
		this->Create(ltr);
	}
	~STRING(){
		this->Clear();
	}
	STRING Name(){return STRING("STRING");}
	LETTER &operator[](INT_L index){
		Vindex(index,this->sz);
		return this->str[index];
	}
	STRING &operator=(LETTERS(ltr)){
		this->Assign(ltr);
		return *this;
	}
	STRING &operator=(STRING &obj){
		this->Assign(obj.str,obj.sz);
		return *this;
	}
	STRING &operator+=(LETTERS(ltr)){
		this->Merge(ltr);
		return *this;
	}
	STRING &operator+=(STRING obj){
		this->Merge(obj.str,obj.sz);
		return *this;
	}
	LOGIC operator==(LETTERS(ltr)){
		return this->Compare(ltr);
	}
	LOGIC operator==(STRING &obj){
		return this->Compare(obj.str,obj.sz);
	}
	INT_L Size() {
		return this->sz;
	}
	friend std::ostream& operator<< (std::ostream &out, const STRING &obj){
		if(obj.str)out << obj.str;else out << "NULL";
		return out;
	}
};

template <typename dTYPE>
class LINK {
private:
	POINTER lnk=NULL;
public:
	LINK(dTYPE &obj){
		this->lnk=(POINTER)&obj;
	}
	STRING Name(){return STRING("LINK");}
	LINK &operator=(dTYPE val){
		if(this->lnk)*(dTYPE*)this->lnk=val;
		return *this;
	}
	friend std::ostream& operator<< (std::ostream &out, const LINK &obj){
		if(obj.lnk)out << *(dTYPE*)obj.lnk;else out << "NULL";
		return out;
	}
};

template <>
class LINK<POINTER> {
private:
	STRING name;
	POINTER lnk=NULL;
	template <class nCLASS>
	void Create(nCLASS &obj){
		this->name=obj.Name();
		this->lnk=(POINTER)&obj;
	}
public:
	LINK(){}
	template <class nCLASS>
	LINK(nCLASS &obj){
		this->Create(obj);
	}
	STRING Name(){return STRING("LINK");}
	template <class nCLASS>
	LINK &operator=(nCLASS &obj){
		this->Create(obj);
		return *this;
	}
	template <class nCLASS>
	nCLASS* operator->(){return this->lnk;}
	//template <typename nCLASS>
	//nCLASS& operator* (){return *this->lnk;}
};

template <typename dTYPE>
INT_L Size(dTYPE &var){return sizeof(&var);}

STRING Type(LOGIC &var){return STRING("LOGIC");}
STRING Type(RANGE &var){return STRING("RANGE");}
STRING Type(LETTER &var){return STRING("LETTER");}
STRING Type(BYTE &var){return STRING("BYTE");}
STRING Type(INT_S &var){return STRING("INT_S");}
STRING Type(INT_W &var){return STRING("INT_W");}
STRING Type(INT_M &var){return STRING("INT_M");}
STRING Type(INT_L &var){return STRING("INT_L");}
STRING Type(INT_T &var){return STRING("INT_T");}
STRING Type(INT_B &var){return STRING("INT_B");}
STRING Type(FLOAT &var){return STRING("FLOAT");}
STRING Type(DOUBLE &var){return STRING("DOUBLE");}
STRING Type(POINTER &var){return STRING("POINTER");}
template <class nCLASS>
STRING Type(nCLASS &obj){return obj.Name();}

template <typename dTYPE>
class Array {
protected:
	//STRING type;
	LOGIC how=true;
	dTYPE *vars=NULL;
	INT_L it=0,sz=0,tl=0,rv=0;
public:
	//Array();
	Array(INT_L rv=100,INT_L sz=0){
		this->sz=sz;this->rv=rv;this->tl=sz+rv;
		this->vars=new dTYPE[this->tl];
		this->it=this->how?0:this->sz;
		this->Fill();
	}
	virtual ~Array(){
		if(this->vars)delete this->vars;
		//if(this->type)delete this->type;
	}
	void Init(INT_L rv=100,INT_L sz=0){
		this->sz=sz;this->rv=rv;this->tl=sz+rv;
		this->vars=new dTYPE[this->tl];
		this->it=this->how?0:this->sz;
		this->Fill();
	}
	STRING Name(){return STRING("Array");}
	virtual STRING Type();
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
		Vindex(it,this->sz);
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
	LOGIC Type(LETTERS(stp)){
		LOGIC res=false;
		if(this->Type()==stp)res=true;
		return res;
	}
	dTYPE Take(INT_L index){
		Vindex(index,this->sz);
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
			else Vindex(index,this->sz);
			return this->vars[index];
		}else return 0;
	}
	void Put(dTYPE val,INT_L index=0){
		if(index!=0)Vindex(index,this->sz);
		else index=this->it;
		this->sz++;
		cout << "i=" << index << endl;
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
			else Vindex(index,this->sz);
			while(index<this->sz){
				if(this->vars[index]==val){res=index+1;break;}
				index+=rt;
			}
		}
		return res;
	}
	void Print(){
	INT_L sz=this->Size();
	cout << this->Type() << "(" << sz << "," << this->Total() << ")" << endl;
	for (INT_L nx=1;nx<sz+1;nx++)
		cout << nx <<  " = " << this->Get(nx) << endl;
	}//*(INT_W*)
};

template <> STRING Array<LOGIC>::Type(){return STRING("LOGIC");}
template <> STRING Array<RANGE>::Type(){return STRING("RANGE");}
template <> STRING Array<LETTER>::Type(){return STRING("LETTER");}
template <> STRING Array<BYTE>::Type(){return STRING("BYTE");}
template <> STRING Array<INT_S>::Type(){return STRING("INT_S");}
template <> STRING Array<INT_W>::Type(){return STRING("INT_W");}
template <> STRING Array<INT_M>::Type(){return STRING("INT_M");}
template <> STRING Array<INT_L>::Type(){return STRING("INT_L");}
template <> STRING Array<INT_T>::Type(){return STRING("INT_T");}
template <> STRING Array<INT_B>::Type(){return STRING("INT_B");}
template <> STRING Array<FLOAT>::Type(){return STRING("FLOAT");}
template <> STRING Array<DOUBLE>::Type(){return STRING("DOUBLE");}
template <> STRING Array<POINTER>::Type(){return STRING("POINTER");}

#define FILE_variables
#endif

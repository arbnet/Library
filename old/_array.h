/*
Массивы
*/
#ifndef FILE_types
#include "_types.h"
#endif

struct KeyValue {
	STRING key;
	ANY value;
};

template <typename dTYPE>
class Array {
protected:
	LOGIC dbl=false;
	LOGIC how=false;
	dTYPE *vars=NULL;
	INT_L it=0,sz=0,tl=0,rv=0;
	dTYPE& Add(INT_L index){
		if(index!=0)t::index(index,this->sz);
		else index=this->sz;
		this->sz++;
		if(this->sz>this->tl){
			this->tl+=this->rv;
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
		return this->vars[index];
	}
	dTYPE& Get(INT_L index){
		t::index(index,this->sz);
		return this->vars[index];
	}
public:
	Array(INT_L rv=10){
		this->tl=this->rv=rv;
		this->vars=new dTYPE[this->tl];
	}
	Array(const Array &obj): dbl(true),how(obj.how),vars(obj.vars),it(obj.it),sz(obj.sz),tl(obj.tl),rv(obj.rv){}
	~Array(){this->Clear();}
	void Clear(){
		if(this->vars){
			if(!this->dbl){delete []this->vars;this->dbl=false;}
			this->vars=NULL;this->it=this->sz=this->tl=this->rv=0;
		}
	}
	operator bool(){return this->sz?true:false;}
	dTYPE& operator[](INT_L index){
		return this->sz?this->Get(index):this->Add(index);
	}
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
		this->Index(how?-1:0);this->how=how;
	}
	void Index(INT_L it){
		t::index(it,this->sz);
		if(it<this->sz)this->it=it;
	}
	void Reset(){
		this->it=this->how?this->sz:0;
	}
	LOGIC Next(){
		LOGIC res=false;
		if(this->how){
			if(this->it>0){this->it--;res=true;}
		}else{
			if(this->it<this->sz-1){this->it++;res=true;}
		}
		return res;
	}
	void Put(dTYPE val,INT_L index=0){
		this->Add(index)=val;
	}
	dTYPE Take(INT_L index=-1){
		dTYPE val;
		if(this->sz){
			t::index(index,this->sz);
			val=this->vars[index];this->sz--;
			while(index<this->sz){
				this->vars[index]=this->vars[index+1];index++;
			}
		}else val=0;
		return val;
	}
#ifdef FILE_interface
	friend std::ostream& operator<< (std::ostream &out,const Array<dTYPE> &oar){
		Array<dTYPE> obj(oar);
		INT_L sz=obj.Size();
		i::tab();out <<"Array<"<<Name<dTYPE><<">("<<sz<<','<<obj.Total()<<','<<obj.Reserve()<<"){";
		if(sz){out<<std::endl;
			for (INT_L nx=1;nx<sz+1;nx++){i::tab(1);
				out <<'['<<nx<<"] "<<obj.Get(nx);
				if(obj.Index()==nx)out <<" <-";
				out<<std::endl;
			}
			i::tab();
		}
		out <<'}'<<std::endl;
		return out;
	}
#endif	
};
ID_TYPE(21,Array<LOGIC>)
ID_TYPE(22,Array<RANGE>)
ID_TYPE(23,Array<LETTER>)
ID_TYPE(24,Array<BYTE>)
ID_TYPE(25,Array<INT_S>)
ID_TYPE(26,Array<INT_W>)
ID_TYPE(27,Array<INT_M>)
ID_TYPE(28,Array<INT_L>)
ID_TYPE(29,Array<INT_T>)
ID_TYPE(30,Array<INT_B>)
ID_TYPE(31,Array<FLOAT>)
ID_TYPE(32,Array<DOUBLE>)
ID_TYPE(33,Array<POINTER>)
/*template <> STRING Array<LOGIC>::Type(){return STRING("LOGIC");}
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
*/
/*
class Args {
private:
	Array<POINTER> apr;
public:
	Args(){}
	template<typename... aARG>
	Args(aARG... args){
		(this->Put(args), ...);
	}
	~Args(){this->apr.Clear();}
	void Clear(){
		if(this->apr.Size()){this->apr.Reset();
			do{
				delete static_cast<ANY*>(this->apr.Get());
			}while(this->apr.Next());
			this->apr.Clear();
		}
	}
	INT_L Size(){return this->apr.Size();}
	INT_L Total(){return this->apr.Total();}
	INT_L Index(){return this->apr.Index();}
	INT_L Reserve(){return this->apr.Reserve();}
	void Reset(){this->apr.Reset();}
	void Index(INT_L ix){this->apr.Index(ix);}
	void How(LOGIC hw){this->apr.How(hw);}
	void Reserve(INT_L rv){this->apr.Reserve(rv);}
	LOGIC Next(){return this->apr.Next();}
	operator bool(){return this->apr.Size()?true:false;}
	ANY& operator[](INT_L index){
		if(!this->apr.Size()){
			ANY *vdt=new ANY;this->apr.Put((POINTER)vdt);
		}
		return *(ANY*)this->apr.Get(index);
	}
	//template<typename... aARG>
	//void Set(aARG... args){
	//	this->Clear();(this->Put(args), ...);
	//}
	template <typename dTYPE>
	void Put(dTYPE val){
		ANY *vdt=new ANY(val);this->apr.Put(vdt);
	}
	ANY Take(INT_L index=0){
		ANY res;
		if(this->apr.Size()){
			POINTER pnt=this->apr.Take(index);
			res=*(ANY*)pnt;
			delete static_cast<ANY*>(pnt);
		}
		return res;
	}
	template <typename dTYPE>
	INT_L Find(dTYPE val){
		INT_L res=0,idx=this->apr.Index();
		if(this->apr.Size()){this->apr.Reset();
			do{
				if((*this)[0]==val){
					res=this->apr.Index();break;
				}
			}while(this->apr.Next());
			if(!res)this->apr.Index(idx);
		}
		return res;
	}
#ifdef FILE_interface
	friend std::ostream& operator<< (std::ostream &out,Args &obj){
		i::tab();out<<"Args("<<obj.Size()<<"){";
		if(obj){
			ANY van;INT_L idx=obj.Index();
			obj.Reset();out<<std::endl;
			do{van=obj[0];
				i::tab(1);out<<'['<<obj.Index()<<"] "<<van.Type()<<'('<<van<<')'<<std::endl;
			}while(obj.Next());
			obj.Index(idx);
			i::tab();
		}
		out<<'}'<<std::endl;
		return out;
	}
#endif	
};
ID_TYPE(17,Args)

template <>
class Array<DATETIME> {
private:
	Array<POINTER> apr;
public:
	STRING Class(){return STRING("Array");}
	STRING Type(){return STRING("DATETIME");}
	INT_L Size(){return this->apr.Size();}
	INT_L Total(){return this->apr.Total();}
	INT_L Index(){return this->apr.Index();}
	INT_L Reserve(){return this->apr.Reserve();}
	void Reset(){this->apr.Reset();}
	void Index(INT_L ix){this->apr.Index(ix);}
	void How(LOGIC hw){this->apr.How(hw);}
	void Reserve(INT_L rv){this->apr.Reserve(rv);}
	LOGIC Next(){return this->apr.Next();}
	void Set(time_t val){
		if(this->apr.Size())*(DATETIME*)this->apr.Get()=val;
	}
	void Put(DATETIME val){
		DATETIME *dtm=new DATETIME(val);
		this->apr.Put(dtm);
	}
	DATETIME Get(INT_L index=0){
		return this->apr.Size()?*(DATETIME*)this->apr.Get(index):DATETIME();
	}
	DATETIME Take(INT_L index){
		DATETIME res;
		if(this->apr.Size()){
			POINTER pnt=this->apr.Take(index);
			res=*(DATETIME*)pnt;
			delete static_cast<DATETIME*>(pnt);
		}
		return res;
	}
};
ID_TYPE(34,Array<DATETIME>)

template <>
class Array<STRING> {
private:
	Array<POINTER> apr;
public:
	STRING separator;
	operator bool(){return this->apr.Size()?true:false;}
	STRING Class(){
		STRING res("Array");res+=this->Type();
		return res;
	}
	STRING Type(){return STRING("STRING");}
	INT_L Size(){return this->apr.Size();}
	INT_L Total(){return this->apr.Total();}
	INT_L Index(){return this->apr.Index();}
	INT_L Reserve(){return this->apr.Reserve();}
	void Reset(){this->apr.Reset();}
	void Index(INT_L ix){this->apr.Index(ix);}
	void How(LOGIC hw){this->apr.How(hw);}
	void Reserve(INT_L rv){this->apr.Reserve(rv);}
	LOGIC Next(){return this->apr.Next();}
	void Set(CHARS(ltr),INT_L index=0){
		if(this->apr.Size())*(STRING*)this->apr.Get(index)=ltr;
	}
	void Put(const STRING &str){
		this->apr.Put(new STRING(str));
	}
	void Put(CHARS(ltr)){
		STRING *str=new STRING(ltr);this->apr.Put(str);
	}
	STRING Get(INT_L index=0){
		return this->apr.Size()?*(STRING*)this->apr.Get(index):STRING("");
	}
	STRING Take(INT_L index=0){
		STRING res;
		if(this->apr.Size()){
			POINTER pnt=this->apr.Take(index);
			res=*(STRING*)pnt;
			delete static_cast<STRING*>(pnt);
		}
		return res;
	}
	STRING Text(){
		STRING res;
		INT_L tmp=this->apr.Index();
		if(this->Size()){this->Reset();
			do{
				res+=this->separator;
				res+=this->Get();
			}while(this->Next());
		}
		this->apr.Index(tmp);
		return res;
	}
#ifdef FILE_interface
	friend std::ostream& operator<< (std::ostream &out,Array<STRING> &obj){
		i::tab();out<<"Array<STRING>("<<obj.Size()<<"){";
		if(obj){
			INT_L tix=obj.Index();
			obj.Reset();out<<std::endl;
			do{
				i::tab(1);out<<'['<<obj.Index()<<"] "<<obj.Get()<<std::endl;
			}while(obj.Next());
			obj.Index(tix);i::tab();
		}
		out<<'}'<<std::endl;
		return out;
	}
#endif
};
ID_TYPE(35,Array<STRING>)

class Associative {
private:
	Array<POINTER> apr;
	ANY& Value(STRING key){
		INT_L index=this->Find(key);
		if(index)return (*(KeyValue*)this->apr.Get(index)).value;
		else{
			KeyValue *skv=new KeyValue;
			(*skv).key=key;this->apr.Put(skv);
			return (*skv).value;
		}
	}
public:
	Associative(){}
	Associative(const Associative &obj): apr(obj.apr){
		//Count(1);
	}
	~Associative(){this->Clear();}
	void Clear(){
		if(this->apr.Size()){
			//if(Count())Count(-1);
			//else{
				this->apr.Reset();
				do{
					delete static_cast<KeyValue*>(this->apr.Get());
				}while(this->apr.Next());
			//}
			this->apr.Clear();
		}
	}
	INT_L Size(){return this->apr.Size();}
	INT_L Total(){return this->apr.Total();}
	INT_L Index(){return this->apr.Index();}
	INT_L Reserve(){return this->apr.Reserve();}
	void Reset(){this->apr.Reset();}
	void Index(INT_L ix){this->apr.Index(ix);}
	void How(LOGIC hw){this->apr.How(hw);}
	void Reserve(INT_L rv){this->apr.Reserve(rv);}
	LOGIC Next(){return this->apr.Next();}
	//template <class oCLASS>
	//explicit operator oCLASS(){return *static_cast<oCLASS*>(**this->Value());}
	//operator bool(){return this->apr.Size()?true:false;}
	ANY& operator[](const STRING &key){
		return this->Value(key);
	}
	ANY& operator[](CHARS(ltr)){
		return this->Value(STRING(ltr));
	}
	STRING Key(INT_L index=0){
		return (*(KeyValue*)this->apr.Get(index)).key;
	}
	ANY& Value(INT_L index=0){
		return (*(KeyValue*)this->apr.Get(index)).value;
	}
	void Cut(STRING key){
		INT_L ix=this->Find(key);
		if(ix){
			POINTER pnt=this->apr.Take(ix);
			delete static_cast<KeyValue*>(pnt);
		}
	}
	INT_L Find(STRING key,INT_L index=0){
		INT_L res=0;
		if(this->apr.Size()){
			INT_L tmp=this->apr.Index();this->apr.Reset();
			do{//(*(KeyValue*)this->apr.Get(index)).key
				if(key==this->Key(index)){
					res=this->apr.Index();break;
				}
			}while(this->apr.Next());
			this->apr.Index(tmp);
		}
		return res;
	}
#ifdef FILE_interface
	friend std::ostream& operator<< (std::ostream &out,const Associative &oas){
		Associative obj(oas);
		i::tab();out<<"Associative("<<obj.Size()<<"){";
		if(obj.Size()){
			ANY van;INT_L idx=obj.Index();
			obj.Reset();out<<std::endl;
			do{van=obj.Value();
				i::tab(1);out<<'['<<obj.Index()<<"] "<<obj.Key()<<'=';
				switch(van.Id()){
					case 18: out<<(Associative)van;break;
					case 35: out<<(Array<STRING>)van;break;
					default: out<<van;
				}
				out<<std::endl;
			}while(obj.Next());
			obj.Index(idx);i::tab();
		}
		out<<'}'<<std::endl;
		return out;
	}
#endif	
	//operator void*(){
	//	std::cout<< "ZZZZZZZZZZ";
	//	return this;
	//}
};
ID_TYPE(18,Associative)
*/
#define FILE_array

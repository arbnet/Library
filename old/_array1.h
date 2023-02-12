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
	LOGIC how=false;
	dTYPE *vars=NULL;
	INT_L it=0,sz=0,tl=0,rv=0;
public:
	Array(INT_L rv=10){
		this->tl=this->rv=rv;
		this->vars=new dTYPE[this->tl];
	}
	~Array(){this->Clear();}
	STRING Class(){return STRING("Array");}
	STRING Type();
	void Clear(){
		if(this->vars){
			delete []this->vars;this->vars=NULL;
			this->it=this->sz=this->tl=this->rv=0;
		}
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
	void Set(dTYPE val){
		if(this->sz>0){
			this->vars[this->it]=val;
		}
	}
	void Put(dTYPE val,INT_L index=0){
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
		this->vars[index]=val;
	}
	dTYPE Get(INT_L index=0){
		if(this->sz>0){
			if(index==0)index=this->it;
			else t::index(index,this->sz);
			return this->vars[index];
		}else return 0;
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
	friend std::ostream& operator<< (std::ostream &out,const Array<dTYPE> &obj){
		return obj.sz?out << obj.str:out << "NULL";
	}
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

template <>
class Array<STRING> {
private:
	Array<POINTER> apr;
public:
	STRING separator;
	operator bool(){return this->apr.Size()?true:false;}
	STRING Class(){return STRING("Array");}
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
	friend std::ostream& operator<< (std::ostream &out,Array<STRING> &obj){

		out << "Array<STRING>{" << std::endl;
		if(obj){
			INT_L tix=obj.Index();
			obj.Reset();
			do{//if(t::t)out.width(t::t+1);
				out << "[" << obj.Index() << "] " << obj.Get() << std::endl;
			}while(obj.Next());
			obj.Index(tix);
		}else{
			//if(t::t)out.width(t::t+1);
			out << "empty" << std::endl;
		}
		//if(t::t)out.width(t::t);
		out << "}" << std::endl;
		return out;
	}
};

class Args {
private:
	Array<POINTER> apr;
	template <typename dTYPE>
	void Put(dTYPE val){
		ANY *vdt=new ANY(val);this->apr.Put(vdt);
	}
	template<typename dTYPE,typename... aARG>
	void Put(dTYPE val,aARG... args){
		this->Put(val);this->Put(args...);
	}
public:
	Args(){}
	template<typename dTYPE>
	Args(dTYPE val){this->Put(val);}
	template<typename dTYPE,typename... aARG>
	Args(dTYPE val,aARG... args){this->Put(val);this->Put(args...);}
	/*~Args(){
		if(this->apr.Size()){this->apr.Reset();
			do{
				//delete static_cast<ANY*>(this->apr.Get());
			}while(this->apr.Next());
		}
	}*/
	operator bool(){return this->apr.Size()?true:false;}
	STRING Class(){return STRING("Args");}
	INT_L Size(){return this->apr.Size();}
	INT_L Total(){return this->apr.Total();}
	INT_L Index(){return this->apr.Index();}
	INT_L Reserve(){return this->apr.Reserve();}
	void Reset(){this->apr.Reset();}
	void Index(INT_L ix){this->apr.Index(ix);}
	void How(LOGIC hw){this->apr.How(hw);}
	void Reserve(INT_L rv){this->apr.Reserve(rv);}
	LOGIC Next(){return this->apr.Next();}
	ANY& operator[](INT_L index){
		if(!this->apr.Size()){
			ANY *vdt=new ANY;this->apr.Put(vdt);
		}
		return *(ANY*)this->apr.Get(index);
	}
	/*ANY Take(INT_L index){
		ANY res;
		if(this->apr.Size()){
			POINTER pnt=this->apr.Take(index);
			res=*(ANY*)pnt;
			delete static_cast<ANY*>(pnt);
		}
		return res;
	}*/
};

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
	~Associative(){this->Clear();}
	void Clear(){
		if(this->apr.Size()){this->apr.Reset();
			do{
				delete static_cast<KeyValue*>(this->apr.Get());
			}while(this->apr.Next());
			this->apr.Clear();
		}
	}
	operator bool(){return this->apr.Size()?true:false;}
	STRING Class(){return STRING("Associative");}
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
	ANY& operator[](const STRING &key){
		return this->Value(key);
	}
	ANY& operator[](CHARS(ltr)){
		STRING key(ltr);
		return this->Value(key);
	}
	Associative& operator=(Associative obj){
		this->apr.Clear();this->apr=obj.apr;
		return *this;
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
};

#define FILE_array

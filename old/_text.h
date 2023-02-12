#ifndef FILE_variables
#include "_variables.h"
#endif

class TEXT {
private: 
	INT_W pt=32;
	INT_L sz=0,tl=0;
	LETTER* txt=NULL;
	void Clear(){
		if(this->txt)delete []this->txt;
	}
	void Create(POINTER plt,INT_W sz){
		this->tl=v::volume(sz,this->pt);
		this->txt=new LETTER[this->tl];
		Addrs adr={this->txt,plt};
		v::copy(adr,sz);*(BYTE*)adr.a1=0;
		this->sz=sz;
	}
	void Assign(POINTER plt,INT_W sz){
		if(!this->Compare(plt,sz)){
			if(this->tl==v::volume(sz,this->pt)){
				Addrs adr={this->txt,plt};
				v::copy(adr,sz);*(BYTE*)adr.a1=0;
				this->sz=sz;
			}else{
				this->Clear();this->Create(plt,sz);
			}
		}
	}
	void Merge(POINTER plt,INT_W sz){
		if(this->sz){
			Addrs adr;
			INT_L sn=this->sz+sz,tl=v::volume(sn,this->pt);
			if(tl!=this->tl){this->tl=tl;
				LETTER* txt=new LETTER[tl];
				adr.a1=txt;adr.a2=this->txt;
				v::copy(adr,this->sz);
				delete []this->txt;this->txt=txt;
			}else{
				adr.a1=this->txt;adr.a2=plt;
				v::shift(adr.a1,this->sz);
			}
			v::copy(adr,sz);*(BYTE*)adr.a1=0;
			this->sz=sn;
		}else this->Create(plt,sz);
	}
	void Eject(STRING str,INT_L index){
		index=this->Search(str,index);
		if(index)this->Cut(index,str.Size());
	}
	INT_L Search(STRING str,INT_L index){
		INT_L res=0;
		if(this->sz){
			v::index(index,this->sz);
			POINTER pse=this->txt;
			if(index>0)v::shift(pse,index);
			INT_L mx=this->sz-str.Size();
			while(index<mx){
				if(v::compare(pse,&str,str.Size())){
					res=index+1;break;
				}
				v::shift(pse);index++;
			}
		}
		return res;
	}
	LOGIC Compare(POINTER plt,INT_W sz){
		LOGIC res=false;
		if(this->sz==sz)
			if(v::compare(this->txt,plt,sz))
				res=true;
		return res;
	}
public:
	TEXT(INT_W pt=16){this->pt=pt;}
	TEXT(LETTERS(ltr)){
		this->Create(POINTER(ltr),v::lsize(ltr));
	}
	TEXT(STRING obj){
		this->Create(&obj,obj.Size());
	}
	~TEXT(){this->Clear();}
	LETTER& operator[](INT_L index){
		v::index(index,this->sz);
		return this->txt[index];
	}
	TEXT& operator=(LETTERS(ltr)){
		this->Assign(POINTER(ltr),v::lsize(ltr));
		return *this;
	}
	TEXT& operator=(TEXT &obj){
		this->Assign(&obj,obj.sz);
		return *this;
	}
	TEXT& operator+=(LETTERS(ltr)){
		this->Merge(POINTER(ltr),v::lsize(ltr));
		return *this;
	}
	TEXT& operator+=(TEXT &obj){
		this->Merge(&obj,obj.sz);
		return *this;
	}
	TEXT& operator-=(LETTERS(ltr)){
		this->Eject(STRING(ltr),0);
		return *this;
	}
	TEXT& operator-=(TEXT &obj){
		this->Eject(&obj,0);
		return *this;
	}
	LOGIC operator==(LETTERS(ltr)){
		return this->Compare(POINTER(ltr),v::lsize(ltr));
	}
	LOGIC operator==(TEXT &obj){
		return this->Compare(&obj,obj.sz);
	}
	LOGIC operator==(STRING &obj){
		return this->Compare(&obj,obj.Size());
	}
	INT_L Size(){return this->sz;}
	INT_L Total(){return this->tl;}
	INT_W Part(){return this->pt;}
	void Part(INT_W pt){this->pt=pt;}
	LETTER* operator&(){return this->txt;}
	
	INT_L Find(LETTERS(ltr),INT_L index=0){
		return this->Search(STRING(ltr),index);
	}
	INT_L Find(STRING str,INT_L index=0){
		return this->Search(str,index);
	}
	void Cut(INT_L index,INT_L size){
		if(size){v::index(index,this->sz);
			if(size<this->sz-index){
				Addrs adr;
				this->sz-=size;
				INT_L tl=v::volume(this->sz,this->pt);
				if(tl!=this->tl){this->tl=tl;
					LETTER* txt=new LETTER[tl];
					adr.a1=txt;adr.a2=this->txt;
					if(index)v::copy(adr,index);
					v::shift(adr.a2,size);
					tl=this->sz-index;v::copy(adr,tl);
					delete []this->txt;this->txt=txt;
				}else{
					adr.a1=this->txt;
					if(index)v::shift(adr.a1,index);
					adr.a2=adr.a1;v::shift(adr.a2,size);
					tl=this->sz-index;v::copy(adr,tl);
				}
				*(BYTE*)adr.a1=0;
			}
		}
	}
	void Put(STRING &str,INT_L index){
		v::index(index,this->sz);
		INT_W sz;
		Addrs adr;
		POINTER pnt;
		INT_L sn=this->sz+str.Size(),tl=v::volume(sn,this->pt);
		if(tl!=this->tl){this->tl=tl;
			LETTER* txt=new LETTER[tl];
			adr.a1=txt;adr.a2=this->txt;
			if(index)v::copy(adr,index);
			pnt=adr.a2;adr.a2=&str;
			v::copy(adr,str.Size());
			adr.a2=pnt;
			sz=this->sz-index;v::copy(adr,sz);
			delete []this->txt;this->txt=txt;
		}else{
			adr.a1=this->txt;v::shift(adr.a1,index);
			pnt=adr.a1;
			adr.a2=adr.a1;v::shift(adr.a1,str.Size());
			sz=this->sz-index;v::copy(adr,-sz);
			adr.a1=pnt;adr.a2=&str;
			v::copy(adr,str.Size());
		}
		this->sz=sn;
	}
	friend std::ostream& operator<< (std::ostream &out, const TEXT &obj){
		if(obj.txt)out << obj.txt;else out << "NULL";
		return out;
	}
};

//namespace t {}

#define FILE_text

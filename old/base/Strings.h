
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

/*
Изюминки для компиляции
*/
namespace z {
	template<bool> struct Logic;
	template<> struct Logic<true>{static const LOGIC v=true;};
	template<> struct Logic<false>{static const LOGIC v=false;};
	template <bool tBool, class oCLASS=void>
	struct IfType {};
	template <class oCLASS>
	struct IfType<true, oCLASS> {typedef oCLASS type;};
	template <bool tBool, class oCLASS=void >
	using Enable = typename IfType<tBool,oCLASS>::type;
	template <typename T1,typename T2>
	struct Match {static const bool v=false;};
	template <typename T0>
	struct Match<T0,T0> {static const bool v=true;};
	template <typename dTYPE, typename ...aTYPE>
	struct IsMatch{static constexpr bool v{(Match<dTYPE,aTYPE>::v || ...)};};
	template <typename dTYPE>
	constexpr bool isBaseA=IsMatch<dTYPE,LOGIC,LETTER,RANGE,BYTE,INT_S,INT_W,INT_M,INT_L,INT_T,INT_B,FLOAT,DOUBLE,POINTER,DATETIME,STRING>::v;
	template <typename dTYPE>
	constexpr bool isBaseB=IsMatch<dTYPE,LOGIC,LETTER,RANGE,BYTE,INT_S,INT_W,INT_M,INT_L,INT_T,INT_B,FLOAT,DOUBLE,POINTER>::v;
	template<class oCLASS>
	struct IsArray{static const bool v=false;};
	template<class oCLASS>
	struct IsArray<oCLASS[]> {static const bool v=true;};
	template<class oCLASS,INT_L sZ>
	struct IsArray<oCLASS[sZ]> {static const bool v=true;};
	template <typename dTYPE>
	class Ostream {
		template <typename dT>
		static auto test(BYTE)->decltype(std::declval<std::ostream&>()<<std::declval<dT>(),Logic<true>());
		template<typename>
		static auto test(...)->Logic<false>;
	public:
		Ostream()=delete;
		static constexpr bool v=decltype(test<dTYPE>(0))::v;
	};
}

template<INT_W> struct Type;
template<typename dTYPE> struct Id {static const INT_W id=0;};
template<typename dTYPE> constexpr LETTER Name[]="UNKNOWN";
#define ID_TYPE(nID,dTYPE) \
template<> struct Type<nID> {typedef dTYPE type;};\
template<> constexpr LETTER Name<dTYPE>[]=#dTYPE;\
template<> struct Id<dTYPE> {static const INT_W id=nID;};

template<class ostream,typename dTYPE> 
z::Enable<!z::Ostream<dTYPE>::v,ostream&> operator<<(ostream &out, const dTYPE& obj){
	return out<<Name<dTYPE>;
}

#define HAS_METHOD(mTD)\
template <class oCLASS, class=void>\
struct Has_##mTD {static const bool v=false;};\
template <class oCLASS>\
struct Has_##mTD<oCLASS,decltype(void(&oCLASS::mTD))> {static const bool v=true;};

#define FILE_zests

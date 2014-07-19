/** @file
 * テンプレートクラスの定義/実装
 * @date 2009/05/08
 * @author 杉野
 **/

#pragma once
#include "Exception.h"

namespace sugino{
namespace system{
	/** 0除算サンプル */
	CreateException( ZeroDivideException,"0で割れません" );
	
	/** 配列外参照 */
	CreateException( OutofArrayException,"配列外を参照しています" );

	/** NULL参照例外 */
	CreateException( NullPointerException,"NULL Pointer" );

	/** オートポインタテンプレート */
	template<class type>
	/** オートポインタクラス */
	class AutoPtr{
	private:
		type	*obj;
		//オートポインタ同士の代入を許可しない
		AutoPtr( const AutoPtr &p){ obj=NULL; }
		void operator = ( const AutoPtr &p ) {}
	public:
		AutoPtr() { obj = 0; }
		AutoPtr( type *p ){ obj=p; }
		~AutoPtr() { dispose(); }

		/** objがあるなら開放処理 */
		void dispose(){ 
			if(obj){
				delete obj;
				obj=NULL;
			}
		}
		
		AutoPtr &operator = ( type *p ){
		//	#if _DEBUG || DEBUG
		//		if(!obj) throw NullPointerException();
		//	#endif
			dispose();
			obj=p;
			return *this;
		}
		type *operator ->(){ 
			#if _DEBUG || DEBUG
				if(!obj) throw NullPointerException();
			#endif
			return obj;
		}
		type &operator  *(){ 
			#if _DEBUG || DEBUG
				if(!obj) throw NullPointerException();
			#endif
			return *obj; 
		}
		type **operator  &(){ return &obj; }
		operator type *(){ return obj; } //ポインタ型へのキャスト
	};

	/** 配列用テンプレート */
	template<class type>
	/** オート配列  */
	class AutoArray
	{
	private:
		type *obj;
		int length;		//要素数[]
		
		AutoArray( const AutoArray &p ){}
		void operator = ( const AutoArray &p ){}
	public:
		AutoArray(){ obj=NULL; length=0; }
		AutoArray( int size ){ obj=0; resize(size); }
		~AutoArray() { dispose(); }
		int size() const { return length; } 
		
		void resize( int size )
		{ 
			dispose();
			obj = new type[size];
			length=size;
		}

		void dispose()
		{
			if(obj){ delete []obj;obj=0;length=0; }
		}

		type &operator []( int index ){
			#if _DEBUG || DEBUG
				if(!obj) throw NullPointerException();
				if( ! (index>=0 && index<length) ) throw OutofArrayException();
			#endif
			return obj[index];
		}
		operator type *(){ return obj; }
	};
}}
/** @file
 * �e���v���[�g�N���X�̒�`/����
 * @date 2009/05/08
 * @author ����
 **/

#pragma once
#include "Exception.h"

namespace sugino{
namespace system{
	/** 0���Z�T���v�� */
	CreateException( ZeroDivideException,"0�Ŋ���܂���" );
	
	/** �z��O�Q�� */
	CreateException( OutofArrayException,"�z��O���Q�Ƃ��Ă��܂�" );

	/** NULL�Q�Ɨ�O */
	CreateException( NullPointerException,"NULL Pointer" );

	/** �I�[�g�|�C���^�e���v���[�g */
	template<class type>
	/** �I�[�g�|�C���^�N���X */
	class AutoPtr{
	private:
		type	*obj;
		//�I�[�g�|�C���^���m�̑���������Ȃ�
		AutoPtr( const AutoPtr &p){ obj=NULL; }
		void operator = ( const AutoPtr &p ) {}
	public:
		AutoPtr() { obj = 0; }
		AutoPtr( type *p ){ obj=p; }
		~AutoPtr() { dispose(); }

		/** obj������Ȃ�J������ */
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
		operator type *(){ return obj; } //�|�C���^�^�ւ̃L���X�g
	};

	/** �z��p�e���v���[�g */
	template<class type>
	/** �I�[�g�z��  */
	class AutoArray
	{
	private:
		type *obj;
		int length;		//�v�f��[]
		
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
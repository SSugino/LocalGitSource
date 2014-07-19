/** @file
 * ���w���̓N���X��`
 * @date 2009/05/25
 * @author ����
 **/
#pragma once

#include <math.h>
#include <float.h>

#include "../system/Application.h"
#include "../system/Template.h"

namespace sugino{
namespace math{

	/** math.h���b�p�[�N���X */
	class Math{
	public:
		/** sin�l
		 * @param[in] float r	:�@�p�x
		 * @return				:�@sin�l
		 **/
		static float sin( float r ){ return ::sinf( r ); }
		
		/** cos�l
		 * @param[in] float r	:�@�p�x
		 * @return				:�@cos�l
		 **/
		static float cos( float r ){ return ::cosf( r ); }
		
		/** ��v�Z
		 * @param[in] float r	:�@�p�x
		 * @return				:�@��l
		 **/
		static float sqrt( float r ){ return ::sqrtf( r ); }

		/** atan2
		 * @param [in] float y	:�@���W
		 * @param [in] float x	:
		 * @return			    :�@����
		 **/
		static float atan2( float y, float x ){ return ::atan2f( y,x ); }
		
		/** acos
		 * @param [in] float c  :�A�[�N�R�T�C���l
		 **/
		static float acos( float c ){ return ::acosf( c ); }
		
		/** rand 
		 * @return				: 0.0f�`1.0f
		 **/
		static float rand(){ return ::rand()/(float)RAND_MAX; }
		
		/** �� */
		static float const PI;
	};

	/** float�Ɋւ��鏈���͂������ōs���N���X( 0���Z�`�F�b�N�Ȃ�) */
	class Float{
	public:
		/** �t���ɂ��� @note�@������g�����Ƃɂ��0���Z���`�F�b�N���܂� */
		static float inv( float s ){
		#if _DEBUG || DEBUG
			if( s == 0 ) throw system::ZeroDivideException();
		#endif
			return 1.0f/s ;
		}

		/** Not a Number �`�F�b�N */
		static bool inNan( float f ) { return ::_isnan(f)!=0; }

		/** ��Βl */
		static float abs( float a ){ if( a<0 ) a=-a; return a; }

		/** atof�֐� NULL�����܂Ō��Ȃ� */
		static float Atof( const char *s )
		{ 
			char buf[32];						//32���炢����΂����񂶂�Ȃ�����
			for( int i=0;i<31 && *s;i++ ) { buf[i]=*s;s++ ;}
			return  (float)atof(buf);
		}
	};
}
}
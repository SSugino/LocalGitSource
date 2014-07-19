/** @file
 * 数学入力クラス定義
 * @date 2009/05/25
 * @author 杉野
 **/
#pragma once

#include <math.h>
#include <float.h>

#include "../system/Application.h"
#include "../system/Template.h"

namespace sugino{
namespace math{

	/** math.hラッパークラス */
	class Math{
	public:
		/** sin値
		 * @param[in] float r	:　角度
		 * @return				:　sin値
		 **/
		static float sin( float r ){ return ::sinf( r ); }
		
		/** cos値
		 * @param[in] float r	:　角度
		 * @return				:　cos値
		 **/
		static float cos( float r ){ return ::cosf( r ); }
		
		/** √計算
		 * @param[in] float r	:　角度
		 * @return				:　√値
		 **/
		static float sqrt( float r ){ return ::sqrtf( r ); }

		/** atan2
		 * @param [in] float y	:　座標
		 * @param [in] float x	:
		 * @return			    :　距離
		 **/
		static float atan2( float y, float x ){ return ::atan2f( y,x ); }
		
		/** acos
		 * @param [in] float c  :アークコサイン値
		 **/
		static float acos( float c ){ return ::acosf( c ); }
		
		/** rand 
		 * @return				: 0.0f～1.0f
		 **/
		static float rand(){ return ::rand()/(float)RAND_MAX; }
		
		/** π */
		static float const PI;
	};

	/** floatに関する処理はこここで行うクラス( 0除算チェックなど) */
	class Float{
	public:
		/** 逆数にする @note　これを使うことにより0除算をチェックします */
		static float inv( float s ){
		#if _DEBUG || DEBUG
			if( s == 0 ) throw system::ZeroDivideException();
		#endif
			return 1.0f/s ;
		}

		/** Not a Number チェック */
		static bool inNan( float f ) { return ::_isnan(f)!=0; }

		/** 絶対値 */
		static float abs( float a ){ if( a<0 ) a=-a; return a; }

		/** atof関数 NULL文字まで見ない */
		static float Atof( const char *s )
		{ 
			char buf[32];						//32ぐらい見ればいいんじゃないかと
			for( int i=0;i<31 && *s;i++ ) { buf[i]=*s;s++ ;}
			return  (float)atof(buf);
		}
	};
}
}
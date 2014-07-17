/** @file
 * 行列クラスの定義
 * @date 2009/05/29
 * @author 杉野
 **/
#pragma once

#include "../system/Window.h"
#include "Vector.h"

namespace sugino{
namespace math{

	class Matrix4x4;
	//省略したいから	
	typedef Matrix4x4 Matrix;	
	
	/** 3 × 3 行列 */
	class Matrix3x3
	{
	public:
		union{
			/** 行列 */
			float mat[3][3];
			struct{
				float m00,m01,m02;
				float m10,m11,m12;
				float m20,m21,m22;
				};
			};

			Matrix3x3(){};
			/** 初期化
			 * @param [in] float x00 :行列への代入値
			 * @param [in] float x01 :行列への代入値
			 * @param [in] float x02 :行列への代入値
			 * @param [in] float x10 :行列への代入値
			 * @param [in] float x11 :行列への代入値
			 * @param [in] float x12 :行列への代入値
			 * @param [in] float x20 :行列への代入値
			 * @param [in] float x21 :行列への代入値
			 * @param [in] float x22 :行列への代入値			
			 */
			Matrix3x3( float x00,float x01,float x02,
					   float x10,float x11,float x12,
					   float x20,float x21,float x22 );
			
			/** 回転・スケールを表すＹベクトルと位置ベクトルの行列の初期化　向き移動回転の初期化
			 * @param [out] Vector2 &up_axis : スケールＹベクトル (x,y)
			 * @param [out] Vector2 &pos	 : 位置ベクトル(x,y)
			 * @return *this				 : 初期化された行列	 
			 **/
			Matrix3x3 &Initialize( Vector2 &up_axis,Vector2& pos ) 
			{
				m00 = up_axis.y, m01=-up_axis.x, m02 = 0;
				m10 = up_axis.x, m11= up_axis.y, m12 = 0;
				m20 = pos.x,	 m21= pos.y ,	 m22 = 1;

				return *this;
			};
			
			/** 回転・スケールを表すＹベクトルとカメラ位置ベクトルの行列の初期化
			 * @param [out] Vector2 &ay		 : 回転逆行列 (x,y)
			 * @param [out] Vector2 &pos	 : 逆位置ベクトル(x,y)
			 * @return *this				 : 初期化された行列
			 **/
			Matrix3x3 &View( Vector2 &ay,Vector2& pos ) 
			{
				const Vector2 &ax = ay.RotateInvert90();
				m00 = ax.x, m01= ay.x , m02 = 0;
				m10 = ax.y, m11= ay.y , m12 = 0;
				
				m20 = -( pos.x*m00 + pos.y*m10 );		//反転
				m21 = -( pos.x*m01 + pos.y*m11 );
				m22 = 1;

				return *this;
			};

			/** 行列掛け合わせ(*) */
			//行列同士の掛け算は内容的には変換を加える事(足し算)になる
			Matrix3x3 operator * ( const Matrix3x3 &m ) const;
			
			/** 行列掛け合わせ(*=) */
			Matrix3x3 &operator *= ( const Matrix3x3 &m ){ return *this=*this *m; }

			/** 各メンバ足し算 */
			Matrix3x3 operator + ( const Matrix3x3 &m ) const;
			
			/** 各メンバ引き算 */
			Matrix3x3 operator - ( const Matrix3x3 &m ) const;
			
			/** 実数倍 */
			Matrix3x3 operator * ( const float f ) const;

			/** 単位行列 */
			Matrix3x3 &Identity() { return *this=unit_matrix; }

			/** 回転行列
			 * @param[in]	float r : ラジアン値
			 **/
			Matrix3x3 &Rotate( float r );

			/** スケーリング行列
			 * @param [in] float sx : スクリーン座標(0～1.0)
			 * @param [in] float sy : スクリーン座標(0～1.0)
			 **/ 
			Matrix3x3 &Scale( float sx,float sy );
			/** スケーリングベクトル行列
			 * @param[out] Vector2 &s : スクリーンベクトル座標(0～1.0)
			 * return Scale( s.x,s.y ): スクリーン座標
			 **/
			Matrix3x3 &Scale( const Vector2 &s ){ return Scale( s.x,s.y ); }

			/** スクリーン変換行列
			 * @param [in] int w	:画面幅
			 * @param [in] int h	:画面高さ
			 **/
			Matrix3x3 &Screen( int w,int h );

			/** 平行移動
			 * @param[in] float x : 平行移動座標先 x (0.0～1.0)
			 * @param[in] float y : 平行移動座標先 y (0.0～1.0)
			 **/
			Matrix3x3 &Translate( float x,float y );
			
			/** 平行移動
			 * @param[in] const Vector2 &v : ベクトル座標( x, y )
			 * @return 平行移動先のベクトル座標( v.x,v.y )
			 **/
			Matrix3x3 &Translate( const Vector2 &v ){ return Translate( v.x,v.y ); }
			
			/** 転置(対角に入れ替え)行列にする */
			Matrix3x3 &Transpose();
			
			/** 逆行列にする */
			Matrix3x3 &Inverse();

			/** ベクトル*行列
			 * 原点(0,0)からの原点変更
			 * @param [in] const Vector2 &v	: ベクトル(x,y)
			 * return ベクトルのスクリーン変換座標
			 **/
			inline Vector2 TransformVector( const Vector2 &v ) const
			{
				Vector2 v2;
				v2.x = v.x*m00 + v.y*m10 + m20;
				v2.y = v.x*m01 + v.y*m11 + m21;
	
				return v2;
			};

			/** ベクトル*回転行列( 2*2 )
			 * @param [in] const Vector2 &v
			 * return ベクトルの回転変換スクリーン座標
			 **/
			inline Vector2 TransformRotation( const Vector2 &v )
			{
				Vector2 v2;
				v2.x = v.x*m00 + v.y*m10;
				v2.y = v.x*m01 + v.y*m11;
	
				return v2;
			};

			/**  単位行列 */
			static const Matrix3x3 unit_matrix;
	};

	/** 4 * 4 行列 */
	class Matrix4x4
	{
	public:
		union{
			/** 行列 */
			float mat[4][4];
			struct{
				float m00,m01,m02,m03;
				float m10,m11,m12,m13;
				float m20,m21,m22,m23;
				float m30,m31,m32,m33;
				};
			};


			Matrix4x4(){};
			/** 初期化
			 * @param [in] float x00 :行列への代入値
			 * @param [in] float x01 :行列への代入値
			 * @param [in] float x02 :行列への代入値
			 * @param [in] float x03 :行列への代入値
			 * @param [in] float x10 :行列への代入値
			 * @param [in] float x11 :行列への代入値
			 * @param [in] float x12 :行列への代入値
			 * @param [in] float x13 :行列への代入値
			 * @param [in] float x20 :行列への代入値
			 * @param [in] float x21 :行列への代入値
			 * @param [in] float x22 :行列への代入値
			 * @param [in] float x23 :行列への代入値
			 * @param [in] float x30 :行列への代入値
			 * @param [in] float x31 :行列への代入値
			 * @param [in] float x32 :行列への代入値
			 * @param [in] float x33 :行列への代入値
			 */
			Matrix4x4( float x00,float x01,float x02,float x03,
					   float x10,float x11,float x12,float x13,
					   float x20,float x21,float x22,float x23,
					   float x30,float x31,float x32,float x33);
			
			/** クォータニオンから行列初期化(４元数) */
			//コンストラクタ
			Matrix4x4( Quaternion &q )
			{
				const float &x = q.v.x,&y = q.v.y,&z = q.v.z;
				const float &w = q.w;
				
				float x2 = x+x , y2 =y+y  , z2 =z+z;

				float xx = x*x2, yy =y*y2 , zz =z*z2;
				float xy = x*y2, xz =x*z2 , wx =w*x2;
				float yz = y*z2, wy =w*y2 , wz =w*z2;

				m00=1-(yy+zz);	m01=xy+wz;		m02=xz-wy;		m03=0;
				m10=xy-wz;		m11=1-(xx+zz);	m12=yz+wx;		m13=0;
				m20=xz+wy;		m21=yz-wx;		m22=1-(xx+yy);	m23=0;
				m30=0;			m31=0;			m32=0;			m33=1;
			}


			/** 回転・スケールを表す位置ベクトルの行列の初期化　向き移動回転の初期化
			 * @param [in] Vector3 &vec	 : 回転ベクトル
			 * @param [in] Vector3 &up	 : "仮"の上方向ベクトル
			 * @param [in] Vector3 &pos	 : 位置ベクトル(x,y)
			 * @return *this			 : 初期化された行列	 
			 **/
			
			Matrix4x4 &Initialize( Vector3 &rot_vec,Vector3 &up,Vector3& pos );

			/** カメラ行列
			 * @param [in] Vector3 &vec		 : Z方向ベクトル
			 * @param [in] Vector3 &up		 : 上方向ベクトル
			 * @param [in] Vector3 &pos		 : 逆位置ベクトル(x,y,z)
			 * @return *this				 : 初期化された行列
			 **/
			Matrix4x4 &View( Vector3 &zvec,Vector3 &up,Vector3& pos ) ;
			
			/**
			 * @param [in] Vector3 &pos		 : カメラ場所座標
			 * @param [in] Vector3 &at		 : 見てる先（対象）
			 * @param [in] Vector3 &up		 : 仮の上方向ベクトル
			 * @return *this				 : カメラ行列が初期化された行列				
			 **/
			Matrix4x4 LookAt( Vector3 &pos,Vector3 &at,Vector3 &up )
			{
				Vector3 v;
				v = at - pos;								//見る対象先 - カメラの位置
				v.Normalize();
				return View( v,up,pos );
			}

			/** 行列掛け合わせ(*) */
			//行列同士の掛け算は内容的には変換を加える事(足し算)になる
			Matrix4x4 operator * ( const Matrix4x4 &m ) const;
			
			/** 行列掛け合わせ(*=) */
			Matrix4x4 &operator *= ( const Matrix4x4 &m ){ return *this=*this *m; }

			/** 各メンバ足し算 */
			Matrix4x4 operator + ( const Matrix4x4 &m ) const;
			
			/** 各メンバ引き算 */
			Matrix4x4 operator - ( const Matrix4x4 &m ) const;
			
			/** 実数倍 */
			Matrix4x4 operator * ( const float f ) const;

			/** 単位行列 */
			Matrix4x4 &Identity() { return *this=unit_matrix; }

			/** Z軸回転行列
			 * @param[in]	float r : ラジアン値
			 * @note 度数指定は X/180 * PI
			 **/
			Matrix4x4 &RotateZ( float r );

			/** X軸回転行列
			 * @param[in]	float r : ラジアン値
			 * @note 度数指定は X/180 * PI
			 **/
			Matrix4x4 &RotateX( float r );

			/** Y軸回転行列
			 * @param[in]	float r : ラジアン値
			 * @note 度数指定は X/180 * PI
			 **/
			Matrix4x4 &RotateY( float r );
			
			/** 任意"点"で回転
			 * @param [in]	&rot	:回転行列
			 * @param [in]	&pos	:任意座標
			 **/
			Matrix4x4 &RotatePos( Matrix4x4 &rot,Vector3 &pos );

			/** 任意"軸"で回転
			 * @param [in] Vector &vz	:任意軸 (0,1,0)ならY軸 (1,0,0)ならX軸
			 * @param [in] float	r	:回転度数
			 * vzが1以上ならNormalize()すること！
			 **/
			Matrix4x4 &RotateAxis( Vector3 &vz, float r );
			
			/** スケーリング行列
			 * @param [in] float sx : スクリーン座標(0～1.0)
			 * @param [in] float sy : スクリーン座標(0～1.0)
	   		 * @param [in] float sz : スクリーン座標(0～1.0)
			 **/ 
			Matrix4x4 &Scale( float sx,float sy,float sz );
			
			/** スケーリングベクトル行列
			 * @param [in] Vector3 &s		: スクリーンベクトル座標(0～1.0)
			 * return Scale( s.x,s.y,s.z )	: スクリーン座標
			 **/
			Matrix4x4 &Scale( const Vector3 &s ){ return Scale( s.x,s.y,s.z ); }

			/** スクリーン変換行列(性能がいいほう)
			 * @param [in]	int x	:写す場所
			 * @param [in]	int y	:写す場所
			 * @param [in]	int w	:画面幅
			 * @param [in]	int h	:画面高さ
			 **/
			Matrix4x4 &Screen( int x,int y,int w,int h );
			
			/** スクリーン変換行列( Rectで渡す )
			 * @param [in] const Rect &r	:範囲指定
			 **/
			Matrix4x4 &Screen( const system::Rect &r  ){ return Screen(r.left,r.top,r.right,r.bottom); }
			
			/** スクリーン変換行列
			 * @param [in] int w	:画面幅
			 * @param [in] int h	:画面高さ
			 **/
			Matrix4x4 &Screen( int w,int h ) { return Screen( 0,0,w,h); }
			
			/** 平行移動
			 * @param[in] float x : 平行移動座標先 x (0.0～1.0)
			 * @param[in] float y : 平行移動座標先 y (0.0～1.0)
			 * @param[in] float z : 平行移動座標先 z (0.0～1.0)
			 **/
			Matrix4x4 &Translate( float x,float y,float z );
			
			/** 平行移動
			 * @param[in] const Vector3 &v : ベクトル座標( x,y,z )
			 * @return 平行移動先のベクトル座標( v.x,v.y,v.z )
			 **/
			Matrix4x4 &Translate( const Vector3 &v ){ return Translate( v.x,v.y,v.z ); }
			
			/** 転置(対角に入れ替え)行列にする */
			Matrix4x4 &Transpose();
			
			/** 逆行列にする */
			static Matrix4x4 Inverse( Matrix4x4 &m );
			
			/** 固有値を求める */
			float Determinant(){
				return(	mat[0][0] *Cofactor(0,0) + mat[0][1]*Cofactor(0,1)+
						mat[0][2] *Cofactor(0,2) + mat[0][3]*Cofactor(0,3));
			}

			/** 余因子を求める
			 * @attention 配列の参照と同じになるように(y,x)の順番にする
			 **/
			float Cofactor( int y,int x );
			
			/** ベクトル*行列
			 * 原点(0,0)からの原点変更
			 * @param [in] const Vector3 &v	: ベクトル(x,y,z)
			 * return ベクトルのスクリーン変換座標
			 **/
			inline Vector3 TransformVector( const Vector3 &v ) const
			{
				Vector3 v2;
				v2.x = v.x*m00 + v.y*m10 + v.z*m20 + m30;
				v2.y = v.x*m01 + v.y*m11 + v.z*m21 + m31;
				v2.z = v.x*m02 + v.y*m12 + v.z*m22 + m32;
	
				return v2;
			};
			
			/** 4次元ベクトルを結果として得る
			 *	@return	Vector4	:wを含むベクトル
			 **/
			inline Vector4 Transform( Vector3 &v ) const
			{
				Vector4 v2;
				v2.x = v.x*m00 + v.y*m10 + v.z*m20 + m30;
				v2.y = v.x*m01 + v.y*m11 + v.z*m21 + m31;
				v2.z = v.x*m02 + v.y*m12 + v.z*m22 + m32;
				v2.w = v.x*m03 + v.y*m13 + v.z*m23 + m33;

				return v2;
			};
			
			/** ベクトル*回転行列( 2*2 )
			 * @param [in] const Vector3 &v
			 * return ベクトルの回転変換スクリーン座標
			 **/
			inline Vector3 TransformRotation( Vector3 &v ) const
			{
				Vector3 v2;
				v2.x = v.x*m00 + v.y*m10 + v.z*m20;
				v2.y = v.x*m01 + v.y*m11 + v.z*m21;
				v2.z = v.x*m02 + v.y*m12 + v.z*m22;

				return v2;
			};

			/** プロジェクション行列（見たい範囲を決める)
			 * @param [in] float wid		:見たい幅
			 * @param [in] float hei		:見たい高さ
			 * @param [in] float zn			:near_plane	手前
			 * @param [in] float zf			:far_plane	奥行き
			 * 2:2:1:∞はパースがきつい 2:2:2:∞ は普通		∞はいくつでもいい
			 **/
			Matrix4x4 &Perspective( float wid, float hei,float zn, float zf );
			
			/** シャドウ行列(任意の高さで影の大きさを変化させる
			 * @param [in] float xs		:光線のX方向
			 * @param [in] float zs		:光線のY方向
			 * @param [in] float height	:対象の高さ(ローカル頂点までの高さ)
			 * @return 影の行列
			 **/
			Matrix4x4 &Shadow( float xs,float zs,float height ){
				m00 = 1,		m01= 0 , m02 = 0			,m03 = 0;
				m10 = xs,		m11= 0 , m12 = zs			,m13 = 0;
				m20 = 0,		m21= 0 , m22 = 1			,m23 = 0;
				m30 = -height*xs,	m31=0  , m32 = -height*zs	,m33 = 1;

				return *this;
			};

			/** シャドウ行列
			 * @param [in] float		xs		:光線のX方向
			 * @param [in] float		zs		:光線のY方向
			 * @param [in] float		height	:対象の高さ(ローカル頂点までの高さ)
			 * @param [in] Matrix4x4	&rot	:回転行列
			 * @return 影の行列
			 **/
			Matrix4x4 &Matrix4x4::Shadow( float xs,float zs, float height,Matrix4x4 &rot )
			{
				float xx = -height*xs;
				float yy = height;
				float zz = -height*zs;

				float t1,t2,t3,t4,t5,t6;

				t1 = rot.m00+rot.m10*xs;
				t2 = rot.m20+rot.m10*zs;
				t3 = rot.m01+rot.m11*xs;
				t4 = rot.m21+rot.m11*zs;
				t5 = rot.m02+rot.m12*xs;
				t6 = rot.m22+rot.m12*zs;

				float x = xx*rot.m00+yy*rot.m10+zz*rot.m20;
				float y = xx*rot.m01+yy*rot.m11+zz*rot.m21;
				float z = xx*rot.m02+yy*rot.m12+zz*rot.m22;

				m00=t1*rot.m00+t2*rot.m20; m01=t1*rot.m01+t2*rot.m21; m02=t1*rot.m02+t2*rot.m22; m03=0;
				m10=t3*rot.m00+t4*rot.m20; m11=t3*rot.m01+t4*rot.m21; m12=t3*rot.m02+t4*rot.m22; m13=0,
				m20=t5*rot.m00+t6*rot.m20; m21=t5*rot.m01+t6*rot.m21; m22=t5*rot.m02+t6*rot.m22; m23=0,
				m30=x;					   m31=y;					  m32=z;					 m33=1;

				return *this;
			}

			/** 反射行列
			 * @paran [in]	Vector3	&az			:Z軸
			 * @paran [in]	Vector3	&up			:上方向軸
			 * @paran [in]	Vector3	&s			:スケール
			 **/
			Matrix4x4 &Matrix4x4::Reflect( const Vector3 &az,const Vector3 &up,const Vector3 &s );
			
			/**  単位行列 */
			static const Matrix4x4 unit_matrix;
	};
}}
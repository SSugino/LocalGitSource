/** @file
 * �s��N���X�̎���
 * @date 2009/05/29
 * @author ����
 **/

#include "Vector.h"
#include "Matrix.h"
using namespace sugino;
using namespace math;

//�P�ʍs��
const Matrix3x3 Matrix3x3::unit_matrix( 1,0,0,
					0,1,0,
					0,0,1 );

const Matrix4x4 Matrix4x4::unit_matrix( 1,0,0,0,
					0,1,0,0,
					0,0,1,0,
					0,0,0,1 );


/** ������
 * @param [in] float x00 :�s��ւ̑���l
 * @param [in] float x01 :�s��ւ̑���l
 * @param [in] float x02 :�s��ւ̑���l
 * @param [in] float x10 :�s��ւ̑���l
 * @param [in] float x11 :�s��ւ̑���l
 * @param [in] float x12 :�s��ւ̑���l
 * @param [in] float x20 :�s��ւ̑���l
 * @param [in] float x21 :�s��ւ̑���l
 * @param [in] float x22 :�s��ւ̑���l			
 */
Matrix3x3::Matrix3x3(float x00, float x01, float x02, 
					 float x10, float x11, float x12,
					 float x20, float x21, float x22 )
{
	m00 = x00;	m01 = x01;	m02 = x02;	
	m10 = x10;	m11 = x11;	m12 = x12;
	m20 = x20;	m21 = x21;	m22 = x22;
}

Matrix3x3 Matrix3x3::operator *( const Matrix3x3 &m ) const
{
	Matrix3x3 mm;
	
	//����ň��
	mm.m00 = m00*m.m00 + m01*m.m10 + m02*m.m20;
	mm.m01 = m00*m.m01 + m01*m.m11 + m02*m.m21;
	mm.m02 = m00*m.m02 + m01*m.m12 + m02*m.m22;

	mm.m10 = m10*m.m00 + m11*m.m10 + m12*m.m20;
	mm.m11 = m10*m.m01 + m11*m.m11 + m12*m.m21;
	mm.m12 = m10*m.m02 + m11*m.m12 + m12*m.m22;

	mm.m20 = m20*m.m00 + m21*m.m10 + m22*m.m20;
	mm.m21 = m20*m.m01 + m21*m.m11 + m22*m.m21;
	mm.m22 = m20*m.m02 + m21*m.m12 + m22*m.m22;

	return mm;
}

//�e�s��p�l�����Z

Matrix3x3 Matrix3x3::operator +(const Matrix3x3 &m) const
{
	return Matrix3x3( m00+m.m00	,m01+m.m01,	m02+m.m02,
					  m10+m.m10	,m11+m.m11,	m12+m.m12,
					  m20+m.m20	,m21+m.m21,	m22+m.m22 );
}

Matrix3x3 Matrix3x3::operator -(const Matrix3x3 &m) const
{
	return Matrix3x3( m00-m.m00	,m01-m.m01,	m02-m.m02,
					  m10-m.m10	,m11-m.m11,	m12-m.m12,
					  m20-m.m20	,m21-m.m21,	m22-m.m22 );
}

Matrix3x3 Matrix3x3::operator *(const float f) const
{
	return Matrix3x3( m00*f	,m01*f	,m02*f,
					  m10*f	,m11*f	,m12*f,
					  m20*f	,m21*f	,m22*f );
}

/** ��]�s��
 * @param[in]	float r : ���W�A���l
 **/
Matrix3x3 &Matrix3x3::Rotate( float r )
{
	float c = Math::cos(r), s = Math::sin(r);
	m00 = c;	m01 = s;	m02 = 0;
	m10 =-s;	m11 = c;	m12 = 0;
	m20 = 0;	m21 = 0;	m22 = 1;

	return *this;
}

/** �X�P�[�����O�s��
 * @param [in] float sx : �X�N���[�����W(0�`1.0)
 * @param [in] float sy : �X�N���[�����W(0�`1.0)
 **/ 
Matrix3x3 &Matrix3x3::Scale(  float sx,float sy )
{
	m00 =  sx;	m01 = 0;	m02 = 0;
	m10 =  0;	m11 = sy;	m12 = 0;
	m20 =  0;	m21 = 0;	m22 = 1;
	
	return *this;
}

/** �X�N���[���ϊ��s��
 * @param [in] int w	:��ʕ�
 * @param [in] int h	:��ʍ���
 **/
Matrix3x3 &Matrix3x3::Screen( int x,int y )
{
	m00 =  y/2.0f;	m01 = 0;	m02 = 0;
	m10 =  0;	m11 = (-y/2.0f);	m12 = 0;
	m20 =  x/2.0f;	m21 = y/2.0f;	m22 = 1;
	
	return  *this;
}

/** ���s�ړ�
 * @param[in] float x : ���s�ړ����W�� x (0.0�`1.0)
 * @param[in] float y : ���s�ړ����W�� y (0.0�`1.0)
 **/
Matrix3x3 &Matrix3x3::Translate( float x,float y )
{
	m00 =  1;	m01 = 0;	m02 = 0;
	m10 =  0;	m11 = 1;	m12 = 0;
	m20 =  x;	m21 = y;	m22 = 1;

	return *this;
}

/** �]�u(�Ίp�ɓ���ւ�)�s��ɂ��� */
Matrix3x3 &Matrix3x3::Transpose()
{
	float tmp;					//����ւ��p�ϐ�
	tmp = m01;m01=m10;m10=tmp;
	tmp = m02;m02=m20;m20=tmp;
	tmp = m21;m21=m12;m12=tmp;

	return *this;
}

/** �t�s��ɂ��� */
Matrix3x3 &Matrix3x3::Inverse()
{
	float det = (m00*m11*m22 - m00*m12*m21) + (m01*m12*m20 - m01*m10*m22) + (m02*m10*m21 - m02*m11*m20);
	det=Float::inv( det );
	Matrix3x3 m;
	m.m00 = (m11*m22)-(m12*m21)	, m.m01 = (m21*m02)-(m22*m01) , m.m02 = (m01*m12)-(m02*m11);
	m.m10 = (m12*m20)-(m10*m22)	, m.m11 = (m22*m00)-(m20*m02) ,	m.m12 = (m02*m10)-(m00*m12);
	m.m20 = (m10*m21)-(m11*m20)	, m.m21 = (m20*m01)-(m21*m00) ,	m.m22 = (m00*m11)-(m01*m10);
	
	return *this = m*det;
}

/////////////////////////////////////////////////////////////////////////////////////
//																				/////
//			�S�~�S�s��															/////
//																				/////
/////////////////////////////////////////////////////////////////////////////////////
Matrix4x4::Matrix4x4(	float x00, float x01, float x02, float x03, 
			float x10, float x11, float x12, float x13,
			float x20, float x21, float x22, float x23,
			float x30, float x31, float x32, float x33
			)
{
	m00 = x00;	m01 = x01;	m02 = x02; m03 = x03;	
	m10 = x10;	m11 = x11;	m12 = x12; m13 = x13;
	m20 = x20;	m21 = x21;	m22 = x22; m23 = x23;
	m30 = x30;	m31 = x31;	m32 = x32; m33 = x33;
}


//�l�����Z

Matrix4x4 Matrix4x4::operator *( const Matrix4x4 &m ) const
{
	Matrix4x4 mm;
	
	//����ň��
	mm.m00 = m00*m.m00 + m01*m.m10 + m02*m.m20 + m03*m.m30;
	mm.m01 = m00*m.m01 + m01*m.m11 + m02*m.m21 + m03*m.m31;
	mm.m02 = m00*m.m02 + m01*m.m12 + m02*m.m22 + m03*m.m32;
	mm.m03 = m00*m.m03 + m01*m.m13 + m02*m.m23 + m03*m.m33;


	mm.m10 = m10*m.m00 + m11*m.m10 + m12*m.m20 + m13*m.m30;
	mm.m11 = m10*m.m01 + m11*m.m11 + m12*m.m21 + m13*m.m31;
	mm.m12 = m10*m.m02 + m11*m.m12 + m12*m.m22 + m13*m.m32;
	mm.m13 = m10*m.m03 + m11*m.m13 + m12*m.m23 + m13*m.m33;

	mm.m20 = m20*m.m00 + m21*m.m10 + m22*m.m20 + m23*m.m30;
	mm.m21 = m20*m.m01 + m21*m.m11 + m22*m.m21 + m23*m.m31;
	mm.m22 = m20*m.m02 + m21*m.m12 + m22*m.m22 + m23*m.m32;
	mm.m23 = m20*m.m03 + m21*m.m13 + m22*m.m23 + m23*m.m33;

	mm.m30 = m30*m.m00 + m31*m.m10 + m32*m.m20 + m33*m.m30;
	mm.m31 = m30*m.m01 + m31*m.m11 + m32*m.m21 + m33*m.m31;
	mm.m32 = m30*m.m02 + m31*m.m12 + m32*m.m22 + m33*m.m32;
	mm.m33 = m30*m.m03 + m31*m.m13 + m32*m.m23 + m33*m.m33;

	return mm;
}

//�l�����Z

Matrix4x4 Matrix4x4::operator +(const Matrix4x4 &m) const
{
	return Matrix4x4( m00+m.m00,m01+m.m01,m02+m.m02,m03+m.m03, 
			�@m10+m.m10,m11+m.m11,m12+m.m12,m13+m.m13,
			�@m20+m.m20,m21+m.m21,m22+m.m22,m23+m.m23,
			�@m30+m.m30,m31+m.m31,m32+m.m32,m33+m.m33 );
}

Matrix4x4 Matrix4x4::operator -(const Matrix4x4 &m) const
{
	return Matrix4x4( m00-m.m00,m01-m.m01,m02-m.m02,m03-m.m03,
			�@m10-m.m10,m11-m.m11,m12-m.m12,m13-m.m13,
			�@m20-m.m20,m21-m.m21,m22-m.m22,m23-m.m23,
			�@m30-m.m30 ,m31-m.m31,m32-m.m32, m33-m.m33 );
}

Matrix4x4 Matrix4x4::operator *(const float f) const
{
	return Matrix4x4( m00*f,m01*f,m02*f,m03*f, 
			�@m10*f,m11*f,m12*f,m13*f,
			�@m20*f,m21*f,m22*f,m23*f,
			�@m30*f,m31*f,m32*f,m33*f );
}

/** Z����]�s��
 * @param[in]	float r : ���W�A���l
 * @note �x���w��� X/180 * PI
 **/
Matrix4x4 &Matrix4x4::RotateZ( float r )
{
	float c = Math::cos(r), s = Math::sin(r);
	m00 = c;	m01 = s;	m02 = 0;	m03 = 0;
	m10 =-s;	m11 = c;	m12 = 0;	m13 = 0;
	m20 = 0;	m21 = 0;	m22 = 1;	m23 = 0;
	m30 = 0;	m31 = 0;	m32 = 0;	m33 = 1;


	return *this;
}

/** X����]�s��
 * @param[in]	float r : ���W�A���l
 * @note �x���w��� X/180 * PI
 **/
Matrix4x4 &Matrix4x4::RotateX( float r )
{
	float c = Math::cos(r), s = Math::sin(r);
	m00 = 1;	m01 = 0;	m02 = 0;	m03 = 0;
	m10 = 0;	m11 = c;	m12 = s;	m13 = 0;
	m20 = 0;	m21 = -s;	m22 = c;	m23 = 0;
	m30 = 0;	m31 = 0;	m32 = 0;	m33 = 1;


	return *this;
}

/** Y����]�s��
 * @param[in]	float r : ���W�A���l
 * @note �x���w��� X/180 * PI
**/
Matrix4x4 &Matrix4x4::RotateY( float r )
{
	float c = Math::cos(r), s = Math::sin(r);
	m00 = c;	m01 = 0;	m02 = -s;	m03 = 0;
	m10 = 0;	m11 = 1;	m12 = 0;	m13 = 0;
	m20 = s;	m21 = 0;	m22 = c;	m23 = 0;
	m30 = 0;	m31 = 0;	m32 = 0;	m33 = 1;


	return *this;
}

/** �C��"�_"�ŉ�]
 * @param [in]	&rot	:��]�s��
 * @param [in]	&pos	:�C�Ӎ��W
 **/
Matrix4x4 &Matrix4x4::RotatePos( Matrix4x4 &rot, Vector3 &pos )
{	
	float x	=-(rot.m00*pos.x+rot.m10*pos.y+rot.m20*pos.z)+pos.x;
	float y =-(rot.m01*pos.x+rot.m11*pos.y+rot.m21*pos.z)+pos.y;
	float z =-(rot.m02*pos.x+rot.m12*pos.y+rot.m22*pos.z)+pos.z;


	m00 = rot.m00,		m01= rot.m01, m02 = rot.m02 ,m03 = 0;
	m10 = rot.m10,		m11= rot.m11, m12 = rot.m12 ,m13 = 0;
	m20 = rot.m20,		m21= rot.m21, m22 = rot.m22 ,m23 = 0;
	m30 =	x	 ,		m31=	y 	, m32 =		z   ,m33 = 1;

	return *this;

}

/** �C��"��"�ŉ�]
 * @param [in] Vector &vz	:�C�ӎ� (0,1,0)�Ȃ�Y�� (1,0,0)�Ȃ�X��
 * @param [in] float	r	:��]�x��
 * vz��1�ȏ�Ȃ�Normalize()���邱�ƁI
 **/
Matrix4x4 &Matrix4x4::RotateAxis( Vector3 &vz, float r)
{
	float c = Math::cos( r );
	float ic = 1 - c;
	float s = Math::sin( r );
	const float &zx = vz.x;
	const float &zy = vz.y;
	const float &zz = vz.z;

	m00 = zx*zx*ic +c	,	m01= zx*zy*ic +zz*s,	m02 = zx*zz*ic -zy*s ,m03 = 0;
	m10 = zx*zy*ic -zz*s	,m11= zy*zy*ic +c,	m12 = zy*zz*ic +zx*s ,m13 = 0;
	m20 = zx*zz*ic +zy*s	,m21= zy*zz*ic -zx*s,	m22 = zz*zz*ic +c		,m23 = 0;
	m30 = 0 		,m31= 0 ,m32 =	0,m33 = 1;

	return *this;
}

/** �X�P�[�����O�s��
 * @param [in] float sx : �X�N���[�����W(0�`1.0)
 * @param [in] float sy : �X�N���[�����W(0�`1.0)
 * @param [in] float sz : �X�N���[�����W(0�`1.0)
**/ 
Matrix4x4 &Matrix4x4::Scale(  float sx,float sy,float sz )
{
	m00 =  sx;	m01 = 0;	m02 = 0;	m03 = 0;
	m10 =  0;	m11 = sy;	m12 = 0;	m13 = 0;
	m20 =  0;	m21 = 0;	m22 = sz;	m23 = 0;
	m30 =  0;	m31 = 0;	m32 = 0;	m33 = 1;
	
	return *this;
}

/** �X�N���[���ϊ��s��(���\�������ق�)
 * @param [in]	int x	:�ʂ��ꏊ
 * @param [in]	int y	:�ʂ��ꏊ
 * @param [in]	int w	:��ʕ�
 * @param [in]	int h	:��ʍ���
 **/
Matrix4x4 &Matrix4x4::Screen( int x,int y,int w,int h )
{
	float _w = (w-x) *0.5f;
	float _h = (h-y) *0.5f;
	float _x = x + _w;
	float _y = y + _h;

	m00 =  _w;	m01 = 0;	m02 = 0;	m03 = 0;
	m10 =  0;	m11 = -_h;	m12 = 0;	m13 = 0;
	m20 =  0;	m21 = 0;	m22 = 1;	m23 = 0;
	m30 =  _x;	m31 = _y;	m32 = 0;	m33 = 1;
	
	return  *this;
}

/** ���s�ړ�
 * @param[in] float x : ���s�ړ����W�� x (0.0�`1.0)
 * @param[in] float y : ���s�ړ����W�� y (0.0�`1.0)
 * @param[in] float z : ���s�ړ����W�� z (0.0�`1.0)
 **/
Matrix4x4 &Matrix4x4::Translate( float x,float y,float z )
{
	m00 =  1;	m01 = 0;	m02 = 0;	m03 = 0;
	m10 =  0;	m11 = 1;	m12 = 0;	m13 = 0;
	m20 =  0;	m21 = 0;	m22 = 1;	m23 = 0;
	m30 =  x;	m31 = y;	m32 = z;	m33 = 1;

	return *this;
}

/** �]�u(�Ίp�ɓ���ւ�)�s��ɂ��� */
Matrix4x4 &Matrix4x4::Transpose()
{
	float tmp;					//����ւ��p�ϐ�
	tmp = m01;m01=m10;m10=tmp;
	tmp = m02;m02=m20;m20=tmp;
	tmp = m03;m03=m30;m30=tmp;
	tmp = m21;m21=m12;m12=tmp;
	tmp = m31;m31=m13;m13=tmp;
	tmp = m32;m32=m23;m23=tmp;

	return *this;
}

/** ��]�E�X�P�[����\���ʒu�x�N�g���̍s��̏������@�����ړ���]�̏�����
 * @param [in] Vector3 &vec	 : ��]�x�N�g��
 * @param [in] Vector3 &up	 : "��"�̏�����x�N�g��
 * @param [in] Vector3 &pos	 : �ʒu�x�N�g��(x,y)
 * @return *this			 : ���������ꂽ�s��	 
 **/
Matrix4x4 &Matrix4x4::Initialize( Vector3 &rot_vec, Vector3 &up, Vector3 &pos)
{
	Vector ax,ay;
	ax = Vector::Cross( up , rot_vec );		//���v���ŉE�������o��
	ax.Normalize();						//sin�l�𐳋K��
	ay = Vector::Cross( rot_vec , ax );		//���v���Ő����ȏ�������o��

	m00 = ax.x,		m01= ax.y	,m02 = ax.z			,m03 = 0;
	m10 = ay.x,		m11= ay.y	,m12 = ay.z			,m13 = 0;
	m20 = rot_vec.x,	m21= rot_vec.y	,m22 = rot_vec.z		,m23 = 0;
	m30 = pos.x,		m31= pos.y	,m32 = pos.z			,m33 = 1;

	return *this;
}
/** �J�����s��
 * @param [in] Vector3 &vec		 : Z�����x�N�g��
 * @param [in] Vector3 &up		 : ������x�N�g��
 * @param [in] Vector3 &pos		 : �t�ʒu�x�N�g��(x,y,z)
 * @return *this				 : ���������ꂽ�s��
 **/
Matrix4x4 &Matrix4x4::View( Vector3 &zvec, Vector3 &up, Vector3 &pos )
{
	Vector ax,ay;
	ax = Vector::Cross( up , zvec );		//���v���ŉE�������o��
	ax.Normalize();						//sin�l�𐳋K��
	ay = Vector::Cross( zvec , ax );		//���v���Ő����ȏ�������o��

	float x,y,z;
	x = pos.x*ax.x	+ pos.y*ax.y	+ pos.z*ax.z;
	y = pos.x*ay.x	+ pos.y*ay.y	+ pos.z*ay.z;
	z = pos.x*zvec.x + pos.y*zvec.y	+ pos.z*zvec.z;

	m00 = ax.x,		m01= ay.x, m02 = zvec.x ,m03 = 0;
	m10 = ax.y,		m11= ay.y, m12 = zvec.y ,m13 = 0;
	m20 = ax.z,		m21= ay.z, m22 = zvec.z ,m23 = 0;
	m30 = -x,		m31= -y	 , m32 = -z	   ,m33 = 1;

	return *this;
}

/** �v���W�F�N�V�����s��i�������͈͂����߂�)
 * @param [in] float wid		:��������
 * @param [in] float hei		:����������
 * @param [in] float zn			:near_plane	��O
 * @param [in] float zf			:far_plane	���s��
 * 2:2:1:���̓p�[�X������ 2:2:2:�� �͕���		���͂����ł�����
 **/
Matrix4x4 &Matrix4x4::Perspective( float wid, float hei, float zn, float zf )
{
	//���炩���ߍ���Ă���
	float zn2 = zn*2;		//near*2
	float zw  = zn2/wid;	//near*2/width
	float zh  = zn2/hei;	//near*2/height
	float znf = zf/(zf-zn); //far / far-near
	float nf  = zn*znf;		//near*(far / far-near)

	m00 = zw,		m01= 0 , m02 = 0	,m03 = 0;
	m10 = 0,		m11= zh, m12 = 0	,m13 = 0;
	m20 = 0,		m21= 0 , m22 = znf	,m23 = 1;
	m30 = 0,		m31= 0 , m32 = -nf	,m33 = 0;
		
	return *this;
}

/** �t�s��ɂ��� */
Matrix4x4 Matrix4x4::Inverse( Matrix4x4 &m )
{
	float dtm = Float::inv( m.Determinant() );
	Matrix c;
	for( int i=0;i<4;i++ ){
	//�]�l�~�ŗL�l
		c.mat[i][0] = m.Cofactor( 0,i )*dtm;
		c.mat[i][1] = m.Cofactor( 1,i )*dtm;
		c.mat[i][2] = m.Cofactor( 2,i )*dtm;
		c.mat[i][3] = m.Cofactor( 3,i )*dtm;
	}
	return c;
}

/** �]���q�����߂�
 * @attention �z��̎Q�ƂƓ����ɂȂ�悤��(y,x)�̏��Ԃɂ���
 **/
float Matrix4x4::Cofactor( int y, int x )
{
	int sgn = 1;
	if( (y+x)&1 ) sgn =-1;
	int x1 = (x+1)&3,x2=(x+2)&3,x3=(x+3)&3;
	int y1 = (y+1)&3,y2=(y+2)&3,y3=(y+3)&3;

	return (+(mat[y1][x1] * mat[y2][x2] * mat[y3][x3] +
			  mat[y1][x2] * mat[y2][x3] * mat[y3][x1] +
			  mat[y1][x3] * mat[y2][x1] * mat[y3][x2] )
			-(mat[y3][x1] * mat[y2][x2] * mat[y1][x3] +
			  mat[y3][x2] * mat[y2][x3] * mat[y1][x1] +
			  mat[y3][x3] * mat[y2][x1] * mat[y1][x2] )) * sgn;
}

//���ˍs��
Matrix4x4 &Matrix4x4::Reflect( const Vector3 &az,const Vector3 &up,const Vector3 &s )
{
	//�������玲����蒼��
	Vector3 ax = Vector3::Cross( up,az ).Normalize();
 	Vector3 ay = Vector3::Cross( az,ax );

	//�]�u�����X�P�[�����O�s��
	Matrix4x4 m;
	m00 = ax.x*s.x;	m01 = ay.x*s.y;	m02 = az.x*s.z;	m03 = 0;
	m10 = ax.y*s.x;	m11 = ay.y*s.y;	m12 = az.y*s.z;	m13 = 0;
	m20 = ax.z*s.x;	m21 = ay.z*s.y;	m22 = az.z*s.z;	m23 = 0;
	m30 = 0;			m31 = 0,		m32= 0,			m33 = 1;

	//��]�s��
	m.m00 = ax.x;		m.m01 = ax.y;		m.m02 = ax.z;		m.m03 = 0;
	m.m10 = ay.x;		m.m11 = ay.y;		m.m12 = ay.z;		m.m13 = 0;
	m.m20 = az.x;		m.m21 = az.y;		m.m22 = az.z;		m.m23 = 0;
	m.m30 = 0;		m.m31 = 0,		m.m32= 0,			m.m33 = 1;

	*this *= m;
	return *this;

};
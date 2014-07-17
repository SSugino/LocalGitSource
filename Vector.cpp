/** @file
 * �֐��N���X�̎���
 * @date 2009/05/26
 * @author ����
 **/

#include "Vector.h"
using namespace sugino;
using namespace math;
/** zero��` */
const Vector2 Vector2::zero( 0,0 );

const Vector3 Vector3::zero( 0,0,0 );

/** ���ʏ�̐��`���
 * @param [in] Quaternion	&q1		:4����1
 * @param [in] Quaternion	&q2		:4����2
 * @param [in] float		t		:����
 * @return ���ʏ�̐��K�����ꂽ�l
 **/
Quaternion Quaternion::Slerp( const Quaternion &q1, const Quaternion &q2, const float t)
{
	Quaternion q;
	float c,tt;
	c = ( Vector3::Dot( q1.v,q2.v ) + q1.w*q2.w ); if(c<0) c=-c;

	if( c<1.0f ){
		float theta = Math::acos( c ),s = Math::sin( theta );
		tt = sinf( t*theta )/s;
		q = (q2-q1)*tt+q1;
	}else{
		q = (q2-q1)*t+q1;
	}
	return q.Normalize();
}

/** �������R���X�g���N�^
 * @param [in] Vector3	&p1		:�|���S�����W
 * @param [in] Vector3	&p2		:�|���S�����W
 * @param [in] Vector3	&p3		:�|���S�����W
 **/
Plane::Plane( Vector3 &p1, Vector3 &p2,Vector3 &p3 )
{	
	//�@��������
	Vector v1,v2;
	v1 = p2 - p1;
	v2 = p3 - p1;

	n = Vector::Cross( v1,v2 ).Normalize(); //���K�������@���x�N�g�����Ƃ߂�

	//���������� ���Ό����Ȃ̂Ń}�C�i�X��t����
	d = -Vector3::Dot( p1,n );
}	

/** �ʂƐ��̌������擾
 * @param [in] Vector3	&s		:���̊J�n���W
 * @param [in] Vector3	&e		:���̏I�����W
 * @param [in] Vector3	*ret	:��_���W
 * @return *ret	 0:�����Ȃ�	1:��������	-1:�ʂɐ���
 **/
int Plane::Intersect( Vector3 &s, Vector3 &e, Vector3 *ret )
{
	//��������
	float s1= Plane::Side( s );
	float s2= Plane::Side( e );					//�_�̈ʒu�𒲂ׂ�

	if( s1*s2 >0 ) return 0;					//�������ĂȂ�
	//����
	float d1=s1;if( d1<0 )d1=-d1;					//�����܂ł�"����"�̋��� //�֐���Distance��Side���ς��Ȃ�����
	float d2=s2;if( d2<0 )d2=-d2;
	float dist = d1+d2;						//�S�̂̋���
	
	if( dist==0 ){ *ret = s;return -1;}				//����
	//"����"�܂ł̒��������߂�
	float t = (dist - d2)/dist;					//�S�̂���̊��������߂� MAX��1�ƍl���ĥ��
	*ret = s+( e - s )*t;						//���`���
	return 1;
}

/** �ʂƃ��C�i����)�̌�_���擾
 * @param [in] Vector3	&pos	:�n�_
 * @param [in] Vector3	&ray	:�����̕����i�P�ʃx�N�g��)
 * @param [in] Vector3	*ret	:��_���W
 * @return *ret	 0:�����Ȃ�	1:��������	2:���Α��ɐ���	-1:�ʂɐ���
 **/
int Plane::IntersectRay( Vector3 &pos, Vector3 &ray, Vector3 *ret )
{
	float dt= -Vector3::Dot( n,ray );
	float s1= Side( pos );			//�������������ق���
	
	if( dt==0 )
	{
		if( s1!=0 )return 0;		//�����łȂ��������Ȃ�
		else{
			*ret= pos;
			return -1;				//�ʏ�
		}
	}

	//"����"�܂ł̒��������߂�
	float t = (s1/dt);					
	*ret = pos+ ray*t;				//��_���߂�		
	if( t>0 ) return 1;				//�������Ă���
	return 2;
	
}

/** �|���S���̓��O����
 * @param [in]	Vector3	&a		:���ʏ�̓_1
 * @param [in]	Vector3	&b		:���ʏ�̓_2
 * @param [in]	Vector3	&c		:���ʏ�̓_3
 * @param [in]	Vector3	&pos	:���ʏ�̌�_
 * @return	true:�����Ă���		false:�����ĂȂ�
 **/
bool Plane::Inside( Vector3 &a,Vector3 &b, Vector3 &c, Vector3 &pos )
{
	Vector v1,v2,v3;		//�e���_�ւ̃x�N�g��
	Vector crs1,crs2,crs3;	//�e�@���x�N�g��
	v1 = a - pos;			//���_�ւ̃x�N�g��
	v2 = b - pos;
	v3 = c - pos;
	
	//�@���̕������o��
	crs1 = Vector::Cross( v1,v2 );
	crs2 = Vector::Cross( v2,v3 );
	crs3 = Vector::Cross( v3,v1 );
	//�����������H
	float dot1,dot2,dot3;
	dot1 = Vector::Dot( crs1,n );	if( dot1<0 )return false;			//cos�l�����@�܂蔽�Ε���
	dot2 = Vector::Dot( crs2,n );	if( dot2<0 )return false;
	dot3 = Vector::Dot( crs3,n );	if( dot3<0 )return false;
	if( (dot1*dot2*dot3)>=0 ) return true;		//cos�l���S�Đ��Ȃ�
	return  false;
}
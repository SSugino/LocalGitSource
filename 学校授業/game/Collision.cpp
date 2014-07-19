/** @file
 * �e����N���X�̎���
 * @date 2009/06/05
 * @author ����
 **/

#include "Collision.h"
#include "../math/Vector.h"
#include "../math/Matrix.h"



using namespace sugino;
using namespace game;
using namespace math;
using namespace graphics;

/** �Փ˔��� �~�Ɖ~ */ 
bool Collision2D::Collision( BoundingCircle2D &a,BoundingCircle2D &b )
{
	/*
	Vector2 pos = b.p-a.p;		// b�~���S����a�~���S�̈ʒu�x�N�g��
	float r = a.s + b.s;		// ���v���a
	float len = pos.Length2();	// b�~���S����a�~���S�̒���
	float l = len-r*r;			// ���S�Ԃ̒��� - ���v���a �␳��
	if( r > len )				// ���S�̋��������a�̍��v����������
	{
		return true;
	}
	return false;
	*/
	Vector2 v=a.p-b.p;
	if( v.Length2() < ( a.s+b.s)*( a.s+b.s) )return true;
	return false;

}

/** �������� �~�Ɖ~ */
bool Intersect2D::Intersect( BoundingCircle2D &a,BoundingCircle2D &b,Vector2 *ret )
{	
	Vector2 v=a.p-b.p;
	if( v.Length2() < ( a.s+b.s)*( a.s+b.s) )
	{
		float s = ( a.s+b.s)*( a.s+b.s)-v.Length2();
		v.Normalize();
		*ret = v*s;		
		return true;
	}
	return false;
}

/** �Փ˔��� �l�p�Ǝl�p */
bool Collision2D::Collision( BoundingBox2D &a,BoundingBox2D &b )
{
	Vector2 v = a.p - b.p;			//�����x�N�g��
	float dx = v.x;					//�x�N�g���l�̕ۑ�
	float sizex = a.s.x + b.s.x;	//���v�T�C�Y	

	if( dx < 0 ){ dx = -dx;	}		//���Ȃ�[���]]
	if( dx > sizex ){ return false;	}//���S�̋��������a�̍��v����������
	
	float dy = v.y;
	float sizey = a.s.y + b.s.y;	//���aY

	if( dy < 0 ){ dy = -dy; }
	if( dy > sizey ) { return false; }

	return true;
}

/** �������� �l�p�Ǝl�p */
bool Intersect2D::Intersect( BoundingBox2D &a,BoundingBox2D &b,Vector2 *ret )
{
	float ax = 1,ay = 1;			//�Ƃ��Ēu��

	float vx = a.p.x - b.p.x;	if( vx<0 ){ vx=-vx; ax = -1; }
	float sizex = b.s.x + a.s.x;	//���a
	float dx = sizex - vx;			//�߂荞�ݗ�
	if( dx < 0 )  return false;			//�߂荞��łȂ��Ȃ�

	float vy = a.p.y - b.p.y;	if( vy<0 ){ vy=-vy; ay = -1; }
	float sizey = b.s.y + a.s.y;	//���a
	float dy = sizey - vy;			//�߂荞�ݗ�
	if( dy < 0 )  return false;			//�߂荞��łȂ��Ȃ�

	if( dx < dy )					//�߂荞�ݗʂ�������
	{
		*ret = Vector2(dx*ax,0);
	}else{
		*ret = Vector2(0,dy*ay);
	}
	return true;
}

/** �Փ˔��� ���Ɛ� */
bool Collision2D::Collision( BoundingLine2D &a,BoundingLine2D &b )
{	
	Vector2 aa,bb;			//�v�Z�p
	Vector2 av1,av2,av3;	//�����x�N�g���p
	Vector2 bv1,bv2,bv3;
	av1 = a.e-a.s;				
	av2 = b.e-a.s;			//�΂߂̃x�N�g��
	av3 = b.s-a.s;			//		�V�i��������

	float c1 = aa.Cross( av1,av2 );	//���C���a�̏I�_��
	float c2 = aa.Cross( av1,av3 );	//���C���a�̎n�_��
	
	if( c1 * c2 > 0 )return false;
	
	bv1 = b.e-b.s;
	bv2 = a.e-b.s;
	bv3 = a.s-b.s;

	float c3 = bb.Cross( bv1,bv2 );	//���C���`�̏I�_��
	float c4 = bb.Cross( bv1,bv3 );	//���C���`�̎n�_��

	if( c3 * c4 > 0 )return false;

	return true;
}

/** �Փ˔��� �~�Ɛ� */
bool Collision2D::Collision( BoundingCircle2D &a,BoundingLine2D &b )
{
	Vector2 p;					//��_�p
	Vector2 lv;					//���C��
	Vector2 v1,n;				//�P�ʃx�N�g��
	float dot;					//���ϒl�p

	v1 = a.p - b.s;								//���C���n�_����~�̒��S�Ɍ�����
	lv = b.e - b.s;								//���C���̎n�_����I�_�Ɍ������x�N�g��
	n  = Vector2::NormalVector(lv);				//�P�ʃx�N�g���p�ɃR�s�[�i���C���n�_�I�_�j
	dot = Vector2::Dot( v1,n );					//���ϋ��߂�	(cos�l�����܂�) X

	if( dot < 0 ){ p = b.s; }						//�n�_�@���ς��n�_���O��
	else if( dot*dot > lv.Length2() ){ p = b.e;}	//�I�_  �O��
	else p=n*dot+b.s;								//��_

	return ((a.p-p).Length2()<a.s*a.s);				//�c�������@> ���a���
}

/** �������� �~�Ɛ� */
bool Intersect2D::Intersect( BoundingCircle2D &a, BoundingLine2D &b, Vector2 *ret)
{
	Vector2 p;					//��_�p
	Vector2 lv;					//���C��
	Vector2 v1,n;				//�P�ʃx�N�g��
	float dot;					//���ϒl�p

	v1 = a.p - b.s;							//���C���n�_�Ɍ�����
	lv = b.e - b.s;								//���C���̎n�_����I�_�Ɍ������x�N�g��
	n  = Vector2::NormalVector(lv);				//�P�ʃx�N�g���p�ɃR�s�[�i���C���n�_�I�_�j
	dot = Vector2::Dot( v1,n );					//���ϋ��߂�	(cos�l�����܂�) X

	if( dot < 0 ){ p = b.s; }						//�n�_�@���ς��n�_���O��
	else if( dot*dot > lv.Length2() ){ p = b.e;}	//�I�_  �O��
	else p=n*dot+b.s;								//��_
	
	float len = (a.p-p).Length2();					//(�~�̒��S - ��_�̒���)
	Vector2 vy = a.p - p;							//��_����̕����x�N�g
	float v = a.s*a.s - vy.Length2();				//�␳��
	Vector2 pn = vy.Normalize();					//Y���̒P�ʃx�N�g��
	if ( len < a.s*a.s )							//(�~�̒��S - ��_�̒���)�@< ���a��� (���������̂Ȃ�)  
	{
		*ret = pn * v;	
		return true;
	}
	return false;
}

/** �������� �~�Ǝl�p */
bool Collision2D::Collision( BoundingCircle2D &a, BoundingBox2D &b)
{			
	float px=a.p.x;
	if( px> (b.p.x+b.s.x) ) px=(b.p.x+b.s.x);
	if( px< (b.p.x-b.s.x) ) px=(b.p.x-b.s.x);

	float py=a.p.y;
	if( py> (b.p.y+b.s.y) ) py=(b.p.y+b.s.y);
	if( py< (b.p.y-b.s.y) ) py=(b.p.y-b.s.y);

	Vector2 p(px,py);
	Vector2 v = p-a.p;		//�ŋߓ_����~�ւ̃x�N�g��	
	float len = v.Length2();
	float r = a.s*a.s;		//���a���悵�Ă���
	
	if( len < r	)			//�ŋߓ_����~�܂ł̒��������a��菬����
	{
		return true;
	}
	return false;
}

/** �ڐG���� �~�Ǝl�p */
bool Intersect2D::Intersect( BoundingCircle2D &a, BoundingBox2D &b, Vector2 *ret )
{
	//�ŋߓ_
	float px=a.p.x;
	if( px> (b.p.x+b.s.x) ) px=(b.p.x+b.s.x);
	if( px< (b.p.x-b.s.x) ) px=(b.p.x-b.s.x);

	float py=a.p.y;
	if( py> (b.p.y+b.s.y) ) py=(b.p.y+b.s.y);
	if( py< (b.p.y-b.s.y) ) py=(b.p.y-b.s.y);

	Vector2 p(px,py);
	Vector2 v = p-a.p;		//�ŋߓ_����~�ւ̃x�N�g��	
	float len = v.Length2();
	float r = a.s*a.s;		//���a���悵�Ă���

	if( len < r	)			//�ŋߓ_����~�܂ł̒��������a��菬����
	{
		float cr = len-r;				//�␳��
		*ret = v.Normalize() * cr;//���K��*����
		return true;
	}
	return false;
}


/** �Փ˔��� �~�Ɖ�]�� */
bool Collision2D::Collision( BoundingCircle2D &a, BoundingOBox2D &b)
{
	Vector2 cv = a.p - b.p;						//�~�ւ̃x�N�g��
	const Vector2 &vy = b.r;							//r(Y����]�x)��
	const Vector2 &vx = b.r.RotateInvert90();			//X���������̂�r����-90�x�������̂�X���Ƃ���
	
	float dx = Vector2::Dot( cv,vx );			//���ϋ��߂�	(cos�l�����܂�) X��"����"�����܂�
	if( dx >  +b.s.x )  dx= +b.s.x;
	if( dx <  -b.s.x )  dx= -b.s.x;

	float dy = Vector2::Dot( cv,vy );			//Y����̒���dy�����܂�
	if( dy >  +b.s.y ) dy= +b.s.y;
	if( dy <  -b.s.y ) dy= -b.s.y;
	
	Vector2 p=b.p + vx*dx + vy*dy;

	Vector2 v = p - a.p;						//�ŋߓ_����~�ւ̃x�N�g��	
	float len = v.Length2();					//�������
	float r = a.s*a.s;							//���a����悵�Ă���
	
	if( len < r	)								//�ŋߓ_����~�܂ł̒��������a��菬���� �~�Ɖ~�̔���
	{
		return true;				
	}
	return false;
}

/** �������� �~�Ɖ�]�� */
bool Intersect2D::Intersect( BoundingCircle2D &a, BoundingOBox2D &b, Vector2 *ret)
{
	Vector2 cv = a.p - b.p;						//�~�ւ̃x�N�g��
	const Vector2 &vy = b.r;							//r(Y����]�x)��
	const Vector2 &vx = b.r.RotateInvert90();			//X���������̂�r����-90�x�������̂�X���Ƃ���
	
	float dx = Vector2::Dot( cv,vx );			//���ϋ��߂�	(cos�l�����܂�) X��"����"�����܂�
	if( dx >  +b.s.x )  dx= +b.s.x;
	if( dx <  -b.s.x )  dx= -b.s.x;

	float dy = Vector2::Dot( cv,vy );			//Y����̒���dy�����܂�
	if( dy >  +b.s.y ) dy= +b.s.y;
	if( dy <  -b.s.y ) dy= -b.s.y;
	
	Vector2 p = b.p + vx*dx + vy*dy;			//�ŋߓ_�����߂� ���̒��S����[X��]���"���ϒl" + [Y��]���"���ϒl"
	Vector2 v = p - a.p;						//�ŋߓ_����~�ւ̃x�N�g��	
	float len = v.Length2();					//�������
	float r = a.s*a.s;							//���a����悵�Ă���
	
	if( len < r	)								//�ŋߓ_����~�܂ł̒��������a��菬���� �~�Ɖ~�̔���
	{
		float cr = len-r;						//�␳��
		*ret = v.Normalize() * cr;				//���K��*����
		return true;				//HIT�@�I!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	return false;
}

/** �ڐG���� ��]��*��]��(OBB*OBB) */
bool Collision2D::Collision( BoundingOBox2D &a, BoundingOBox2D &b )
{
	Vector2 abv = a.p - b.p;					//��a���甠b�̃x�N�g��
	//��
	const Vector2 &ay = a.r;					//��A��Y��	(�P�ʃx�N�g��)	
	const Vector2 &ax = a.r.RotateInvert90();	//��A��X���@(�P�ʃx�N�g��)
	
	const Vector2 &by = b.r;					//��B��Y��	(�P�ʃx�N�g��)
	const Vector2 &bx = b.r.RotateInvert90();	//��B��X���@(�P�ʃx�N�g��)

	float len;									//�e����ł̋���
	
	float ax_bx;								//��A��[X]����ł̔�B��[X]����̒���
	float ax_by;								//��A��[X]����ł̔�B��[Y]����̒���

	float ay_bx;								//��A��[Y]����ł̔�B��[X]����̒���
	float ay_by;								//��A��[Y]����ł̔�B��[Y]����̒���

	//ax���
	len = Vector2::Dot( ax,abv );if(len<0) len=-len;		//�}�C�i�X�̒l�͐��Ɂi��Βl�j
	ax_bx= Vector2::Dot( ax,bx );if(ax_bx<0) ax_bx=-ax_bx;
	ax_by= Vector2::Dot( ax,by );if(ax_by<0) ax_by=-ax_by;
	if( len > a.s.x + ax_bx*b.s.x+ax_by*b.s.y ) return false;

	//ay���
	len = Vector2::Dot( ay,abv );if(len<0) len=-len;
	ay_bx= Vector2::Dot( ay,bx );if(ay_bx<0) ay_bx=-ay_bx;
	ay_by= Vector2::Dot( ay,by );if(ay_by<0) ay_by=-ay_by;
	if( len > a.s.y + ay_bx*b.s.x+ay_by*b.s.y ) return false;

	//bx���
	//ax_bx�̒l�Ȃǂ͏�Ŏ���Ă���̂ŏ����ĂȂ�
	len = Vector2::Dot( bx,abv );if(len<0) len=-len;		
	if( len > b.s.x + ax_bx*a.s.x+ay_bx*a.s.y ) return false;

	//by���
	len = Vector2::Dot( by,abv );if(len<0) len=-len;
	if( len > b.s.y + ax_by*a.s.x+ay_by*a.s.y ) return false;

	return true;
}

/** �������� ��]��*��]��(OBB*OBB) */
bool Intersect2D::Intersect( BoundingOBox2D &a, BoundingOBox2D &b, Vector2 *ret)
{
	Vector2 abv = a.p - b.p;					//��a���甠b�̃x�N�g��
	//��
	const Vector2 &ay = a.r;					//��A��Y��	(�P�ʃx�N�g��)	
	const Vector2 &ax = a.r.RotateInvert90();	//��A��X���@(�P�ʃx�N�g��)
	
	const Vector2 &by = b.r;					//��B��Y��	(�P�ʃx�N�g��)
	const Vector2 &bx = b.r.RotateInvert90();	//��B��X���@(�P�ʃx�N�g��)
	
	float len;									//�e����ł̋���  �߂荞�ݗ�
	
	float ax_bx;								//��A��[X]����ł̔�B��[X]����̒���
	float ax_by;								//��A��[X]����ł̔�B��[Y]����̒���

	float ay_bx;								//��A��[Y]����ł̔�B��[X]����̒���
	float ay_by;								//��A��[Y]����ł̔�B��[Y]����̒���

	Vector2 min_axis;							//�ŏ���
	float min;									//�␳�ŏ��l
	float sgn;									//���̕������]
	//ax���
	sgn=1;
	len  = Vector2::Dot( ax,abv );if(len<0) { len=-len;sgn=-1; }		//�}�C�i�X�̒l�͐��Ɂi��Βl�j���������t������
	ax_bx= Vector2::Dot( ax,bx );if(ax_bx<0) ax_bx=-ax_bx;				//���̒���	
	ax_by= Vector2::Dot( ax,by );if(ax_by<0) ax_by=-ax_by;
	//�������Ă�Ȃ�
	len = (a.s.x + ax_bx*b.s.x+ax_by*b.s.y) - len;						// ���̒������m�̍��v - ���� = �߂荞�ݗ�										
	
	if( len < 0 ) return false;											//�������ĂȂ�
	 min=len;															//min�ɒZ���ق��̂߂荞�ݗʕۑ� 
	 min_axis = ax*sgn; 												//�ŏ��l�̓����Ă鎲�̕��������߂�
	
	//ay���
	sgn=1;
	len  = Vector2::Dot( ay,abv );if(len<0) { len=-len;sgn=-1; }
	ay_bx= Vector2::Dot( ay,bx );if(ay_bx<0) ay_bx=-ay_bx;
	ay_by= Vector2::Dot( ay,by );if(ay_by<0) ay_by=-ay_by;
	len = (a.s.y + ay_bx*b.s.x+ay_by*b.s.y) - len;
	if( len < 0 ) return false;
	if( min>len ){ min=len;min_axis = ay*sgn; }							//�O������Z��			


	//bx���
	//ax_bx�̒l�Ȃǂ͏�Ŏ���Ă���̂ŏ����ĂȂ�
	sgn=1;
	len = Vector2::Dot( bx,abv );if(len<0) { len=-len;sgn=-1; }		
	len = (b.s.x + ax_bx*a.s.x+ay_bx*a.s.y) - len;						//�߂荞�ݗ�
	if( len < 0 ) return false;
	if( min>len ){ min=len;min_axis = bx*sgn; }							//�O������Z���Ȃ�X�V			

	//by���
	sgn=1;
	len = Vector2::Dot( by,abv );if(len<0) { len=-len;sgn=-1; }
	len = (b.s.y + ax_by*a.s.x+ay_by*a.s.y) - len;						//�߂荞��
	if( len < 0 ) return false;
	if( min>len ){ min=len;min_axis = by*sgn; }							//�O������Z���Ȃ�X�V

	*ret = min_axis * min;												//�ŏ����ŕ␳����
	return true;
}

/** �ڐG���� ��]���Ǝl�p(OBB*AABB) */
bool Collision2D::Collision( BoundingOBox2D &a, BoundingBox2D &b)
{
	BoundingOBox2D b2( b.p,0,b.s );
	return Collision( a,b2 );
}

//------------------------------------------------------------------------------------------
/** �ڐG���� ���Ƌ� */
bool Collision3D::Collision( BoundingSphere3D &a, BoundingSphere3D &b )
{
	Vector3 v= a.p-b.p;
	if( v.Length2() < ( a.s+b.s)*( a.s+b.s) )return true;
	return false;
}

/** �������� ���Ƌ� */
bool Intersect3D::Intersect( BoundingSphere3D &a, BoundingSphere3D &b,Vector3 *ret )
{
	Vector3 v = a.p-b.p;
	float s  = a.s+b.s;//���a�̍��v
	float len = v.Length2();//���
	
	if( s*s > len ){
		len = Math::sqrt( len );
		*ret = v*( (s-len) /len );	//���������߂�

		return true;
	}
	return false;
}

/** �ڐG���� AABB��AABB*/
bool Collision3D::Collision( BoundingBox3D &a, BoundingBox3D &b )
{
	Vector3 v = a.p - b.p;			//�����x�N�g��
	float dx = v.x;					//�x�N�g���l�̕ۑ�
	float sizex = a.s.x + b.s.x;	//���v�T�C�Y	

	if( dx < 0 ){ dx = -dx;	}		//���Ȃ�[���]]
	if( dx > sizex ){ return false;	}//���S�̋��������a�̍��v����������
	
	float dy = v.y;
	float sizey = a.s.y + b.s.y;	//���aY

	if( dy < 0 ){ dy = -dy; }
	if( dy > sizey ) { return false; }

	float dz = v.z;
	float sizez = a.s.z + b.s.z;	//���aZ
	if( dz < 0 ){ dz = -dz; }
	if( dz > sizez ) { return false; }

	return true;
}

/** �������� AABB��AABB*/
bool Intersect3D::Intersect( BoundingBox3D &a,BoundingBox3D &b,Vector3 *ret )
{
	float ax=1,ay=1,az=1;										//�Ƃ��Ēu��

	float vx = a.p.x - b.p.x;	if( vx<0 ){ vx=-vx; ax = -1; }
	float sizex = b.s.x + a.s.x;								//���a
	float dx = sizex - vx;										//�߂荞�ݗ�
	if( dx < 0 )  return false;									//�߂荞��łȂ��Ȃ�

	float vy = a.p.y - b.p.y;	if( vy<0 ){ vy=-vy; ay = -1; }
	float sizey = b.s.y + a.s.y;								//���a
	float dy = sizey - vy;										//�߂荞�ݗ�
	if( dy < 0 )  return false;									//�߂荞��łȂ��Ȃ�

	float vz = a.p.z - b.p.z;	if( vz<0 ){ vz=-vz; az = -1; }
	float sizez = b.s.z + a.s.z;								//���a
	float dz = sizez - vz;										//�߂荞�ݗ�
	if( dz < 0 )  return false;									//�߂荞��łȂ��Ȃ�

	if( dx<dy )
	{
		if( dx<dz )ret->setVector( dx*ax,0,0 );
		else	   ret->setVector( 0,0,dz*az );
	}else{
		if( dy<dz )ret->setVector( 0,dy*ay,0 );
		else	   ret->setVector( 0,0,dz*az );
	}
	return true;
}

/** �Փ˔��� ���Ɛ� */
bool Collision3D::Collision( BoundingSphere3D &a,BoundingLine3D &b )
{
	Vector3 p;					//��_�p
	Vector3 lv;					//���C��
	Vector3 v1,n;				//�P�ʃx�N�g��
	float dot;					//���ϒl�p

	v1 = a.p - b.s;								//���C���n�_���狅�̒��S�Ɍ�����
	lv = b.e - b.s;								//���C���̎n�_����I�_�Ɍ������x�N�g��
	n  = Vector3::NormalVector(lv);				//�P�ʃx�N�g���p�ɃR�s�[�i���C���n�_�I�_�j
	dot = Vector3::Dot( v1,n );					//���ϋ��߂�	(cos�l�����܂�) X

	if( dot < 0 ){ p = b.s; }						//�n�_�@���ς��n�_���O���Ȃ�[�𒆐S�Ƃ���
	else if( dot*dot > lv.Length2() ){ p = b.e;}	//�I�_  �O���Ȃ�O���̒[�𒆐S�Ƃ���
	else p=n*dot+b.s;								//��_
	
	BoundingSphere3D bs( p,b.width );				//��_�𒆐S�Ƃ�����

	if( Collision( a,bs ) )
	{
		return true;//( (a.p-p).Length2()<a.s*a.s );			//�c�������@< ���a���
	}
	return false;
}

/** �������� ���Ɛ� */
bool Intersect3D::Intersect( BoundingSphere3D &a, BoundingLine3D &b, Vector3 *ret)
{
	Vector3 p;										//��_�p
	Vector3 lv;										//���C��
	Vector3 v1,n;									//�P�ʃx�N�g��
	float dot;										//���ϒl�p

	v1 = a.p - b.s;									//���C���n�_�Ɍ�����
	lv = b.e - b.s;									//���C���̎n�_����I�_�Ɍ������x�N�g��
	n  = Vector3::NormalVector(lv);					//�P�ʃx�N�g���p�ɃR�s�[�i���C���n�_�I�_�j
	dot = Vector3::Dot( v1,n );						//���ϋ��߂�	(cos�l�����܂�) X

	if( dot < 0 ){ p = b.s; }						//�n�_�@���ς��n�_���O��
	else if( dot*dot > lv.Length2() ){ p = b.e;}	//�I�_  �O��
	else p=n*dot+b.s;								//��_
	
	BoundingSphere3D bs( p,b.width );				//��_�𒆐S�Ƃ�����
	if( Intersect( a,bs,ret ))						//�����m�̔���
	{
		return true;
	}
	return false;
}

//��_�̋��܂鋅�Ɛ��̔���
bool Intersect3D::Intersect( BoundingSphere3D &a, BoundingLine3D &b, Vector3 *ret, Vector3 *cross_pos )
{
	Vector3 p;										//��_�p
	Vector3 lv;										//���C��
	Vector3 v1,n;									//�P�ʃx�N�g��
	float dot;										//���ϒl�p

	v1 = a.p - b.s;									//���C���n�_�Ɍ�����
	lv = b.e - b.s;									//���C���̎n�_����I�_�Ɍ������x�N�g��
	n  = Vector3::NormalVector(lv);					//�P�ʃx�N�g���p�ɃR�s�[�i���C���n�_�I�_�j
	dot = Vector3::Dot( v1,n );						//���ϋ��߂�	(cos�l�����܂�) X

	if( dot < 0 ) p = b.s;							//�n�_�@���ς��n�_���O��
	else if( dot*dot > lv.Length2() ) p = b.e;		//�I�_  �O��
	else p=n*dot+b.s;								//��_
	*cross_pos = p;									//��_��Ԃ�
	BoundingSphere3D bs( p,b.width*1.5f );			//��_�𒆐S�Ƃ����� ���̔��a���m�Ŕ��肷��̂ŃX�C���O�������Ă������悤�ɔ��a�T�C�Y�������傫�����Ă���
	if( Intersect( a,bs,ret ))						//�����m�̔���
	{
		return true;
	}
	return false;
}

/** �ڐG����@���ƃ��C(�����j */
bool Collision3D::Collision( BoundingSphere3D &a,Vector3 &pos,Vector3 &ray )
{
	Vector p;					//��_�p
	Vector lv;					//���C��
	Vector v1,n;				//�P�ʃx�N�g��
	float dot;					//���ϒl�p

	v1 = a.p - pos;									//���C���n�_���狅�̒��S�Ɍ�����
	dot = Vector3::Dot( v1,ray );					//���ϋ��߂�	(cos�l�����܂�) X

	if( dot < 0 ){ p = pos; }						//�n�_�@���ς��n�_���O���Ȃ�pos�𒆐S��
	else p= pos+ray*dot;								//��_
	
	if( (a.p-p).Length2()<a.s*a.s ) return true;
	return false;
}

/** ��������@���ƃ��C(�����j */
bool Intersect3D::Intersect( BoundingSphere3D &a,Vector3 &pos,Vector3 &ray, Vector3 *cross_pos ) 
{
	Vector v  = pos - a.p;
	float d   = Vector3::Dot( ray,v );			//���������߂�
	float len = v.Length2() - a.s*a.s;
	if( len > 0 && d >0 ) return false;		//���������Ε����Ȃ̂�
	float dr= d*d - len;
	if( dr < 0  ) return false;					//�ڐG���ĂȂ�
	float t=(-d)-Math::sqrt(dr);
	if(t<0)t=0;
	*cross_pos = pos+ray*t;						//��_�̈ʒu�����߂�
	return true;
}


/** �ڐG����@���Ǝl�p(AABB) */
bool Collision3D::Collision( BoundingSphere3D &a, BoundingBox3D &b)
{
	float px = a.p.x;
	if( px> (b.p.x+b.s.x)) px=(b.p.x+b.s.x);
	if( px< (b.p.x-b.s.x)) px=(b.p.x-b.s.x);

	float py=a.p.y;
	if( py> (b.p.y+b.s.y)) py=(b.p.y+b.s.y);
	if( py< (b.p.y-b.s.y)) py=(b.p.y-b.s.y);

	
	float pz=a.p.z;
	if( pz> (b.p.z+b.s.z)) pz=(b.p.z+b.s.z);
	if( pz< (b.p.z-b.s.z)) pz=(b.p.z-b.s.z);

	Vector3 p(px,py,pz);
	Vector3 v = p - a.p;					//�ŋߓ_���狅�ւ̃x�N�g��	
	float len = v.Length2();
	float r = a.s*a.s;						//���a���悵�Ă���
	
	if( len < r	) return true;				//�ŋߓ_����~�܂ł̒��������a��菬����
	return false;
}

/** �������� ���Ǝl�p�iAABB) */
bool Intersect3D::Intersect( BoundingSphere3D &a, BoundingBox3D &b, Vector3 *ret )
{
	//�N���b�s���O���ŋߓ_�����߂�
	float px = a.p.x;
	if( px> (b.p.x+b.s.x)) px=(b.p.x+b.s.x);
	if( px< (b.p.x-b.s.x)) px=(b.p.x-b.s.x);
	float py=a.p.y;
	if( py> (b.p.y+b.s.y)) py=(b.p.y+b.s.y);
	if( py< (b.p.y-b.s.y)) py=(b.p.y-b.s.y);
	float pz=a.p.z;
	if( pz> (b.p.z+b.s.z)) pz=(b.p.z+b.s.z);
	if( pz< (b.p.z-b.s.z)) pz=(b.p.z-b.s.z);

	//�ŋߓ_
	Vector3 p(px,py,pz);

	Vector3 v = p - a.p;					//�ŋߓ_���狅�ւ̃x�N�g��
	//Normalize����O��0��������
	if( v==0 ){ v.x=0.001f,v.y=0.001f,v.z=0.001f; }

	float len = v.Length2();
	float r = a.s*a.s;						//���a���悵�Ă���
	
	if( len < r	){							//���Ƌ��̔���𓯂�
		float cr = len-r;					//�␳��
		*ret = v.Normalize() * cr;			//���K��*����
		return true;
	}
	return false;
}

/** �ڐG����@���Ɖ�]��(OBB) */
bool Collision3D::Collision( BoundingSphere3D &a, BoundingOBox3D &b )
{
	Vector3 cv = a.p - b.p;							//�~�ւ̃x�N�g��
	const Vector3 &vy = b.ay;						//�e�����Q�ƌ^��
	const Vector3 &vx = b.ax;						
	const Vector3 &vz = b.az;
	
	//�e������ϋ��߂ߒ������擾���N���b�s���O
	float dx = Vector3::Dot( cv,vx );				
	if( dx >  +b.s.x )  dx= +b.s.x;				
	if( dx <  -b.s.x )  dx= -b.s.x;

	float dy = Vector3::Dot( cv,vy );	
	if( dy >  +b.s.y ) dy= +b.s.y;
	if( dy <  -b.s.y ) dy= -b.s.y;
	
	float dz = Vector3::Dot( cv,vz );
	if( dz >  +b.s.z ) dz= +b.s.z;
	if( dz <  -b.s.z ) dz= -b.s.z;
	
	Vector3 p= b.p + vx*dx + vy*dy + vz*dz;	//�ŋߓ_�����߂�

	Vector3 v = p - a.p;						//�ŋߓ_����~�ւ̃x�N�g��	
	float len = v.Length2();					//�������
	float r = a.s*a.s;							//���a����悵�Ă���
	
	if( len < r	) return true;					//���Ƌ��̔��������
	return false;
}

/** �������� ���Ɖ�]��(OBB) */
bool Intersect3D::Intersect( BoundingSphere3D &a, BoundingOBox3D &b, Vector3 *ret)
{
	Vector3 cv = a.p - b.p;							//�~�ւ̃x�N�g��
	const Vector3 &vy = b.ay;						//�e�����Q�ƌ^��
	const Vector3 &vx = b.ax;						
	const Vector3 &vz = b.az;
	
	//�e������ϋ��߂ߒ������擾���N���b�s���O
	float dx = Vector3::Dot( cv,vx );				
	if( dx >  +b.s.x )  dx= +b.s.x;				
	if( dx <  -b.s.x )  dx= -b.s.x;

	float dy = Vector3::Dot( cv,vy );	
	if( dy >  +b.s.y ) dy= +b.s.y;
	if( dy <  -b.s.y ) dy= -b.s.y;
	
	float dz = Vector3::Dot( cv,vz );
	if( dz >  +b.s.z ) dz= +b.s.z;
	if( dz <  -b.s.z ) dz= -b.s.z;
	
	Vector3 p= b.p + vx*dx + vy*dy + vz*dz;	//�ŋߓ_�����߂�

	Vector3 v = p - a.p;						//�ŋߓ_����~�ւ̃x�N�g��	
	float len = v.Length2();					//�������
	float r = a.s*a.s;							//���a����悵�Ă���
	
	if( len < r	)
	{
		if( v.Length() )						//0���Z���ł邩���ׂ�
		{
			float cr = len-r;					//�␳��
			*ret = v.Normalize() * cr;			//���K��*����
			return true;						//���Ƌ��̔��������
		}
	}
	return false;
}
/** �����璷����␳�ʁA�␳���������߂� */
bool Collision3D::AxisLength( Vector3 &axis, Vector3 &dist,BoundingOBox3D &a, BoundingOBox3D &b, float *length,float *sign )
{
	//�e����ł̒���
	float asx = Vector3::Dot( axis,a.ax )*a.s.x;if( asx<0 )asx= -asx;	//���Ȃ畄�����]
	float asy = Vector3::Dot( axis,a.ay )*a.s.y;if( asy<0 )asy= -asy;
	float asz = Vector3::Dot( axis,a.az )*a.s.z;if( asz<0 )asz= -asz;
	
	float bsx = Vector3::Dot( axis,b.ax )*b.s.x;if( bsx<0 )bsx= -bsx;
	float bsy = Vector3::Dot( axis,b.ay )*b.s.y;if( bsy<0 )bsy= -bsy;
	float bsz = Vector3::Dot( axis,b.az )*b.s.z;if( bsz<0 )bsz= -bsz;

	float len = Vector3::Dot( axis,dist );								//���Ԃ̋���
	*sign = +1;															//�␳�����𐳂ŏ�����
	if( len<0 ) { len= -len;*sign=-1;}									//�������Ȃ畄�����]�A�␳���������]
	*length = (asx+asy+asz)+(bsx+bsy+bsz) -len;
	return *length > 0;													//����������Ƃ��̂ݕ␳�ʂ�Ԃ�
}

/** �ڐG����( OBB �� OBB ) */
bool Collision3D::Collision( BoundingOBox3D &a, BoundingOBox3D &b )
{
	Vector3 v = a.p - b.p;					//��a���甠b�̃x�N�g��
	Vector3 axis;
	float tlen,tsgn;							//�����p
	
	//��a�̊e���㔻��
	if( !AxisLength( a.ax,v, a,b, &tlen,&tsgn )) return false;
	if( !AxisLength( a.ay,v, a,b, &tlen,&tsgn )) return false;
	if( !AxisLength( a.az,v, a,b, &tlen,&tsgn )) return false;
	//��b�̊e���㔻��
	if( !AxisLength( b.ax,v, a,b, &tlen,&tsgn )) return false;
	if( !AxisLength( b.ay,v, a,b, &tlen,&tsgn )) return false;
	if( !AxisLength( b.az,v, a,b, &tlen,&tsgn )) return false;
	
	//�����m�̎��̊O�ςŋ��߁A�����Ŕ���
	axis = Vector3::Cross( a.ax,b.ax );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;
	axis = Vector3::Cross( a.ax,b.ay );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;
	axis = Vector3::Cross( a.ax,b.az );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;

	axis = Vector3::Cross( a.ay,b.ax );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;
	axis = Vector3::Cross( a.ay,b.ay );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;
	axis = Vector3::Cross( a.ay,b.az );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;

	axis = Vector3::Cross( a.az,b.ax );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;
	axis = Vector3::Cross( a.az,b.ay );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;
	axis = Vector3::Cross( a.az,b.az );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;

	return true;
}

/** �������� ���Ɣ�*/
bool Intersect3D::Intersect( BoundingOBox3D &a, BoundingOBox3D &b, Vector3 *ret )
{
	Vector3 v = a.p - b.p;					//��a���甠b�̃x�N�g��
	Vector3 axis;
	float tlen,tsgn,min,sgn;							//�����p
	
	//��a�̊e���㔻��
	if( !Collision3D::AxisLength( a.ax,v, a,b, &tlen,&tsgn )) return false;
	min = tlen;	sgn = tsgn; *ret = a.ax;
	if( !Collision3D::AxisLength( a.ay,v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=a.ay; }
	if( !Collision3D::AxisLength( a.az,v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=a.az; }
	//��b�̊e���㔻��
	if( !Collision3D::AxisLength( b.ax,v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=b.ay; }
	if( !Collision3D::AxisLength( b.ay,v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=b.ay; }
	if( !Collision3D::AxisLength( b.az,v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=b.ay; }
	//�����m�̎��̊O�ςŋ��߁A�����Ŕ���
	axis = Vector3::Cross( a.ax,b.ax );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }
	
	axis = Vector3::Cross( a.ax,b.ay );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.ax,b.az );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.ay,b.ax );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.ay,b.ay );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.ay,b.az );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.az,b.ax );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.az,b.ay );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.az,b.az );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	* ret *= min*sgn;
	return true;
}

bool Collision3D::Collision( BoundingBox3D &a, Vector3 &pos, Vector3 &ray )
{
	float min=0,max=FLT_MAX,tmp;		//tmp�͓���ւ��p
	//�e���Œ��ׂ�
	float x1 = ( a.p.x-a.s.x ),x2 = ( a.p.x+a.s.x );
	if( ray.x ){						//�X��������
		float t1 = (x1-pos.x)/ray.x;//if( t1<0 ) t1 =-(t1);		//�t��������Bx1�܂ł̒����Ȃ̂Ń}�C�i�X�Ȃ畄�����]			
		float t2 = (x2-pos.x)/ray.x;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2�̊֌W��t1�̂ق����傫���Ȃ��Ă��܂����ꍇ����ւ���
		
		if( t1>min ) min = t1;						//�ŏ��l�X�V
		if( t2<max ) max = t2;						//�ő�l�X�V
	}else{
		if( pos.x <x1 || pos.x > x2 )return false;	//x1�`x2�Ԃ̊O���B�������Ȃ�
	}

	float y1 = ( a.p.y-a.s.y ),y2 = ( a.p.y+a.s.y );
	if( ray.y ){									//�X��������
		float t1 = (y1-pos.y)/ray.y;//if( t1<0 ) t1 =-(t1);		//�t��������Bx1�܂ł̒����Ȃ̂Ń}�C�i�X�Ȃ畄�����]			
		float t2 = (y2-pos.y)/ray.y;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp; }			//t1,t2�̊֌W��t1�̂ق����傫���Ȃ��Ă��܂����ꍇ����ւ���
		
		if( t1>min ) min = t1;						//�ŏ��l�X�V
		if( t2<max ) max = t2;						//�ő�l�X�V
		
		if( min > max ) return false;				//�������ĂȂ� 
	}else{											//�X���Ȃ�
		if( pos.y <y1 || pos.y > y2 )return false;	//x1�`x2�Ԃ̊O���B�������Ȃ�
	}

	float z1 = ( a.p.z-a.s.z ),z2 = ( a.p.z+a.s.z );
	if( ray.z ){	//�X��������
		float t1 = (z1-pos.z)/ray.z;//if( t1<0 ) t1 =-(t1);		//�t��������Bx1�܂ł̒����Ȃ̂Ń}�C�i�X�Ȃ畄�����]			
		float t2 = (z2-pos.z)/ray.z;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2�̊֌W��t1�̂ق����傫���Ȃ��Ă��܂����ꍇ����ւ���
		
		if( t1>min ) min = t1;						//�ŏ��l�X�V
		if( t2<max ) max = t2;						//�ő�l�X�V
		
		if( min > max ) return false;				//�������ĂȂ� 
	}else{
		if( pos.y <y1 || pos.y > y2)return false;	//x1�`x2�Ԃ̊O���B�������Ȃ�
	}
	return true;
}

bool Intersect3D::Intersect(  BoundingBox3D &a,Vector3 &pos,math::Vector3 &ray,Vector3 *ret )
{
	float min=0,max=FLT_MAX,tmp;		//tmp�͓���ւ��p
	//�e���Œ��ׂ�
	float x1 = ( a.p.x-a.s.x ),x2 = ( a.p.x+a.s.x );
	if( ray.x ){						//�X��������
		float t1 = (x1-pos.x)/ray.x;//if( t1<0 ) t1 =-(t1);		//�t��������Bx1�܂ł̒����Ȃ̂Ń}�C�i�X�Ȃ畄�����]			
		float t2 = (x2-pos.x)/ray.x;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2�̊֌W��t1�̂ق����傫���Ȃ��Ă��܂����ꍇ����ւ���
		
		if( t1>min ) min = t1;						//�ŏ��l�X�V
		if( t2<max ) max = t2;						//�ő�l�X�V
	}else{
		if( pos.x <x1 || pos.x > x2 )return false;	//x1�`x2�Ԃ̊O���B�������Ȃ�
	}

	float y1 = ( a.p.y-a.s.y ),y2 = ( a.p.y+a.s.y );
	if( ray.y ){									//�X��������
		float t1 = (y1-pos.y)/ray.y;//if( t1<0 ) t1 =-(t1);		//�t��������Bx1�܂ł̒����Ȃ̂Ń}�C�i�X�Ȃ畄�����]			
		float t2 = (y2-pos.y)/ray.y;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp; }			//t1,t2�̊֌W��t1�̂ق����傫���Ȃ��Ă��܂����ꍇ����ւ���
		
		if( t1>min ) min = t1;						//�ŏ��l�X�V
		if( t2<max ) max = t2;						//�ő�l�X�V
		
		if( min > max ) return false;				//�������ĂȂ� 
	}else{											//�X���Ȃ�
		if( pos.y <y1 || pos.y > y2 )return false;	//x1�`x2�Ԃ̊O���B�������Ȃ�
	}

	float z1 = ( a.p.z-a.s.z ),z2 = ( a.p.z+a.s.z );
	if( ray.z ){	//�X��������
		float t1 = (z1-pos.z)/ray.z;//if( t1<0 ) t1 =-(t1);		//�t��������Bx1�܂ł̒����Ȃ̂Ń}�C�i�X�Ȃ畄�����]			
		float t2 = (z2-pos.z)/ray.z;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2�̊֌W��t1�̂ق����傫���Ȃ��Ă��܂����ꍇ����ւ���
		
		if( t1>min ) min = t1;						//�ŏ��l�X�V
		if( t2<max ) max = t2;						//�ő�l�X�V
		
		if( min > max ) return false;				//�������ĂȂ� 
	}else{
		if( pos.y <y1 || pos.y > y2)return false;	//x1�`x2�Ԃ̊O���B�������Ȃ�
	}
	*ret = pos + ray * min;
	return true;
}

bool Collision3D::Collision( BoundingOBox3D &a, Vector3 &pos1, Vector3 &ray1 )
{
	float min=0,max=FLT_MAX,tmp;
	Matrix irot( a.ax.x,a.ay.x,a.az.x,0,
				 a.ax.y,a.ay.y,a.az.y,0,
				 a.ax.z,a.ay.z,a.az.z,0,
				 0,0,0,1);

	Vector pos = irot.TransformRotation( pos1 - a.p );
	Vector ray = irot.TransformRotation( ray1 );

	//X��
	float x1 = -a.s.x ,x2 = +a.s.x;
	if( ray.x ){						//�X��������
		float t1 = (x1-pos.x)/ray.x;//if( t1<0 ) t1 =-(t1);
		float t2 = (x2-pos.x)/ray.x;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2�̊֌W��t1�̂ق����傫���Ȃ��Ă��܂����ꍇ����ւ���
		
		if( t1>min ) min = t1;						//�ŏ��l�X�V
		if( t2<max ) max = t2;						//�ő�l�X�V
	}else{
		if( pos.x <x1 || pos.x > x2 )return false;	//x1�`x2�Ԃ̊O���B�������Ȃ�
	}

	float y1 = -a.s.y,y2 = a.s.y;
	if( ray.y ){									//�X��������
		float t1 = (y1-pos.y)/ray.y;//if( t1<0 ) t1 =-(t1);		//�t��������Bx1�܂ł̒����Ȃ̂Ń}�C�i�X�Ȃ畄�����]			
		float t2 = (y2-pos.y)/ray.y;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp; }			//t1,t2�̊֌W��t1�̂ق����傫���Ȃ��Ă��܂����ꍇ����ւ���
		
		if( t1>min ) min = t1;						//�ŏ��l�X�V
		if( t2<max ) max = t2;						//�ő�l�X�V
		
		if( min > max ) return false;				//�������ĂȂ� 
	}else{											//�X���Ȃ�
		if( pos.y <y1 || pos.y > y2 )return false;	//x1�`x2�Ԃ̊O���B�������Ȃ�
	}

	float z1 = -a.s.z,z2 = +a.s.z;
	if( ray.z ){	//�X��������
		float t1 = (z1-pos.z)/ray.z;//if( t1<0 ) t1 =-(t1);		//�t��������Bx1�܂ł̒����Ȃ̂Ń}�C�i�X�Ȃ畄�����]			
		float t2 = (z2-pos.z)/ray.z;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2�̊֌W��t1�̂ق����傫���Ȃ��Ă��܂����ꍇ����ւ���
		
		if( t1>min ) min = t1;						//�ŏ��l�X�V
		if( t2<max ) max = t2;						//�ő�l�X�V
		
		if( min > max ) return false;				//�������ĂȂ� 
	}else{
		if( pos.y <y1 || pos.y > y2)return false;	//x1�`x2�Ԃ̊O���B�������Ȃ�
	}
	
	return true;
}

bool Intersect3D::Intersect( BoundingOBox3D &a, Vector3 &pos1, Vector3 &ray1, Vector3 *ret)
{
	float min=0,max=FLT_MAX,tmp;
	Matrix irot( a.ax.x,a.ay.x,a.az.x,0,
				 a.ax.y,a.ay.y,a.az.y,0,
				 a.ax.z,a.ay.z,a.az.z,0,
				 0,0,0,1);

	Vector pos = irot.TransformRotation( pos1 - a.p );
	Vector ray = irot.TransformRotation( ray1 );

	//X��
	float x1 = -a.s.x ,x2 = +a.s.x;
	if( ray.x ){						//�X��������
		float t1 = (x1-pos.x)/ray.x;//if( t1<0 ) t1 =-(t1);
		float t2 = (x2-pos.x)/ray.x;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2�̊֌W��t1�̂ق����傫���Ȃ��Ă��܂����ꍇ����ւ���
		
		if( t1>min ) min = t1;						//�ŏ��l�X�V
		if( t2<max ) max = t2;						//�ő�l�X�V
	}else{
		if( pos.x <x1 || pos.x > x2 )return false;	//x1�`x2�Ԃ̊O���B�������Ȃ�
	}

	float y1 = -a.s.y,y2 = a.s.y;
	if( ray.y ){									//�X��������
		float t1 = (y1-pos.y)/ray.y;//if( t1<0 ) t1 =-(t1);		//�t��������Bx1�܂ł̒����Ȃ̂Ń}�C�i�X�Ȃ畄�����]			
		float t2 = (y2-pos.y)/ray.y;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp; }			//t1,t2�̊֌W��t1�̂ق����傫���Ȃ��Ă��܂����ꍇ����ւ���
		
		if( t1>min ) min = t1;						//�ŏ��l�X�V
		if( t2<max ) max = t2;						//�ő�l�X�V
		
		if( min > max ) return false;				//�������ĂȂ� 
	}else{											//�X���Ȃ�
		if( pos.y <y1 || pos.y > y2 )return false;	//x1�`x2�Ԃ̊O���B�������Ȃ�
	}

	float z1 = -a.s.z,z2 = +a.s.z;
	if( ray.z ){	//�X��������
		float t1 = (z1-pos.z)/ray.z;//if( t1<0 ) t1 =-(t1);		//�t��������Bx1�܂ł̒����Ȃ̂Ń}�C�i�X�Ȃ畄�����]			
		float t2 = (z2-pos.z)/ray.z;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2�̊֌W��t1�̂ق����傫���Ȃ��Ă��܂����ꍇ����ւ���
		
		if( t1>min ) min = t1;						//�ŏ��l�X�V
		if( t2<max ) max = t2;						//�ő�l�X�V
		
		if( min > max ) return false;				//�������ĂȂ� 
	}else{
		if( pos.y <y1 || pos.y > y2)return false;	//x1�`x2�Ԃ̊O���B�������Ȃ�
	}
	*ret = pos1 + ray1*min;
	return true;
}

void Frustum::Create( Matrix4x4 &view, float wid, float hei, float near_len, float far_len)
{
	float nw = wid/2; 
	float nh = hei/2;							//���ɑ΂��Ă̊���

	float fw = far_len*nw;
	float fh = far_len*nh;

	//�e�_�̏�����
	p[0].setVector( -nw, +nh,near_len);
	p[1].setVector( +nw, +nh,near_len);
	p[2].setVector( +nw, -nh,near_len);
	p[3].setVector( -nw, -nh,near_len);
	
	p[4].setVector( -fw, +fh,far_len);
	p[5].setVector( +fw, +fh,far_len);
	p[6].setVector( +fw, -fh,far_len);
	p[7].setVector( -fw, -fh,far_len);

	//�e�_���J�����̈ʒu�A���W�ɂ���
	for( int i=0;i<8;i++ ) p[i].setVector( p[i] );
	
	//�ʂ̏�����
	pnear	= Plane( p[0],p[3],p[2] );
	top		= Plane( p[0],p[1],p[5] );
	bottom	= Plane( p[3],p[7],p[6] );
	left	= Plane( p[0],p[4],p[7] );
	right	= Plane( p[1],p[2],p[6] );
	pfar	= Plane( p[4],p[5],p[6] );

}

void Frustum::Draw( IRender *r, Matrix4x4 &vps, Vector3 &dir, Color &col)
{
	//r->setShedingMode( r->SM_WIRE );
	//r->setCullMode( r->CM_CW );

	WORD index[6][4] = { {0,3,2,1},{0,1,5,4},{3,7,6,2},{0,4,7,3},{1,2,6,5},{4,5,6,7} };
	ColorTVertex tv[4];
	WORD face[6]={ 0,1,2,0,2,3 };
	Color amb = col*0.5f; amb.a=1;
	Vector3 norm[] = { pnear.n,top.n,bottom.n,left.n,right.n,pfar.n };
	for( int i=0;i<6;i++ )
	{
		float dt = -Vector3::Dot( norm[i],dir ); if( dt<0 )dt=0;
		DWORD c = amb + col*dt;

		for(int j=0;j<4;j++ )
		{
			Vector4 v=vps.Transform( p[ index[i][j] ] );
			v.w = 1/v.w;
			v.x*=v.w;v.y*=v.w;v.z*=v.w;
			tv[j] = ColorTVertex( v,c );
		}
		r->drawPolygon( tv,face,4,2 );
	}
	r->setShedingMode( r->SM_GOURAUD );
}

bool Frustum::Collision( BoundingSphere3D &sp)
{
	// �œK���ׁ̈A�s���ɍ��킹�Ĕ��肷�鏇�Ԃ����߂邱��(�悭�g���ł��낤�͈͂��画�肷��
	if( pnear.Side( sp.p )+sp.s	< 0 )	return false;
	if( pfar.Side( sp.p )+sp.s < 0 )	return false;
	if( left.Side( sp.p )+sp.s < 0 ) return false;
	if( right.Side( sp.p )+sp.s< 0 ) return false;
	if( top.Side( sp.p )+sp.s  < 0 ) return false;
	if( bottom.Side( sp.p )+sp.s < 0 ) return false;

	return true;						//�����S��+��(������͈͓��j
}

bool Frustum::Collision( BoundingOBox3D &obox )
{
	Vector3 pos[8]={ Vector3( -obox.s.x,+obox.s.y,-obox.s.z ),
					 Vector3( +obox.s.x,+obox.s.y,-obox.s.z ),
					 Vector3( +obox.s.x,-obox.s.y,-obox.s.z ),
					 Vector3( -obox.s.x,-obox.s.y,-obox.s.z ),
					 
					 Vector3( -obox.s.x,+obox.s.y,+obox.s.z ),
					 Vector3( +obox.s.x,+obox.s.y,+obox.s.z ),
					 Vector3( +obox.s.x,-obox.s.y,+obox.s.z ),
					 Vector3( -obox.s.x,-obox.s.y,-obox.s.z )
		};
	Matrix4x4 m( obox.ax.x ,obox.ay.x,obox.ax.z, 0,
				 obox.ax.y ,obox.ay.y,obox.ax.y, 0,
				 obox.ax.z ,obox.ay.z,obox.ax.z, 0,
				 obox.p.x,obox.p.y,obox.p.z,1 );
	int i;
	for( i=0;i<8;i++){
		pos[i]=m.TransformVector( pos[i] );
		//�s���ɍ��킹�ĥ��
		if( pnear.Side( pos[i])<0 )	continue;
		if( pfar.Side( pos[i])< 0 )	continue;
		if( left.Side( pos[i])< 0 )	continue;
		if( right.Side( pos[i])< 0 )continue;
		if( top.Side( pos[i])< 0 ) 	continue;
		if( bottom.Side( pos[i])< 0 )continue;
		
		return true;							//�S�ʓ����Ȃ�
	}
	return false;
}					
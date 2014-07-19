/** @file
 * �e�o�E���e�B���O(�Փ�)�I�u�W�F�N�g�̕\���N���X�̎���
 * @date 2009/06/05
 * @author ����
 **/

#include "BoundingObject.h"
#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "../graphics/IRender.h"
#include "../graphics/D3D9Render.h"

using namespace sugino;
using namespace game;
using namespace math;
using namespace graphics;

Object3D BoundingObject::box;
Object3D BoundingObject::sphere;

/** �~�`�� */
void BoundingObject::Draw( graphics::IRender *render,BoundingCircle2D &bobj,math::Matrix3x3 &m,DWORD col )
{
	ColorTVertex	tv[2];	
	WORD			index[2]={ 0,1 };
	float			r=0;
	float			ar=(Math::PI*2)/16;
	Vector2			s,e;
	s=(Vector2(r)*bobj.s)+bobj.p;		//�I�u�W�F�N�g�̔��a���i�񂾂Ƃ�����n�_�Ƃ���
	
	r+=ar;
	for(int i=0;i<16;i++)
	{
		e=(Vector2(r)*bobj.s)+bobj.p;
		tv[0]=ColorTVertex( m.TransformVector(s),col );
		tv[1]=ColorTVertex( m.TransformVector(e),col );
		render->drawLine( tv,index,2,1 );

		s=e;r+=ar;
	}	
};

/** ���`�� */
void BoundingObject::Draw( graphics::IRender *render,BoundingLine2D &bobj,math::Matrix3x3 &m,DWORD col )
{
	ColorTVertex	tv[2];	
	WORD			index[2]={ 0,1 };
	Vector2			s,e;
	s = bobj.s;						//�n�_
	e = bobj.e;						//�I�_

	tv[0]=ColorTVertex( m.TransformVector(s),col );
	tv[1]=ColorTVertex( m.TransformVector(e),col );
	render->drawLine( tv,index,2,1 );

};

/** AABB�`�� �l�p�@*/
void BoundingObject::Draw( graphics::IRender *render,BoundingBox2D &bobj,math::Matrix3x3 &m,DWORD col )
{
	ColorTVertex	tv[4];
	WORD			index[4][2]={ {0,1},{1,2},{2,3},{3,0} };

	Vector2			v1( -bobj.s.x , +bobj.s.y );		//�I�u�W�F�N�g�̎n�_�x�N�g��
	Vector2			v2( +bobj.s.x , +bobj.s.y );
	Vector2			v3( +bobj.s.x , -bobj.s.y );
	Vector2			v4( -bobj.s.x , -bobj.s.y );
		
	Matrix3x3 mm;
	mm.Initialize( Vector2(0,1),bobj.p );				//Vector2(0,1)�͏����
	mm *= m;

	tv[0] = ColorTVertex( mm.TransformVector( v1 ),col );
	tv[1] = ColorTVertex( mm.TransformVector( v2 ),col );
	tv[2] = ColorTVertex( mm.TransformVector( v3 ),col );
	tv[3] = ColorTVertex( mm.TransformVector( v4 ),col );

	render->drawLine( tv,&index[0][0],4,4 );
};

/** OBB�`�� �@��]�l�p */
void BoundingObject::Draw( graphics::IRender *render,BoundingOBox2D &bobj,math::Matrix3x3 &m,DWORD col )
{

	ColorTVertex	tv[4];
	WORD			index[4][2]={ {0,1},{1,2},{2,3},{3,0} };

	Vector2			v1( -bobj.s.x , +bobj.s.y );		//�I�u�W�F�N�g�̎n�_�x�N�g�� 
	Vector2			v2( +bobj.s.x , +bobj.s.y );
	Vector2			v3( +bobj.s.x , -bobj.s.y );
	Vector2			v4( -bobj.s.x , -bobj.s.y );
		
	Matrix3x3 mm;
	mm.Initialize( bobj.r,bobj.p );						// bobj.r�͏�����x�N�g��
	mm *= m;

	tv[0] = ColorTVertex( mm.TransformVector( v1 ),col );
	tv[1] = ColorTVertex( mm.TransformVector( v2 ),col );
	tv[2] = ColorTVertex( mm.TransformVector( v3 ),col );
	tv[3] = ColorTVertex( mm.TransformVector( v4 ),col );

	render->drawLine( tv,&index[0][0],4,4 );
};

//-----------------------------------------------------------------------------------
void BoundingObject::Draw( BoundingSphere3D &sp, IRender *render, Matrix4x4 &vps, Vector3 &dir, Color &col )
{
	if( sphere.getVertexCount()==0 ) 
	{
		sphere.CreateSphere( 16,12,0xffFFFFFF );			//���˂����邽�߂ɔ��łȂ���΂Ȃ�Ȃ�
	}

	DirectionLight3D light( dir,col );
	Color amb = col*0.5f;
	amb.a = col.a;//�����p
	//amb.a = 1;

	Matrix m,tmp;
	m.Scale( sp.s,sp.s,sp.s );
	m *= tmp.Translate( sp.p );

	sphere.Transform( m,vps );
	sphere.NormalNomalize();
	sphere.Lighting( light,amb );
	sphere.Draw( render );
}

void BoundingObject::Draw(sugino::game::BoundingBox3D &bbox, IRender *render, Matrix4x4 &vps, Vector3 &dir, Color &col )
{
	if( box.getVertexCount()==0 ) box.CreateBox(col);
	DirectionLight3D light( dir,col );
	Color amb = col *0.5f;amb.a = col.a;//amb.a = 1;				//����

	Matrix m,tmp;
	m.Scale( bbox.s );
	m *= tmp.Translate( bbox.p );

	box.Transform( m,vps );
	box.NormalNomalize();
	box.Lighting( light,amb );
	box.Draw( render );
}

void BoundingObject::Draw( BoundingOBox3D &obox, IRender *render, Matrix4x4 &vps,Vector3 &dir,Color &col)
{
	if( box.getVertexCount()==0 ) box.CreateBox(col);
	
	DirectionLight3D light( dir,col );								//����col�̃��C�g�𓖂Ă�
	Color amb = col*0.5f;
	amb.a = col.a;//amb.a = 1;

	Matrix rm( obox.ax.x,obox.ax.y,obox.ax.z,0,
			   obox.ay.x,obox.ay.y,obox.ay.z,0,
			   obox.az.x,obox.az.y,obox.az.z,0,
			   obox.p.x, obox.p.y,obox.p.z, 1 );

	Matrix m,tmp;
	m.Scale( obox.s );
	
	m *= rm;
	
	box.Transform( m,vps );
	box.NormalNomalize();
	box.Lighting( light,amb );
	box.Draw( render );
}

void BoundingObject::Draw( BoundingLine3D &bl, IRender *render, Matrix4x4 &vps, Vector3 &dir, Color &col)
{
	Vector center = ( bl.e+bl.s ) * 0.5f;
	Vector vec = ( bl.e-bl.s );
	float length = vec.Length()*0.5f;
	vec.Normalize();

	//��]���𖳗����쐬
	Matrix m;
	Vector &c = Vector3::Cross( vec,Vector(0,0,1) );
	if( c.Length2()!=0 ) c.Normalize();else c.setVector( 0,1,0 );
	float d = acosf( Vector3::Dot( vec,Vector(0,0,1 ) ));	//�x���擾
	m.RotateAxis( c, -d );

	BoundingOBox3D obox( center,Vector(bl.width,bl.width,length),m );
	Draw( obox,render,vps,dir,col );

	//������
	//BoundingSphere3D s2( bl.s,bl.width*2 );
	BoundingSphere3D s2( bl.s,bl.width );
	Draw( s2,render,vps,dir,col );
	//BoundingSphere3D s3( bl.e,bl.width*2 );	
	BoundingSphere3D s3( bl.e,bl.width );
	Draw( s3,render,vps,dir,col );
}

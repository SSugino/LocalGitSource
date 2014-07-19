/** @file
 * 2D�I�u�W�F�N�g�N���X�̎���
 * @date 2009/05/27
 * @author ����
 **/

#include "Object2D.h"
#include "XML.h"
#include "../math/Vector.h"
#include "../graphics/IRender.h"
using namespace sugino;
using namespace game;
using namespace math;
using namespace graphics;

void Object2D::Create(int n, float size, DWORD col, float sr )
{
	//--------------------------------
	//			�m��
	//----------------------------------
	/** ���_�� */
	vertex.resize( n+1 );
	/** �e�N�X�`�����_ */
	tvertex.resize( n+1 );
	/** �ʐ� */
	face.resize( n );
	/** �Ӑ� */
	edge.resize( n );

	/** �쐬 */
	int i;
	
	/** ���[�J�����W */
	vertex[0].p = Vector2::zero;
	/** �e�N�X�`��UV���W */
	vertex[0].t = Vector2( 0.5f,0.5f );
	vertex[0].c= col;

	for(i=1;i<vertex.size();i++)
	{
		/** ���[�J�����W */
		vertex[i].p = Vector2(sr)*size;
		/** �e�N�X�`�����W */
		vertex[i].t		= ( vertex[i].p/2 );
		vertex[i].t.y	=  -vertex[i].t.y;
		vertex[i].t	   +=	vertex[0].t;
		/** ���_�F */	
		vertex[i].c = col;
		/** �p�x�ړ� */
		sr -=( Math::PI*2/n );	// �p�� / 360 = 1�p�Ԃ̊p�x 
	}

	//�ʃf�[�^�쐬
	for(i=0;i<face.size();i++)
	{
		/** �ʂ��쐬 */
		setFace( &face[i],0,i+1,(i+1)%n+1 );
	}
	//���f�[�^�쐬
	for(i=0;i<edge.size();i++)
	{
		setEdge( &edge[i],i+1,(i+1)%n+1 );
	}
}

void Object2D::Transform( math::Vector2 &pos,float zoom,int w,int h )
{
	int i;
	int wid = w/2;
	int hei = h/2;
 
	for( i=0;i<vertex.size();i++ )
	{
		Vector2 tv = vertex[i].p;
		tv = (tv * zoom) + pos;
		tv.x =  ( tv.x * hei )+(float)wid;
		tv.y =  ( tv.y * (-hei) )+(float)hei;

		tvertex[i] = graphics::TextureColorTVertex( tv,vertex[i].t,vertex[i].c );
	}
}

void Object2D::Transform( math::Vector2 &pos,math::Vector2 &scale,float r,int w,int h )
{
	int i;
	float c = Math::cos( r );
	float s = Math::sin( r );
	int wid = w/2;
	int hei = h/2;

	for( i=0;i<vertex.size();i++ )
	{
		Vector2 tv = vertex[i].p, v=tv;
		
		/** �g�� */
		v.x = (tv.x * scale.x );			//���͕�������
		v.y = (tv.y * scale.y );
		
		/** ��] */
		tv.x = (v.x*c) - (v.y*s); 
		tv.y = (v.x*s) + (v.y*c);


		/** �ړ� */
		tv = ( tv + pos );
		
		tv.x =  ( tv.x * hei )+(float)wid;
		tv.y =  ( tv.y * (-hei) )+(float)hei;

		tvertex[i] = graphics::TextureColorTVertex( tv,vertex[i].t,vertex[i].c );
	}
}

void Object2D::Transform( const Matrix3x3 &m, const Matrix3x3 &m2 )
{
	for( int i=0;i<vertex.size();i++ )
	{
		Vector2 p;
		vertex[i].gp = m.TransformVector( vertex[i].p );	//���_�ړ��ϊ�
		p = m2.TransformVector( vertex[i].gp );				//�J�����X�N���[���ϊ�	
		tvertex[i] = graphics::TextureColorTVertex( p,vertex[i].t,vertex[i].c );
	}
}

void Object2D::Transform( Matrix3x3 *m, Matrix3x3 &cm)
{
	int i;
	for( i=0;i<vertex.size();i++)
	{
		/** �����̍s�� */
		Matrix3x3 &m1 = m[vertex[i].blend1 ];
		/** �e�̍s�� */
		Matrix3x3 &m2 = m[vertex[i].blend2 ];
		/** m1��m2�̕�� */
		Matrix3x3 mm = ( m1 - m2  ) * vertex[i].weight + m2 ;

		vertex[i].gp = mm.TransformVector( vertex[i].p );
		Vector2 v	 = cm.TransformVector( vertex[i].gp );
		tvertex[i]	 = TextureColorTVertex( v,vertex[i].t,vertex[i].c );
	}
}




void Object2D::Draw(sugino::graphics::IRender *r)
{
	r->drawPolygon( tvertex,&face[0][0],vertex.size(),face.size() );
}

void Object2D::DrawEdge(sugino::graphics::IRender *r)
{
	r->drawLine( tvertex,&edge[0][0],vertex.size(),edge.size() );
}

void Object2D::Lighting( Light2D *lights, int l_count )
{
	int i,j;
	for( i=0;i<vertex.size();i++ )
	{
		Vector2 &pos = vertex[i].gp;		//���_�ʒu
		Color c( vertex[i].c );				//���_�F
		for( j=0;j<l_count;j++ )
		{
			lights[j].getBrightColor( &c,pos );
		}
		tvertex[i].c=c;
	}
}

//�e�L�X�g����ǂݍ���Create�֐�
char *Object2D::Create( char *text )
{
	XMLElement xobj( "OBJECT",text );			//�^�u[OBJECT]������
	//�e�탁�����m�� (�e�L�X�g����w�蕶��������o��)
	/** ���_ */
	vertex.resize( atoi( xobj["vertex"] ) );	//���̒���vertex�������T��
	/** �e�N�X�`�����_ */
	tvertex.resize( vertex.size() );			//���̒���
	/** �ʐ� */
	face.resize( atoi( xobj["face"] ) );		//���̒���face�������T��
	/** �Ӑ� */
	edge.resize( atoi( xobj["edge"] ) );		//���̒���edge�������T��
	/** ���O */
	name = xobj["name"] ;						//���̒���name�������T��
	
	int i;
	//POSITION
	{
		XMLElement xpos( "POSITION",xobj.getText() );	//OBJECT�^�O���ŒT��
		char *p = xpos.getText();
		for( i=0;i<vertex.size();i++ )
		{
			float x = Float::Atof( p=strstr(p,"(")+1 );	//( �� ,  ,  )�̕���
			float y = Float::Atof( p=strstr(p,",")+1 );	//(   , �� , )
			vertex[i].p.setVector(x,y);					//���W������
		}
	}
	
	//COLOR
	{
		XMLElement xcol( "COLOR", xobj.getText() );
		char *p = xcol.getText();
		for( i=0;i<vertex.size();i++ )
		{
			DWORD col = strtoul( p=strstr(p,"(")+1 ,NULL,16 );
			vertex[i].c = col;
		}
	}
	
	//UV
	{
		XMLElement xuv( "UV",xobj.getText() );
		char *p = xuv.getText();
		for( i=0;i<vertex.size();i++ )
		{
			float x = Float::Atof( p=strstr(p,"(")+1 );	//( �� ,  ,  )�̕���
			float y = Float::Atof( p=strstr(p,",")+1 );	//(   , �� , )
			vertex[i].t.setVector(x,y);					//���W������
		}
	}
	
	//WEIGHT
	{
		XMLElement xwei( "WEIGHT",xobj.getText() );
		char *p = xwei.getText();
		for( i=0;i<vertex.size();i++)
		{
			float wei = Float::Atof( p=strstr(p,"(")+1 );	//( �� ,  ,  )�̕���
			vertex[i].weight = wei;
		}
	}

	//BLEND_INDEX
	{
		XMLElement	xblend( "BLEND_INDEX",xobj.getText() );
		char *p = xblend.getText();
		for( i=0;i<vertex.size();i++)
		{
			BYTE blend1 = (BYTE)atoi(  p=strstr(p,"(")+1 );
			BYTE blend2 = (BYTE)atoi(  p=strstr(p,",")+1 );
			vertex[i].blend1 = blend1;
			vertex[i].blend2 = blend2;
		}
	}
	//FACE 
	{
		XMLElement xface( "FACE",xobj.getText() );
		char *p = xface.getText();
		for( i=0;i<face.size();i++ )
		{
			WORD n1 = (WORD)atoi( p=strstr(p,"(")+1 );
			WORD n2 = (WORD)atoi( p=strstr(p,",")+1 );
			WORD n3 = (WORD)atoi( p=strstr(p,",")+1 );
			setFace( &face[i],n1,n2,n3 );
		}
	}

	//EDGE
	{
		XMLElement xedge( "EDGE",xobj.getText() );
		char *p = xedge.getText();
		for(i=0;i<edge.size();i++)
		{
			WORD n1 = (WORD)atoi( p=strstr(p,"(")+1 );
			WORD n2 = (WORD)atoi( p=strstr(p,",")+1 );
			setEdge( &edge[i],n1,n2 );
		}
	}
	return text+xobj.getLength();		//��������Ԃ�
}

void Object2D::BlendObject( Object2D& obj1,Object2D &obj2,float t )
{
	/** ���_�����قȂ�ꍇ�͏������Ȃ� */
	if( this->vertex.size() != obj1.vertex.size() ||
		obj1.vertex.size()	!= obj2.vertex.size() ) return;

	for( int i=0;i<vertex.size();i++ )
	{
		const Vector2 &v1 = obj1.vertex[i].p;
		const Vector2 &v2 = obj2.vertex[i].p;

		vertex[i].p = ( v2 - v1 )*t + v1;			//���`���
	}
}


void Model2D::Load( char *name )
{
	DWORD size;
	system::AutoPtr<char> text;

	FILE *fp = fopen( name,"rb" );		//�w��t�@�C����ǂݍ���
	if( fp==NULL ) throw system::Exception( "�t�@�C�����I�[�v���G���[" );
	fseek( fp,0,2 );
	size = ftell(fp);					//�t�@�C���T�C�Y�擾
	fseek( fp,0,0 );
	
	text = new char[size+1];
	fread( text,size,1,fp );
	fclose( fp );						//�����܂œǂݍ���
	
	/** ���f���̓ǂݍ��� */
	XMLElement xmodel( "MODEL2D",text );//��قǓǂݍ��񂾃e�L�X�g����"MODEL2D"�^�O����
	int cnt = atoi( xmodel["count"] );	//���̒���count�������T���@�����𐔒l�ϊ�
	
	objs.resize( cnt );					//�����oobjs�������m��

	char* p = xmodel.getText();			//���̃e�L�X�g�����Ƃ��Ă���
	for(int i=0;i<cnt;i++ )
	{
		p = objs[i].Create(p);			//Create�߂�l���󂯎��=���̃I�u�W�F�N�g���Q�Ƃ���
	}

	/** �{�[���̓ǂݍ��� */
	XMLElement xbone( "BONE_LIST",text );
	
	bones.resize( atoi( xbone["count"]) );		//�{�[�����������m��
	char* p2=xbone.getText();					//BONE_LIST�������ۑ�
	for(int i=0;i<bones.size();i++ )
	{
		XMLElement xbone2d( "BONE2D",p2 );
		bones[i].name = xbone2d["name"] ;
		bones[i].local.x = Float::Atof(xbone2d["x"] );	//�����l��( )�Ŋ����ĂȂ��̂�
		bones[i].local.y = Float::Atof(xbone2d["y"] );
		bones[i].parent  = atoi( xbone2d["parent"] );
		p2+=xbone2d.getLength();							//���{�[�����Q��
	}
}


int ModelAnimation2D::setMotionMatrix( KeyFrame2D *kf,float *time )
{
	int n = 0,ret = 0;
	/** �Đ��ʒu���� */
	while( kf[n+1].motion && kf[n+1].time< *time ) n++;		//�����L�[�t���[���z���motion�Ǝ��Ԃ���v���Ă��邩
	
	/** ��Ԓl(0.0�`1.0)�����߂� */
	float start = kf[n].time;	//�J�n���ԕۑ�
	float end = kf[n+1].time;	//�I�����ԕۑ�
	/** ���Ԃ̕�� */
	float t = ( *time - start ) / ( end - start );

	Motion2D *now = kf[n+0].motion;		//���̃��[�V����
	Motion2D *next = kf[n+1].motion;	//���̃��[�V����

	if( next==NULL )
	{
		next = kf[0].motion;			//0�ԖڎQ��
		ret=2;							//�Ō�܂ŗ����Ȃ�
		if( t>=1 )						//��Ԓl�ő�Ȃ�
		{ *time=0; ret=1; }				//���Ԃ�0�ɂ����[�v�A�j���I��
	}

	/** ��� */
	int i;
	for( i=0;i<amat.size();i++,now++,next++ )
	{
		float	r = (	next->r		- now->r	)*t	+now->r;
		Vector2 s = (	next->size	- now->size	)*t +now->size;
		/** �s��|�����킹 */
		amat[i] = Matrix3x3().Scale( s )  *  Matrix3x3().Rotate( r ); 

	}

	return ret;
}

void ModelAnimation2D::setBoneMatrix( Matrix3x3 &root )
{
	int i;
	Matrix3x3 t1,t2;
	for( i=0;i<amat.size();i++)
	{
		Vector2 &v = bone[i].local;
		t1.Translate( -v );			//�n�_��e�{�[����
		t2.Translate( v );

		if( i )
		{
			amat[i] = ( t1 * amat[i] * t2 ) * amat[bone[i].parent];	//�e�ƍ���
		}else{
			amat[i] = ( t1 * amat[i] * t2 ) * root;					//�C�ӂ̍s��
		}
	}
}

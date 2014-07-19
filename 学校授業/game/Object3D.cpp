/** @file
 * 3D�I�u�W�F�N�g�N���X�̎���
 * @date 2009/07/21
 * @author ����
 **/

#include "Object3D.h"
#include "XML.h"
#include "../math/Vector.h"
#include "../graphics/IRender.h"
using namespace sugino;
using namespace game;
using namespace math;
using namespace graphics;
using namespace system;

void Object3D::Create( int v_size,int f_size )
{
	//--------------------------------
	//			�m��
	//----------------------------------
	/** ���_�� */
	vertex.resize( v_size );
	/** �e�N�X�`�����_ */
	tvertex.resize( vertex.size() );
	/** �ʐ� */
	face.resize( f_size );
}

//XML�`���œǂݍ���
char *Object3D::Load( char *text )
{
	XMLElement xobj( "OBJECT",text );			//�^�u[OBJECT]������
	//�e�탁�����m�� (�e�L�X�g����w�蕶��������o��)
	/** ���_ */
	vertex.resize( atoi( xobj["vertex"] ) );	//���̒���vertex�������T��
	/** �e�N�X�`�����_ */
	tvertex.resize( vertex.size() );			
	/** �ʐ� */
	face.resize( atoi( xobj["face"] ) );		//���̒���face�������T��
	/** ���O */
	name = xobj["name"] ;						//���̒���name�������T��
	
	int i;
	//POSITION
	{
		XMLElement xpos( "POSITION",xobj.getText() );	//OBJECT�^�O���ŒT��
		char *p = xpos.getText();
		for( i=0;i<vertex.size();i++ )
		{
			float x = Float::Atof( p=strstr(p,"(")+1 );	//( ��,  ,  )�̕���
			float y = Float::Atof( p=strstr(p,",")+1 );	//(   ,��, �@)
			float z = Float::Atof( p=strstr(p,",")+1 );	//(   ,  , �� )

			vertex[i].p.setVector(x,y,z);					//���W������
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

	//NORMAL
	{
		XMLElement xnormal( "NORMAL",xobj.getText() );
		char *p = xnormal.getText();
		for( i=0;i<vertex.size();i++ )
		{
			float x = Float::Atof( p=strstr(p,"(")+1 );	//( �� ,  ,  )�̕���
			float y = Float::Atof( p=strstr(p,",")+1 );	//(   , �� , )
			float z = Float::Atof( p=strstr(p,",")+1 );	//(   ,  , �� )

			vertex[i].n.setVector(x,y,z);				//���W������
		}
	}

	//COLOR
	{
		XMLElement xcol( "COLOR", xobj.getText() );
		char *p = xcol.getText();
		for( i=0;i<vertex.size();i++ )
		{
			DWORD col = strtoul( p=strstr(p,"(")+1 ,NULL,16 );		//16�i�Ŏ擾
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

	return text+xobj.getLength();		//��������Ԃ�
}

void Object3D::Transform( const Matrix4x4 &m1, const Matrix4x4 &m2 )
{
	for( int i=0;i<vertex.size();i++ )
	{
		Vertex3D		&ver = vertex[i];		//���_�f�[�^
		ver.gn = m1.TransformRotation( ver.n );
		ver.gp = m1.TransformVector( ver.p );	//�O���[�o�����W�Ɏ����Ă���
		Vector4 v = m2.Transform( vertex[i].gp );					//�J�����X�N���[���ϊ�
		v.w  = 1/v.w;			
		v.x *= v.w;v.y *=v.w;v.z *=v.w;//�t���|����

		tvertex[i] = graphics::TextureColorTVertex( v,ver.t,ver.c );
	}
}
//�A�j���̕ϊ�
void Object3D::Transform( Matrix4x4 *m,Matrix &m2 )
{
	for( int i=0;i<vertex.size();i++ )
	{
		Vertex3D	&ver = vertex[i];		//���_�f�[�^
		Matrix		&mm1 = m[ver.blend1];	//�u�����h�Q�Ɓ@�i�ǂ̃{�[�����Q�Ƃ��邩)
		Matrix		&mm2 = m[ver.blend2];
		Matrix m1=(mm1-mm2)*ver.weight+mm2;				//��]�ɐe(weight)�̉e����^����
		
		Vector4 v;

		ver.gn = m1.TransformRotation(ver.n );					//�J�����X�N���[���ϊ�
		ver.gp = m1.TransformVector( ver.p );
		v = m2.Transform( ver.gp );
		v.w  = 1/v.w;			
		v.x *= v.w;v.y *=v.w;v.z *=v.w;//�t���|����
		tvertex[i] = graphics::TextureColorTVertex( v,ver.t,ver.c );
			
	}
}

//�w�i�̕ϊ�
void Object3D::Transform( Matrix4x4 &vps )
{
	for(int i=0;i<vertex.size();i++)
	{
		Vertex3D &v = vertex[i];
		for(int i=0;i<vertex.size();i++)
		{
			v.gp =v.p; v.gn = v.n;				//�w�i�͕ϊ�����K�v���Ȃ�
			Vector4 tv = vps.Transform( v.gp );
			tv.w =1/tv.w;
			tv.x *= tv.w;tv.y *=tv.w;tv.z=1/tv.w;
			tvertex[i] = TextureColorTVertex( tv,v.t,v.c );
		}
	}
}


void Object3D::Draw( graphics::IRender *r)
{
	r->drawPolygon( tvertex,&face[0][0],vertex.size(),face.size() );
}

//----------------------------------------------------------------------------------------
void Object3D::CreateBox( DWORD col )
{
	Create( 8,6*2 );	//���_�� �ʐ�
	//���_��������
	//��O�̒��_
	vertex[0].p = math::Vector3( -1,+1,-1 );	vertex[0].c = col;
	vertex[1].p = math::Vector3( +1,+1,-1 );	vertex[1].c = col;
	vertex[2].p = math::Vector3( +1,-1,-1 );	vertex[2].c = col;
	vertex[3].p = math::Vector3( -1,-1,-1 );	vertex[3].c = col;
	//���̒��_
	vertex[4].p = math::Vector3( -1,+1,+1 );	vertex[4].c = col;
	vertex[5].p = math::Vector3( +1,+1,+1 );	vertex[5].c = col;
	vertex[6].p = math::Vector3( +1,-1,+1 );	vertex[6].c = col;
	vertex[7].p = math::Vector3( -1,-1,+1 );	vertex[7].c = col;
	
	//�e�N�X�`���̏�����
	vertex[0].t.setVector( 0,0 );
	vertex[1].t.setVector( 2,0 );
	vertex[2].t.setVector( 2,2 );
	vertex[3].t.setVector( 0,2 );

	vertex[4].t.setVector( 0,0 );
	vertex[5].t.setVector( 2,0 );
	vertex[6].t.setVector( 2,2 );
	vertex[7].t.setVector( 0,2 );


	for(int i=0;i<vertex.size();i++)
	{
		vertex[i].n = vertex[i].p;		//���_���W�����̂܂ܖ@���x�N�g����
		vertex[i].n.Normalize();		//�P�ʃx�N�g����
	}

	//�ʏ����� (�l�p�ŏ�����)
	setFace( &face[0],0,1,2,3 );
	setFace( &face[2],1,5,6,2 );
	setFace( &face[4],4,7,6,5 );
	setFace( &face[6],0,3,7,4 );
	setFace( &face[8],4,5,1,0 );
	setFace( &face[10],7,3,2,6 );
}

void Object3D::CreatePlane( int nx,int nz,DWORD col )
{
	Create( (nx+1)*(nz+1),nx*nz *2 );	//���_�� �ʐ�*2(�O�p�`�Ȃ̂� *2)
	//�������߂Ē��_��������
	float sx,sz=-1,ssx=-1;//ssx�͎�O��X�̃X�^�[�g�n�_
	float ax,az;
	ax = 2.0f/nx;	//x��
	az = 2.0f/nz;	//z��

	int i,j,n=0;
	for( i=0;i<=nz;i++ )	//x��   <'='�@�@�C�R�[�����厖
	{
		sx=ssx;
		for( j=0;j<=nx;j++ ) //z
		{
			vertex[n].p.setVector( sx,0,sz );
			vertex[n].n.setVector( 0,1,0 );//�ʂ̌���
			vertex[n].c  = col;//rand()|0xFF000000;
									//�e�N�X�`��������
			sx += ax; n++;//�������� / �i�߂�
		}
		sz += az;	//��������
	}

	//�ʏ����� (�l�p�ŏ�����)
	n=0;				//�ʂ̏ꏊ�i���j
	int b1=0;//�i
	int b2=nx+1;//b1�̏�̒i
	for( i=0;i<nz;i++ )	//  < �C�R�[������Ȃ�
	{
		for( j=0;j<nx;j++ ) //
		{
			setFace( &face[n],b1+j,b2+j,b2+j+1,b1+j+1 );
			n+=2;
		}
		b1+=(nx+1);
		b2+=(nx+1);
	}
}

// nr:�~�̕����� *v:��](���_)�f�[�^ nv:�f�[�^�� 
void Object3D::CreateRotate( int nr, Vector2 *v, int nv, DWORD col )
{
	Create( nr*nv + 2, nr*(nv-1)*2 + (nr*2) );

	//�������߂Ē��_��������
	int i,j,n=1;
	float ar = (Math::PI*2)/nr;			//	x��  ������/360
	//��̓_
	vertex[0].p.setVector( 0,v[0].y,0 );
	vertex[0].c=col;
	//vertex[0].c=rand()|0xFF0000FF;

	//����̓_
	for( i=0;i<nv;i++ )			//�c
	{
		float size,y,r=0;
		size = v[i].x;			//���a
		y	 = v[i].y;			//�n�_

		for( j=0;j<nr;j++ )		//�~���[�v
		{
			float z = Math::cos( r )*size;		//Cos
			float x = Math::sin( r )*size;		//Sin
			
			vertex[n].p.setVector( x,y,z );
			vertex[n].c = col;
			//vertex[n].c=rand()|0xFF00FF00;
									//�e�N�X�`��������
			n++;
			r += ar;				//�~�̑���
		}
	}
	//���̓_
	vertex[n].p.setVector( 0,v[nv-1].y,0 );
	vertex[n].c = col;
	//vertex[n].c=rand()|0xFFFF0000;
	
	//----------------------------------------------------------------
	n=0;							//�ʂ̏ꏊ�i���j
	//��̖�
	for(int i=0;i<nr;i++) setFace( &face[n++],0,1+i,1+(i+1)%nr );
	//���̖�
	int b1=1;						//�i
	int b2=nr+1;					//b1�̉��̒i
	
	for( i=1;i<nv;i++ )	//  < �C�R�[������Ȃ�
	{
		for( j=0;j<nr;j++ )
		{
			setFace( &face[n],b1+j,b2+j,b2+(j+1)%nr,b1+(j+1)%nr );
			n+=2;					//���Ă���ʂ�i�߂�
		}
		b1+=nr; //��i����
		b2+=nr;	//b1�̈�i����
	}
	//���̖�
	
	int end = vertex.size()-1;		//�I�_�̖ʁi���S�j
	for(i=0;i<nr;i++)setFace( &face[n++],end,(end-1)-i,(end-1)-(i+1)%nr );		//�����v���Ŗʂ��o��	��b1�ɒ���
	//for(i=0;i<nr;i++)setFace( &face[n++],end,b1+i+1,(b1+i)%nr );
	this->Normalize();
}

void Object3D::CreateCylinder( int nr, int n, float top, float bottom, DWORD col )
{
	system::AutoArray< math::Vector2 > v( n+1 );
	float y=1,ay = 2.0f/n ;
	float s = top,as=( bottom-top)/n;

	for( int i=0;i<v.size();i++)
	{
		v[i].setVector( s,y );
		s+= as;
		y-= ay;
	}

	CreateRotate( nr,&v[0],v.size(),col );
}

void Object3D::CreateSphere(int nr, int ny, DWORD col)
{	
	AutoArray< math::Vector2 > v(nr+2);
	float ar = Math::PI/ny;			//��_�Ԃ̊p�x �����ŗǂ�
	float r = ar;
	for(int i=0;i<v.size();i++)
	{
		v[i].setVector( Math::sin( r ), Math::cos( r ) );	//�~��`���x�N�g�����Z�b�g
		r+=ar;
	}
	CreateRotate( nr,v,ny,col );
	//��̓_�Ɖ��̓_���ړ�
	vertex[0].p.y = +1;
	vertex[ vertex.size()-1 ].p.y = -1;
}

//----------------------------------------------------------------------------------------------------
void Object3D::Normalize()
{
	AutoArray< WORD > count( vertex.size() );//���ϒl�擾�p
	int i;
	for(i=0;i<vertex.size();i++)
	{
		vertex[i].n.setVector( 0,0,0 );
		count[i] = 0;
	}
	for(i=0;i<face.size();i++)
	{
		ModelFace &f = face[i];
		const Vector3 &v1 = vertex[f[0]].p;
		const Vector3 &v2 = vertex[f[1]].p;
		const Vector3 &v3 = vertex[f[2]].p;

		Vector3 n = Vector3::Cross( v2-v1,v3-v1 );
		if( n.Length2() )//�������H�����͂��邩�H
		{
			n.Normalize();
			vertex[f[0]].n += n;count[f[0]]++;
			vertex[f[1]].n += n;count[f[1]]++;
			vertex[f[2]].n += n;count[f[2]]++;
		}
	}
	for(i=0;i<vertex.size();i++)
	{
		if(count[i]) vertex[i].n /= (float) count[i];
	}
}

//-----------------------------------------------------------------------------------------
void Object3D::Lighting( Light3D **lights, int count, Color &amb )
{
	int i,j;
	for( i=0;i<vertex.size();i++ )
	{
		Vertex3D &v = vertex[i];
		Color out_col(amb),col( v.c );
		for( j=0;j<count;j++ )
		{
			out_col += lights[j]->GetLightingColor( v.gp, v.gn,col );
		}
		tvertex[i].c = out_col;
	}	
}

void Object3D::Lighting( Light3D &_light, math::Color &amb)
{
	Light3D *light[] = { &_light };
	Lighting( light,1,amb );			//������̃��C�e�B���O���� count��1
}

//------------------------------------------------------------------------------------------
void Object3D::setFogAlpha( float fog_s, float fog_e )
{
	int i;
	float fog_len = fog_e - fog_s;							//�͈�

	for( i=0;i<vertex.size();i++ )
	{
		Color c( tvertex[i].c );
		float fz = ( (1/tvertex[i].w)-fog_s )/fog_len;		//0�`1�����܂�
		if( fz<0 ) fz=0;else if( fz>1.0f )fz = 1.0f;
		//�A���t�@�l�Ŕw�i�ɗn�����܂�
		tvertex[i].c = tvertex[i].c & 0xFFFFFF|(DWORD)( 255-fz*255 )<<24;	//�A���t�@�l�݂̂����炵�Ă���
	}
}

void Object3D::setFogColor( float fog_s, float fog_e, Color &fog_col )
{
	int i;
	float fog_len = fog_e - fog_s;							//�͈�

	for( i=0;i<vertex.size();i++ )
	{
		Color c( tvertex[i].c );
		float fz = ( (1/tvertex[i].w)-fog_s )/fog_len;		//0�`1�����܂�
		if( fz<0 ) fz=0;else if( fz>1.0f )fz = 1.0f;
		//�t�H�O�̐F�i�w�i�F�j�ɕ�Ԃŗn�����܂�
		tvertex[i].c = ( fog_col - c )*fz + c;
	}
}

//---------------------------------------------------------------------------------------------
void Model3D::Load( const char *file_name )
{
	DWORD size;
	system::AutoPtr<char> text;

	FILE *fp = fopen( file_name,"rb" );		//�w��t�@�C����ǂݍ���
	if( fp==NULL ) throw system::Exception( "���f���t�@�C���I�[�v���G���[" );
	fseek( fp,0,2 );
	size = ftell(fp);						//�t�@�C���T�C�Y�擾
	fseek( fp,0,0 );
	
	text = new char[size+1];
	fread( text,size,1,fp );
	fclose( fp );							//�����܂œǂݍ���
	
	/** ���f���̓ǂݍ��� */
	XMLElement xmodel( "MODEL3D",text );//��قǓǂݍ��񂾃e�L�X�g����"MODEL3D"�^�O����
	int cnt = atoi( xmodel["count"] );	//���̒���count�������T���@�����𐔒l�ϊ�
	
	objs.resize( cnt );					//���f����(count)�������oobjs�Ƀ������m��

	char* p = xmodel.getText();			//���̃e�L�X�g�����Ƃ��Ă���
	for(int i=0;i<cnt;i++ )
	{
		p = objs[i].Load(p);			//Object3D::Load�߂�l���󂯎��=���̃I�u�W�F�N�g���Q�Ƃ���
	}

	/** �{�[���̓ǂݍ��� */
	try{											//BONE_LIST���Ȃ���������Ȃ����
		XMLElement xbone( "BONE_LIST",text );

		bones.resize( atoi( xbone["count"]) );		//�{�[�����������m��
		char* p2=xbone.getText();					//BONE_LIST�������ۑ�
		for(int i=0;i<bones.size();i++ )
		{
			XMLElement xbone3d( "BONE3D",p2 );
			bones[i].name = xbone3d["name"] ;
			bones[i].local.x = Float::Atof(xbone3d["x"] );	//�����l��( )�Ŋ����ĂȂ��̂�
			bones[i].local.y = Float::Atof(xbone3d["y"] );
			bones[i].local.z = Float::Atof(xbone3d["z"] );
			bones[i].parent  = atoi( xbone3d["parent"] );
			p2+=xbone3d.getLength();							//���{�[�����Q��
		}
	 }catch( system::Exception& ){}
}

//------------------------------------------------------------------------------------------
int ModelAnimation3D::setMotion( Matrix4x4 &root, KeyFrame3D *kf, float *time )
{
	int n = 0,ret = 0;
	//���݂��ǂ̃��[�V���������Ă��邩���ׂ�
	while( kf[n+1].motion && kf[n+1].time < *time ) n++;

	//���̃��[�V�����Ǝ��̃��[�V�����Ԃ̎��Ԃ�0�`1.0�ɂ���
	float t = *time - kf[n].time;
	t = t/( kf[n+1].time - kf[n].time);
	if( t>1 ){ t = 1; }

	Motion3D *now = kf[n].motion;					//���݂̃A�j���[�V����
	Motion3D *next = kf[n+1].motion;				//���̃A�j���[�V����

	//���̃��[�V�������Ȃ��ꍇ�͐擪�ɖ߂�
	if(next==0){
		next=kf[0].motion;
		if(t>=1){ *time=0; ret=1; }
	}

	int i;
	Matrix m;
	for( i=0;i<mat.size();i++ )
	{
		//���
		Quaternion &q = Quaternion::Slerp( now->rot,next->rot, t );

		mat[i] = Matrix(q);			//��]�s����擾
		now++;next++;
		//Bone�����_�Ƃ��ĉ�]�������s���
		Vector &v = bone[i].local;
		m.RotatePos( mat[i] ,v );		//�C�ӓ_�ŉ�]������

		if( i ) mat[i] = m*mat[ bone[i].parent ];
		else    mat[0] = m*root;
	}
	return ret;
}
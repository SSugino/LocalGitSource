/** @file
 * 2D�I�u�W�F�N�g�N���X�̒�`
 * @date 2009/05/27
 * @author ����
 **/

#include "../graphics/IRender.h"
#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "../math/Color.h"

#pragma once

namespace sugino{
namespace game{
	class Light2D;	//����`

	/** �� */
	typedef WORD ModelEdge[2];				
	
	/** �� */
	typedef WORD ModelFace[3];
	
	/** 2D���_�\���� */
	struct Vertex2D
	{
		/** ���[�J�����W */
		math::Vector2 p;
		
		/** ���_�F */
		DWORD	c;
		
		/** �O���[�o�����W���̒��_�f�[�^ */
		math::Vector2 gp;

		/** �e�N�X�`�����W */
		math::Vector2 t;
		
		/** �{�[���s��̃C���f�b�N�X */
		WORD blend1;
		
		/** �{�[���s��̃C���f�b�N�X(�e�ԍ�) */
		WORD blend2;
		/** �E�F�C�g�l */
		float weight;

		Vertex2D(){}
		Vertex2D(math::Vector2 &pos,DWORD col,math::Vector2 &tex ) :
		p(pos),c(col),t(tex){ }
		
	};

	/** 2D���[�V�����f�[�^�\���� */
	struct Motion2D
	{
		/** �p�x */
		float		r;
		/** �傫�� */
		math::Vector2 size;

		Motion2D(){}
		Motion2D( float rr,const math::Vector2 &s ) : r(rr),size(s) {}
		/** ���W�A���l������ */
		Motion2D( float rr ) : r(rr),size( 1,1) {}
		/** �p�x�w�� */
		Motion2D( int rr ) : r(rr*math::Math::PI/180),size(1,1) {}
	};

	/** 2D�L�[�t���[���f�[�^�\����
	 *	�A�j���[�V�����̏I����NULL	
	 **/
	struct KeyFrame2D
	{
		/** ���� */
		float		time;		//���^�C���ڂ�"�R��"���o���ƂȂ�
		/** �p�x��T�C�Y�̓��������[�V�����f�[�^���󂯎�� */
		Motion2D	*motion;
	};
	
	/** 2D�{�[��(�֐�)�f�[�^�\���� */
	struct Bone2D
	{
		/** �{�[���� */
		std::string name;		
		/** ���_ */
		math::Vector2 local;
		/** �e�{�[���ԍ� */
		int parent;
		
		Bone2D(){}
		Bone2D( math::Vector2 &v,int p ) : local(v),parent(p){} 
	};

	/** 2D�}�`�I�u�W�F�N�g */
	class Object2D
	{
	private:
		/** ���_ */
		system::AutoArray<Vertex2D>							vertex;
		/** �e�N�X�`�� */
		system::AutoArray<graphics::TextureColorTVertex>	tvertex;
		/** �� */
		system::AutoArray<ModelFace>						face;
		/** �� */
		system::AutoArray<ModelEdge>						edge;

		/** �I�u�W�F�N�g�� */
		std::string											name;
	public:
		/** ���p�`�̍쐬
		 * @param [in] int	 n		:�p��
		 * @param [in] floatg size	:�T�C�Y
		 * @param [in] DWORD  col	:�F
		 * @param [in] float  sr	:�J�n�p�x
		 **/
		void Create( int n,float size,DWORD col,float sr=0 );
		/** �e�L�X�g�f�[�^���炨�Ԃ������Ƃ�����������
		 * @param [in] text	: �e�L�X�g�f�[�^
		 * @return �ǂݍ��񂾃e�L�X�g�̏I���A�h���X
		 **/
		char *Create( char *text );
		/** ���W�ϊ�
		 * @param [in] Vector2 &pos	: ���w���W�n�̍��W
		 * @param [in]   float zoom : �g��T�C�Y(0.0-1.0)
		 * @param [in]	   int  w	: ��ʕ�
		 * @param [in]	   int  h	: ��ʍ���
		 **/
		void Transform( math::Vector2 &pos,float zoom,int w,int h );
		/** ���W�ϊ�
		 * @param [in] Vector2 &pos		: ���w���W�n�̍��W
		 * @param [in] Vector2 &scale	: �g��T�C�Y�̍��W(0.0-1.0)
		 * @param [in]	 float  r		: ���W�A���l(PI/180���etc)
		 * @param [in]	   int  w		: ��ʕ�
		 * @param [in]	   int  h		: ��ʍ���
		 **/
		void Transform( math::Vector2 &pos, math::Vector2 &scale,float r,int w,int h );
		/** ���W�ϊ�(�s��)
		 * @param [in] Matrix3x3 &m		:�s��( �ړ����]��X�P�[�� )���ł����܂�Ȃ��i�O���[�o���ϊ��p�j
		 * @param [in] Matrix3x3 &m2	:�s��( �ړ����]��X�P�[���E�J���� )
		 **/
		void Transform( const math::Matrix3x3 &m, const math::Matrix3x3 &m2 );
		
		/** ���W�ϊ� �s����u�����h����
		 * @param [in] Matrix3x3 &m		:�s��̔z��
		 * @param [in] Matrix3x3 &m2	:
		 **/
		void Transform( math::Matrix3x3 *m, math::Matrix3x3 &m2 );

		/** �I�u�W�F�N�g�̖ʕ`��
		 * @param [in] IRender *r		:�摜���@
		 **/
		void Draw( graphics::IRender *r );

		/** �I�u�W�F�N�g�̐���`�悷��
		 * @param [in] IRender *r		:�摜���@
		 **/
		void DrawEdge( graphics::IRender *r );

		/** ���f���̃��[�t�B���O
		 * @param [in]	Object2D &obj	:���f���P
		 * @param [in]	Object2D obj2	:���f���Q
		 * @param [in]	   float  t		:��Ԓl
		 **/
		void BlendObject( Object2D &obj1,Object2D &obj2,float t );

		//--------------------------------------------------------
		//	utility functions ���[�e�B���e�B�֐�
		//--------------------------------------------------------
		/** �ʂ�������
		 * @param [out]	f:	�����������
		 * @param [in] n1:	�C���f�b�N�X�P
		 * @param [in] n2:	�C���f�b�N�X�Q
		 * @param [in] n3:	�C���f�b�N�X�R
		 **/
		static void setFace( ModelFace *f,WORD n1,WORD n2,WORD n3 )
		{
			(*f) [0]=n1;
			(*f) [1]=n2;
			(*f) [2]=n3;
		}

		/** �ӂ�������
		 * @param [out] f:	�����������
		 * @param [in] n1:	�C���f�b�N�X�P
		 * @param [in] n2:	�C���f�b�N�X�Q
		 **/
		static void setEdge( ModelEdge *f,WORD n1,WORD n2 )
		{
			(*f)[0]=n1;
			(*f)[1]=n2;
		}

		//----------------------------------------------------------
		// getter
		//----------------------------------------------------------
		/** ���_�擾 */
		Vertex2D &getVertex( int i ){ return vertex[i]; }

		/** ���_"��"�擾 */
		int getVertexCount(){ return vertex.size(); }

		/** �ʎ擾 */
		ModelFace &getFace( int i ){ return face[i]; }

		/** ��"��"�擾 */
		int getFaceCount(){ return face.size(); }

		/** �ӎ擾 */
		ModelEdge &getEdge( int i ){ return edge[i]; }

		/**��"��"�擾 */
		int getEdgeCount(){ return edge.size(); }

		/** �����̌v�Z
		 * @param [out] Light2D Lights	: �������������F
		 * @param [out] int		l_count : ���C�e�B���O�������
		 **/
		void Lighting( Light2D *lights,int l_count );
		/** �v�������_�����O(���炩���ߍ��) 
		 *	���_�f�[�^�����̒��_�ɑ�����Ă���
		 **/
		void PreTransform()
		{ 
			for(int i=0;i<vertex.size();i++)
			{
				vertex[i].p = vertex[i].gp;
			}
		}
		/** �v�������_�����O�̃��C�e�B���O
		 *  ���_�����̐F�ɖʂ̐F�������Ă���
		 **/
		void PreLighting()
		{
			for(int i=0;i<vertex.size();i++)
			{
				vertex[i].c = tvertex[i].c;
			}
		}
		/** �F�̃Z�b�g */
		void setColor( DWORD c )
		{
			for(int i=0;i<vertex.size();i++) vertex[i].c = c;
		}

		/** ���O�̎擾 */
		const std::string &getName(){ return  name; } 
	};

	/** 2D�}�`�����N���X */
	class Light2D
	{
	public:
		/** �����̈ʒu */
		math::Vector2 pos;
		/** ���̐F */
		math::Color dif;
		/** �����x(Attenuation) */
		float att;
		/** �R���X�g���N�^ */
		Light2D(){}
		/** ������
		 * @param [in] Vector2	 &p	: �����̃x�N�g�����W
		 * @param [in] Color	&di : �����̐F
		 * @param [in] float	 at : �����x�i�����̓͂�����) 0�`��
		 **/
		Light2D( math::Vector2 &p,math::Color &di,float at ) { pos=p; dif = di;att=1/at; }
		
		/** �������g�̏�����
		 * @param [in] Vector2	 &p	: �����̃x�N�g�����W
		 * @param [in] Color	&di : �����̐F
		 * @param [in] float	 at : �����x�i�����̓͂�����)
		 **/
		void Init( math::Vector2 &p,math::Color &di,float at ) { *this=Light2D( p,di,at ) ;}
		
		/** �f�X�g���N�^ */
		virtual ~Light2D(){}

		/** �����̖��邳���A�����̒��_�ɉ����Ă���
		 * @param[out] Color	*oc	:���_�̐F
		 * @param[out] Vector2	&vp :�o�������F�̈ʒu
		 **/
		virtual void getBrightColor( math::Color *oc,math::Vector2 &vp ) const
		{
			float length = (vp-pos).Length()*att;
			float bright = 1-length;
			if( bright<0 ) return;
			if( bright>1 ) bright =1;
			*oc += dif*bright;		//���邳�����_�J���[�ɉ�����
		}
	};
	
	/** 2D�I�u�W�F�N�g�܂Ƃ߂Ĉ����N���X */
	class Model2D
	{
	private:
		/** �I�u�W�F�N�g�z�� */
		system::AutoArray<Object2D> objs;			//�����Ȃ̂�'s'���t���Ă�
		/** �{�[���z�� */
		system::AutoArray<Bone2D>	bones;
	public:
		Model2D(){};
		
		/** ���[�h
		 * @param [in] char* name	:�ǂݍ��ރt�@�C��
		 **/
		void Load( char *name );

		/** �j�� */
		void Dispose(){ objs.dispose(); }

		/** �I�u�W�F�N�g���̎擾 */
		int getObjectCount(){ return objs.size(); }

		/** �Q��
		 * @param [in] no	:�Q�Ƃ������z��̗v�f
		 * @return objs[no]	:�Q�Ƃ����z��
		 **/
		Object2D &operator[] (int no){ return objs[no]; }

		/** ���O����Q��
		 * @param[in] const char* name	:���f����
		 * @return	objs[i]				:���f���̔z��
		 **/
		Object2D &operator[] ( const char* name )
		{
			for( int i=0;i<objs.size();i++ )
			{
				if( objs[i].getName() == name ) return objs[i];	//���O����v�����炻�̔z���Ԃ�
			}
			throw system::Exception( "���݂��Ȃ����f�����Q�Ƃ��܂���(���f�����m�F)" );
		}


		/** �{�[���z��擾 */
		Bone2D *getBone(){ return bones; }
		/**	�w�肵���z�񂩂�擾 */
		Bone2D &getBone(int no){ return bones[no]; };
		/** �w�肵�����O����擾 */
		Bone2D &getBone( const char *name )
		{
			for( int i=0;i<bones.size();i++ )
			{
				if( bones[i].name == name ) return bones[i];	//���O����v�����炻�̔z���Ԃ�
			}
			throw system::Exception( "���݂��Ȃ��{�[�����Q�Ƃ��܂���(�{�[�����m�F)" );
		};

		int getBoneCount() { return bones.size(); };

	};
	
	/** �P�̃��f���ŕ����̃A�j���[�V�������o����N���X */
	class ModelAnimation2D
	{
	private:
		/** �Q�Ɨp�̃{�[���|�C���^ */
		Bone2D					*bone;
		/** �A�j���[�V�����s��̔z�� */
		system::AutoArray<math::Matrix3x3>	amat;
		
		/** ���[�V�����̐ݒ�
		 * @param [out] KeyFrame *kf	:�L�[�t���[��
		 * @param [out] float *time		:����
		 **/
		int setMotionMatrix( KeyFrame2D *kf,float *time);
		/** �{�[���̐ݒ�
	 	 * @param [out] Matrix3x3 &root	:�S���Ɋ|����s��
		 **/
		void setBoneMatrix( math::Matrix3x3 &root );
		
		
	public:
		void Init( Bone2D *b,int bc ) { bone=b; amat.resize(bc); }
		void Init( Model2D &m ) { Init( m.getBone(),m.getBoneCount() ); }
		int setAnimation( math::Matrix3x3 &root,KeyFrame2D *kf,float *time )
		{
			int ret = setMotionMatrix( kf,time );
			setBoneMatrix( root );
			return ret;
		}
		math::Matrix3x3 *getMatrix(){ return &amat[0]; }
	};
}}
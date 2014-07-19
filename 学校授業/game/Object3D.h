/** @file
 * 3D�I�u�W�F�N�g�N���X�̒�`
 * @date 2009/07/21
 * @author ����
 **/

#include "../graphics/IRender.h"
#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "../math/Color.h"
#include "Object2D.h"
#pragma once

namespace sugino{
namespace game{
	class Light3D;			//����`
	class DirectionLight3D;
	class PointLight3D;

	typedef DirectionLight3D DirectLight;	//�ȗ�
	typedef PointLight3D	 PointLight;

	/** 3D���_�\���� */
	struct Vertex3D
	{
		/** ���[�J�����W */
		math::Vector3 p;
		/** ���_�F */
		DWORD	c;
		/** �g�����X�t�H�[����̃O���[�o�����W���̒��_�f�[�^ */
		math::Vector3 gp;
		/** �e�N�X�`�����W */
		math::Vector2 t;
		/** �@���x�N�g�� */
		math::Vector3 n;		//�ʂ̌����i�P�ʃx�N�g��)
		/** �g�����X�t�H�[����̖@���x�N�g�� */
		math::Vector3 gn;
		/** �{�[���s��̃C���f�b�N�X */
		WORD blend1;				//���_���ǂ̃{�[�����Q�Ƃ��邩
		/** �{�[���s��̃C���f�b�N�X(�e�ԍ�) */
		WORD blend2;
		/** �E�F�C�g�l */
		float weight;				//�e�{�[���̉e�����ǂ̂��炢�󂯂邩
	};
	//----------------------------------------------------------------------------------------
	/** �A�j���̃��[�V�����\���� */
	struct Motion3D{
		math::Quaternion rot;
		Motion3D( math::Vector3 &axis,float rot ) : rot(axis,rot) {}
		Motion3D( float a,float b,float c,float d ) : rot(a,b,c,d) {}
	};
	//----------------------------------------------------------------------------------------
	/** �L�[�t���[���\���� */
	struct KeyFrame3D{
		float			time;			//����
		Motion3D	 *motion;			//���[�V�����f�[�^
	};
	//----------------------------------------------------------------------------------------
	/** 3D�{�[��(�֐�)�f�[�^�\���� */
	struct Bone3D
	{	
		/** �{�[���� */
		std::string name;

		/** ���_ */
		math::Vector3 local;			//x,y,z�̍��W��Load�œǂݍ��܂����
		/** �e�{�[���ԍ� */
		int parent;
		/** �R���X�g���N�^ */
		Bone3D(){}
		/** ������
		 * @param [in] Vector3	&v	:�������W
		 * @param [in] int p		:�e�{�[���̔ԍ�
		 */
		Bone3D( math::Vector3 &v,int p ) : local(v),parent(p){} 
	};
	//-----------------------------------------------------------------------------------------
	/** 3D�}�`�I�u�W�F�N�g */
	class Object3D
	{
	private:
		/** ���_ */
		system::AutoArray<Vertex3D>							vertex;
		/** �e�N�X�`�� */
		system::AutoArray<graphics::TextureColorTVertex>	tvertex;
		/** �� */
		system::AutoArray<ModelFace>						face;
		/** �I�u�W�F�N�g�� */
		std::string											name;
	public:
		/** .RDX��XML�`���ł̓ǂݍ��݁A�I�u�W�F�N�g��������
		 * @param [in] char *text	: �e�L�X�g�f�[�^z
		 * @return �ǂݍ��񂾃e�L�X�g�̏I���A�h���X
		 **/
		char *Load( char* text );

		/** �I�u�W�F�N�g���擾 */
		const char *getName(){ return name.data(); }
		
		/** �ʁA���_���̃T�C�Y���������m��
		 * @param [in] int	v_size 	:���_��
		 * @param [in] int	f_size  :�ʌ�
		 **/
		void Create( int v_size, int f_size );		//�������m��
		
		/** �I�u�W�F�N�g�̖ʕ`��
		 * @param [in] IRender *r		:�摜���@
		 **/
		void Draw( graphics::IRender *r );
		
		/** ���W�ϊ�
		 * @param [in] const Matrix4x4 &m1	:�s��
		 * @param [in] const Matrix4x4 &m2	:�J�����s��
		 **/
		void Transform( const math::Matrix4x4 &m1,const math::Matrix4x4 &m2 );
		
		/** �A�j���p���W�ϊ�
		 * @param [in] Matrix4x4 *m	:�A�j���z��
		 * @param [in] Matrix4x4 &m2	:�J�����s��
		 **/
		void Transform( math::Matrix4x4 *m,math::Matrix4x4 &m2 );

		/** �w�i�p�ϊ�
	 	 * @param [in] Matrix4x4 &vps	:camera*Projection*screen�̃J�����s��
		 **/
		void Transform( math::Matrix4x4 &vps );

		/** �v���g�����X�t�H�[��
		 *  ���_�����ϊ����ꂽ���W�����̍��W�ɕϊ����Ă���
		 **/
		void PreTransform()
		{ 
			for(int i=0;i<vertex.size();i++)
			{
				vertex[i].p = vertex[i].gp;
				vertex[i].n = vertex[i].gn;
			}
		}
		
		/** �������C�g�̃��C�e�B���O
		 * @param [out] Light3D **lights	:Light3D�h���̌����f�[�^�z��
		 * @param [in]	int		count		:�z��̌��@light���Q�Ƃ���Ƃ��̓Y������
		 * @param [out] Color	&amb		:���F
		 **/
		void Lighting( Light3D **lights,int count,math::Color &amb );
		
		/** ���C�g��̃��C�e�B���O
		 * @param [out] Light3D **lights	:Light3D�h���̌����f�[�^�z��
		 * @param [out] Color	&amb		:���F
		 **/
		void Lighting( Light3D &_light, math::Color &amb );
		
		//---------------------------------------------------------
		//setter
		//---------------------------------------------------------
		//�O�p�`
		static void setFace( ModelFace *f,WORD n1,WORD n2,WORD n3 )
		{
			Object2D::setFace( f,n1,n2,n3 );		//Object2D��setFace���g�킹�Ă��炤
		}
		//�l�p�`�ŏ�����
		static void setFace( ModelFace *f,WORD n1,WORD n2,WORD n3,WORD n4 ){
			setFace( f+0,n1,n2,n3 );setFace( f+1,n1,n3,n4 );
		}
		//----------------------------------------------------------
		// getter
		//----------------------------------------------------------
		/** ���_�擾 */
		Vertex3D &getVertex( int i ){ return vertex[i]; }

		/** ���_"��"�擾 */
		int getVertexCount(){ return vertex.size(); }

		/** �ʎ擾 */
		ModelFace &getFace( int i ){ return face[i]; }

		/** ��"��"�擾 */
		int getFaceCount(){ return face.size(); }
		
		/** �v�����C�e�B���O */
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
		
		/** �g�����X�t�H�[����̐F�̃Z�b�g */
		void setTColor( DWORD c )
		{
			for(int i=0;i<vertex.size();i++) tvertex[i].c = c;
		}

		/** �@���x�N�g�������� */
		void Normalize();

		/** �@���̐��K��
		 * �s��ɃX�P�[�����O����ꂽ��P�ʃx�N�g���������Ȃ萳�m�Ƀ��C�e�B���O����Ȃ��̂�
		 * ���K��������B�i�g��k���s���Ƃ��̂ݎg�p)
		 **/
		void NormalNomalize()
		{
			for(int i=0;i<vertex.size();i++)
			{
				if( vertex[i].gn.Length2()!=0 ) vertex[i].gn.Normalize();
			}
		}

		/** �A���t�@�l�ɂ�閶�̉��o
		 * proj��Persective�Ō�̈����i������͈́j�ɍ��킹��
		 * @param [in] float fog_s	:�J�n�ʒu
		 * @param [in] float fog_e	:�I���ʒu
		 **/
		void setFogAlpha( float fog_s,float fog_e );
		
		/** �w�i�F�ɂ�閶�̉��o
		 * proj��Persective�Ō�̈����i������͈́j�ɍ��킹��
		 * @param [in] float fog_s	:�J�n�ʒu
		 * @param [in] float fog_e	:�I���ʒu
		 * @param [in] Color fog_col:�w�i�F
		 **/
		void setFogColor( float fog_s,float fog_e,math::Color &fog_col );

		//-----------------------------------------------------------
		//				��{�}�`
		//-----------------------------------------------------------
		/** ���쐬 */
		void CreateBox( DWORD col );
		
		/** �쐬
		 * @param [in] int	nx	:������
		 * @param [in] int	nz	:���s������
		 * @param [in] DWORD col	:�F
		 **/
		void CreatePlane( int nx,int nz,DWORD col );
		
		/** ���쐬
		 * @param [in] int		nr	:�~�̕�����
		 * @param [in] Vector2	*v	:��]�f�[�^
		 * @param [in] int		nv	:�f�[�^�̌�
		 * @param [in] DWORD		col	:�F
		 **/
		void CreateRotate( int nr,math::Vector2 *v,int nv,DWORD col );

		/** ���쐬
		 * @param [in] int	nr		: �~����
		 * @param [in] int	n		: �c����
		 * @param [in] float top		: ��ʃT�C�Y
		 * @param [in] float bottom	: ���ʃT�C�Y
		 * @param [in] DWPRD	col		: �F
		 **/
		void CreateCylinder( int nr,int n,float top,float bottom,DWORD col );

		/** ���쐬
		 * @param [in] int	nr		:�~����
		 * @param [in] int	ny		:�c����
		 * @param [in] DWORD col		:�F
		 **/
		void CreateSphere( int nr,int ny,DWORD col );		
	};

	//---------------------------------------------------------------------------------------
	/** 3D���C�g�i�����j���N���X */
	class Light3D
	{
	private:
		math::Color col;			//���̐F
	protected:
		/** ���邳�擾
		 * @param [in] Vector3 &up	:�g�����X�t�H�[����̒��_���W
		 * @param [in] Vector3 &norm	:�@���x�N�g��/�P�ʃx�N�g��(�ʂ̌���)
		 **/
		virtual float GetBright( math::Vector3 &vp,math::Vector3 &norm ) = 0;
	public:
		/** �R���X�g���N�^ */
		Light3D(){}
		/** �f�X�g���N�^ */
		virtual ~Light3D(){}
		
		/** �����F���������
		 * @param [in] Color &_col	:���̐F
		 **/
		Light3D( const math::Color &_col ) : col(_col){}
		
		/** �����F�擾 
		 * @param [in] Vector3	&vp		:�g�����X�t�H�[����̒��_���W
		 * @param [in] Vector3	&norm	:�@���x�N�g��/�P�ʃx�N�g��(�ʂ̌���)
		 * @param [in] Color	&dif	:���̐F
		 **/
		virtual math::Color GetLightingColor( math::Vector3 &vp,math::Vector3 &norm,math::Color &dif ){
			return dif * getColor() * GetBright( vp,norm );
		}
		
		/** ���C�g�̐F�擾
		 * @return col	:�����F
		 **/
		math::Color &getColor(){ return col; }
	};

	//---------------------------------------------------------------------------------------
	/** ���s���� Light3D�h�� */
	class DirectionLight3D : public Light3D		//���̕����݂̂̃f�[�^�������C�g�@��j����������S�̂��Ƃ炷
	{
	protected:
		math::Vector3	dir;					//���C�g���W
		/** ���邳�̎擾
		 * @param [in]	Vector3 &vp		:�g�����X�t�H�[����̒��_���W
		 * @param [in]	Vector3 &norm	:�@���x�N�g��/�P�ʃx�N�g��(�ʂ̌���)
		 **/
		virtual float GetBright( math::Vector3 &vp,math::Vector3 &norm ){
			float bright = math::Vector3::Dot( norm, -dir );
			if( bright<0 ) bright = 0;
			return bright;
		}
	public:
		/**�@�R���X�g���N�^
		 *@param [in] Vector3	_dir	:���C�g�𓔂��ꏊ
		 *@param [in] Color		&col	:�������C�g�̐F
		 **/
		DirectionLight3D( math::Vector3 _dir, math::Color &col ) :
		  Light3D( col ),dir(_dir) {}

		/** ���C�g���W�擾 */
		math::Vector3 getDir(){ return dir; }
	};

	//----------------------------------------------------------------------------------------
	/** �_���� Light3D�h�� */
	class PointLight3D : public Light3D				//�S�������Ƃ炷���C�g�@��j�ꕔ�����Ƃ炷
	{
	private:
		math::Vector3	pos;			//���C�g���W
		float			att;			//�����͂�����
		/** ���邳�擾
		 * @param [in] Vector3 vp		:�g�����X�t�H�[����̒��_���W
		 * @param [in] Vector3 norm		:�@���x�N�g���i�P�ʃx�N�g��)
		 * @return float bright*len		:��������W�ɂ�閾�邳
		 **/
		virtual float GetBright( math::Vector3 &vp, math::Vector3 &norm )
		{
			math::Vector3 dir		= pos - vp;
			float len				= 1 - dir.Length()*att; if(len<0)len=0;
			float bright			= math::Vector3::Dot( norm,dir.Normalize() );
			if( bright<0 ) bright	= 0;
			return bright * len;
		}
	public:
		/**�@�R���X�g���N�^
		 *@param [in] Vector3	_dir	:���C�g�𓔂��ꏊ
		 *@param [in] Color		&col	:�������C�g�̐F
		 *@param [in] float		_att	:�����͂�����(1�ł͏��Ȃ�����j
		 **/
		PointLight3D( math::Vector3 &_pos, math::Color &col, float _att ) :
		  Light3D( col ), pos(_pos), att(1/_att) {}

		  /** �����擾 */
		  math::Vector3 getPos(){ return pos; }
	};
	
	//--------------------------------------------------------------------------------------
	/** ���f��(�I�u�W�F�N�g�W����)�f�[�^ */
	class Model3D
	{
	private:
		/** �����I�u�W�F�N�g�������z�� */
		system::AutoArray< Object3D >	objs;
		/** �����{�[���������z�� */
		system::AutoArray< Bone3D >		bones;
	public:
		/** �R���X�g���N�^ */
		Model3D(){};
		
		/** ���[�h
		 * @param [in] char* file_name	: �ǂݍ��ރt�@�C���� RDX�t�@�C��
		 **/
		void Load( const char *file_name );

		/** �j�� */
		void Dispose(){ objs.dispose(); }

		/** �I�u�W�F�N�g���̎擾
		 * @return �z���
		 **/
		int getObjectCount(){ return objs.size(); }

		/** �w��I�u�W�F�N�g�̃A�h���X�擾 */
		Object3D *getObject(int no){return &objs[no];}

		/** �Q��
		 * @param [in] no	:�Q�Ƃ������z��̗v�f
		 * @return objs[no]	:�Q�Ƃ����z��̒��g
		 **/
		Object3D &operator[] (int no){ return objs[no]; }

		/** ���O����Q��
		 * @param[in] const char* model_name	:���f����
		 * @return	objs[i]						:���f���̔z��
		 **/
		Object3D &operator[] ( const char* model_name )
		{
			for( int i=0;i<objs.size();i++ )
			{
				if( objs[i].getName() == model_name ) return objs[i];	//���O����v�����炻�̔z��̒��g��Ԃ�
			}
			throw system::Exception( "���݂��Ȃ����f�����Q�Ƃ��܂���(���f�����m�F)" );
		}
		
		/**	�w�肵���z��ԍ�����擾 */
		Bone3D &getBoneArray(int no){ return bones[no]; };
		
		/** �{�[���z��擾 */
		inline Bone3D *getBone(){ return &bones[0]; }

		/** �{�[�����擾 */
		inline int getBoneCount() { return bones.size(); };

		/** �w�肵�����O����擾 */
		Bone3D &getBoneName( const char *name )
		{
			for( int i=0;i<bones.size();i++ )
			{
				if( bones[i].name == name ) return bones[i];	//���O����v�����炻�̔z���Ԃ�
			}
			throw system::Exception( "���݂��Ȃ��{�[�����Q�Ƃ��܂���(�{�[�����m�F)" );
		};
	};
	
	//---------------------------------------------------------------------------------------
	/** �P�̃��f���ŕ����̃A�j���[�V�������o����N���X */
	class ModelAnimation3D{
	private:
		Bone3D			*bone;						//��]�̌��_�f�[�^�z��
		system::AutoArray< math::Matrix4x4 > mat;	//�A�j���[�V�����s��̔z��
	public:
		void Init( Bone3D *_bone,int size ){
			bone = _bone;
			mat.resize( size );
		}
		/** �A�j���[�V�����̃��[�V�������Z�b�g */
		int setMotion( math::Matrix4x4 &root,KeyFrame3D *kf,float *time );

		math::Matrix4x4 *getMatrix(){ return &mat[0]; }
	};
}}
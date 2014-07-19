/** @file 
 * �R�c�J��������N���X
 * @date 2009/07/27
 * @author ����
 **/
//--------------------------------------------------------
#pragma once

#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "Collision.h"

namespace sugino{
	namespace game{
		
		class Camera3D;						//����`
		typedef Camera3D	Camera;			//�ȗ�
		/** �J��������N���X 
		 * �ʒu�E�g��E��]�̎w�肪�\�ȃJ����
		 * ���̃J�������瓾����s����g���āA�\��������̂�Transform����
		 **/
		class Camera3D{
		private:
			/** Z ���x�N�g�� */
			math::Vector		z_axis;
			/** X ���x�N�g�� */
			math::Vector		x_axis;
			/** Y ���x�N�g�� */
			math::Vector		y_axis;
					
			/** ���W�ʒu */
			math::Vector		pos;
			/** ���Ă��镨�i���Ă���ꏊ) LookAt�֐���At�̕��� */
			math::Vector		at;
			/** �X�N���[���T�C�Y */
			int					wid;		//�X�N���[����
			int					hei;		//�X�N���[������

			/** �ړ��� */
			math::Vector3		move;

			/** �\���͈͂̎�O */
			float				camera_near;
				
			/** �\���͈͂̉� */
			float				camera_far;

			/** �J�����s�� */
			math::Matrix4x4		camera;
			/** �X�N���[���s�� */
			math::Matrix4x4		screen;
			/** �v���W�F�N�V����  */
			math::Matrix4x4		proj;
			
			/** �|�����킹�����̎󂯎��p */
			math::Matrix4x4		view;

			/** ��]�s�� */
			math::Matrix4x4		rotate;

			/** �X�V�t���O */
			bool				is_update;

			/** �J�����s��̍X�V �|�����킹�s��
			 * camera * proj * screen
			 **/
			void Update()
			{
				if( is_update ){										//�X�V�t���O�����Ă���
					
					math::Vector3 vec = (at - pos).Normalize();				//���Ă�������x�N�g��
					camera.View( vec, math::Vector(0,1,0),this->pos );		//�r���[�s��
					view = camera*proj*screen;							//View�s��ϐ��Ɋ|�����킹���s������X�V����
					is_update = false;
				}
			}
		public:

			/** �R���X�g���N�^ */
			Camera3D() : z_axis(0,0,1),x_axis(1,0,0),y_axis(0,1,0),pos(0,0,0),at(0,0,1),move(0,0,0),wid(640),hei(480),camera_near(0),camera_far(0)
			{};

			/** �f�X�g���N�^ */
			~Camera3D(){};

			/** �������R���X�g���N�^ */
			Camera3D( math::Vector3 &_pos,math::Vector3 &at,int sc_wid,int sc_hei,float per_wid, float per_hei,float _near, float _far ) :
			pos(_pos),wid(sc_wid),hei(sc_hei),camera_near(_near),camera_far(_near),z_axis(math::Vector(0,0,1)),x_axis(math::Vector(1,0,0)),y_axis(math::Vector(0,1,0))
			{		
				//Update�̍s��|�����킹���ɕK�v�ȏ�����
				this->setPosition( _pos );
				this->setAt( at );
				
				setScreen(sc_wid,sc_hei);
				camera.View( z_axis,y_axis,_pos  );				//View�s����
				camera.LookAt( _pos,at,math::Vector( 0,1,0 ) );		//LookAt�s����
				proj.Perspective( per_wid,per_hei,_near,_far );	//Persective���
				is_update = true;					
			}

			//----------------------�������֐�--------------------------------------------
			/** ������
			 * @attention �e�s��( Camera/Screen/Persective/LookAt )�����������Ă���
			 * @param [in]	Vector3		pos		;�J�����̍��W
			 * @param [in]	Vector3		at		:���Ă���Ώ�
			 * @param [in]	int			sc_wid	:�X�N���[����
			 * @param [in]	int			sc_hei	:�X�N���[������
			 * @param [in]	float		per_wid	:�p�[�X�̕�
			 * @param [in]	float		per_hei	:�p�[�X�̍���
			 * @param [in]	float		_near	:�`��͈͂̎�O
			 * @param [in]	float		_far	:�`��͈͂̉��s��
			 **/
			void Initialize( math::Vector3 &pos,math::Vector3 &_at,int sc_wid,int sc_hei,float per_wid, float per_hei,float _near, float _far )
			{
				camera.View( z_axis,y_axis,pos  );				//View�s����
				camera.LookAt( pos,at,math::Vector( 0,1,0 ) );	//LookAt�s����
				screen.Screen( sc_wid,sc_hei );					//Screen�s����
				proj.Perspective(per_wid,per_hei,_near,_far );	//Persective���
				setPosition( pos );								//�J�����̍��W������
				camera_near = _near;							//�`��͈͂������Ă���
				camera_far  = _far;
				at		= _at;									//���Ă���Ώۂ������Ă���
				is_update = true;
			}

			/** LookAt�s�񏉊���(�����o�ϐ�Matrix4x4 camera)
			 * @param [in] Vector3 &pos		 : �J�����ꏊ���W
			 * @param [in] Vector3 &at		 : ���Ă��i�Ώہj
			 * @param [in] Vector3 &up		 : ���̏�����x�N�g��
			 **/ 
			void setLookAt( math::Vector3 &_pos,math::Vector3 &_at, math::Vector3 &_up ){
				at = _at;									//�����o�ϐ��ɒl������
				pos = _pos;
								//�����o�ϐ�camera��LookAt�s��
				is_update = true;
			}

			/** �X�N���[���s��̍쐬 */
			void setScreen( int w,int h ){
				wid = w;hei = h;		//�l���Ƃ��Ă���
				screen.Screen(w,h);		//�����o�ϐ�screen��Screen�s������
				is_update = true;
			}

			/** �X�N���[���s��̍쐬�iRect�œn��) */
			void setScreen( system::Rect &rc ){
				wid = rc.right - rc.left;
				hei = rc.bottom - rc.top;
				if( wid<0 ) wid = -wid;	//�������]
				if( hei<0 ) hei = -hei;
				screen.Screen( rc );
				is_update = true;
			}
			
			/** �p�[�Z�N�e�B�u�s��̍쐬 */
			void setPercective( float per_wid,float per_hei,float _near,float _far ){
				camera_near = _near;							//�`��͈͂������Ă���
				camera_far  = _far;
				//proj.Perspective(per_wid,per_hei,_near,_far );
				proj.Perspective(2*(per_wid/(float)per_hei ),2,_near,_far );
				//proj.Perspective( 2*1.333333f,2,_near,_far );
				is_update = true;
			}
			


			//---------------------�s��擾------------------------------------
			/** �J�����s��( View�s��Ă�ł������ )�擾
			 * @return �|�����킳�ꂽcamera�s��
			 **/
			math::Matrix4x4 &getCameraMatrix(){ Update(); return camera; }
			
			/** �X�N���[��( Screen�s��Ă�ł������)�擾
			 * @return �|�����킳�ꂽScreen�s��
			 **/
			math::Matrix4x4 &getScreenMatrix(){ return screen; }
			
			/** �v���W�F�N�V����( Perspective�s��Ă�ł������)�擾
			 * @return �|�����킳�ꂽPerspective�s��
			 **/
			math::Matrix4x4 &getProjectionMatrix(){ return proj; }

			/** �J����*�v���W�F�N�V����*�X�N���[���擾
			 * @return �X�V���I������J�����s��
			 **/
			math::Matrix4x4 &getMatrix(){ 
				Update();				//�|�����킹
				return view;
			}



			//----------------�J�����̉�]-----------------------------
			/** �C�ӓ_�ɂ��J������X��]
			 * @param [in] float		r		:��]�x��
			 * @param [in] Vector3		&target	:�C�ӓ_
			 **/
			void RotateXPosition( float r,math::Vector3 &target ){
				math::Matrix m;
				m.RotateX( r );
				rotate.RotatePos( m,target );	
				pos = rotate.TransformVector( pos );

				is_update = true; 
			};
			/** �C�ӓ_�ɂ��J������Y��]
			 * @param [in] float		r		:��]�x��
			 * @param [in] Vector3		&target	:�C�ӓ_
			 **/
			void RotateYPosition( float r,math::Vector3 &target ){
				math::Matrix m;
				m.RotateY( r );
				rotate.RotatePos( m,target );	
				pos = rotate.TransformVector( pos );

				is_update = true; 
			};
			/** �C�ӓ_�ɂ��J������Z��]
			 * @param [in] float		r		:��]�x��
			 * @param [in] Vector3		&target	:�C�ӓ_
			 **/
			void RotateZPosition( float r,math::Vector3 &target ){
				math::Matrix m;
				m.RotateZ( r );
				rotate.RotatePos( m,target );	
				pos = rotate.TransformVector( pos );

				is_update = true; 
			};
			
			/** "��"�ɂ��C��"�_"�̉�]
			 * @param [in] Vector3	axis	:�w�莲 Vector(0,1,0)��Y�� (1,0,0)��X�� (1,1,0)��XY���Ȃ�
   			 * @param [in] Vector3	target	:�C�ӓ_
			 * @param [in] float	r		:��]�x��
			 **/
			void Rotate( math::Vector3 &axis,math::Vector3 &target,float r ) {
				math::Matrix m;
				if( axis.x>1 || axis.y>1 || axis.z>1 ) m.RotateAxis( axis.Normalize(), r );	//�傫���l�͐��K�����Ă��

				m.RotateAxis( axis, r );
				rotate.RotatePos( m, target );

				pos = rotate.TransformVector( pos );	//���W�X�V
				at = rotate.TransformRotation( at );
			
				is_update = true;						//�s��X�V
			}

					
			/** �J���������Ă����̐ݒ�
			 * @param [in] const Vector3 &_at	:�J���������Ă���Ώۍ��W
			 **/
			void setAt( const math::Vector &_at ){ at = _at; 	is_update = true; }

			/** �J�������W�̐ݒ�
			 * @param [in] const Vector3 &p	:�J�����̍��W
			 **/
			void setPosition( const math::Vector3 &p ){ pos=p; is_update = true; }
			
			/** �\���͈͂̎w�� */
			/** ��O�̐ݒ�
			 * @param [in] float _near	:�J�����Ō����O�̕\���͈�
			 **/
			void setNear( float _near ){ camera_near = _near; 	is_update = true;} //��O
			
			/** ���s���̐ݒ�
			 * @param [in] float _far	:�J�����Ō��鉜�s���̕\���͈�
			 **/
			void setFar( float _far ){ camera_far = _far; 	is_update = true;} //��

			//--------------------get----------------------------------------
			/** �J�����̍��W�擾
			 * @return pos		:�ʒu
			 **/
			math::Vector3 &getPosition(){ return pos; }
			
			/** �\���͈͂̎擾 */
			/** ��O�擾
			 * @return camera_near	:��O
			 **/
			float getNear(){ return camera_near; }
			
			/** ���s���擾
			 * @return camera_far	:���s��
			 **/
			float getFar(){ return camera_far; }
			
			//�ړ��ʂ̎擾
			math::Vector3 &getMove(){ return move; }

			/** ���Ă����̎擾 */
			math::Vector3 &getAt(){ return at; } 
			
			/** �s���"Z��"�擾 */
			math::Vector3 getView_axis(){ return math::Vector3( camera.m02,camera.m12,camera.m22 ); }
			
			/** �s���"Y��"�擾 */
			math::Vector3 getUp_axis(){ return math::Vector3( camera.m01,camera.m11,camera.m21 ); }
			
			/** �s���"X��"�擾 */
			math::Vector3 getRight_axis(){ return math::Vector3( camera.m00,camera.m10,camera.m20 ); }

		};
	}
}
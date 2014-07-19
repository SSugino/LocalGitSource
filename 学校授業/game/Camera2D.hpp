/** @file 
 * �Q�c�J��������N���X
 *	
 **/
//--------------------------------------------------------
#pragma once

#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "Collision.h"

namespace sugino{
	namespace game{

		/** �J��������N���X 
		 * �ʒu�E�g��E��]�̎w�肪�\�ȃJ����
		 * ���̃J�������瓾����s����g���āA�\��������̂�Transform����
		 * 
		 **/
		class Camera2D{
		private:
			/** Y ���x�N�g�� */
			math::Vector2		axis;

			/** ���W�ʒu */
			math::Vector2		pos;

			/** �{�� */
			float				zoom;
			
			/** �ŏ��{�� */
			float				min_zoom;
			
			/** �ő�{�� */
			float				max_zoom;
			
			/** �J�����s�� */
			math::Matrix3x3		camera;

			/** �X�N���[���s�� */
			math::Matrix3x3		screen;

			/** �J�����~�X�N���[�� */
			math::Matrix3x3		view;

			/** �X�V�t���O */
			bool				is_update;
			
			/** �J�����s��̍X�V */
			void Update()
			{
				if( is_update ){		//�X�V�t���O�����Ă���
					camera.View( axis*(1/zoom),pos  );
					view = camera*screen;
					is_update = false;
				}
			}
		public:

			/** �R���X�g���N�^ */
			Camera2D() : axis(0,1),pos(0,0),zoom(1) {
				setScreen( 640,480 );
				setZoomRange( 0.01f,100 );	// �P/�P�O�O�{�`�P�O�O�{�܂�
				Update();
			};
			
			/** �g��k���͈͂̎w�� */
			void setZoomRange( float min,float max ){
				min_zoom = min;
				max_zoom = max;
			}
			
			/** �X�N���[���s��̍쐬 */
			void setScreen( int w,int h ){ screen.Screen(w,h); is_update = true; }

			/** �f�X�g���N�^ */
			~Camera2D(){};

			/** ��ʉ�]
			 * @param[in]  r :  ��]���x
			 **/
			void addRotation( float r ){ axis = axis.Rotate( r ); is_update = true; } 
			
			/** ��ʉ�]�w�� */
			void setRotation( float r ){ axis = math::Vector2(r).Rotate90(); is_update = true; } 
			float getRotation(){ return math::Vector2::Angle(math::Vector2(0,1),axis); }

			/** ��ʊg��k��
			 * @param[in]  z:   �{�����x
			 **/
			void addZoom( float z ){ setZoom( zoom + z ); }
			
			/** �Y�[���w�� */
			void setZoom( float z ){
				zoom = z;
				// �N���b�v����
				if( zoom > max_zoom ) zoom = max_zoom;
				if( zoom < min_zoom ) zoom = min_zoom;
				is_update = true;
			}
			
			/** �Y�[���擾 */
			float getZoom(){ return zoom; }

			/** ��ʈړ�
			 *	@param[in]	m : �ړ���
			 **/
			void addPosition( math::Vector2 &m ){ setPosition( pos + m ); }
			
			/** �ʒu�w�� 
			 *	@param[in]	&p : �ʒu
			 **/
			void setPosition( const math::Vector2 &p ){ pos=p; is_update = true; }
			const math::Vector2 &getPos(){ return pos; }
						
			/** �J�����擾 */
			math::Matrix3x3 &getCameraMatrix(){ Update(); return camera; }
			
			/** �X�N���[���擾 */
			math::Matrix3x3 &getScreenMatrix(){ return screen; }
			
			/** �J����*�X�N���[���擾 */
			math::Matrix3x3 &getMatrix(){ 
				Update();
				return view; 
			}

			/** �J��������]���Ă��Ȃ����̂`�`�a�a���擾���� */
			game::BoundingBox2D createAABB(){
				float aspect = (screen.m20)/(screen.m21);	//��������
				return game::BoundingBox2D( pos,math::Vector2( zoom*aspect,zoom ) );
			}
					
			/** �J�������\�����Ă���n�a�a���擾���� */
			game::BoundingOBox2D createOBB(){
				float aspect = (screen.m20)/(screen.m21);
				return game::BoundingOBox2D( pos,axis,math::Vector2( zoom*aspect,zoom ) );

			}
			
					

		};

	}
}
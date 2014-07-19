/** @file
 * ���˕Ԃ蔻��̒�`
 * @date 2009/09/30
 * @author ����
 **/
#pragma once


#include "Collision.h"
#include "../math/Matrix.h"
#include "../baseball/Ball.h"

namespace sugino{
namespace game{

	/** ���˕Ԃ蔻��N���X */
	class Rebound
	{
	public:
		
		/** ���˕Ԃ蔻�� ����
		 * @param [in]  a   : ���肵����BoundingSphere3D
		 * @param [in]  b   : ���肵����BoundingOBox3D
		 * @param [out] ret : ���W�␳�ʂ��i�[����Vector3�N���X
		 * @param [out] rif : ���ˍs����i�[
		 * @retval true		: �ڐG���Ă��� 
		 * @retval false	: �ڐG���Ă��Ȃ�
		 **/
		static bool Bound( const BoundingSphere3D &a, const BoundingOBox3D &b, math::Vector3 *ret, math::Matrix4x4 *rif );
		
		/** ���˔���
		 * @param [in]  a   : ���肵����BoundingSphere3D
		 * @param [in]  b   : ���肵����BoundingOBox3D
		 * @param [out] ret : ���W�␳�ʂ��i�[����Vector3�N���X
		 * @param [out] pow : �␳���������ړ��ʂɗ^����䗦
		 * @retval true  : �ڐG���Ă��� 
		 * @retval false : �ڐG���Ă��Ȃ�
		 **/
		static bool Refrect( BoundingSphere3D &a, BoundingLine3D &b, math::Vector3 *mov );

		static bool Refrect( BoundingSphere3D &a, BoundingOBox3D &b, math::Vector3 *mov );
		/** ���Ă���悩���������90�x�E�������
		 * @param [in] Vector3	&dir	:����
		 * @param [in] Vector3	&up		:�㎲
		 **/
		static math::Vector3 getRightAxis( const math::Vector3 &dir, const math::Vector3 &up ); 
	};
}}
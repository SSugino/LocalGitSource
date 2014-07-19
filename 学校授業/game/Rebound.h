/** @file
 * ’µ‚Ë•Ô‚è”»’è‚Ì’è‹`
 * @date 2009/09/30
 * @author ™–ì
 **/
#pragma once


#include "Collision.h"
#include "../math/Matrix.h"
#include "../baseball/Ball.h"

namespace sugino{
namespace game{

	/** ’µ‚Ë•Ô‚è”»’èƒNƒ‰ƒX */
	class Rebound
	{
	public:
		
		/** ’µ‚Ë•Ô‚è”»’è «ª
		 * @param [in]  a   : ”»’è‚µ‚½‚¢BoundingSphere3D
		 * @param [in]  b   : ”»’è‚µ‚½‚¢BoundingOBox3D
		 * @param [out] ret : À•W•â³—Ê‚ğŠi”[‚µ‚½Vector3ƒNƒ‰ƒX
		 * @param [out] rif : ”½Ës—ñ‚ğŠi”[
		 * @retval true		: ÚG‚µ‚Ä‚¢‚é 
		 * @retval false	: ÚG‚µ‚Ä‚¢‚È‚¢
		 **/
		static bool Bound( const BoundingSphere3D &a, const BoundingOBox3D &b, math::Vector3 *ret, math::Matrix4x4 *rif );
		
		/** ”½Ë”»’è
		 * @param [in]  a   : ”»’è‚µ‚½‚¢BoundingSphere3D
		 * @param [in]  b   : ”»’è‚µ‚½‚¢BoundingOBox3D
		 * @param [out] ret : À•W•â³—Ê‚ğŠi”[‚µ‚½Vector3ƒNƒ‰ƒX
		 * @param [out] pow : •â³‚ğ‘ã“ü‚µ‚½ˆÚ“®—Ê‚É—^‚¦‚é”ä—¦
		 * @retval true  : ÚG‚µ‚Ä‚¢‚é 
		 * @retval false : ÚG‚µ‚Ä‚¢‚È‚¢
		 **/
		static bool Refrect( BoundingSphere3D &a, BoundingLine3D &b, math::Vector3 *mov );

		static bool Refrect( BoundingSphere3D &a, BoundingOBox3D &b, math::Vector3 *mov );
		/** Œ©‚Ä‚¢‚éæ‚©‚ç•ûŒü‚©‚ç90“x‰E²‚ğì‚é
		 * @param [in] Vector3	&dir	:•ûŒü
		 * @param [in] Vector3	&up		:ã²
		 **/
		static math::Vector3 getRightAxis( const math::Vector3 &dir, const math::Vector3 &up ); 
	};
}}
/** @file
 * �����Ǘ��N���X�̒�`
 * @date 2009/05/13
 * @author ����
 **/

#pragma once

#include "IRender.h"

namespace sugino{
	namespace graphics{
		/**Render�̐����Ǘ��N���X
		 * ��XRender�������Ă��ˑ��֌W�ɉe���^���Ȃ��ׂ̃N���X
		 **/
		class RenderFactory
		{
		public:
			enum RENDER_ID
			{
				RENDER_D3D9
			};
			//ID������Ȃ�Render�𐶐�����
			static IRender *Create( int id );
		};	
	}
}




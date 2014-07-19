/** @file
 * 生成管理クラスの定義
 * @date 2009/05/13
 * @author 杉野
 **/

#pragma once

#include "IRender.h"

namespace sugino{
	namespace graphics{
		/**Renderの生成管理クラス
		 * 後々Renderが増えても依存関係に影響与えない為のクラス
		 **/
		class RenderFactory
		{
		public:
			enum RENDER_ID
			{
				RENDER_D3D9
			};
			//IDがあるならRenderを生成する
			static IRender *Create( int id );
		};	
	}
}




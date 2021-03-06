/** @file
 * 生成管理クラスの実装
 * @date 2009/05/13
 * @author 杉野
 **/
#include "RenderFactory.h"
#include "../system/Exception.h"
#include "D3D9Render.h"

using namespace sugino;
using namespace graphics;

IRender *RenderFactory::Create( int id )
{
	IRender *r = NULL;
	switch( id )
	{
	case RENDER_D3D9:
		r = new_s D3D9Render();
		break;
	default:
		throw system::Exception( "Render生成に失敗しました");
		
	}
	return r;
}
/** @file
 * •`‰æŠÖ”ƒCƒ“ƒ^[ƒtƒF[ƒXƒNƒ‰ƒXŽÀ‘•
 * @date 2009/05/13
 * @author ™–ì
 **/

#include "IRender.h"

using namespace sugino;
using namespace graphics;

//ü‚ð1–{ˆø‚­
void IRender::drawLine(int x1, int y1, int x2, int y2, DWORD col1, DWORD col2)
{
	ColorTVertex tv[] = {
							ColorTVertex((float) x1,(float)y1, col1),
							ColorTVertex((float) x2,(float)y2, col2)
	};
	WORD index[] = {0,1};
	this->drawLine( tv,index,2,1 );
}


//—Ìˆæ“h‚è‚Â‚Ô‚µ
void IRender::drawFill(const sugino::system::Rect &r, DWORD col)
{
	float x1 = (float)r.left  ,y1 = (float)r.top ;
	float x2 = (float)r.right ,y2 = (float)r.bottom;

	ColorTVertex tv[]={
		ColorTVertex( x1,y1,col ),ColorTVertex( x2,y1,col ),
		ColorTVertex( x1,y2,col ),ColorTVertex( x2,y2,col )
	};
	WORD index[]={ 0,1,2, 2,1,3 };
	this->drawPolygon( tv,index,4,2 );
}

//ŽlŠp‚Ì˜g
void IRender::drawBox(const sugino::system::Rect &r, DWORD col)
{
	float x1 = (float)r.left  ,y1 = (float)r.top ;
	float x2 = (float)r.right  ,y2 = (float)r.bottom ;

	ColorTVertex tv[]={
		ColorTVertex( x1,y1,col ),ColorTVertex( x2,y1,col ),
		ColorTVertex( x1,y2,col ),ColorTVertex( x2,y2,col )
	};
	WORD index[]={ 0,1, 1,3, 3,2, 2,0 };
	this->drawLine( tv,index,4,4 );//‘æ‚Rˆø” ’¸“_”,‘æ4 ‰½–{‚©H
}

//‰æ‘œ‚Ì•`‰æ
void IRender::drawImage(const sugino::system::Rect &r, sugino::graphics::IImage *img, const sugino::system::Rect &src )
{

	this->setTexture(img);		//‰æ‘œ‚ðÝ’è

	float x1=r.left+0.5f,y1=r.top+0.5f;
	float x2=r.right+0.5f,y2=r.bottom+0.5f;

	float iw=(1/(float)img->getWidth());
	float ih=(1/(float)img->getHeight());
	float tx1=src.left*iw,ty1=src.top*ih;
	float tx2=src.right*iw,ty2=src.bottom*ih;

	TextureTVertex tv[]={
		TextureTVertex(x1,y1,tx1,ty1),TextureTVertex(x2,y1,tx2,ty1),
		TextureTVertex(x1,y2,tx1,ty2),TextureTVertex(x2,y2,tx2,ty2)
	};
	WORD index[]={0,1,2, 2,1,3};
	this->drawPolygon(tv,index,4,2);
	this->setTexture(NULL);
}

//‰æ‘œ‚Ì•`‰æ
void IRender::drawImage(const sugino::system::Rect &r, sugino::graphics::IImage *img, const sugino::system::Rect &src, DWORD col )
{
	this->setTexture(img);		//‰æ‘œ‚ðÝ’è

	float x1=r.left+0.5f,y1=r.top+0.5f;
	float x2=r.right+0.5f,y2=r.bottom+0.5f;

	float iw=(1/(float)img->getWidth());
	float ih=(1/(float)img->getHeight());
	float tx1=src.left*iw,ty1=src.top*ih;
	float tx2=src.right*iw,ty2=src.bottom*ih;

	TextureColorTVertex tv[]={
		TextureColorTVertex(x1,y1,tx1,ty1,col),TextureColorTVertex(x2,y1,tx2,ty1,col),
		TextureColorTVertex(x1,y2,tx1,ty2,col),TextureColorTVertex(x2,y2,tx2,ty2,col)
	};
	

	WORD index[]={0,1,2, 2,1,3};
	this->drawPolygon(tv,index,4,2);
	this->setTexture(NULL);
}

/** @file
 * ウィンドウズオブジェクトクラスの定義
 * @date 2009/05/08
 * @author 杉野
 **/

#pragma once;
#include <list>
#include <algorithm>
#include "Application.h"

namespace sugino{
/** システムの領域 */
namespace system{
	
	class Window;									//仮定義

	/** イベント処理のインターフェースクラス */
	class IWindowEvent
	{
	public:
		/** メッセージ処理
		 * @param [in] Window *w;(派生からなる)対象ウィンドウ画面
		 * @param [in] UINT msg ;メッセージ
		 * @param [in] WPARAM wp;パラメータ
		 * @param [in] LPARAM lp;パラメータ
		 * @reeval 1ほかのイベント処理しない
		 * @reeval 0する
		 **/
		virtual int Event( Window *w,UINT msg,WPARAM wp,LPARAM lp ) =0;
		virtual ~IWindowEvent(){}					//開放処理
	};

	/** ウィンドウ管理クラス */
	class Window
	{
	private:
		HWND hwnd;									//ウィンドウハンドル
		BOOL active;								//TURE:アクティブ FALSE:非アクティブ
		static LRESULT CALLBACK WindowProc( HWND hw,UINT msg,WPARAM wp,LPARAM lp);//メッセージ処理
 		
		/** イベント処理関数　
		 * @param [in] UINT msg	 : 行う処理
		 * @param [in] WPARAM wp : メッセージパラメータ
		 * @param [in] LPARAM lp : メッセージパラメータ
		 **/
		int Evant( UINT msg,WPARAM wp,LPARAM lp );	//メッセージを受け取ってアクティブ状態を取得、戻り値が帰るまでメッセージ処理
		std::list<IWindowEvent *> wevent;			//イベントリスト
		void EventClear();							//リストの削除
		
	public:
		Window();
		~Window(){ EventClear();Destroy(); }
		
		/** ウィンドウ生成 */
		int Create();
		
		/** ウィンドウ開放 */
		void Destroy(){ if(hwnd){ DestroyWindow( hwnd ); hwnd=NULL; } }
		
		/** 表示 */
		void Show(){ ShowWindow( hwnd, SW_SHOW ); }
		
		/** 非表示 */
		void Hide(){ ShowWindow( hwnd, SW_HIDE ); }
		
		/**　再描画要求　*/
		void Refresh(){ InvalidateRect( hwnd,NULL,FALSE ); }
		
		/** ハンドル取得
		 * return	hwnd	:ウィンドウハンドル
		 */
		inline HWND getHwnd(){ return hwnd; }		//ハンドル取得

		/** 画面状態取得
		 * return	IsWindowVisible( hwnd )!=0
		 **/
		inline bool isVisible(){ return IsWindowVisible( hwnd )!=0;   };

		/** アクティブ状態の取得
		 * return active	:アクティブ
		 * @note	true:アクティブ	false:非アクティブ
		 **/
		inline bool isActive(){ return active!=0; };

		/** 画面設定の取得
		 * return	GetWindowLong( hwnd,GWL_STYLE )
		 **/
		inline DWORD getStyle(){ return GetWindowLong( hwnd,GWL_STYLE ); }

		/** イベント処理リストに追加 */ 
		void addEvent( IWindowEvent *e ){ wevent.push_back(e);	}
		
		/** イベント用関数 */
		static bool doEvents();
		
		/** スタイル定義 */
		enum WINDOW_STYLE{
			/** サイズを変更不可にする */
			WS_GAME = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			/** タイトル */
			WS_POP = WS_POPUP | WS_SYSMENU,
			/** 通常ウィンドウ */
			WS_APP = WS_OVERLAPPEDWINDOW
		};

		/** デフォルトは中央に位置変更 */
		enum WINDOW_SET_POSITION{ WSP_CENTER = 0x8000000 };

		/** タイトルのセット
		 * @param [in]	タイトル名
		 **/
		void setTitle( const char* t ){ SetWindowText( hwnd,t); }
		
		/** サイズのセット（クライアント領域) 
		 * @param [in]	int	w	:幅
		 * @param [in]	int h	:高さ
		 **/
		void setSize( int w,int h );
		
		/** サイズの取得
		 * @param [out]	int* w	:幅
		 * @param [out]	inj* h	:高さ
		 * @note 引数は変数を入れる	
		 * int wid,hei; getStyle( &wid,&hei );
		 **/
		void getSize( int *w,int *h ); //別の場所に実体があるので*
		
		/** スタイルのセット
		 * @param [in] s スタイル定数
		 **/
		void setStyle( WINDOW_STYLE s );
		
		/** ウィンドウの位置をセット*/
		void setPosition( int x = WSP_CENTER,int y=WSP_CENTER );
		
		/** アイコン設定 
		 * @param [in]	res_name :リソース名
		 **/
		void setIcon( char* res_name );
		
		/** リソースファイルの読み込み
		 * @param [in]	リソースファイル名
		 **/
		void setMenu( const char* res_name );	
		
		/** 初期設定を一括で作成
		 * @param [in] const char* t :タイトル
		 * @param [in] int w		 :画面幅
		 * @param [in] int h		 :画面高さ
		 * @param [in] WINDOW_STYLE  :WS_GAME サイズ変更不可
		 *							 :WS_POP  タイトル
		 *							 :WS_APP  通常ウィンドウ
		 **/
		int Create( const char* t,int w,int h,WINDOW_STYLE s );
	};

	/** 終了イベントを行う関数　:　IWindowEventからの派生
	 *  @note ×ボタンでウィンドウを閉じた時にアプリケーションを終了する際にはウィンドウに対して登録する
	 **/
	class DestroyEvent : public IWindowEvent 
	{
		int Event( Window *w,UINT msg,WPARAM wp,LPARAM lp );
	};

	/** ペイントイベント用インターフェースクラス
	 * @noteペイント処理を行うアプリケーションはこのクラスをインプリメント(インターフェースを派生元に追加)し
	 * Updateをオーバーライドする
	 **/
	class IPaintEvent {
	public:
	/** 再描画イベント
	 * @param dc 描画領域のデバイスコンテキスト
	 **/
	virtual void Update( HDC dc ) = 0;
	};

	/** ペイントイベント処理クラス : IWindowEventからの派生
	 * ペイントイベントのイベントを行うクラス
	 **/
	class PaintEvent : public IWindowEvent {
	private:
		IPaintEvent *pevent;				//自身のポインタを持つ
		int Event( Window *w,UINT msg,WPARAM wp,LPARAM lp );
	public:
		/** コンストラクタ
		 * @param [in]	IPaintEvent	*p	:ペイント処理の実装を行ったクラスのポインタを初期化する
		 **/
		PaintEvent( IPaintEvent *p ) { pevent=p; }
	};
	
	/** メニューイベントインタフェースクラス */
	class IMenuEvent {
	public:
		/** メニューID処理イベント
		 * @param [in] UINT	id :リソース内のID
		 **/
		virtual void Menu( UINT id ) = 0;
	};
	
	/** IDイベント処理クラス
	 *	IDイベントのイベントを行うクラス
	 **/
	class MenuEvent : public IWindowEvent {
	private:
		IMenuEvent *mevent;
		int Event( Window *w, UINT msg,WPARAM wp,LPARAM lp );
	public:
		/** コンストラクタ
		 * @param [in] IMenuEvent *m	:ペイント処理の実装を行ったクラスのポインタを初期化する
		 **/
		MenuEvent( IMenuEvent *m ) { mevent=m; }
	};
	
	/** POINT構造体クラス (中身はx,y)*/
	class Point : public POINT {
	public:
		Point(){}
		Point( int px,int py ){ x=px, y=py; }
	};

	/** RECT構造体クラス(中身はx1,y1,x2,y2) */
	class Rect : public RECT{
	public:
		Rect(){}
		Rect( int x1,int y1,int x2,int y2 ){
			left=x1, top=y1, right=x2, bottom=y2; }
	};
	
	/** フォント簡易利用クラス */
	class Font{
		HFONT font;
	public:
		Font(){ font = NULL; }
		/** サイズと書式指定
		 * @param [in] int		height :文字の高さ
		 * @param [in] char*	name	:書式名(全角文字)
		 * @note 第2引数書体名: ＭＳ ゴシック　ＭＳ 明朝など
		 **/
		Font( int height,char *name ){ Create( height,name ); }	
		
		/** 解放 */
		~Font(){ Delete(); }
		void Delete(){ if(font){ DeleteObject(font); font=NULL; } }
		
		/** フォントの作成
		 * @param [in] int	height	:文字の高さ
		 * @param [in] char	*name	:書式名(全角文字)
		 * @param [in] int	bold	:太さ
		 * @param [in] BOOL italic	:イタリック体
		 * @note 第2引数書体名: ＭＳ ゴシック　ＭＳ 明朝など
		 **/
		void Create( int height,char *name,int bold=FW_REGULAR,BOOL italic=FALSE ){
			Delete();
			font = ::CreateFont( height,	//高さ
					0,0,0,					//文字列,テキストの角度,x軸角度
					bold,					//フォントの太さ
					italic,0,0,				//イタリック体,アンダーライン,打ち消し線
					SHIFTJIS_CHARSET,		//文字セット
					OUT_DEFAULT_PRECIS,		//出力精度
					CLIP_DEFAULT_PRECIS,	//クリッピング精度
					PROOF_QUALITY,			//出力品質
					FIXED_PITCH | FF_MODERN,//ピッチとファミリー
					name );					//書体名
				}
		/**キャスト */
		operator HFONT() const { return font; }
	};	
}}

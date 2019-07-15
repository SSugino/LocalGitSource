using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// ConstやStaticやenumなど外部アクセス用変数
/// </summary>
public class StaticVariable : MonoBehaviour
{
	//---- Const Variable ----
	// マスの横数
	public const int MASS_WIDTH		= 7;
	public const int MASS_HEIGHT	= 7;

	// Unity上でのテクスチャのカードスケール(XY共通)
	public const int CARD_SCALE		= 15;
	public const float CARD_Z		= -195f;
	//---- Static Variable ----
	// 先手のターンを１として相手ターンに移ると＋１．つまり２ターン目に
	public static int TURN			= 1;

	//---- Enum Variable ----
	public enum CARD_TYPE
	{
		CHARACTER	= 0,		// キャラ
		DOMINION	= 1,		// 領地
		EVENT		= 2,		// イベント
		FIELD		= 3,		// フィールド
	}

	// キャラの種族
	public enum RACE_TYPE
	{
		NONE = 0,			// 領地とかイベントとかに割り当て
		HUMAN =1,
		DEAMON=2,
		ANIMAL=3,
		ANGEL =4,
	}

	// 能力データ
	public enum MOVE_TYPE
	{
		NONE = 0,   // 移動不可
		ONE,        //１歩
		TWO,        //２歩
		THREE,      //３歩
	}

	public enum ATTACK_TYPE
	{
		NONE,
		FRONT_T,    //Ｔ
		CROSS,      //×
		PLUS,       //＋
		BACK_T,
	}

	// 開始時のデッキ定義
	// 民兵×１２　村×１
	// 無効化などイベント
	public static string[] StartDeck = { "c0006", "c0006", "c0006", "c0006", "c0006", "c0006", "c0027", "c0031", "c0032", "c0034" };

	public static Vector2 CARD_SIZE = new Vector2(107, 152);

	public const float START_MASS_X = -270f;
	public const float START_MASS_Y = -145f;

	/// <summary>
	/// ボード上のマスの状態
	/// ０：未設置
	/// １：プレイヤーが設置
	/// ２：敵が設置
	/// </summary>
	public static int[,] FieldBoardState = new int[MASS_WIDTH,MASS_HEIGHT];

	// マス座標を定義
	//(プレイヤー左下のイベントを０，０とする
	// ボード幅によってずれてくるので手打ち
	public static Vector2[,] FieldBoradPos = new Vector2[,]
	{
		{ new Vector2(-270f,-145),new Vector2(-163f,-145), new Vector2(-54,-145),new Vector2(54,-145),new Vector2(162,-145)	  ,new Vector2(271,-145)	 ,new Vector2(378,-145)},
		{ new Vector2(-270f,5)	 ,new Vector2(-163f,5	), new Vector2(-54,-5)  ,new Vector2(54,5)	 ,new Vector2(162,5)      ,new Vector2(271,-5)	 ,new Vector2(378,5)},
		{ new Vector2(-270f,158) ,new Vector2(-163f,158 ), new Vector2(-54, 158),new Vector2(54, 158),new Vector2(162,158) ,new Vector2(271,158) ,new Vector2(378,158)},
	
		// ここ以下はとりあえず適当
		{ new Vector2(-270f,3)	 ,new Vector2(-163f,158 ), new Vector2(-54,-145),new Vector2(54,-145),new Vector2(162,-145),new Vector2(271,-145),new Vector2(378,3)},
		{ new Vector2(-270f,4)	 ,new Vector2(-163f,158 ), new Vector2(-54,-145),new Vector2(54,-145),new Vector2(162,-145),new Vector2(271,-145),new Vector2(378,4)},
		{ new Vector2(-270f,5)	 ,new Vector2(-163f,158 ), new Vector2(-54,-145),new Vector2(54,-145),new Vector2(162,-145),new Vector2(271,-145),new Vector2(378,5)},
		{ new Vector2(-270f,6)	 ,new Vector2(-163f,158 ), new Vector2(-54,-145),new Vector2(54,-145),new Vector2(162,-145),new Vector2(271,-145),new Vector2(378,6)},
	};

	/// <summary>
	/// 引数のプレイヤーから見て左を０とする
	/// イベントや墓地を除く
	/// フィールドマスの指定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="row">自分から見た列番目</param>
	/// <param name="number">列から右に何マス目か</param>
	/// <returns></returns>
	public static Vector3 GetFieldPos(Player.PLAYING player,int row,int number)
	{
		if( row >= MASS_WIDTH )
		{
			// 範囲外
			Debug.LogError("Mass Num AreaOver");
		}
		if (number >= MASS_HEIGHT)
		{
			// 範囲外
			Debug.LogError("Mass Num AreaOver");
		}

		switch (player)
		{
			case Player.PLAYING.MINE:
				return FieldBoradPos[row, number];

			case Player.PLAYING.AI:
				return FieldBoradPos[6 - row,number];
		}
		return Vector3.zero;
	}


}

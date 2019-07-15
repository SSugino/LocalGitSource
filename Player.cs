using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// プレイヤー
/// 墓地枚数やデッキ枚数、財源などを管理
/// </summary>
public class Player : MonoBehaviour
{
	[SerializeField]
	private Camera main;

	// 所持財源 Stone.csが管理
	// public int HaveMoney { get; private set; }
	// 所持行動力
	//public int HaveAction { get; private set; }

	public List<Card> DeckList = new List<Card>();

	// 操作は敵か味方か
	public enum PLAYING
	{
		MINE,
		AI,
	}
	public PLAYING play;
	
	/// <summary>
	/// 初期配置へ置く
	/// </summary>
	public void StartCard()
	{
		// 初期デッキ
		for (int i = 0; i < StaticVariable.StartDeck.Length; i++)
		{
			var id = StaticVariable.StartDeck[i];
			this.DeckList.Add(new Card(id));
		}

		for (int i = 0; i < this.DeckList.Count; i++)
		{
			// 初期村
			if ( this.DeckList[i].Master.ID == "c0027")
			{
				var pos = StaticVariable.FieldBoradPos[1, 6];
				var card = Card.Create(this.transform, this.DeckList[i].Master.ID,pos);
				card.SetSprite(true);
				card.State = Card.SetState.SET;
			}
			// 初期イベント
			if (this.DeckList[i].Master.ID == "c0031")
			{
				var pos = StaticVariable.FieldBoradPos[0, 0];
				var card = Card.Create(this.transform, this.DeckList[i].Master.ID, pos);
				card.SetSprite(false);
				card.State = Card.SetState.SET;
			}
			if (this.DeckList[i].Master.ID == "c0032")
			{
				var pos = StaticVariable.FieldBoradPos[0, 1];
				var card = Card.Create(this.transform, this.DeckList[i].Master.ID, pos);
				card.SetSprite(false);
				card.State = Card.SetState.SET;
			}
			if (this.DeckList[i].Master.ID == "c0034")
			{
				var pos = StaticVariable.FieldBoradPos[0, 2];
				var card = Card.Create(this.transform, this.DeckList[i].Master.ID, pos);
				card.SetSprite(false);
				card.State = Card.SetState.SET;
			}

			// 民兵はデッキ
			if (this.DeckList[i].Master.ID == "c0006")
			{
				var pos = StaticVariable.FieldBoradPos[0, 4];
				var card  = Card.Create(this.transform, this.DeckList[i].Master.ID, pos);
				card.SetSprite(false);
			}
		}
	}

	/// <summary>
	/// ボード上カードの選択
	/// </summary>
	/// <returns>タップしたカードクラス</returns>
	public Card Click()
	{
		if (Input.GetMouseButtonDown(0))
		{
			Ray ray = main.ScreenPointToRay(Input.mousePosition);
			RaycastHit2D hit2d = Physics2D.Raycast((Vector2)ray.origin, (Vector2)ray.direction);
			if( hit2d )
			{
				// カードプレハブのコライダーからそのカード情報の取得
				return hit2d.collider.GetComponentInParent<Card>();
			}
		}
		return null;
	}

}

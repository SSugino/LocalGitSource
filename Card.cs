using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UniRx;

/// <summary>
/// 各種カード(Modelクラス)
/// </summary>
public class Card : MonoBehaviour
{
	[SerializeField]
	private SpriteRenderer cardImage;

	[SerializeField]
	private Text cardName;

	[SerializeField]
	private Text cardEffectText;

	[SerializeField]
	private BoxCollider2D button;

	public MastCard Master { private set; get; }

	/// <summary>
	/// フィールドに出したかなどの状態
	/// </summary>
	public enum SetState
	{
		NONE,		// 置いていない
		SET,		// 表向き
		LIE,		// 裏向き
		SIDE,		// 横向き
	}
	public SetState State { private get; set; }

	/// <summary>
	///リストなどの初期化用
	/// </summary>
	/// <param name="cardID"></param>
	public Card(string cardID)
	{
		var m = MastCard.dicCardsMast[cardID];
		Master = m;
		State = SetState.NONE;
	}

	public void Init(string cardID)
	{
		Master = MastCard.dicCardsMast[cardID];
		State = SetState.NONE;
	}

	/// <summary>
	/// カードオブジェクト生成
	/// </summary>
	/// <param name="parent">親ヒエラルキー</param>
	/// <param name="cardID">カードID</param>
	/// <param name="pos">表示位置</param>
	/// <returns>GameObject</returns>
	public static Card Create(Transform parent,string cardID,Vector3 pos)
	{
		GameObject prefab = (GameObject)Resources.Load("Prefabs/Card");
		var makeObject = Instantiate(prefab, parent).GetComponent<Card>();
		makeObject.Init(cardID);
		makeObject.transform.position = pos;
		return makeObject;
	}

	/// <summary>
	/// カード柄の変更
	/// </summary>
	/// <param name="isStart">true:裏向きスタート</param>
	public void SetSprite(bool isStart)
	{
		string cardID = Master.ID;
		// カード種類によってカード表裏を変える
		if (!isStart)
		{
			// 開始時は裏テクスチャからスタート
			cardID = "c0000";
		}
		var res = Resources.Load<Sprite>("Textures/Card/" + cardID);
		this.cardImage.sprite = res;
	}




	GameObject test;
	private void Start()
	{
		test = GameObject.Find("BoardCamera");
	}
	private void Update()
	{
		if (Input.GetKey(KeyCode.W))
		{
			this.transform.position = this.transform.position + new Vector3(0f, 0.3f, 0f);
		}
		else if (Input.GetKey(KeyCode.S))
		{
			this.transform.position = this.transform.position - new Vector3(0f, 0.3f, 0f);
		}

		if (Input.GetKey(KeyCode.A))
		{
			this.transform.position = this.transform.position - new Vector3(0.3f, 0f, 0f);
		}
		else if (Input.GetKey(KeyCode.D))
		{
			this.transform.position = this.transform.position + new Vector3(0.3f, 0f, 0f);
		}

		if (Input.GetKey(KeyCode.UpArrow))
		{
			test.transform.position = this.transform.position + new Vector3(0f, 10.3f, 0f);
		}
		else if (Input.GetKey(KeyCode.DownArrow))
		{
			test.transform.position = this.transform.position - new Vector3(0f, 10.3f, 0f);
		}

	}

}

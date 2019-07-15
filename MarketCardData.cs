using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// 市場に並ぶカードデータ
/// </summary>
public class MarketCardData : MonoBehaviour
{
	[SerializeField]
	private Camera mainCam;

	[SerializeField]
	private Image image;

	[SerializeField]
	private ButtonTap button;

	public MastCard Mast { get; private set; }

	private void Start()
	{
	
	}

	public MarketCardData()
	{
	
	}

	/// <summary>
	/// コンストラクタ以降の初期化(絵柄の切り替えなどはこちらで行う）
	/// カードデータのセット
	/// </summary>
	/// <param name="card"></param>
	public void Set(MastCard card)
	{
		this.Mast = card;
		var res = Resources.Load<Sprite>("Textures/Card/" + card.ID);
		// マスタ→スプライトに変換したものを市場商品として表示
		image.sprite = res;
	}

	/// <summary>
	/// ボード上カードの選択
	/// </summary>
	/// <returns>タップしたカードクラス</returns>
	public MastCard Click()
	{
		/*
		if (Input.GetMouseButtonDown(0))
		{
			Ray ray = mainCam.ScreenPointToRay(Input.mousePosition);
			RaycastHit2D hit2d = Physics2D.Raycast((Vector2)ray.origin, (Vector2)ray.direction);
			if (hit2d)
			{
				// カードプレハブのコライダーからそのカード情報の取得
				return this.Mast;
			}
		}*/
		return null;
	}

	float times = 0;
	public MastCard LongTap()
	{
		if (Input.GetMouseButton(0))
		{
			Ray ray = mainCam.ScreenPointToRay(Input.mousePosition);
			RaycastHit2D hit2d = Physics2D.Raycast((Vector2)ray.origin, (Vector2)ray.direction);
			if (hit2d)
			{
				times += Time.deltaTime;
				if (times >= 1.5f)
				{
					// カードプレハブのコライダーからそのカード情報の取得
					times = 0;
					return this.Mast;
				}
			}
			return null;
		}
		times = 0;
		return null;
	}
}

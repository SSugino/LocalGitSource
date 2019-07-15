using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

/// <summary>
/// 購入市場の管理クラス
/// </summary>
public class MarketManager : MonoBehaviour
{
	// 領地など
	public List<Card> OthersMarketList = new List<Card>();

	// キャラ
	public List<Card> LowMarketList		= new List<Card>();
	public List<Card> MiddleMarketList	= new List<Card>();
	public List<Card> HighMarketList	= new List<Card>();

	/// <summary>
	/// 市場リストの初期か
	/// データは持っているという状態なのでそこからどう生成するか
	/// </summary>
	public void Init()
	{
		foreach (var Value in MastCard.dicCardsMast.Values)
		{
			// その他の区域
			if( Value.cardType != StaticVariable.CARD_TYPE.CHARACTER)
			{
				this.OthersMarketList.Add(new Card(Value.ID));
			}
			// キャラ確定
			else
			{
				if (Value.ID == "c0006") continue;	//民兵は含まない
				//ランクによっての処理わけ
				if(Value.buyCost >= 7)
				{
					this.HighMarketList.Add(new Card(Value.ID));
				}
				else if(Value.buyCost >= 4)
				{
					this.MiddleMarketList.Add(new Card(Value.ID));
				}
				else
				{
					this.LowMarketList.Add(new Card(Value.ID));
				}
			}
		}

		// その他市場に関しては枚数が欲しい為同じものを2重で追加する
		int count = OthersMarketList.Count;
		for (int i=0;i < count;i++)
		{
			OthersMarketList.Add(OthersMarketList[i]);
		}
		
		// リストシャッフル
		this.OthersMarketList = this.OthersMarketList.OrderBy(aa => System.Guid.NewGuid() ).ToList();
		this.LowMarketList = this.LowMarketList.OrderBy(bb => System.Guid.NewGuid()).ToList();
		this.MiddleMarketList = this.MiddleMarketList.OrderBy(cc => System.Guid.NewGuid()).ToList();
		this.HighMarketList = this.HighMarketList.OrderBy(dd => System.Guid.NewGuid()).ToList();
		int a = 0;
	}

}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

/// <summary>
/// マーケットUIのヒエラルキーにつけるUI管理
/// </summary>
public class MarketUICtrl : MonoBehaviour
{
	// SerializeField
	[SerializeField][Header("市場の管理クラス")]
	private MarketManager marketManager;

	[SerializeField]
	private GameObject rootOther;

	[SerializeField]
	private RectTransform mainCanvas;

	[SerializeField, Header("市場のカードroot 低級")]
	private MarketCardData marketLowCardObj;
	[SerializeField, Header("市場のカードroot 中級")]
	private MarketCardData marketMiddleCardObj;
	[SerializeField, Header("市場のカードroot 高級")]
	private MarketCardData marketHighCardObj;
	[SerializeField, Header("市場のカードroot その他")]
	private MarketCardData[] marketOtherCardObj = new MarketCardData[5];

	[SerializeField]
	private MarketCardData cardDetailObj;

	[SerializeField]
	private Text moneyText;

	public enum STATE
	{
		MAIN,
		DETAIL,	// 詳細
		BUY,	// 買う？
	}
	STATE state = STATE.MAIN;

	public  STATE CurrentState { get { return state; } }

	/// <summary>
	/// CanvasをONにすることでUIの表示
	/// </summary>
	/// <param name="player">現在プレイ中</param>
	public void Open(Stone player )
	{
		this.mainCanvas.gameObject.SetActive(true);

		var low = marketManager.LowMarketList[0];
		marketLowCardObj.Set(low.Master);
		marketLowCardObj.gameObject.GetComponent<ButtonTap>().SetCallBack(() =>
		{
			if (CurrentState != STATE.MAIN ||
			this.cardDetailObj.gameObject.activeSelf) return;
			this.cardDetailObj.Set(low.Master);
			this.cardDetailObj.gameObject.SetActive(true);
			ChangeState(STATE.DETAIL);
		});

		var middle = marketManager.MiddleMarketList[0];
		marketMiddleCardObj.Set(middle.Master);
		marketMiddleCardObj.gameObject.GetComponent<ButtonTap>().SetCallBack(() =>
		{
			if (CurrentState != STATE.MAIN ||
			this.cardDetailObj.gameObject.activeSelf) return;
			this.cardDetailObj.Set(middle.Master);
			this.cardDetailObj.gameObject.SetActive(true);
			ChangeState(STATE.DETAIL);
		});

		var high = marketManager.HighMarketList[0];
		marketHighCardObj.Set(high.Master);
		marketHighCardObj.gameObject.GetComponent<ButtonTap>().SetCallBack(() =>
		{
			if (CurrentState != STATE.MAIN ||
			this.cardDetailObj.gameObject.activeSelf) return;
			this.cardDetailObj.Set(high.Master);
			this.cardDetailObj.gameObject.SetActive(true);
			ChangeState(STATE.DETAIL);
		});

		for (int i = 0; i < marketOtherCardObj.Length; i++)
		{

			var other = marketManager.OthersMarketList[i];
			marketOtherCardObj[i].Set(other.Master);
			marketOtherCardObj[i].gameObject.GetComponent<ButtonTap>().SetCallBack(() =>
			{
				if (CurrentState != STATE.MAIN ||
			this.cardDetailObj.gameObject.activeSelf) return;
				this.cardDetailObj.Set(other.Master);
				this.cardDetailObj.gameObject.SetActive(true);
				ChangeState(STATE.DETAIL);
			});

		}

		// 石カウンターから所持財源を表示
		this.moneyText.text = player.Counter.ToString();
		state = STATE.MAIN;
	}

	public void Close()
	{
		this.mainCanvas.gameObject.SetActive(false);
	}

	/// <summary>
	/// 市場のカード絵柄などのセット
	/// </summary>
	public void Init ()
	{


	}

	// Update is called once per frame
	void Update()
	{
		switch(state)
		{
			case STATE.MAIN:
				/*
				// 長押し詳細
				if (marketLowCardObj.LongTap() != null)
				{
					this.cardDetailObj.Set(marketLowCardObj.Mast);
					this.cardDetailObj.gameObject.SetActive(true);
					state = STATE.DETAIL;
					break;
				}
			

				if (marketMiddleCardObj.LongTap() != null)
				{
					this.cardDetailObj.Set(marketMiddleCardObj.Mast);
					this.cardDetailObj.gameObject.SetActive(true);
					state = STATE.DETAIL;
					break;
				}
				if (marketHighCardObj.LongTap() != null)
				{
					this.cardDetailObj.Set(marketHighCardObj.Mast);
					this.cardDetailObj.gameObject.SetActive(true);
					state = STATE.DETAIL;
					break;
				}
				for(int i=0; i< this.marketOtherCardObj.Length;i++)
				{
					if (marketOtherCardObj[i].LongTap() != null)
					{
						this.cardDetailObj.Set(marketOtherCardObj[i].Mast);
						this.cardDetailObj.gameObject.SetActive(true);
						state = STATE.DETAIL;
						break;
					}
				}
				*/
				break;
			case STATE.DETAIL:
				//　カード詳細を開いているはずなので
				if( this.cardDetailObj.LongTap() != null )
				{
					// 詳細を閉じる
					this.cardDetailObj.gameObject.SetActive(false);
					state = STATE.MAIN;
				}
				break;
		}
	}

	/// <summary>
	/// 強制的にstate変更
	/// コールバック内でstateを直変更男はできない
	/// </summary>
	/// <param name="_state"></param>
	void ChangeState(STATE _state)
	{
		state = _state;
	}

}

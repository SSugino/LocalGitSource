using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// シーンのゲームループ
/// </summary>
public class _Start : MonoBehaviour
{
	// Player
	[SerializeField]
	private Player player;

	[SerializeField]
	private Player enemy;

	[SerializeField]
	private MarketManager market;

	[SerializeField]
	private MarketUICtrl marketUICtrl;

	enum STATE
	{
		LOAD,
		INIT,
		MAIN,
	}
	STATE state = STATE.LOAD;

	MastCard CardMaster = new MastCard();

	// 以下の石カウンターへは Stone.Typeから配列にアクセスしてくださ
	[SerializeField]
	private Stone[] PlayerStones = new Stone[3];

	[SerializeField]
	private Stone[] EnemyStones = new Stone[3];


	void Start ()
	{
		// 各種マスタ
		this.CardMaster.Init();


		this.state = STATE.LOAD;
	}

	// Update is called once per frame
	void Update ()
	{
		switch (this.state)
		{
			case STATE.LOAD:

				this.state = STATE.INIT;
				break;
			case STATE.INIT:
				this.market.Init();
				this.player.StartCard();
				this.enemy.StartCard();
				this.marketUICtrl.Init();

				this.state = STATE.MAIN;
				PlayerStones[(int)Stone.TYPE.MONEY].Add(2);
				PlayerStones[(int)Stone.TYPE.ACTION].Add(2);
				break;
			case STATE.MAIN:

				//if( player.Click() != null)
				if(Input.GetKey(KeyCode.Space))
				{
					this.marketUICtrl.Open(PlayerStones[(int)Stone.TYPE.MONEY]);
				}


				break;
		}

	}
}

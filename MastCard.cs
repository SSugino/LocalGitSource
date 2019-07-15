using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// カードのマスタ
/// </summary>
public class MastCard
{
	public string ID { get; private set; }
	public string Name { get; private set; }
	public StaticVariable.RACE_TYPE Race { get; private set; }
	public StaticVariable.CARD_TYPE cardType { get; private set; }
	public int buyCost { get; private set; }    // 購入コスト
	public int HP { get; private set; }
	public int Hit { get; private set; }
	public int Atk { get; private set; }
	public StaticVariable.MOVE_TYPE moveType { get; private set; }
	public StaticVariable.ATTACK_TYPE attackType { get; private set; }
	public string Effect { get; private set; }

	// こちらからアクセスしてください
	public static Dictionary<string, MastCard> dicCardsMast = new Dictionary<string, MastCard>();

	public void Init()
	{
		var card = CSVReader.LoadAssetsCSV("card");

		const int cell = 1;
		// エクセル上で最初はコメントかついらない情報なので2行目からのデータ
		for(int  i= cell;i< card.Count;i++)
		{
			var add = new MastCard();
			//　詳細データ
			add.ID = card[i][0];
			add.Name = card[i][1];
			add.Race = (StaticVariable.RACE_TYPE)int.Parse(card[i][2]);
			add.cardType = (StaticVariable.CARD_TYPE) int.Parse(card[i][3]);
			add.buyCost = int.Parse(card[i][4]);
			add.HP = int.Parse(card[i][5]);
			add.Hit = int.Parse(card[i][6]);
			add.Atk = int.Parse(card[i][7]);
			add.moveType = (StaticVariable.MOVE_TYPE)int.Parse(card[i][8]);
			switch(card[i][9])
			{
				case "t":
					add.attackType = StaticVariable.ATTACK_TYPE.FRONT_T;
					break;
				case "x":
					add.attackType = StaticVariable.ATTACK_TYPE.CROSS;
					break;
				case "p":
					add.attackType = StaticVariable.ATTACK_TYPE.PLUS;
					break;
				case "b":
					add.attackType = StaticVariable.ATTACK_TYPE.BACK_T;
					break;
				default:
					add.attackType = StaticVariable.ATTACK_TYPE.NONE;
					break;
			}

			add.Effect = card[i][10];

			dicCardsMast.Add(add.ID, add);
		}
	}

}
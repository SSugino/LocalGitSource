using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// UIの石カウンター
/// UIの移動も含めさせます
/// </summary>
public class Stone : MonoBehaviour
{
	public enum TYPE
	{
		ACTION = 0,
		MONEY = 1,
		HP = 2,
	}
	[SerializeField]
	TYPE type;

	private readonly int MoneyMAX = 15;
	private readonly int CastleHPMAX = 10;
	private readonly int ActionMAX = 6;

	private int counter = 0;

	public int Counter { get { return counter; } }

	private void Awake()
	{
		var sprite = GetComponent<SpriteRenderer>();

		string[] paths = {"stone003","stone004","stone002" };
		sprite.sprite = Resources.Load<Sprite>("Textures/Material/" + paths[(int)this.type]);
	}


	public void Add(int value)
	{
		counter += value;
		float move = 7.5f * value;
		// マスの移動（UI）
		this.gameObject.transform.localPosition += new Vector3(0,move,0);
		switch (type)
		{
			case TYPE.ACTION:
				if ( counter > ActionMAX)
				{
					counter = ActionMAX;
				}
				break;
			case TYPE.MONEY:
				if (counter > MoneyMAX)
				{
					counter = MoneyMAX;
				}
				break;
			case TYPE.HP:
				if (counter > CastleHPMAX)
				{
					counter = CastleHPMAX;
				}
				break;

		}
	}

	public void  Sub(int value)
	{
		counter -= value;
		this.gameObject.transform.localPosition += new Vector3(0, -7.5f, 0);
		if ( 0 > counter )
		{
			counter = 0;
		}
	}
}

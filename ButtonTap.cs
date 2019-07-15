using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System;

/// <summary>
/// タップ判定関係
/// </summary>
public class ButtonTap : MonoBehaviour, IPointerClickHandler
									, IPointerUpHandler
									, IPointerDownHandler
									,IDragHandler
{
	[SerializeField]
	Image image;

	/// <summary>
	/// タップされた時の外部で定義された挙動
	/// </summary>
	private Action OnClickCallBack { get; set; }

	/// <summary>
	/// タップされた時の挙動
	/// </summary>
	/// <param name="function"></param>
	public void SetCallBack( Action function = null)
	{
		OnClickCallBack = function;
	}

	// Use this for initialization
	void Start () {

	}
	
	// Update is called once per frame
	void Update () {

	}

	// ---------------------------------------------------------------------------
	// 以下関数はコールバックで呼び出すのでパブリックですが外部からメンバ関数の
	// コールしない (コンポネントに設定されたオブジェクトをタップ時に自動で呼ばれる
	public void OnPointerClick(PointerEventData eventData)
	{
		if (OnClickCallBack == null) return;
		OnClickCallBack();
	}

	public void OnPointerUp(PointerEventData eventData)
	{
		Debug.Log("Up");
	}

	public void OnPointerDown(PointerEventData eventData)
	{
		Debug.Log("Down");
	}

	virtual public void OnDrag(PointerEventData eventData)
	{
		Debug.Log("Drag");
	}

}

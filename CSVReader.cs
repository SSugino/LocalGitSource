using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class CSVReader
{
	/// <summary>
	/// ファイル名(内部でパスは指定
	/// csvはUTF-8ね
	/// エクセルで形式指定の書き出し
	/// サクラエディタでUTF-8で保存の手順を踏まないと読み込めない
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	public static List<string[]> LoadAssetsCSV( string path )
	{
		var resource = Resources.Load("Csv/" +path) as TextAsset;
		if( resource == null )
		{
			Debug.LogError("csv Data is null");
		}
		StringReader reader = new StringReader(resource.text);
		List<string[]> strList = new List<string[]>();
		// カンマ区切りしつつ
		while(reader.Peek() != -1)
		{
			var line = reader.ReadLine();   //1行見込み
			var sp = line.Split(',');       //カンマ区切り
			strList.Add(sp);
		}
		return strList;
	}

}

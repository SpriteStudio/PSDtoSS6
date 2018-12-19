/*==============================================================================
  File Name: PSDtoSS6_GeneratePivotLayer.jsx
  Title: PSDtoSS6_GeneratePivotLayer
  Version: 2017/09/05
  Author: Web Technology Corp.
  Author URI: http://www.webtech.co.jp/

  Sprite Studio6用のツールPSDtoSS6で原点用レイヤーを作成します。

==============================================================================*/
 #target photoshop

{
    {
        //作業するドキュメントを取得
        var active_doc = activeDocument;

        var kinsoku_flg = false;             //禁則文字のレイヤーを検出
        var doumei_flg = false;             //同名のレイヤーを検出
        var zenkaku_flg = false;             //全角のレイヤーを検出
            
        //ルート階層にあるレイヤーセットを取得
        var docLayerSets= activeDocument.layerSets;
        //ルート階層にある全てのレイヤーを取得
        var docArtLyaers= activeDocument.layers;
        var docArtLyaerCount = docArtLyaers.length

		{
            namecheck_array = new Array(docArtLyaerCount);
	        for (i = 0; i < docArtLyaerCount; i++)
	        {
	            namecheck_array[i] = "";
	            //レイヤ名の取得
	            layer_name = docArtLyaers[i].name;
			    if (-1 != docArtLyaers[i].name.indexOf("@") )
			    {
		            namecheck_array[i] = layer_name;
			    }
				else
				{
		            //ファイル名の重複がないか処理
		            layer_name = chack_name(namecheck_array,layer_name);
		            //名前のリストに追加
		            namecheck_array[i] = layer_name;
				}
			}
		}
	    if ( ( doumei_flg == 0 ) && ( zenkaku_flg == 0 ) && ( kinsoku_flg == 0 ) )
	    {
	        //ファイル名の重複チェックのための配列
	        name_array = new Array(docArtLyaerCount);
	        pivotname_array = new Array(docArtLyaerCount);

			var makeLayerNum = 0;
			var pivotLayerNum = 0;
	        for (i = 0; i < docArtLyaerCount; i++)
	        {
	            name_array[i] = "";
	            pivotname_array[i] = "";
	            if ( true == docArtLyaers[i].isBackgroundLayer )
	            {
	                //背景は出力しない
	                continue;
	            }
	            if (-1 != docArtLyaers[i].name.indexOf("@") )
	            {
	                continue;
	            }
	            if (-1 != docArtLyaers[i].name.indexOf("root") )
	            {
	                continue;
	            }
	            if (-1 != docArtLyaers[i].name.indexOf("+") )
	            {
	                //原点レイヤー名を保存
	                pivotname_array[pivotLayerNum] = docArtLyaers[i].name;
	                pivotLayerNum++;
	                continue;
	            }
	            //レイヤ名の取得
	            layer_name = docArtLyaers[i].name;
	            //ファイル名の重複がないか処理
	            layer_name = chack_name(name_array,layer_name);
	            //名前のリストに追加
	            name_array[makeLayerNum] = layer_name;
	            makeLayerNum++;


	            if (( doumei_flg == 1 )  || ( zenkaku_flg == 1 )||(kinsoku_flg == 1) )
	            {
	                break;
	            }
	        }
	        {
	            //先頭のレイヤーをアクティブにする
	            active_doc.activeLayer = active_doc.layers[0];
	            for ( i = makeLayerNum - 1;  i >= 0; i-- )
	            {
	                //レイヤー名が存在する
	                if ( name_array[i] != "" ) 
	                {
	                    //+の付いた原点レイヤーで同名が存在するかチェック
	                    var onazi = 0
	                    for ( j = 0;  j < pivotLayerNum; j++ )
	                    {
	                        if ( name_array[i] == pivotname_array[j].substr( 1 ) )
	                        {
	                            onazi = 1;
	                            break;
	                        }
	                    }
	                    //同名のレイヤーが無い場合はレイヤーを原点生成
	                    if( onazi == 0 )
	                    {
	                        var layObj = activeDocument.artLayers.add();
	                        layObj.name  = "+" + name_array[i];
	                    }
	                }
	            }
	        }
		    if ( ( doumei_flg == 0 ) && ( zenkaku_flg == 0 ) && ( kinsoku_flg == 0 ) )
		    {
		        alert("PSDtoSS6用原点レイヤーを作成しました。");
		    }
		}
    }
}

// =======================================================
/* 以下関数 */

//配列内にobjがあるかをチェック
function check_array(target_array,obj){
    for (var i in target_array){
        if (target_array[i].toUpperCase() == obj.toUpperCase()){
            return true;
            break;
            }
    }
    return false;
 }

//名前の重複を調べて再割当て
function chack_name(array1,name1){
    if ( CheckLength(name1,1) == true )
    {
        //全角が含まれている
        zenkaku_flg = true;             //同名のレイヤーを検出
        alert("全角文字のレイヤーがあります：" + name1 + "\r\nレイヤー名を確認してください。\r\n出力を中断します。");
    }
    if( CheckKinsokuLength(name1)  == true) 
    {
        kinsoku_flg = true;             //同名のレイヤーを検出
        alert("禁則文字のレイヤーがあります：" + name1 + "\r\nレイヤー名を確認してください。\r\n出力を中断します。");
    }

    //名前の変換
    //[.]を_に置換する
    name1 = name1.replace( ".", "_") ;

    if (! check_array(array1,name1)){
        return name1;
     }else{
        alert("同名のレイヤーがあります：" + name1 + "\r\nレイヤー名を確認してください。\r\n大文字と小文字は区別されません。\r\n出力を中断します。");
         doumei_flg = true; //同名レイヤーが含まれている
         //序数
         var tag = 1;
         //同一名が100以上になることは考えない
         while (tag < 100){
            var name2 = name1 + "_" + tag;
            if (! check_array(array1,name2)){
                return name2;
            }
            tag += 1;
        }
    }
}

/****************************************************************
* 全角/半角文字判定
*
* 引数 ： str チェックする文字列
* flg 0:半角文字、1:全角文字
* 戻り値： true:含まれている、false:含まれていない 
*
****************************************************************/
function CheckLength(str,flg) {
    for (var i = 0; i < str.length; i++) {
        var c = str.charCodeAt(i);
        // Shift_JIS: 0x0 ～ 0x80, 0xa0 , 0xa1 ～ 0xdf , 0xfd ～ 0xff
        // Unicode : 0x0 ～ 0x80, 0xf8f0, 0xff61 ～ 0xff9f, 0xf8f1 ～ 0xf8f3
        if ( (c >= 0x0 && c < 0x81) || (c == 0xf8f0) || (c >= 0xff61 && c < 0xffa0) || (c >= 0xf8f1 && c < 0xf8f4)) {
            if(!flg) return true;
        } else {
            if(flg) return true;
        }
    }
    return false;
}

/****************************************************************
* 禁則文字判定
*
* 戻り値： true:含まれている、false:含まれていない 
*
****************************************************************/
function CheckKinsokuLength(str) {
    for (var i = 0; i < str.length; i++) 
    {
        var c = str.charCodeAt(i);
        ///.と#を判定
//        if ( (c == 0x2e) || (c == 0x23) ) 
        if (c == 0x23)  
        {
            return true;
        } 
    }
    return false;
}


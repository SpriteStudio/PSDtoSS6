/*==============================================================================
  File Name: PSDtoSS6_Export.jsx
  Title: PSDtoSS6_Export
  Version: 2017/09/05
  Author: Web Technology Corp.
  Author URI: http://www.webtech.co.jp/

  Sprite Studio6用のツールPSDtoSS6で使用するpngファイル、テキストファイルを出力します

==============================================================================*/
 #target photoshop
 
// 実行フラグ
var do_flag = 1;

/*
//フォルダ選択ダイアログ
var filename = app.activeDocument.path;
folderName = Folder.selectDialog("フォルダを選択してください", filename);
if ( folderName == null)
{
	do_flag = 0;    //実行しない
}
*/
folderName = app.activeDocument.path;


// =======================================================
// 実行フラグが1（キャンセルボタンが押されていない）であれば
if (do_flag == 1) {
     //出力フォルダを作成
     var st = app.activeDocument.name.lastIndexOf( "." );
     filename = app.activeDocument.name.substring( 0, st );
     folderName = folderName  + "/ssceconv_" + filename;

    filenameZen = CheckLength(filename,1)
    if(filenameZen== true) 
    {
        alert("全角ファイル名のデータはコンバートできません。\n半角ファイル名で保存後、再実行してください。");
    }
    else
    {
        //フォルダの作成
        folderObj = new Folder(folderName);
        if ( folderObj.exists == false )
        {
            //無ければフォルダを作成する
            flag = folderObj.create();    
        }
        else
        {
            flag = true;
        }
     
        if(flag == true) 
        {
            textfilename = folderName + "/" + filename + ".ss6-psdtoss6-info";
            fileObj = new File(textfilename);
            flag = fileObj.open("w");
            if (flag == true)
            {

                //Folder.selectDialog ([prompt])  :出力先を指定
                var outputDir = folderName;
                //選択されてない場合はnullが返る
                //alert(outputDir );
                if (outputDir != null){
                    //作業するドキュメントを取得
                    var active_doc = activeDocument;
                    try {
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
                                if ( true == docArtLyaers[i].isBackgroundLayer )
                                {
                                    //背景は出力しない
                                    continue;
                                }
                                if (-1 != docArtLyaers[i].name.indexOf("@") )
                                {
                                    continue;
                                }
                                //レイヤ名の取得
                                layer_name = docArtLyaers[i].name;
                                //ファイル名の重複がないか処理
                                layer_name = chack_name(namecheck_array,layer_name);
                                //名前のリストに追加
                                namecheck_array[i] = layer_name;
                            }
                        }
                        if ( ( doumei_flg == 0 ) && ( zenkaku_flg == 0 ) && ( kinsoku_flg == 0 ) )
                        {
                            //レイヤの表示状態を退避する配列を作成
                            layer_v_array = new Array(docArtLyaerCount);
                            //全てのレイヤーの表示状態を取得して非表示に
                            for (i = 0; i < docArtLyaerCount; i++){
                                //表示状態の取得
                                layer_v_array[i] = docArtLyaers[i] .visible;
                                //非表示に
                                docArtLyaers[i] .visible = false;
                            }
                            //ファイル名の重複チェックのための配列
                            name_array = new Array(docArtLyaerCount);
                            //ルートレイヤーの書き出し
                            for (i = 0; i < docArtLyaerCount; i++)
                            {
                                if ( true == docArtLyaers[i].isBackgroundLayer )
                                {
                                    //背景は出力しない
                                    continue;
                                }
                                if (-1 != docArtLyaers[i].name.indexOf("@") )
                                {
                                    continue;
                                }
                                //表示にする
                                docArtLyaers[i] .visible = true;
                                //レイヤ名の取得
                                layer_name = docArtLyaers[i].name;
                                //ファイル名の重複がないか処理
                                layer_name = chack_name(name_array,layer_name);
                                //名前のリストに追加
                                name_array[i] = layer_name;


                                if (( doumei_flg == 1 )  || ( zenkaku_flg == 1 )||(kinsoku_flg == 1) )
                                {
                                    break;
                                }

                                //ファイルパスの生成
                                var file_path = outputDir  + "/" + layer_name + ".png";

                                //ファイルの書き出し
                                save_PNG(file_path);
                                //対象レイヤーを非表示に
                                docArtLyaers[i].visible = false;

                                //ファイル名出力
                                textstr = layer_name + ".png#";
                                fileObj.write(textstr);
                            }
                            //レイヤの表示を元の様態に戻す
                            for (i = 0; i < docArtLyaerCount; i++){
                                //表示状態の取得
                                docArtLyaers[i] .visible = layer_v_array[i];
                            }
                            if ( ( doumei_flg == 0 ) && ( zenkaku_flg == 0 ) && ( kinsoku_flg == 0 ) )
                            {
                                alert("PSDtoSS6用ファイルを出力しました。\r\n出力フォルダのss6-psdtoss6-infoファイルを指定してコンバートしてください。");
                            }
                        }

                     } catch (e) {
                        if ( DialogModes.NO != app.playbackDisplayDialogs ) {
                            alert(e);
                        }
                    //return 'cancel'; // quit, returning 'cancel' (dont localize) makes the actions palette not record our script
                    }
                }
                fileObj.close();
            }else{
                alert("情報ファイルが作成できませんでした");
                alert(textfilename);
            } 
        }
        else
        {
            alert("フォルダの作成に失敗しました。");
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

//PNGで保存
function save_PNG(file_path){
    pngFile = new File(file_path);
    pngSaveOptions = new PNGSaveOptions();
    pngSaveOptions.interlaced = false;
    app.activeDocument.saveAs(pngFile, pngSaveOptions, true,Extension.LOWERCASE);
}

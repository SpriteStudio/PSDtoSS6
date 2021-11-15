#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../cui/source/parameters.h"
#include "subwindow.h"

#include <QTextCodec>

QString execPathStr = "";    //実行しているコンバータGUIのパス
QString Outputpath = "";     //出力フォルダ
QString cnvOutputStr = "";   //コンバート結果
bool convert_exec = false;  //コンバート中か
bool convert_error = false;  //コンバートエラーが発生したか
int convet_index = 0;
std::map<QString, QString> map_sortmode;
std::map<int, QString> map_texture_wh;
std::map<int, QString> map_canvasSize;

#define TITLE_VERSION "PSDtoSS6 GUI Ver2.4.3"

//#define TOOLFOLDER "/SpriteStudio/PSDtoSS6"		//v2.0.1
#define TOOLFOLDER "/PSDtoSS6"

convert_parameters cp;

subwindow *sw;//プレビューウィンドウ

template<typename T1, typename T2>
T1 MainWindow::getKey(const std::map<T1, T2> & map, const T2 & value) const
{
    auto itr = map.cbegin();
    for(; itr != map.cend(); itr++)
    {
        if(itr->second == value)
        {
            return itr->first;
        }
    }
    return itr->first;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //フォームの部品にアクセスする場合はuiのメンバを経由する
    ui->setupUi(this);
    //再翻訳
    ui->retranslateUi(this);

    //ドラッグ＆ドロップを有効にする
    setAcceptDrops(true);

    cnvProcess = new QProcess(this);
    // プロセスが終了した時に finished シグナル発信
    connect(cnvProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus )));
    // プロセスからエラー出力があって読み込み可能になったら readyReadStandardError シグナル発信
    connect(cnvProcess, SIGNAL(readyReadStandardError()), this, SLOT(processErrOutput()));

    imgProcess = new QProcess(this);

    //ウィンドウスタイルの定義
    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint
                   | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    //ウィンドウのタイトルをつける
    this->setWindowTitle(TITLE_VERSION);

    //初期化
    convert_exec = false;
    cnvOutputStr.clear();

    //ウィンドウサイズ固定
    setFixedSize( QSize(688,661) );

    ui->comboBox->addItem(map_sortmode["none"] = "Name");
    ui->comboBox->addItem(map_sortmode["rectmax"] = "Area Size");
    ui->comboBox->addItem(map_sortmode["wmax"] = "Width");
    ui->comboBox->addItem(map_sortmode["hmax"] = "Height");
    ui->comboBox->setCurrentIndex(1);

    ui->comboBox_w->addItem(map_texture_wh[0] = "Auto");
    ui->comboBox_w->addItem(map_texture_wh[256] = "256");
    ui->comboBox_w->addItem(map_texture_wh[512] = "512");
    ui->comboBox_w->addItem(map_texture_wh[1024] = "1024");
    ui->comboBox_w->addItem(map_texture_wh[2048] = "2048");
    ui->comboBox_w->addItem(map_texture_wh[4096] = "4096");
    ui->comboBox_w->addItem(map_texture_wh[8192] = "8192");

    ui->comboBox_h->addItem(map_texture_wh[0]);
    ui->comboBox_h->addItem(map_texture_wh[256]);
    ui->comboBox_h->addItem(map_texture_wh[512]);
    ui->comboBox_h->addItem(map_texture_wh[1024]);
    ui->comboBox_h->addItem(map_texture_wh[2048]);
    ui->comboBox_h->addItem(map_texture_wh[4096]);
    ui->comboBox_h->addItem(map_texture_wh[8192]);

    ui->comboBox_canvasSize->addItem(map_canvasSize[0] = "Default");
    ui->comboBox_canvasSize->addItem(map_canvasSize[1] = "PSD Size");

    pushButton_enableset();

    //スタイルシートを読み込む
    QFile file(":/style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());
    qApp->setStyleSheet(styleSheet);

    //Documentsのパスを取得
    data_path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    //    data_path += "/SpriteStudio/PSDtoSS6";
    data_path += TOOLFOLDER;
    // qDebug() << "data_path " << data_path;

    //出力フォルダのパスを確認
    if(Outputpath == "")
    {
        Outputpath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        Outputpath += "/SpriteStudio/PSDtoSS6";
    }

    QDir dir;
    //設定ファイル保存用ディレクトリを作成
    dir.mkpath(data_path);
    //出力フォルダのディレクトリを作成
    dir.mkpath(Outputpath);

    //プレビューウィンドウの初期化
    sw = new subwindow(this);
}

MainWindow::~MainWindow()
{
    delete_convert_info();
    delete ui;
    delete sw;
}

void MainWindow::loadConfig(const QString & fileName)
{
    convert_parameters cp;
    QString outputText =  "";
    outputText = "Load Config json file = " + fileName;

#if _WIN32
    QTextCodec *sjis = QTextCodec::codecForName("Shift-JIS");
    if (!cp.parseConfigJson(sjis->fromUnicode(fileName).toStdString()))
    {
        QString str = QString::fromLocal8Bit(cp.err_string.c_str());
        outputText += "\nLoad error";
    }
    else {
        outputText += "\nLoad Successful";
    }
#else
    if (!cp.parseConfigJson(fileName.toStdString()))
    {
        outputText += "\nLoad error";
    }
    else {
        outputText += "\nLoad Successful";
    }
#endif

    /*
    if ( !cp.parseConfigJson(fileName.toStdString()) )
    {
        QString str = QString::fromLocal8Bit( cp.err_string.c_str() );
                outputText+= "setting json Load error";
    }else{
                outputText+= "setting json Load Successful";
    }
*/

    ui->textBrowser_err->setText(outputText);

    ui->comboBox_w              ->setCurrentText(map_texture_wh[cp.tex_w]);
    ui->comboBox_h              ->setCurrentText(map_texture_wh[cp.tex_h]);
    ui->lineEdit_padding_shape  ->setText(QString::number(cp.tex_padding_shape));
    ui->lineEdit_cell_padding   ->setText(QString::number(cp.pack_padding));
    ui->comboBox                ->setCurrentIndex(cp.sortmode);
    ui->checkBox_ssae           ->setChecked(cp.is_ssaeoutput);
    ui->checkBox_sspj           ->setChecked(cp.is_sspjoutput);
    ui->lineEdit_pri            ->setText(QString::number(cp.addpri));
    ui->checkBox_addnull        ->setChecked(cp.is_addnull);
    ui->checkBox_overwrite      ->setChecked(cp.is_overwrite);
    ui->checkBox_pivot          ->setChecked(cp.is_layerPivotUse);
    ui->checkBox_root           ->setChecked(cp.is_rootLayerUse);
    ui->checkBox_pivot_add      ->setChecked(cp.is_oldPivotUse);
    ui->lineEdit_padding_border ->setText(QString::number(cp.padding_border));
    ui->comboBox_canvasSize     ->setCurrentText(map_canvasSize[cp.canvasSize]);
    ui->lineEdit_padding_inner  ->setText(QString::number(cp.inner_padding));
    ui->textBrowser_output      ->setText(cp.outputpath.c_str());
}

void MainWindow::saveConfig(const QString & fileName)
{

    cp.tex_w                = getKey(map_texture_wh, ui->comboBox_w->currentText());
    cp.tex_h                = getKey(map_texture_wh, ui->comboBox_h->currentText());
    cp.tex_padding_shape    = ui->lineEdit_padding_shape->text().toInt();
    cp.pack_padding         = ui->lineEdit_cell_padding->text().toInt();
    cp.sortmode             = ui->comboBox->currentIndex();
    cp.is_ssaeoutput        = ui->checkBox_ssae->isChecked();
    cp.is_sspjoutput        = ui->checkBox_sspj->isChecked();
    cp.addpri               = ui->lineEdit_pri->text().toInt();
    cp.is_addnull           = ui->checkBox_addnull->isChecked();
    cp.is_overwrite         = ui->checkBox_overwrite->isChecked();
    cp.is_layerPivotUse     = ui->checkBox_pivot->isChecked();
    cp.is_rootLayerUse      = ui->checkBox_root->isChecked();
    cp.is_oldPivotUse       = ui->checkBox_pivot_add->isChecked();
    cp.padding_border       = ui->lineEdit_padding_border->text().toInt();
    cp.canvasSize           = getKey(map_canvasSize, ui->comboBox_canvasSize->currentText());
    cp.inner_padding        = ui->lineEdit_padding_inner->text().toInt();
    cp.outputpath           = ui->textBrowser_output->toPlainText().toStdString();
    cp.outputname           = "";

    QString outputText =  "";

#if _WIN32
    QTextCodec *sjis = QTextCodec::codecForName("Shift-JIS");
    if ( cp.saveConfigJson(sjis->fromUnicode(fileName).toStdString()) )
    {
        outputText = "Config json file = " + fileName + "\n";
        outputText += "Save Successful";
    }else{
        outputText = "Config json file = " + fileName + "\n";
        outputText += "Save Failed";
    }
#else
    if ( cp.saveConfigJson(fileName.toStdString()) )
    {
        outputText = "Config json file = " + fileName + "\n";
        outputText += "Save Successful";
    }else{
        outputText = "Config json file = " + fileName + "\n";
        outputText += "Save Failed";
    }
#endif

    ui->textBrowser_err->setText(outputText);
}


void MainWindow::setText_to_List(QStringList list)
{
    //実行ファイルのパスを保存
    execPathStr = list[0];

    if ( list.length() > 1 )
    {
        int i;
        for ( i = 1; i < list.length(); i++ )
        {
            QString dragFilePath;
            dragFilePath = list[i];
            if ( ( dragFilePath.endsWith(".txt")) || ( dragFilePath.endsWith(".txt")) )
            {
                ui->listWidget->addItem(dragFilePath);
            }
        }
    }
    loadConfig(data_path + "/config.json");
    templistload();

}

void MainWindow::on_pushButton_exit_clicked()
{
    saveConfig(data_path + "/config.json");
    delete_convert_info();
    //アプリケーションの終了
    exit(0);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    if(e->mimeData()->hasUrls())
    {
        QList<QUrl> urlList = e->mimeData()->urls();
        for(int i = 0; i < urlList.size(); i++)
        {
            //ドラッグしたファイルをリストに追加
            //.png以外は弾く
            QString dragFilePath;
            dragFilePath = urlList[i].toLocalFile();
            if (
                    ( dragFilePath.endsWith(".ss6-psdtoss6-info"))
                    || ( dragFilePath.endsWith(".psd"))
                    )
            {
                //同じ名前がリストにある場合は弾く
                bool addname = true;
                int j = 0;
                for ( j = 0; j < ui->listWidget->count(); j++ )
                {
                    QString fileName = ui->listWidget->item(j)->text();
                    if ( fileName == dragFilePath )
                    {
                        addname = false;
                        break;
                    }

                }
                if ( addname == true )
                {
                    ui->listWidget->addItem(dragFilePath);
                }
            }
        }
        pushButton_enableset();
    }
}

void MainWindow::on_pushButton_listclear_clicked()
{
    //リストクリア
    ui->listWidget->clear();
    pushButton_enableset();
}

void MainWindow::on_pushButton_convert_clicked()
{
    //コンバート
    if ( ui->listWidget->count() == 0 )
    {
        QMessageBox msgBox(this);
        msgBox.setText(tr("_registerTextFileText"));
        msgBox.exec();
        return;
    }
    if ( ui->textBrowser_output->toPlainText() == "" )
    {
        QMessageBox msgBox(this);
        msgBox.setText(tr("_selectOutputFolderText"));
        msgBox.exec();
        return;
    }
    else
    {
        //書き込めるフォルダか確かめる
        QString folder = ui->textBrowser_output->toPlainText();
        QFile testDir(folder + "_accessTest.txt");
        if(false == testDir.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox msgBox(this);
            msgBox.setText(tr("_folderNotWritableText"));
            msgBox.exec();
            return;
        }
        testDir.remove();
    }

    if (( ui->listWidget->count() > 0 ) && (convert_exec == false))
    {
        templistsave();            //コンバートリストを保存
        save_convert_info();//コンバート情報ファイルを保存
        buttonEnable( false );   //ボタン無効
        convert_error = false;
        convert_exec = false;  //コンバート中か
        convet_index = 0;
        ui->textBrowser_err->setText(tr(""));           //エラー
        cnvOutputStr = "";

        convert_exec = true;  //コンバート中か
        int i;
        for ( i = 0; i < ui->listWidget->count(); i++ )
        {
            //進行状況表示
            QString st = QString("Exec %1/%2").arg(i+1).arg(ui->listWidget->count());
            ui->textBrowser_status->setText(st);     //ステータス

            //コンバート引数作成
            QString fileName = ui->listWidget->item(i)->text();
            //コンバータの起動
            if (fileName.isEmpty())
            {
                //ファイル名なし
            }
            else
            {
                QString str;
                QString execstr;
                QString execstrSub;

#ifdef _WIN32
                // Windows
                QDir dir = QDir(execPathStr);
                dir.cd("..");
                QString str_current_path = dir.path();
                execstr = str_current_path + "/PSDtoSS6.exe";
                execstrSub = execstr;
#else
                // Mac
                QDir dir = QDir(execPathStr);
                dir.cd("..");
                dir.cd("..");
                dir.cd("..");
                dir.cd("..");
                QString str_current_path = dir.path();
                execstr = str_current_path + "/PSDtoSS6";

                dir = QDir(execPathStr);
                dir.cd("..");
                str_current_path = dir.path();
                execstrSub = str_current_path + "/PSDtoSS6";
#endif

                qDebug() << "\n===========================================";
                qDebug() << "Running " << execstr;

                std::string arg_str = cp.makeArgFromParam();

                qDebug() << (QFile::exists(execstr) ? "File exists: " : "File may not exist:") << execstr;
                if ( QFile::exists(execstr) == false )
                {
                    //ファイルの有無を調べる
                    convert_error = true;
                    cnvOutputStr = cnvOutputStr + "converter file exists false\n";
                    ui->textBrowser_err->setText(cnvOutputStr);
                }

                qDebug() << (QFile::exists(execstrSub) ? "File exists: " : "File may not exist:") << execstrSub;
                if ( QFile::exists(execstrSub) == false )
                {
                    //ファイルの有無を調べる
                    convert_error = true;
                    cnvOutputStr = cnvOutputStr + "converter file exists false\n";
                    ui->textBrowser_err->setText(cnvOutputStr);
                }else{
                    execstr = execstrSub;
                }

#ifdef _WIN32

                execstr= "\"" + execstr + "\"";
                str = execstr + " " + QString(arg_str.c_str()) + " -I " + "\"" + fileName + "\"";
#else
                //execstr= "\"" + execstr + "\"";
                str = execstr + " " + QString(arg_str.c_str()) + " -I " + "\"" + fileName + "\"";

#endif

                cnvProcess->start(str); //パスと引数

                while ( 1 )
                {
                    QThread::sleep(1);  // Wait
                    QCoreApplication::processEvents();
                    if ( cnvProcess->state() != QProcess::Running )
                    {
                        //                        ui->textBrowser_status->setText(tr("Convert end"));
                        break;
                    }
                }
                QCoreApplication::processEvents();
                if ( convert_error == true )
                {
                    break;
                }
                convet_index++;
            }
        }
        if ( convert_error == false )
        {
            ui->textBrowser_status->setText(tr("_convertSuccessText"));
        }
        else
        {
            ui->textBrowser_status->setText(tr("_convertErrorText"));   //ステータス
        }
        buttonEnable( true );   //ボタン有効
        convert_exec = false;  //コンバート中か
    }
}

void MainWindow::processErrOutput()
{
    // 出力を全て取得
    QByteArray output = cnvProcess->readAllStandardError();
    //ワーニングの表示は行わない
    QString str = QString::fromLocal8Bit( output );
    //    int i = str.indexOf("warning");
    //    if ( i == -1 )  //ワーニング以外を表示
    {
        cnvOutputStr = cnvOutputStr + str;
        ui->textBrowser_err->setText(cnvOutputStr);
    }
    //カーソルを最終行へ移動
    QScrollBar *sb = ui->textBrowser_err->verticalScrollBar();
    sb->setValue(sb->maximum());

}
void MainWindow::processFinished( int exitCode, QProcess::ExitStatus exitStatus)
{
    if ( exitStatus == QProcess::CrashExit )
    {
        //        QMessageBox::warning( this, tr("Error"), tr("Crashed") );
        cnvOutputStr = cnvOutputStr + "Error:" + ui->listWidget->item(convet_index)->text();
        ui->textBrowser_err->setText(cnvOutputStr);
        convert_error = true;
        //カーソルを最終行へ移動
        QScrollBar *sb = ui->textBrowser_err->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
    else if ( exitCode != 0 )
    {
        //        QMessageBox::warning( this, tr("Error"), tr("Failed") );
        cnvOutputStr = cnvOutputStr + "Error:" + ui->listWidget->item(convet_index)->text();
        ui->textBrowser_err->setText(cnvOutputStr);
        convert_error = true;
        //カーソルを最終行へ移動
        QScrollBar *sb = ui->textBrowser_err->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
    else
    {
        convert_error = false;
        // 正常終了時の処理
        //        ui->textBrowser_status->setText(tr("Convert Success!"));
        //    QMessageBox::information(this, tr("Ss6Converter"), tr("Convert success"));
    }
}

//出力フォルダ選択ボタン
void MainWindow::on_pushButton_output_clicked()
{
    QString str;
    str = QFileDialog::getExistingDirectory(this, tr("_outputDirectoryText"), Outputpath);

    if ( str != "" ){//選択されたフォルダ名
        Outputpath = str;
    }
    else{//未指定ならドキュメントの位置
        Outputpath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }

    #if Q_WS_WIN
            Outputpath += "\\";
    #else
            Outputpath += "/";
    #endif

    ui->textBrowser_output->setText(Outputpath);
}

//リストの読み込み
void MainWindow::on_pushButton_listload_clicked()
{
    QFileDialog::Options options;
    QString strSelectedFilter;
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("_selectListFileTexte"), ".", tr("text(*.txt)"), &strSelectedFilter, options);

    if ( fileName != "" )
    {
        //リストクリア
        ui->listWidget->clear();

        //読み込んだファイルをリストに設定
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly))//読込のみでオープンできたかチェック
        {
            return;
        }

        QTextStream in(&file);
        while ( !in.atEnd() ) {
            QString str = in.readLine();//1行読込
            ui->listWidget->addItem(str);
        }
    }
    pushButton_enableset();

}

//リストの保存
void MainWindow::on_pushButton_listsave_clicked()
{
    QFileDialog::Options options;
    QString strSelectedFilter;
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("_saveListFileText"), ".", tr("text(*.txt)"), &strSelectedFilter, options);

    if ( fileName != "" )
    {
        //読み込んだファイルをリストに設定
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly))//読込のみでオープンできたかチェック
        {
            return;
        }

        QTextStream out(&file);
        int i;
        for ( i = 0; i < ui->listWidget->count(); i++ )
        {
            QString str = ui->listWidget->item(i)->text();
            out << str << endl; //書込み
        }
    }
}

void MainWindow::buttonEnable( bool flg )
{
    ui->pushButton_fileadd->setEnabled(flg);
    ui->pushButton_listsave->setEnabled(flg);
    ui->pushButton_listload->setEnabled(flg);
    ui->pushButton_listclear->setEnabled(flg);
    ui->pushButton_output->setEnabled(flg);
    ui->pushButton_convert->setEnabled(flg);
    ui->pushButton_exit->setEnabled(flg);
    ui->pushButton_settingsave->setEnabled(flg);
    ui->pushButton_open_help->setEnabled(flg);
}
//ファイル追加
void MainWindow::on_pushButton_fileadd_clicked()
{
    QFileDialog::Options options;
    QString strSelectedFilter;
    QString openFolderName;
    QString addfileName;
    //ユーザーのドキュメントフォルダを指定
    openFolderName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    addfileName = QFileDialog::getOpenFileName(this, tr("_selectConvertFile"), openFolderName, tr("data(*.ss6-psdtoss6-info *.psd)"), &strSelectedFilter, options);

    if ( addfileName != "" )
    {
        //同じ名前がリストにある場合は弾く
        bool addname = true;
        int j = 0;
        for ( j = 0; j < ui->listWidget->count(); j++ )
        {
            QString fileName = ui->listWidget->item(j)->text();
            if ( fileName == addfileName )
            {
                addname = false;
                break;
            }

        }
        if ( addname == true )
        {
            ui->listWidget->addItem(addfileName);
        }
    }
    pushButton_enableset();


}

void MainWindow::pushButton_enableset()
{
    bool flg = true;
    if ( ui->listWidget->count() == 0 )
    {
        flg = false;
    }
    ui->pushButton_listsave->setEnabled(flg);
    ui->pushButton_listclear->setEnabled(flg);
    ui->pushButton_convert->setEnabled(flg);
}

//一時的なコンバートリスト作成
void MainWindow::templistsave(void)
{
    QString fileName = data_path + "/templist";
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))//書込みのみでオープンできたかチェック
    {
        return;
    }

    QTextStream out(&file);
    int i;
    for ( i = 0; i < ui->listWidget->count(); i++ )
    {
        QString fileName = ui->listWidget->item(i)->text();
        out << fileName << endl;
    }
}
//一時的なリストの読み込み
void MainWindow::templistload()
{
    QString fileName = data_path + "/templist";

    if ( fileName != "" )
    {
        //リストクリア
        ui->listWidget->clear();

        //読み込んだファイルをリストに設定
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly))//読込のみでオープンできたかチェック
        {
            return;
        }

        QTextStream in(&file);
        while ( !in.atEnd() ) {
            QString str = in.readLine();//1行読込
            ui->listWidget->addItem(str);
        }
    }
    pushButton_enableset();

}

//コンバート情報ファイルを作成
void MainWindow::save_convert_info()
{
    QString fileName(data_path + "/convert_info.json");
    saveConfig(fileName);
}
//コンバート情報ファイルを削除
void MainWindow::delete_convert_info()
{
    QFile file(data_path + "/convert_info.json");
    if(file.exists())
    {
        file.remove();
    }
}

//設定の保存ボタン
void MainWindow::on_pushButton_settingsave_clicked()
{
    saveConfig(data_path + "/config.json");

    QMessageBox msgBox(this);
    msgBox.setText(tr("_currentSettingsSavedText"));
    msgBox.exec();
}

void MainWindow::on_pushButton_open_help_clicked()
{
    //ヘルプをブラウザで開く
    QUrl url = QUrl( "http://www.webtech.co.jp/help/ja/spritestudio/guide/tool/psdtoss6/" );
    QDesktopServices::openUrl( url );
}

void MainWindow::on_pushButton_Preview_clicked()
{
    //テクスチャ幅, テクスチャ高さ, 優先度設定
    int width = getKey(map_texture_wh, ui->comboBox_w->currentText());
    int height = getKey(map_texture_wh, ui->comboBox_h->currentText());
    int priority = ui->lineEdit_pri->text().toInt();
    //セル配置順, セル間余白, セル内余白
    int sort = ui->comboBox->currentIndex();
    int pad_shape = ui->lineEdit_padding_shape->text().toInt();
    int pad_inner = ui->lineEdit_padding_inner->text().toInt();
    //セル配置間隔, セルマップ縁幅, 基準枠サイズ
    int pad_cell = ui->lineEdit_cell_padding->text().toInt();
    int pad_border= ui->lineEdit_padding_border->text().toInt();
    int canvas = getKey(map_canvasSize, ui->comboBox_canvasSize->currentText());

    //設定されたパラメータの効果を可視化
    sw->Preview(
                width,    height,     priority,
                sort,     pad_shape,  pad_inner,
                pad_cell, pad_border, canvas);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //終了ボタンと同じ挙動
    saveConfig(data_path + "/config.json");
    delete_convert_info();
    //アプリケーションの終了
    exit(0);
}
#include "subwindow.h"
#include "ui_subwindow.h"

#include <QPen>
#include <QRect>
#include <QGraphicsRectItem>

#define TITLE_PREVIEW "PSDtoSS Visualization"

const int winWIDTH = 740;//ウィンドウSIZE
const int winHEIGHT = 230;
const int bufWIDTH = 400;//プレビューSIZE
const int bufHEIGHT = 212;
const int devide = 8;//矩形の一辺=side/devide

subwindow::subwindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::subwindow)
{
    //フォームの部品にアクセスする場合はuiのメンバを経由する
    ui->setupUi(this);
    //再翻訳
    ui->retranslateUi(this);

    //ウィンドウフラグ設定
    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint
                   | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    //ウィンドウタイトル設定
    setWindowTitle(TITLE_PREVIEW);
    //ウィンドウサイズ固定
    this->setFixedSize(QSize(winWIDTH,winHEIGHT));

    //プレビューウィンドウはデフォルトで非表示
    this->hide();

    //チェックボックスの初期化, uiで指定した初期状態から変化するとイベント処理が入る
    ui->cb_tex_width ->setChecked(Qt::Unchecked);
    ui->cb_tex_height ->setChecked(Qt::Unchecked);
    ui->cb_pad_shape ->setChecked(Qt::Unchecked);
    ui->cb_pad_inner ->setChecked(Qt::Unchecked);
    ui->cb_pad_cell ->setChecked(Qt::Unchecked);
    ui->cb_pad_border ->setChecked(Qt::Unchecked);
}

subwindow::~subwindow()
{
    delete ui;
}

/// @note パラメータをもとに別ウィンドウ生成と図の描画まで担当する関数
/// @attention 4096x4096以降の大きいものを生成すると死ぬ傾向がある
/// @param int width      テクスチャ幅
/// @param int height     テクスチャ高さ
/// @param int priority   優先度設定
/// @param int sort       セル配置順
/// @param int pad_shape  セル間余白
/// @param int pad_inner  セル内余白
/// @param int pad_cell   セル配置間隔
/// @param int pad_border セルマップ縁幅
/// @param int canvas     基準枠サイズ
void subwindow::Preview(
        int width,   int height,    int priority,
        int sort,    int pad_shape, int pad_inner,
        int pad_cell,int pad_border,int canvas)
{
    //レイヤーの優先度を確認
    if(priority < 0) priority = 1;
    //並び順を確認
    if(sort < 0 || sort > 3) sort = 0;//Name based
    //パディングまわりの確認
    if(pad_shape < 0) pad_shape = 2;
    if(pad_inner < 0) pad_inner = 0;
    if(pad_cell < 0) pad_cell = 1;
    if(pad_border < 0) pad_border = 4;
    //キャンバス指定の確認
    if(canvas < 0 || canvas > 1) canvas = 0;//Default

    //メンバ変数として保管
    m_width = bufWIDTH;//ウィンドウサイズは固定
    m_height = bufHEIGHT;
    m_priority = priority;
    m_sort = sort;
    m_pad_shape = pad_shape;
    m_pad_inner=pad_inner;
    m_pad_cell=pad_cell;
    m_pad_border=pad_border;
    m_canvas=canvas;

    //チェックボックスの内容でプレビュー
    DisplayPreview();
}

void subwindow::DisplayPreview()
{
    scene.setSceneRect(0,0,m_width,m_height);
    scene.setBackgroundBrush(bg_strong);
    scene.clear();

    //矩形の描画(外側のCellと内側のパーツ)
    if(ui->cb_pad_inner->checkState() == Qt::Checked)
    {
        DrawCellAndPart(&scene, usu_shiro, usu_murasaki);//薄紫と薄青
    }else
    {
        DrawCellAndPart(&scene, bg_strong, usu_murasaki);//グレーと薄青
    }

    //セルマップ縁幅
    if(ui->cb_pad_border->checkState() == Qt::Checked)
    {
        /*
        //指定0で表示されず、指定1で表示する調整
        int left = m_pad_border-1;
        int right = m_width-m_pad_border+1;
        int top = m_pad_border-1;
        int bottom =m_height-m_pad_border+1;
        //格子状の視覚化
        QPen qp = QPen(QBrush(usu_midori),1.0f);
        scene.addLine(0,top,m_width,top,qp);//上
        scene.addLine(0,bottom,m_width,bottom,qp);//下
        scene.addLine(left,0,left,m_height,qp);//左
        scene.addLine(right,0,right,m_height,qp);//右
        */

        //矩形の描画と線の描画では座標の扱いが少し違う
        int left = m_pad_border;
        int right = m_width-m_pad_border;
        int top = m_pad_border;
        int bottom =m_height-m_pad_border;
        //矩形を描画
        auto DrawRECT = [](QGraphicsScene* sc, QRect rc, QColor cl)
        {
            QGraphicsRectItem *item = sc->addRect(rc);
            item->setBrush(QBrush(cl));
        };
        //4つ角の矩形による視覚化
        DrawRECT(&scene, QRect(0,0,left,top), usu_midori);//左上
        DrawRECT(&scene, QRect(0,bottom,m_pad_border,m_pad_border), usu_midori);//左下
        DrawRECT(&scene, QRect(right,0,m_pad_border,m_pad_border), usu_midori);//右上
        DrawRECT(&scene, QRect(right,bottom,m_pad_border,m_pad_border), usu_midori);//右下
    }

    //セル配置間隔
    if(ui->cb_pad_cell->checkState() == Qt::Checked)
    {
        int rc_SizeL = (m_width/devide)+(m_pad_inner*2);
        int posX_01 = m_pad_border;
        int posX_02 = m_pad_border +(rc_SizeL+m_pad_shape);
        int pos_y = m_pad_border+rc_SizeL;

        int amari = 0;
        if(m_pad_cell != 0)//ここで0で割った影響でPreviewできなかった
        {
            amari = (rc_SizeL+m_pad_shape)% m_pad_cell;
        }
        int add = 0;
        if (amari > 0)
        {
            add = m_pad_cell - amari;
            posX_02 += add;
        }
        QPen qp = QPen(QBrush(usu_kiiro),1.0f);
        scene.addLine(posX_01, pos_y+1, posX_02-1, pos_y+1, qp);//0含めた間隔なので1だけ調整
    }

    //セル間余白
    if(ui->cb_pad_shape->checkState() == Qt::Checked)
    {
        int rc_SizeL = (m_width/devide)+(2*m_pad_inner);//内部パディングを含む矩形の一辺
        int posS_x=(m_pad_border+rc_SizeL);
        int posE_x=(m_pad_border+rc_SizeL+m_pad_shape);
        int pos_y=(m_pad_border+rc_SizeL);

        QPen qp = QPen(QBrush(usu_ao),1.0f);
        scene.addLine(posS_x, pos_y+1, posE_x-1, pos_y+1, qp);//0含めた余白なので1だけ調整
    }

    //テクスチャ幅
    if(ui->cb_tex_width->checkState() == Qt::Checked)
    {
        QPen qp = QPen(QBrush(usu_aka),1.0f);
        scene.addLine(0,0,m_width-1,0,qp);//0含めた横幅なので1だけ調整
    }
    //テクスチャ高さ
    if(ui->cb_tex_height->checkState() == Qt::Checked)
    {
        QPen qp = QPen(QBrush(usu_ao),1.0f);
        scene.addLine(0,0,0,m_height-1,qp);//0含めた縦幅なので1だけ調整
    }

    //graphicsViewにsceneを設定
    ui->previewView->setMinimumSize(m_width,m_height);
    ui->previewView->setScene(&scene);
    //ウィンドウ表示
    this->show();
}

void subwindow::DrawCellAndPart(QGraphicsScene* sc, QColor cl_c, QColor cl_p)
{
    //矩形を描画
    auto DrawRECT = [](QGraphicsScene* sc, QRect rc, QColor cl)
    {
        QGraphicsRectItem *item = sc->addRect(rc);
        item->setBrush(QBrush(cl));
    };

    //矩形サイズ
    int rc_sizeL = (m_width/devide)+(2*m_pad_inner);
    int rc_sizeS = (m_width/devide);
    //Y座標
    int posY_L = m_pad_border;
    int posY_S = m_pad_border+m_pad_inner;

    //左側のBOX描画
    int posX_01L = m_pad_border;
    int posX_01S = m_pad_border + m_pad_inner;
    DrawRECT(&scene, QRect(posX_01L,posY_L,rc_sizeL,rc_sizeL), cl_c);
    DrawRECT(&scene, QRect(posX_01S,posY_S,rc_sizeS,rc_sizeS), cl_p);

    //右側のBOX描画
    int amari = 0;
    if(m_pad_cell != 0)//ここで0で割った影響でPreviewできなかった
    {
        amari = (rc_sizeL+m_pad_shape) % m_pad_cell;
    }
    int add = 0;
    //配置間隔で指定した数値の倍数になるように座標を調整する
    if (amari > 0)
    {
        add = m_pad_cell - amari;
    }
    int posX_02L = m_pad_border+(rc_sizeL+m_pad_shape)+add;
    int posX_02S = posX_02L + m_pad_inner;
    DrawRECT(&scene, QRect(posX_02L,posY_L,rc_sizeL,rc_sizeL), cl_c);
    DrawRECT(&scene, QRect(posX_02S,posY_S,rc_sizeS,rc_sizeS), cl_p);
}


void subwindow::on_cb_tex_width_stateChanged(int cb_state)
{
    DisplayPreview();//パラメータ設定後、再び描画処理を行わせる
}

void subwindow::on_cb_tex_height_stateChanged(int cb_state)
{
    DisplayPreview();//パラメータ設定後、再び描画処理を行わせる
}

void subwindow::on_cb_pad_shape_stateChanged(int cb_state)
{
    DisplayPreview();//パラメータ設定後、再び描画処理を行わせる
}

void subwindow::on_cb_pad_inner_stateChanged(int cb_state)
{
    DisplayPreview();//パラメータ設定後、再び描画処理を行わせる
}

void subwindow::on_cb_pad_cell_stateChanged(int cb_state)
{
    DisplayPreview();//パラメータ設定後、再び描画処理を行わせる
}

void subwindow::on_cb_pad_border_stateChanged(int cb_state)
{
    //QMessageBox msgBox(this);    msgBox.setText(QString::number(cb_state));    msgBox.exec();
    DisplayPreview();//パラメータ設定後、再び描画処理を行わせる
}

void subwindow::on_buttonAll_clicked()
{
    //チェックボックスの1つだけ調べる
    if(ui->cb_tex_height->checkState() == Qt::Checked)
    {
        ui->cb_tex_width ->setChecked(Qt::Unchecked);//全選択
        ui->cb_tex_height ->setChecked(Qt::Unchecked);
        ui->cb_pad_shape ->setChecked(Qt::Unchecked);
        ui->cb_pad_inner ->setChecked(Qt::Unchecked);
        ui->cb_pad_cell ->setChecked(Qt::Unchecked);
        ui->cb_pad_border ->setChecked(Qt::Unchecked);
    }else
    {
        ui->cb_tex_width ->setChecked(Qt::Checked);//全解除
        ui->cb_tex_height ->setChecked(Qt::Checked);
        ui->cb_pad_shape ->setChecked(Qt::Checked);
        ui->cb_pad_inner ->setChecked(Qt::Checked);
        ui->cb_pad_cell ->setChecked(Qt::Checked);
        ui->cb_pad_border ->setChecked(Qt::Checked);
    }
}

void subwindow::on_buttonEnd_clicked()
{
    this->close();
}

void subwindow::on_buttonDetail_clicked()
{
    //もっとパラメータについて教えて
    QUrl url = QUrl( "https://www.webtech.co.jp/help/ja/spritestudio/guide/tool/psdtoss6/#about_window" );
    QDesktopServices::openUrl( url );
}

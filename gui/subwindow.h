#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QWidget>
#include <QUrl>
#include <QMessageBox>
#include <QDesktopServices>
#include <QGraphicsScene>

namespace Ui
{
class subwindow;
}

class subwindow : public QWidget
{
    Q_OBJECT

public:
    explicit subwindow(QWidget *parent = nullptr);
    ~subwindow();

public:
    /// @note Previewウィンドウの生成
    void Preview(
            int,int,int,
            int,int,int,
            int,int,int);

private:
    /// @note パラメータの影響範囲を視覚化する
    void DisplayPreview();
    void DrawCellAndPart(QGraphicsScene* , QColor , QColor);

private slots:

    void on_cb_tex_width_stateChanged(int cb_state);
    void on_cb_tex_height_stateChanged(int cb_state);
    void on_cb_pad_shape_stateChanged(int cb_state);
    void on_cb_pad_inner_stateChanged(int cb_state);
    void on_cb_pad_cell_stateChanged(int cb_state);
    void on_cb_pad_border_stateChanged(int cb_state);

    void on_buttonAll_clicked();
    void on_buttonEnd_clicked();

    void on_buttonDetail_clicked();

private:
    Ui::subwindow *ui;
    QGraphicsScene scene;

    //色情報,PSDtoSS6の雰囲気に合わせてる
    QColor usu_murasaki = {135,81,151,255};//薄紫
    QColor usu_shiro = {240,239,239,255};//薄白
    QColor usu_ao = {93,187,234,255};//薄青
    QColor usu_midori = {144,200,153,255};//薄緑
    QColor usu_kiiro = {240,196,110};//薄黄
    QColor usu_aka = {227,100,129};//薄赤
    QColor bg_weak = {50,50,50,255};//薄いグレー(RGB=50)
    QColor bg_strong = {37,37,37,255};//濃いグレー(RGB=37)

    //各種設定, PSDtoSS6側で指定する
    int m_width;        //テクスチャ幅
    int m_height;       //テクスチャ高さ
    int m_priority;     //優先度設定
    int m_sort;         //セル配置順
    int m_pad_shape;    //セル間余白
    int m_pad_inner;    //セル内余白
    int m_pad_cell;     //セル配置間隔
    int m_pad_border;   //セルマップ縁幅
    int m_canvas;       //基準枠サイズ
};

#endif // SUBWINDOW_H

#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QTextCodec>
#include <QTranslator>

#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    //指定の位置に.qmを移動後に.qrcというファイルに記述する必要があった
    //これによりQtResourceSystemと紐づけられ下記から読み出せる
    QString translate_filepath = ":/translations";

    QTranslator *translator = new QTranslator();
    QMessageBox msg;
    if("ja_JP" == QLocale::system().name())
    {
        if(translator->load("translate_ja",translate_filepath))
        {
            a.installTranslator(translator);
            msg.setText("TRUE: " + translate_filepath);
            msg.exec();
        }
    }
    else
    {
        if(translator->load("translate_en",translate_filepath))
        {
            a.installTranslator(translator);
            msg.setText("TRUE: " + translate_filepath);
            msg.exec();
        }
    }

    if(translator->isEmpty())
    {
        msg.setText("FALSE: " + translate_filepath);
        msg.exec();
        return 1;
    }

    MainWindow w;
    w.setText_to_List(a.arguments());
    w.show();

    return a.exec();
}

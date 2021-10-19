#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QTextCodec>
#include <QTranslator>
#include <QMessageBox>

void MsgBox(QString message)
{
#if 0
    QMessageBox msg;
    msg.setText(message);
    msg.exec();
#endif
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    //指定の位置に.qmを移動後に.qrcというファイルに記述する必要があった
    //これによりQtResourceSystemと紐づけられ下記から読み出せる
    QString translate_filepath = ":/translations";

    QTranslator *translator = new QTranslator();
    
    //本来はQLocale::system().name()などを利用すべき
    if(translator->load("translate_ja",translate_filepath))
    {
        a.installTranslator(translator);
        MsgBox("TRUE: " + translate_filepath);
    }
    else if(translator->load("translate_en",translate_filepath))
    {
        a.installTranslator(translator);
        MsgBox("TRUE: " + translate_filepath);
    }   

    if(translator->isEmpty())
    {
        MsgBox("FALSE: " + translate_filepath);
        return 1;
    }

    MainWindow w;
    w.setText_to_List(a.arguments());
    w.show();

    return a.exec();
}
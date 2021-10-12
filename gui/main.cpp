#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QTextCodec>
#include <QTranslator>

#include <QDebug>

QTranslator translator;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    QString translate_filepath = QApplication::applicationDirPath();
    //qDebug() << "DIR: " << QApplication::applicationDirPath();//MacOS
    //qDebug() << "FILE: " << QApplication::applicationFilePath();//MacOS/PSDtoSSGUI

    // 翻訳
    if("ja_JP" == QLocale::system().name())
    {
        if(true == translator.load("translate_ja.qm", translate_filepath))
        {
            qDebug() << "JAPANESE";
        }
    }
    else
    {
        if(true == translator.load("translate_en.qm", translate_filepath))
        {
            qDebug() << "ENGLISH";
        }
    }

    if(true != translator.isEmpty())
    {
        //Qt5.9.9にはlanguageやfilePathはない
        qDebug() << "NOT EMPTY: isEmpty";
    }
    if(true == a.installTranslator(&translator))
    {
        qDebug() << "INSTALL: installTranslator";
    }

    MainWindow w;
    w.setText_to_List(a.arguments());
    w.show();

    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QTextCodec>
#include <QTranslator>

QTranslator translator;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	// 翻訳
	translator.load("translate_ja");
	a.installTranslator(&translator);

    w.setText_to_List(a.arguments());
    w.show();

    return a.exec();
}

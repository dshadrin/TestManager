#include "StdInc.h"
#include "TestManager.h"
#include <QApplication>
#include <QSettings>
#include <QMessageBox>

QString confName;

int main(int argc, char *argv[])
{
    int result = -1;

    try
    {
        fs::path pt(argv[0]);
        fs::path confPath(fs::absolute(pt).replace_extension("conf"));
        QString cPt = QString::fromStdString(fs::absolute(pt).remove_filename().string());
        confName = QString::fromStdString(confPath.string());
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, cPt);

        QApplication a(argc, argv);
        TestManager w;
        w.show();
        result = a.exec();
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(Q_NULLPTR, QString("Critical error"), QString::fromStdString(e.what()));
        result = -1;
    }
    return result;
}

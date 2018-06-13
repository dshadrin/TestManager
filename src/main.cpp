#include "StdInc.h"
#include "TestManager.h"
#include <QApplication>
#include <QSettings>
#include "DatabaseDef.h"

QString confName;
DBPtr DB;

int main(int argc, char *argv[])
{
    fs::path pt(argv[0]);
    fs::path confPath(fs::absolute(pt).replace_extension("conf"));
    QString cPt = QString::fromStdString(fs::absolute(pt).remove_filename().string());
    confName = QString::fromStdString(confPath.string());
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, cPt);

    DB = Database::open(EDBType::PostgreSQL);

    QApplication a(argc, argv);
    TestManager w;
    w.show();
    return a.exec();
}

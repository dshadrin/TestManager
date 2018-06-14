//////////////////////////////////////////////////////////////////////////
#include "Connection.h"
#include <QMessageBox>
#include <sstream>
#include <libpq-fe.h>
//////////////////////////////////////////////////////////////////////////

extern const QString URL_KEY = "/Database/Url";
extern const QString DB_KEY = "/Database/DbName";
extern const QString LOGIN_KEY = "/Database/Login";
extern const QString PWD_KEY = "/Database/Password";
extern const QString SQLITE_KEY = "/Database/SQLiteName";
extern QString confName;

//////////////////////////////////////////////////////////////////////////
Connection::Connection(QWidget *parent)
    : QDialog(parent)
    , settings(confName, QSettings::IniFormat, Q_NULLPTR)
{
    ui.setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_CustomWhatsThis);

    QObject::connect(ui.urlName, SIGNAL(textChanged(QString)), this, SLOT(urlEdit(QString)));
    QObject::connect(ui.dbName, SIGNAL(textChanged(QString)), this, SLOT(dbNameEdit(QString)));
    QObject::connect(ui.login, SIGNAL(textChanged(QString)), this, SLOT(loginEdit(QString)));
    QObject::connect(ui.password, SIGNAL(textChanged(QString)), this, SLOT(passEdit(QString)));
    QObject::connect(ui.sqliteName, SIGNAL(textChanged(QString)), this, SLOT(sqliteNameEdit(QString)));
    QObject::connect(ui.testConnButton, SIGNAL(clicked()), this, SLOT(testConnection()));

    readSettings();
}

//////////////////////////////////////////////////////////////////////////
void Connection::accept()
{
    settings.setValue(URL_KEY, url);
    settings.setValue(DB_KEY, dbName);
    settings.setValue(LOGIN_KEY, login);
    settings.setValue(PWD_KEY, pass);
    settings.setValue(SQLITE_KEY, sqliteName);
    QDialog::accept();
}

//////////////////////////////////////////////////////////////////////////
void Connection::urlEdit(const QString& str)
{
    url = str;
}

//////////////////////////////////////////////////////////////////////////
void Connection::dbNameEdit(const QString& str)
{
    dbName = str;
}

//////////////////////////////////////////////////////////////////////////
void Connection::loginEdit(const QString& str)
{
    login = str;
}

//////////////////////////////////////////////////////////////////////////
void Connection::passEdit(const QString& str)
{
    pass = str;
}

//////////////////////////////////////////////////////////////////////////
void Connection::sqliteNameEdit(const QString& str)
{
    sqliteName = str;
}

//////////////////////////////////////////////////////////////////////////
void Connection::testConnection()
{
    PGconn* connection = nullptr;

    try
    {
        std::ostringstream ss;
        ss << "user=" << login.toStdString() << " password =" << pass.toStdString() << " dbname=" << dbName.toStdString() << " host=" << url.toStdString();

        if (url.isEmpty() || dbName.isEmpty() || login.isEmpty() || pass.isEmpty())
        {
            throw std::runtime_error("Incorrect connection info: " + ss.str());
        }

        connection = PQconnectdb(ss.str().c_str());

        if (PQstatus(connection) == CONNECTION_BAD)
        {
            throw std::runtime_error(QString("Connection to database failed: %1").arg(PQerrorMessage(connection)).toStdString());
            close();
        }
        else
        {
            int ver = PQserverVersion(connection);
            QMessageBox::information(this, tr("Test connection"), QString("Server version: %1").arg(ver), QMessageBox::Ok);
        }
    }
    catch(std::exception& e)
    {
        QMessageBox::warning(this, tr("Test connection"), tr("Connection fail (%1)").arg(e.what()), QMessageBox::Ok);
    }

    if (!connection)
    {
        PQfinish(connection);
    }
}

//////////////////////////////////////////////////////////////////////////
void Connection::readSettings()
{
    url = settings.value(URL_KEY).toString();
    dbName = settings.value(DB_KEY).toString();
    login = settings.value(LOGIN_KEY).toString();
    pass = settings.value(PWD_KEY).toString();
    sqliteName = settings.value(SQLITE_KEY).toString();

    ui.urlName->setText(url);
    ui.dbName->setText(dbName);
    ui.login->setText(login);
    ui.password->setText(pass);
    ui.sqliteName->setText(sqliteName);
}

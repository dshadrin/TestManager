#include "DatabaseDef.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <libpq-fe.h>
#include <sstream>
#include <QSettings>
#include <QMessageBox>

//////////////////////////////////////////////////////////////////////////
extern QString confName;
extern const QString URL_KEY;
extern const QString DB_KEY;
extern const QString LOGIN_KEY;
extern const QString PWD_KEY;
extern const QString SQLITE_KEY;

//////////////////////////////////////////////////////////////////////////
// PostgreSQL implementation
//////////////////////////////////////////////////////////////////////////
class PostgreSQLdb : public Database
{
public:
    //////////////////////////////////////////////////////////////////////////
    PostgreSQLdb() : m_connection(nullptr)
    {
        QSettings settings(confName, QSettings::IniFormat, Q_NULLPTR);
        url = settings.value(URL_KEY).toString();
        dbName = settings.value(DB_KEY).toString();
        login = settings.value(LOGIN_KEY).toString();
        pass = settings.value(PWD_KEY).toString();

        open();
    }

    //////////////////////////////////////////////////////////////////////////
    virtual ~PostgreSQLdb()
    {
        close();
    }

    //////////////////////////////////////////////////////////////////////////
    virtual bool open() override
    {
        bool status = false;
        try
        {
            std::ostringstream ss;
            ss << "user=" << login.toStdString() << " password =" << pass.toStdString() << " dbname=" << dbName.toStdString() << " host=" << url.toStdString();

            if (url.isEmpty() || dbName.isEmpty() || login.isEmpty() || pass.isEmpty())
            {
                throw std::runtime_error("Incorrect connection info: " + ss.str());
            }

            m_connection = PQconnectdb(ss.str().c_str());

            if (PQstatus(m_connection) == CONNECTION_OK)
            {
                status = true;
            }
            else
            {
                throw std::runtime_error(QString("Connection to database failed: %1").arg(PQerrorMessage(m_connection)).toStdString());
            }
        }
        catch (std::exception& e)
        {
            close();
            QMessageBox::critical(Q_NULLPTR, QString("Test connection"), QString("Connection fail (%1)").arg(e.what()));
        }
        return status;
    }

    //////////////////////////////////////////////////////////////////////////
    void close()
    {
        if (m_connection)
        {
            PQfinish(m_connection);
            m_connection = nullptr;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    virtual EDbConnStatus status() override
    {
        EDbConnStatus status = EDbConnStatus::bad;
        if (m_connection != Q_NULLPTR)
        {
            status = (PQstatus(m_connection) == CONNECTION_OK) ? EDbConnStatus::ok : EDbConnStatus::bad;
        }
        return status;
    }

private:
    PGconn * m_connection;
    QSettings settings;
    QString url;
    QString dbName;
    QString login;
    QString pass;
};

//////////////////////////////////////////////////////////////////////////
// Sqlite3 implementation
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Common implementation
//////////////////////////////////////////////////////////////////////////

DBPtr Database::create(EDBType dbtype)
{
    switch (dbtype)
    {
    case EDBType::PostgreSQL:
        return DBPtr(new PostgreSQLdb());
    case EDBType::Sqlite3:
    default:
        return DBPtr();
    }
}

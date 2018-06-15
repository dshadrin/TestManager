#include "DatabaseDef.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <libpq-fe.h>
#include <sstream>
#include <QSettings>
#include <QMessageBox>
#include "CheckDbStructure.h"

//////////////////////////////////////////////////////////////////////////
extern QString confName;
extern const QString URL_KEY;
extern const QString DB_KEY;
extern const QString LOGIN_KEY;
extern const QString PWD_KEY;
extern const QString SQLITE_KEY;

//////////////////////////////////////////////////////////////////////////
namespace
{
    struct SQStingConnectionData
    {
        SQStingConnectionData()
        {
            QSettings settings(confName, QSettings::IniFormat, Q_NULLPTR);
            url = settings.value(URL_KEY).toString();
            dbName = settings.value(DB_KEY).toString();
            login = settings.value(LOGIN_KEY).toString();
            pass = settings.value(PWD_KEY).toString();
        }
        QString url;
        QString dbName;
        QString login;
        QString pass;
    };

    SConnectionData FillConnectionData()
    {
        SConnectionData data;
        memset(&data, 0, sizeof(data));

        SQStingConnectionData qData;
        QString target = QString("tcp:postgresql://%1:5432/%2").arg(qData.url).arg(qData.dbName);
        strcpy(&data.target[0], target.toStdString().c_str());
        strcpy(&data.user[0], qData.login.toStdString().c_str());
        strcpy(&data.password[0], qData.pass.toStdString().c_str());
        return data;
    }
}

//////////////////////////////////////////////////////////////////////////
// PostgreSQL implementation
//////////////////////////////////////////////////////////////////////////
class PostgreSQLdb : public Database
{
public:
    //////////////////////////////////////////////////////////////////////////
    PostgreSQLdb() : m_connection(nullptr)
    {
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
            SQStingConnectionData qData;

            std::ostringstream ss;
            ss << "user=" << qData.login.toStdString() << " password =" << qData.pass.toStdString() << " dbname=" << qData.dbName.toStdString() << " host=" << qData.url.toStdString();

            if (qData.url.isEmpty() || qData.dbName.isEmpty() || qData.login.isEmpty() || qData.pass.isEmpty())
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

    //////////////////////////////////////////////////////////////////////////
    virtual bool checkDbStructure() override
    {
        bool status = false;
        SConnectionData conn = FillConnectionData();

        int check = IsEntityExists(&conn, "S", "revision");
        check = IsEntityExists(&conn, "r", "version");
        check = IsEntityExists(&conn, "r", "version2");

        return status;
    }

private:
    PGconn * m_connection;
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

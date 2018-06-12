#include "DatabaseDef.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <libpq-fe.h>
#include <QDebug>
#include <sstream>

//////////////////////////////////////////////////////////////////////////
// PostgreSQL implementation
//////////////////////////////////////////////////////////////////////////
class PostgreSQLdb : public Database
{
public:
    PostgreSQLdb() : m_connection(nullptr)
    {
        bool status = open("etf", "etf");
        if (!status)
        {
            throw std::runtime_error("Cannot open PostgreSQL connection.");
        }
    }

    virtual ~PostgreSQLdb()
    {
        close();
    }

    bool open(const std::string& login, const std::string& pwd, const std::string& dbname = "etf", const std::string& url = "localhost")
    {
        std::ostringstream ss;
        ss << "user=" << login << " password =" << pwd << " dbname=" << dbname;
        m_connection = PQconnectdb(ss.str().c_str());

        if (PQstatus(m_connection) == CONNECTION_BAD) {

            qDebug() << QString("Connection to database failed: %1").arg(PQerrorMessage(m_connection));
            close();
            return false;
        }
        else
        {
            int ver = PQserverVersion(m_connection);
            qDebug() << QString("Server version: %1").arg(ver);
            return true;
        }
    }

    void close()
    {
        if (m_connection)
        {
            PQfinish(m_connection);
            m_connection = nullptr;
        }
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

DBPtr Database::open(EDBType dbtype)
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

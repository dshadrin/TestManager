#pragma once

#include <boost/phoenix.hpp>
//////////////////////////////////////////////////////////////////////////

enum class EDBType : uint8_t
{
    PostgreSQL,
    Sqlite3
};

class Database;
typedef std::shared_ptr<Database> DBPtr;

//////////////////////////////////////////////////////////////////////////
class Database
{
public:
    virtual ~Database() = default;

    static DBPtr open(EDBType dbtype);

};

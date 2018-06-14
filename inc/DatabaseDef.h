#pragma once

#include <boost/fusion/adapted/struct/define_struct.hpp>
#include <boost/fusion/include/define_struct.hpp>
#include <memory>

//////////////////////////////////////////////////////////////////////////

enum class EDBType : uint8_t
{
    PostgreSQL,
    Sqlite3
};

class Database;
typedef std::shared_ptr<Database> DBPtr;

//////////////////////////////////////////////////////////////////////////
BOOST_FUSION_DEFINE_STRUCT(
(schema), table_version,
(uint64_t, revision)
(std::string, user)
(std::string, description)
(bool, commited)
)

BOOST_FUSION_DEFINE_STRUCT(
(schema), table_test,
(uint64_t, id)
(uint64_t, revision)
(std::string, name)
)

//////////////////////////////////////////////////////////////////////////
class Database
{
public:
    enum class EDbConnStatus : uint8_t
    {
        ok,
        bad
    };

public:
    Database()
        : defaultTestRecord(schema::table_test(0, 0, ""))
        , defaultVersionRecord(schema::table_version(0, "", "", false))
    {}
    virtual ~Database() = default;

    virtual bool open() = 0;
    virtual EDbConnStatus status() = 0;
    virtual bool checkDbStructure() = 0;

    static DBPtr create(EDBType dbtype);

private:
    const schema::table_version defaultVersionRecord;
    const schema::table_test defaultTestRecord;
};

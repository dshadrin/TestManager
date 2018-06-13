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
    Database()
        : tTest(schema::table_test())
        , tVersion(schema::table_version())
    {}
    virtual ~Database() = default;

    static DBPtr open(EDBType dbtype);

private:
    schema::table_version tVersion;
    schema::table_test tTest;
};

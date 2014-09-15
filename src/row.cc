#include <sstream>
#include "row.hpp"

Row::Row(std::vector<sqlite3_value *> values)
{
    if(values.empty()) throw SQLiteException(_SQLITE_ROW_ERR);
    this->values = values;
}

Row::~Row()
{
    // delete this->values;
}

void
Row::invalidate()
{
    this->invalid = true;
}

void
Row::ensure_valid()
{
    if(this->invalid) throw SQLiteException(_SQLITE_INVALID_ROW);
}


std::string
Row::show()
{
    std::stringstream ss;
    ss << "Row(" << &this->values << ")";
    if(this->invalid) ss << "::invalid";
    return ss.str();
}

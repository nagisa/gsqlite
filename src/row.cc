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

std::string
Row::show()
{
    std::stringstream ss;
    ss << "Row(" << &this->values << ")";
    return ss.str();
}

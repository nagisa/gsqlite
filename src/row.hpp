#ifndef _GSQLITE_ROW
#define _GSQLITE_ROW

#include <vector>

#include "sqlite_common.hpp"

class Row : public Showable {
    public:
        Row(std::vector<sqlite3_value *>);
        ~Row();

        std::string show() override;
        // This API is still in brainstorming stage;
        // template<T> T get(int column);
    private:
        std::vector<sqlite3_value *> values;
};


#endif

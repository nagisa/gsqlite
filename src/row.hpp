#pragma once

#include <vector>

#include "sqlite_common.hpp"

class Row : public Showable {
    public:
        Row(std::vector<sqlite3_value *>);
        ~Row();

        // This API is still in brainstorming stage;
        // template<T> T get(int column);
        void invalidate();

        std::string show() override;
    private:
        std::vector<sqlite3_value *> values;

        bool invalid = false;
        void ensure_valid();
};

#include <iostream>
#include <map>
using namespace std;

#ifndef TABLETYPE_H
#define TABLETYPE_H

enum class TableType {
    POINTS,
    LINES,
    SURFACES
};

static map<TableType, string> tableTypesMap = {
        {TableType::POINTS, "points"},
        {TableType::LINES, "lines"},
        {TableType::SURFACES, "surfaces"}
};
#endif // TABLETYPE_H
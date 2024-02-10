#include <iostream>
#include <map>

using namespace std;

#ifndef TABLETYPE_H
#define TABLETYPE_H

enum class TableType {
    POINTS,
    LINES,
    SURFACES,
    CROSS_SECTIONS,
    MATERIALS
};

static map<TableType, string> tableTypesMap = {
        {TableType::POINTS, "points"},
        {TableType::LINES, "lines"},
        {TableType::SURFACES, "surfaces"},
        {TableType::CROSS_SECTIONS, "cross_sections"},
        {TableType::MATERIALS, "materials"}
};
#endif // TABLETYPE_H
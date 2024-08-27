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
    MATERIALS,
    SUPPORTS,
    NODAL_LOADS,
    LINE_LOADS,
    MESH,
    RESULTS,
    NODES,
    CIRCULAR_LINES,
    ALL
};

static map<TableType, string> tableTypesMap = {
    {TableType::POINTS, "points"},
    {TableType::LINES, "lines"},
    {TableType::SURFACES, "surfaces"},
    {TableType::CROSS_SECTIONS, "cross_sections"},
    {TableType::MATERIALS, "materials"},
    {TableType::SUPPORTS, "supports"},
    {TableType::NODAL_LOADS, "nodal_loads"},
    {TableType::LINE_LOADS, "line_loads"},
    {TableType::MESH, "mesh"},
    {TableType::RESULTS, "results"},
    {TableType::NODES, "nodes"},
    {TableType::CIRCULAR_LINES, "circular_lines"},
    {TableType::ALL, "all tables"}
};

#endif // TABLETYPE_H

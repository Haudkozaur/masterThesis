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
    LINE_SUPPORTS,
    SURFACE_SUPPORTS,
    SLAB_POINT_LOADS,
    SLAB_LINE_LOADS,
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
    {TableType::LINE_SUPPORTS, "line_supports"},
    {TableType::SURFACE_SUPPORTS, "surface_supports"},
    {TableType::SLAB_POINT_LOADS, "slab_point_loads"},
    {TableType::SLAB_LINE_LOADS, "slab_line_loads"},
    {TableType::ALL, "all tables"}
};

#endif // TABLETYPE_H

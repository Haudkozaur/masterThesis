#include "FreeFemExecutor.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>

// Constructor
FreeFemExecutor::FreeFemExecutor(const DataBaseManagers::DataBaseFreeFEMPreparer &dataPreparer)
    : dataPreparer(dataPreparer) {}

// Generate the FreeFem++ script based on the data
void FreeFemExecutor::generateFreeFemScript(const std::string &scriptPath) {
    std::ofstream script(scriptPath);

    if (!script.is_open()) {
        throw std::runtime_error("Unable to open script file for writing.");
    }

    // Write the FreeFEM++ script sections
    writeMaterialProperties(script);
    writeGeometry(script);  // This includes defining points, borders, and mesh
    writeFiniteElementSpace(script);  // Define w and v here
    writeLoads(script);  // Define loads here
    writeSolveSection(script);  // Solve and apply boundary conditions

    script.close();
}


// Define the finite element space
void FreeFemExecutor::writeFiniteElementSpace(std::ofstream &script) {
    script << "\n// Finite Element Space\n";
    script << "fespace Vh(Th, P2); // P2 finite element space on the mesh\n";
    script << "Vh w, v; // w is the displacement in the z direction, v is the test function\n\n";
}

// Solve section, integrating boundary conditions dynamically
void FreeFemExecutor::writeSolveSection(std::ofstream &script) {
    const auto &lineSupports = dataPreparer.getLineSupports();

    script << "\n// Solve the problem\n";
    script << "solve plate(w, v) = int2d(Th)(D * (dx(w) * dx(v) + dy(w) * dy(v)))\n";
    script << "    - int2d(Th)(surfaceLoadMask * qSurface * v)\n";

    // Dynamically add boundary conditions based on lineSupports map
    for (const auto &support : lineSupports) {
        int lineId = std::get<0>(support.second);
        script << "    + on(line" << lineId << ", w = 0)\n";
    }

    script << ";\n";
    script << "plot(w, wait = 1, fill = 1, value = 1, cmm = \"Deflection of the slab\", nbiso = 10);\n";
}

// Write material properties
void FreeFemExecutor::writeMaterialProperties(std::ofstream &script) {
    const auto &materials = dataPreparer.getMaterials();
    const auto &mainSurface = dataPreparer.getMainSurface();

    if (mainSurface.empty()) {
        throw std::runtime_error("No main surface defined in the model.");
    }

    // Assuming there's only one main surface. Adjust if multiple surfaces are possible.
    const auto &surface = mainSurface.begin()->second;
    int materialId = std::get<5>(surface);  // Accessing materialId (now an int)
    double thickness = std::get<7>(surface) / 1000.0;  // Convert thickness from mm to m

    // Find the material properties using the materialId
    auto materialIt = materials.find(materialId);
    if (materialIt == materials.end()) {
        throw std::runtime_error("Material ID not found in materials map.");
    }

    const std::string &name = std::get<0>(materialIt->second);
    double E = std::get<1>(materialIt->second);
    double nu = std::get<2>(materialIt->second);

    script << "// Material properties for " << name << "\n";
    script << "real h = " << thickness << ";\n";  // Define thickness before using it
    script << "real E = " << E << ";\n";
    script << "real nu = " << nu << ";\n";
    script << "real D = E * h^3 / (12 * (1 - nu^2));\n\n";
}

// Write geometry based on points, lines, and surfaces
// Write geometry based on points, lines, and surfaces
// Write geometry based on points, lines, and surfaces
void FreeFemExecutor::writeGeometry(std::ofstream &script) {
    const auto &points = dataPreparer.getPoints();

    // Writing points
    script << "// Geometry: Points\n";
    for (const auto &point : points) {
        script << "real p" << point.first << "x = " << point.second.first / 1000.0 << ";\n";  // Convert from mm to m
        script << "real p" << point.first << "z = " << point.second.second / 1000.0 << ";\n"; // Convert from mm to m
    }
    script << "\n";

    // Writing lines and circular lines (just the borders, no mesh)
    writeMeshBorders(script);

    // Now, write the mesh section directly here
    writeMesh(script);
}


// Write mesh generation script section (the logic for mesh generation only)
void FreeFemExecutor::writeMesh(std::ofstream &script) {
    const auto &lines = dataPreparer.getLines();
    const auto &circularLines = dataPreparer.getCircularLines();
    const auto &meshMap = dataPreparer.getMesh();
    const auto &openings = dataPreparer.getOpenings();

    script << "\n// Geometry: Surfaces\n";
    script << "mesh Th = buildmesh(\n";

    bool isFirst = true;

    // Adding lines to the mesh
    for (const auto &line : lines) {
        int lineId = line.first;
        if (lineId == -1) continue; // Skip invalid lines

        int numberOfFE = 20;
        auto meshIt = meshMap.find(lineId);
        if (meshIt != meshMap.end()) {
            numberOfFE = std::get<1>(meshIt->second);
        }

        // Check if this line is part of an opening
        bool isOpeningLine = false;
        for (const auto &opening : openings) {
            if (lineId == std::get<0>(opening.second) ||
                lineId == std::get<1>(opening.second) ||
                lineId == std::get<2>(opening.second) ||
                lineId == std::get<3>(opening.second)) {
                isOpeningLine = true;
                break;
            }
        }

        // If it's not the first line, add a '+' between the lines
        if (!isFirst) {
            script << " + ";
        }
        isFirst = false;

        // Add the line with a negative or positive FE number based on whether it's an opening
        if (isOpeningLine) {
            script << "line" << lineId << "(-" << numberOfFE << ")";
        } else {
            script << "line" << lineId << "(" << numberOfFE << ")";
        }
    }

    // Adding circular lines to the mesh
    for (const auto &circularLine : circularLines) {
        int circularLineId = circularLine.first;
        if (circularLineId == -1) continue; // Skip invalid circular lines

        int numberOfFE = 20;
        auto meshIt = meshMap.find(circularLineId);
        if (meshIt != meshMap.end()) {
            numberOfFE = std::get<1>(meshIt->second);
        }

        // Check if this circular line is part of an opening
        bool isOpeningCircularLine = false;
        for (const auto &opening : openings) {
            if (circularLineId == std::get<4>(opening.second)) {
                isOpeningCircularLine = true;
                break;
            }
        }

        // If there were previous lines, add a '+' between the lines
        if (!isFirst) {
            script << " + ";
        }
        isFirst = false;

        // Add the circular line with a negative or positive FE number based on whether it's an opening
        if (isOpeningCircularLine) {
            script << "circularLine" << circularLineId << "(-" << numberOfFE << ")";
        } else {
            script << "circularLine" << circularLineId << "(" << numberOfFE << ")";
        }
    }

    script << "\n);\n"; // Close the buildmesh command
}



// Helper method to write the borders (lines and circular lines)
void FreeFemExecutor::writeMeshBorders(std::ofstream &script) {
    const auto &lines = dataPreparer.getLines();
    const auto &circularLines = dataPreparer.getCircularLines();
    const auto &meshMap = dataPreparer.getMesh();
    const auto &openings = dataPreparer.getOpenings();

    script << "\n// Geometry: Lines\n";

    // Create a vector of lines and reverse the order for the main surface
    std::vector<std::pair<int, std::tuple<int, int, double, double>>> reversedLines(lines.begin(), lines.end());
    std::reverse(reversedLines.begin(), reversedLines.end());

    // Iterate over the reversed lines vector and write them to the script
    for (const auto &line : reversedLines) {
        int lineId = line.first;
        if (lineId == -1) continue; // Skip invalid lines

        int startPoint = std::get<0>(line.second);
        int endPoint = std::get<1>(line.second);
        int numberOfFE = 20;

        auto meshIt = meshMap.find(lineId);
        if (meshIt != meshMap.end()) {
            numberOfFE = std::get<1>(meshIt->second);
        }

        // Check if this line is part of an opening
        bool isOpeningLine = false;
        for (const auto &opening : openings) {
            if (lineId == std::get<0>(opening.second) ||
                lineId == std::get<1>(opening.second) ||
                lineId == std::get<2>(opening.second) ||
                lineId == std::get<3>(opening.second)) {
                isOpeningLine = true;
                break;
            }
        }

        if (isOpeningLine) {
            script << "border line" << lineId << "(t=0, 1) { x = p" << endPoint << "x * (1-t) + p" << startPoint << "x * t; "
                   << "y = p" << endPoint << "z * (1-t) + p" << startPoint << "z * t; }(-" << numberOfFE << ");\n";
        } else {
            script << "border line" << lineId << "(t=0, 1) { x = p" << endPoint << "x * (1-t) + p" << startPoint << "x * t; "
                   << "y = p" << endPoint << "z * (1-t) + p" << startPoint << "z * t; }(" << numberOfFE << ");\n";
        }
    }

    script << "\n// Geometry: Circular Lines\n";

    for (const auto &circularLine : circularLines) {
        int circularLineId = circularLine.first;
        if (circularLineId == -1) continue; // Skip invalid circular lines

        int centerX = std::get<0>(circularLine.second) / 1000.0;
        int centerZ = std::get<1>(circularLine.second) / 1000.0;
        int diameter = std::get<2>(circularLine.second) / 1000.0;
        int numberOfFE = 20;

        auto meshIt = meshMap.find(circularLineId);
        if (meshIt != meshMap.end()) {
            numberOfFE = std::get<1>(meshIt->second);
        }

        // Check if this circular line is part of an opening
        bool isOpeningCircularLine = false;
        for (const auto &opening : openings) {
            if (circularLineId == std::get<4>(opening.second)) {
                isOpeningCircularLine = true;
                break;
            }
        }

        if (isOpeningCircularLine) {
            script << "border circularLine" << circularLineId << "(t=0, 2*pi) { x = " << centerX << " + " << diameter / 2
                   << "*cos(t); y = " << centerZ << " + " << diameter / 2 << "*sin(t); }(-" << numberOfFE << ");\n";
        } else {
            script << "border circularLine" << circularLineId << "(t=0, 2*pi) { x = " << centerX << " + " << diameter / 2
                   << "*cos(t); y = " << centerZ << " + " << diameter / 2 << "*sin(t); }(" << numberOfFE << ");\n";
        }
    }
}


// Write loads script section
void FreeFemExecutor::writeLoads(std::ofstream &script) {
    const auto &surfaceLoads = dataPreparer.getSurfaceLoads();

    script << "// Loads\n";

    for (const auto &load : surfaceLoads) {
        int x1 = std::get<0>(load.second) / 1000.0;  // Convert from mm to m
        int z1 = std::get<1>(load.second) / 1000.0;  // Convert from mm to m
        int x2 = std::get<2>(load.second) / 1000.0;  // Convert from mm to m
        int z2 = std::get<3>(load.second) / 1000.0;  // Convert from mm to m
        double F = std::get<4>(load.second);
        script << "real qSurface = " << F << ";\n";
        script << "func surfaceLoadMask = (x >= " << x1 << " && x <= " << x2 << " && y >= " << z1 << " && y <= " << z2 << ");\n";
    }
}

// Execute the FreeFem++ script
void FreeFemExecutor::executeFreeFemScript(const std::string &scriptPath) {
    std::string command = "FreeFem++ " + scriptPath;
    if (system(command.c_str()) != 0) {
        throw std::runtime_error("Error executing FreeFem++ script.");
    }
}

void FreeFemExecutor::runAnalysisAndDisplayResults() {
    generateFreeFemScript("analysis.edp");
    executeFreeFemScript("analysis.edp");
}

void FreeFemExecutor::displayMeshedGeometry() {
    generateFreeFemScript("meshDisplay.edp");

    std::ofstream script("meshDisplay.edp", std::ios_base::app); // Open in append mode
    script << "plot(Th, wait = 1, cmm = \"Meshed Geometry of the Slab\");\n";
    script.close();

    executeFreeFemScript("meshDisplay.edp");
}

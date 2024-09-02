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
    writeGeometry(script);  // This includes defining points, borders, and surfaces
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
void FreeFemExecutor::writeGeometry(std::ofstream &script) {
    const auto &points = dataPreparer.getPoints();
    const auto &lines = dataPreparer.getLines();
    const auto &mainSurface = dataPreparer.getMainSurface();
    const auto &openings = dataPreparer.getOpenings();
    const auto &circularLines = dataPreparer.getCircularLines();
    const auto &meshMap = dataPreparer.getMesh();

    // Writing points
    script << "// Geometry: Points\n";
    for (const auto &point : points) {
        script << "real p" << point.first << "x = " << point.second.first / 1000.0 << ";\n";  // Convert from mm to m
        script << "real p" << point.first << "z = " << point.second.second / 1000.0 << ";\n"; // Convert from mm to m
    }
    script << "\n";

    // Writing lines and circular lines
    writeMesh(script);

    // Writing surfaces
    script << "// Geometry: Surfaces\n";
    script << "mesh Th = buildmesh(\n";

    // Add each line with its number of finite elements
    for (const auto &line : lines) {
        int lineId = line.first;
        auto meshIt = meshMap.find(lineId);
        int numberOfFE = (meshIt != meshMap.end()) ? std::get<1>(meshIt->second) : 20;  // Default value 20
        script << "line" << lineId << "(" << numberOfFE << ") + ";
    }

    // Add each circular line with its number of finite elements
    for (const auto &circularLine : circularLines) {
        int circularLineId = circularLine.first;
        auto meshIt = meshMap.find(circularLineId);
        int numberOfFE = (meshIt != meshMap.end()) ? std::get<1>(meshIt->second) : 20;  // Default value 20
        script << "circularLine" << circularLineId << "(" << numberOfFE << ") - ";
    }

    // Remove the last ' + ' or ' - '
    script.seekp(-3, script.cur);
    script << ");\n";

    script << "\n";
}

// Write mesh generation script section
void FreeFemExecutor::writeMesh(std::ofstream &script) {
    const auto &lines = dataPreparer.getLines();
    const auto &circularLines = dataPreparer.getCircularLines();
    const auto &meshMap = dataPreparer.getMesh();

    script << "\n// Geometry: Lines\n";
    for (const auto &line : lines) {
        int startPoint = std::get<0>(line.second);
        int endPoint = std::get<1>(line.second);
        int lineId = line.first;

        // Find the number of finite elements for this line from the meshMap
        int numberOfFE = 20;  // Default value
        auto meshIt = meshMap.find(lineId);
        if (meshIt != meshMap.end()) {
            numberOfFE = std::get<1>(meshIt->second);
        }

        // Define the border without specifying the number of elements again in buildmesh
        script << "border line" << lineId << "(t=0, 1) { x = p" << startPoint << "x * (1-t) + p" << endPoint << "x * t; "
               << "y = p" << startPoint << "z * (1-t) + p" << endPoint << "z * t; }(" << numberOfFE << ");\n";
    }

    script << "\n// Geometry: Circular Lines\n";
    for (const auto &circularLine : circularLines) {
        int centerX = std::get<0>(circularLine.second) / 1000.0;  // Convert from mm to m
        int centerZ = std::get<1>(circularLine.second) / 1000.0;  // Convert from mm to m
        int diameter = std::get<2>(circularLine.second) / 1000.0;  // Convert from mm to m
        int circularLineId = circularLine.first;

        // Find the number of finite elements for this circular line
        int numberOfFE = 20;  // Default value
        auto meshIt = meshMap.find(circularLineId);
        if (meshIt != meshMap.end()) {
            numberOfFE = std::get<1>(meshIt->second);
        }

        script << "border circularLine" << circularLineId << "(t=0, 2*pi) { x = " << centerX << " + " << diameter / 2
               << "*cos(t); y = " << centerZ << " + " << diameter / 2 << "*sin(t); }(" << numberOfFE << ");\n";
    }

    script << "\n";
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

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
    //writeLoads(script);  // Define loads here
    defineLineLoads(script);
    defineSurfaceLoads(script);
    definePointLoads(script);
    defineSurfaceSupports(script);
    writeSolveSection(script);  // Solve and apply boundary conditions

    script.close();
}


// Define the finite element space
void FreeFemExecutor::writeFiniteElementSpace(std::ofstream &script) {
    script << "\n// Finite Element Space\n";
    script << "fespace Vh(Th, P2); // P2 finite element space on the mesh\n";
    script << "Vh w, v; // w is the displacement in the z direction, v is the test function\n\n";
}


// Definiowanie zmiennych dla sił punktowych przed solve
// Definiowanie zmiennych dla sił punktowych przed solve
// Definiowanie zmiennych dla sił punktowych i ich współrzędnych przed solve
void FreeFemExecutor::definePointLoads(std::ofstream &script) {
    const auto &pointLoads = dataPreparer.getSlabPointLoads();
    int pointLoadIndex = 1;  // Number each point load

    script << "// Point Loads\n";
    for (const auto &load : pointLoads) {
        double x1 = std::get<0>(load.second) / 1000.0;  // Convert mm to meters
        double z1 = std::get<1>(load.second) / 1000.0;  // Convert mm to meters
        double Fz = std::get<2>(load.second) * 1000.0;  // Convert kN to N

        // Define point load variables and mask for small area
        script << "real qPoint" << pointLoadIndex << " = " << Fz << ";\n";
        script << "real xLoad" << pointLoadIndex << " = " << x1 << ";\n";
        script << "real yLoad" << pointLoadIndex << " = " << z1 << ";\n";

        // Define the load mask
        script << "func pointLoadMask" << pointLoadIndex << " = (abs(x - xLoad" << pointLoadIndex << ") < h/2) * (abs(y - yLoad" << pointLoadIndex << ") < h/2);\n";
        // Convert point load into equivalent surface load
        script << "real surfaceLoad" << pointLoadIndex << " = qPoint" << pointLoadIndex << " / ((h/2) * (h/2));\n";

        pointLoadIndex++;
    }
}

void FreeFemExecutor::applyPointLoads(std::ofstream &script) {
    const auto &pointLoads = dataPreparer.getSlabPointLoads();
    int pointLoadIndex = 1;  // Number each point load

    for (const auto &load : pointLoads) {
        // Apply the surface load equivalent to the point load
        script << "    - int2d(Th)((surfaceLoad" << pointLoadIndex << " * v) * pointLoadMask" << pointLoadIndex << ")\n";
        pointLoadIndex++;
    }
}
void FreeFemExecutor::defineLineLoads(std::ofstream &script) {
    const auto &lineLoads = dataPreparer.getSlabLineLoads();
    int lineLoadIndex = 1;  // Number each line load

    script << "// Line Loads\n";
    for (const auto &load : lineLoads) {
        double x1 = std::get<0>(load.second) / 1000.0;  // Convert mm to meters
        double z1 = std::get<1>(load.second) / 1000.0;  // Convert mm to meters
        double x2 = std::get<2>(load.second) / 1000.0;  // Convert mm to meters
        double z2 = std::get<3>(load.second) / 1000.0;  // Convert mm to meters
        double Fz = std::get<4>(load.second) * 1000.0;  // Force in N/m

        // Define line load variables and the load mask for inclined lines
        script << "real qLine" << lineLoadIndex << " = " << Fz << " / (h / 2);\n";  // Convert to N/m²

        // Calculate the length of the inclined line
        script << "real length" << lineLoadIndex << " = sqrt((" << x2 << " - " << x1
               << ")^2 + (" << z2 << " - " << z1 << ")^2);\n";

        // Define the mask for the inclined line load
        script << "func lineLoadMask" << lineLoadIndex << " = (abs((" << z2 << " - " << z1
               << ") * x - (" << x2 << " - " << x1
               << ") * y + " << x2 << " * " << z1
               << " - " << z2 << " * " << x1
               << ") / length" << lineLoadIndex << " < h / 2);\n";

        lineLoadIndex++;
    }
}


void FreeFemExecutor::applyLineLoads(std::ofstream &script) {
    const auto &lineLoads = dataPreparer.getSlabLineLoads();
    int lineLoadIndex = 1;  // Number each line load

    for (const auto &load : lineLoads) {
        // Apply the line load as a surface load over a narrow band
        script << "    - int2d(Th)((qLine" << lineLoadIndex << " * v) * lineLoadMask" << lineLoadIndex << ")\n";
        lineLoadIndex++;
    }
}



// Główna metoda writeSolveSection, gdzie najpierw definiujemy, a potem aplikujemy siły punktowe
// Define the penalty value and surface support regions
void FreeFemExecutor::defineSurfaceSupports(std::ofstream &script) {
    const auto &surfaceSupports = dataPreparer.getSurfaceSupports();
    script << "// Surface Supports\n";

    // Set a large penalty value to enforce boundary conditions
    script << "real penalty = 1e20;\n";

    int supportIndex = 1;
    for (const auto &support : surfaceSupports) {
        double x1 = std::get<0>(support.second) / 1000.0;  // Convert mm to meters
        double z1 = std::get<1>(support.second) / 1000.0;
        double x2 = std::get<2>(support.second) / 1000.0;
        double z2 = std::get<3>(support.second) / 1000.0;

        // Define characteristic function for the supported area
        script << "func supportMask" << supportIndex << " = (x >= " << x1 << " && x <= " << x2
               << ") * (y >= " << z1 << " && y <= " << z2 << ");\n";
        supportIndex++;
    }
}

// Apply the penalty for surface supports in the solve section
void FreeFemExecutor::applySurfaceSupports(std::ofstream &script) {
    const auto &surfaceSupports = dataPreparer.getSurfaceSupports();

    int supportIndex = 1;
    for (const auto &support : surfaceSupports) {
        // Apply the penalty over the supported areas
        script << "    + int2d(Th)(penalty * supportMask" << supportIndex << " * w * v)\n";
        supportIndex++;
    }
}

// Update the solve section to include surface supports
void FreeFemExecutor::writeSolveSection(std::ofstream &script) {
    const auto &lineSupports = dataPreparer.getLineSupports();
    const auto &surfaceLoads = dataPreparer.getSurfaceLoads();

    // Define variables for line, surface, point loads, and surface supports


    script << "\n// Solve the problem\n";
    script << "solve plate(w, v) = int2d(Th)(D * (dx(w) * dx(v) + dy(w) * dy(v)))\n";

    // Apply line loads
    applyLineLoads(script);

    // Apply surface loads
    if (!surfaceLoads.empty()) {
        applySurfaceLoads(script);
    }

    // Apply point loads
    applyPointLoads(script);

    // Apply surface supports
    applySurfaceSupports(script);

    // Apply boundary conditions for both line and circular line supports
    for (const auto &support : lineSupports) {
        int lineId = std::get<0>(support.second);  // Line ID
        int circularLineId = std::get<1>(support.second);  // Circular Line ID

        // Apply condition on line supports
        if (lineId != -1) {
            script << "    + on(line" << lineId << ", w = 0)\n";
        }

        // Apply condition on circular line supports
        if (circularLineId != -1) {
            script << "    + on(circularLine" << circularLineId << ", w = 0)\n";
        }
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
        // Search for lineId in meshMap (manual iteration)
        for (const auto& meshEntry : meshMap) {
            if (std::get<0>(meshEntry.second) == lineId) {
                numberOfFE = std::get<1>(meshEntry.second);
                break;
            }
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
        // Search for circularLineId in meshMap (manual iteration)
        for (const auto& meshEntry : meshMap) {
            if (std::get<0>(meshEntry.second) == circularLineId) {
                numberOfFE = std::get<1>(meshEntry.second);
                break;
            }
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

        // Search for lineId in meshMap (manual iteration)
        for (const auto& meshEntry : meshMap) {
            if (std::get<0>(meshEntry.second) == lineId) {
                numberOfFE = std::get<1>(meshEntry.second);
                break;
            }
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

        double centerX = std::get<0>(circularLine.second) / 1000.0;
        double centerZ = std::get<1>(circularLine.second) / 1000.0;
        double diameter = std::get<2>(circularLine.second) / 1000.0;
        int numberOfFE = 20;

        // Search for circularLineId in meshMap (manual iteration)
        for (const auto& meshEntry : meshMap) {
            if (std::get<0>(meshEntry.second) == circularLineId) {
                numberOfFE = std::get<1>(meshEntry.second);
                break;
            }
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
void FreeFemExecutor::defineSurfaceLoads(std::ofstream &script) {
    const auto &surfaceLoads = dataPreparer.getSurfaceLoads();
    int surfaceLoadIndex = 1;  // Numbering each surface load

    script << "// Surface Loads\n";
    for (const auto &load : surfaceLoads) {
        double x1 = std::get<0>(load.second) / 1000.0;  // Convert mm to meters
        double z1 = std::get<1>(load.second) / 1000.0;  // Convert mm to meters
        double x2 = std::get<2>(load.second) / 1000.0;  // Convert mm to meters
        double z2 = std::get<3>(load.second) / 1000.0;  // Convert mm to meters
        double F = std::get<4>(load.second);  // Force value

        // Define variables for each surface load
        script << "real qSurface" << surfaceLoadIndex << " = " << F << ";\n";
        script << "func surfaceLoadMask" << surfaceLoadIndex << " = (x >= " << x1 << " && x <= " << x2 << " && y >= " << z1 << " && y <= " << z2 << ");\n";
        surfaceLoadIndex++;
    }
}

void FreeFemExecutor::applySurfaceLoads(std::ofstream &script) {
    const auto &surfaceLoads = dataPreparer.getSurfaceLoads();
    int surfaceLoadIndex = 1;  // Numbering each surface load

    for (const auto &load : surfaceLoads) {
        // Apply each surface load with its mask
        script << "    - int2d(Th)(surfaceLoadMask" << surfaceLoadIndex << " * qSurface" << surfaceLoadIndex << " * v)\n";
        surfaceLoadIndex++;
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

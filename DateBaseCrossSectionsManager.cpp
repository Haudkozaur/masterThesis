
#include "DataBaseCrossSectionsManager.h"

DataBaseCrossSectionsManager::DataBaseCrossSectionsManager(const string &dateBaseName) : DataBaseModelObjectsManager(dateBaseName) {

}
void DataBaseCrossSectionsManager::addObjectToDataBase(const string &name, int materialID, double A, double I) {
    string queryAddCrossSection = "INSERT INTO cross_sections (name, material_id, A, I) VALUES ('" + name + "', " + to_string(materialID) + ", " + to_string(A) + ", " + to_string(I) + ");";

    if (validate(materialID, TableType::MATERIALS)) {
        cout << "Material exists" << endl;
        int rc = sqlite3_exec(this->DB, queryAddCrossSection.c_str(), nullptr, nullptr, &this->zErrMsg);
        if (rc != SQLITE_OK) {
            cout << "Error: " << zErrMsg << endl;
        } else {
            cout << "Cross section added successfully" << endl;
        }
    } else {
        cout << "Error: " << "Material doesn't exist in DB" << endl;
    }
    cout << "\n";
}

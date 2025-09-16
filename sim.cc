#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "PMDetectorConstruction.hh"
#include "PMPhysicsList.hh"
#include "PMActionInitialization.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"

int main(int argc, char** argv) {
    G4String macroFile = (argc > 1) ? argv[1] : "";

    G4UIExecutive* ui = nullptr;
    if (macroFile.empty()) { 
        ui = new G4UIExecutive(argc, argv); 
    }

    G4RunManager* runManager = new G4RunManager;
    
    if (G4Threading::IsMultithreadedApplication()) {
        runManager->SetNumberOfThreads(4);
    }

    G4double energy = 5 * MeV;  
    runManager->SetUserInitialization(new PMDetectorConstruction());
    runManager->SetUserInitialization(new PMPhysicsList());
    runManager->SetUserInitialization(new PMActionInitialization(energy));
    
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    if (!macroFile.empty()) {
        UImanager->ApplyCommand("/control/execute " + macroFile);
    } else {
        UImanager->ApplyCommand("/control/macroPath ./macros");
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
    return 0;
}

#include "PMActionInitialization.hh"
#include "PMPrimaryGenerator.hh"
#include "PMRunAction.hh"
#include "G4SystemOfUnits.hh"  

PMActionInitialization::PMActionInitialization(G4double energy)
    : G4VUserActionInitialization(), fEnergy(energy) {}  

PMActionInitialization::~PMActionInitialization() = default;

void PMActionInitialization::BuildForMaster() const {
    SetUserAction(new PMRunAction(fEnergy));  
}

void PMActionInitialization::Build() const {
    SetUserAction(new PMPrimaryGenerator(fEnergy)); 
    SetUserAction(new PMRunAction(fEnergy));    
}

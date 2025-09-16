#ifndef PMACTIONINITIALIZATION_HH
#define PMACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "G4SystemOfUnits.hh"  
#include "PMPrimaryGenerator.hh"
#include "PMRunAction.hh"

class PMActionInitialization : public G4VUserActionInitialization {
public:
    explicit PMActionInitialization(G4double energy);  
    ~PMActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;

private:
    G4double fEnergy;
};

#endif  // PMACTIONINITIALIZATION_HH

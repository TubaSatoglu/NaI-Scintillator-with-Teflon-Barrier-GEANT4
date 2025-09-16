#ifndef PMPRIMARYGENERATOR_HH
#define PMPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

class G4Event;

class PMPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
    explicit PMPrimaryGenerator(G4double energy); 
    virtual ~PMPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event* anEvent);
    
    void SetGammaEnergy(G4double energy);
    void SetSourceDistance(G4double distance);
    void SetSourceRadius(G4double radius);
    void SetBeamSpreadAngle(G4double angle);
    void SetEnergySigma(G4double sigma);

    G4double GetSourceDistance() const;
    G4double GetSourceRadius() const;
    G4double GetBeamSpreadAngle() const;
    G4double GetEnergySigma() const;

private:
    void UpdatePositionAndDirection();

    G4ParticleGun* fParticleGun;
    G4double fBaseEnergy;
    G4double fEnergySigma;
    G4double fSourcePosZ;
    G4double fSourceRadius;
    G4double fBeamSpreadAngle;
};

#endif  // PMPRIMARYGENERATOR_HH

#include "PMSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "PMEventAction.hh"
#include "G4ParticleDefinition.hh"
#include "PMDetectorConstruction.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

PMSensitiveDetector::PMSensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name),
      fTotalOpticalPhotons(0),
      fPhotonsAtScintillator(0),
      fPhotonsAtAluminum(0) {}   

PMSensitiveDetector::~PMSensitiveDetector() {}

void PMSensitiveDetector::Initialize(G4HCofThisEvent*) {
    fTotalOpticalPhotons = 0;
    fPhotonsAtScintillator = 0;
    fPhotonsAtAluminum = 0;
}

G4bool PMSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    G4Track* track = step->GetTrack();

    G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    G4int trackID = track->GetTrackID();
    G4String particleName = track->GetDefinition()->GetParticleName();
    G4double kineticEnergy = step->GetPreStepPoint()->GetKineticEnergy();
    G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
    G4double energyDeposited = step->GetTotalEnergyDeposit();
    G4String volName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    
    if (particleName != "opticalphoton") {
        G4cout << "🔍 Non-optical Particle = " << particleName << G4endl;
    }

    if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
        fTotalOpticalPhotons++;

        G4String volName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();

        if (volName == "ScintillatorPhys") {
            fPhotonsAtScintillator++;
        }

        if (volName == "AluminumPlate" || volName == "AluminumSurface" || volName == "AluminumPhys") {
            fPhotonsAtAluminum++;
            G4cout << "⚡ Optical Photon Detected at Aluminum (SensitiveDetector)! Count: " 
                   << fPhotonsAtAluminum << G4endl;

            PMEventAction* eventAction = static_cast<PMEventAction*>(
                G4EventManager::GetEventManager()->GetUserEventAction());
            if (eventAction) {
                eventAction->AddAluminumPhoton();
            }
        }
    }
    return true;
}

void PMSensitiveDetector::EndOfEvent(G4HCofThisEvent*) {
    G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

    G4cout << "\n======= Event Summary =======" << G4endl;
    G4cout << "🆔 Event ID: " << eventID << G4endl;
    G4cout << "🔹 Total Optical Photons Created: " << fTotalOpticalPhotons << G4endl;
    G4cout << "🔹 Optical Photons Created in Scintillator: " << fPhotonsAtScintillator << G4endl;
    G4cout << "🔹 Optical Photons Detected at Aluminum: " << fPhotonsAtAluminum << G4endl;
    G4cout << "=====================================\n" << G4endl;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if (analysisManager) {
        analysisManager->FillH1(0, fTotalOpticalPhotons);
        analysisManager->FillH1(1, fPhotonsAtScintillator);
        analysisManager->FillH1(2, fPhotonsAtAluminum);
    }
}

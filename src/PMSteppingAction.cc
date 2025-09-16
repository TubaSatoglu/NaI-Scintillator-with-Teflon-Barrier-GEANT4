#include "PMSteppingAction.hh"
#include "PMEventAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VProcess.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4PhysicalConstants.hh"

PMSteppingAction::PMSteppingAction(PMEventAction* eventAction)
    : G4UserSteppingAction(), fEventAction(eventAction) {}

PMSteppingAction::~PMSteppingAction() {}

void PMSteppingAction::UserSteppingAction(const G4Step* step) {
    G4Track* track = step->GetTrack();
    G4ParticleDefinition* particle = track->GetDefinition();

    if (particle == G4OpticalPhoton::Definition()) {
        G4VPhysicalVolume* preVolume = step->GetPreStepPoint()->GetPhysicalVolume();
        if (preVolume && preVolume->GetName() == "AluminumPlate") {
            if (fEventAction) {
                fEventAction->AddAluminumPhoton();
            }
            G4cout << "💡 [SteppingAction] Optical photon detected INSIDE AluminumPlate!" << G4endl;
            track->SetTrackStatus(fStopAndKill);
            return;
        }
        
        G4StepPoint* postStep = step->GetPostStepPoint();
        const G4VProcess* proc = postStep->GetProcessDefinedStep();
        if (proc && proc->GetProcessName() == "OpBoundary") {
            const G4OpBoundaryProcess* boundary = dynamic_cast<const G4OpBoundaryProcess*>(proc);
            if (boundary) {
                G4OpBoundaryProcessStatus status = boundary->GetStatus();
                if (status == Detection || status == Absorption) {
                    if (fEventAction) {
                        fEventAction->AddAluminumPhoton();
                    }
                    G4cout << "💡 [SteppingAction] Optical photon detected at aluminum boundary (status = "
                           << status << ")!" << G4endl;
                    track->SetTrackStatus(fStopAndKill);
                    return;
                }
            }
        }
        return;
    }

    G4StepPoint* preStep  = step->GetPreStepPoint();
    G4StepPoint* postStep = step->GetPostStepPoint();

    G4String particleName = particle->GetParticleName();
    G4double kineticEnergy = preStep->GetKineticEnergy() / MeV;
    G4VPhysicalVolume* volume = preStep->GetPhysicalVolume();
    G4String volumeName = (volume) ? volume->GetName() : "None";
    G4String processName = "None";
    if (postStep->GetProcessDefinedStep()) {
        processName = postStep->GetProcessDefinedStep()->GetProcessName();
    }
    G4double depositedEnergy = step->GetTotalEnergyDeposit() / MeV;

    G4cout << "\n=== Step Information ===\n"
           << "Particle: " << particleName << "\n"
           << "Kinetic Energy: " << kineticEnergy << " MeV\n"
           << "Current Volume: " << volumeName << "\n"
           << "Process: " << processName << "\n"
           << "Deposited Energy: " << depositedEnergy << " MeV\n"
           << "=========================\n" << G4endl;

    if (depositedEnergy > 0 && fEventAction) {
        fEventAction->RecordEnergy(depositedEnergy);
    }
}

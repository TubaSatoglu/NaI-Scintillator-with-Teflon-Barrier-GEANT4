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

// Constructor
PMSteppingAction::PMSteppingAction(PMEventAction* eventAction)
    : G4UserSteppingAction(), fEventAction(eventAction) {}

// Destructor
PMSteppingAction::~PMSteppingAction() {}

// Main stepping function
void PMSteppingAction::UserSteppingAction(const G4Step* step) {
    G4Track* track = step->GetTrack();
    G4ParticleDefinition* particle = track->GetDefinition();

    // Process optical photons separately.
    if (particle == G4OpticalPhoton::Definition()) {
        // First, check if the photon is already inside the aluminum volume.
        G4VPhysicalVolume* preVolume = step->GetPreStepPoint()->GetPhysicalVolume();
        if (preVolume && preVolume->GetName() == "AluminumPlate") {
            if (fEventAction) {
                fEventAction->AddAluminumPhoton();
            }
            G4cout << "💡 [SteppingAction] Optical photon detected INSIDE AluminumPlate!" << G4endl;
            track->SetTrackStatus(fStopAndKill);
            return;
        }
        
        // Otherwise, check for a boundary interaction.
        G4StepPoint* postStep = step->GetPostStepPoint();
        const G4VProcess* proc = postStep->GetProcessDefinedStep();
        if (proc && proc->GetProcessName() == "OpBoundary") {
            const G4OpBoundaryProcess* boundary = dynamic_cast<const G4OpBoundaryProcess*>(proc);
            if (boundary) {
                G4OpBoundaryProcessStatus status = boundary->GetStatus();
                // Check if the photon is “detected” or “absorbed” at a boundary.
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
        return; // End processing for optical photons here.
    }

    // For non-optical particles, output some debug information.
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

    // Record any energy deposit.
    if (depositedEnergy > 0 && fEventAction) {
        fEventAction->RecordEnergy(depositedEnergy);
    }
}

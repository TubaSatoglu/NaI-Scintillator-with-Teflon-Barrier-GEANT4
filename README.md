# Geant4 Detector Simulation

This repository contains Geant4-based simulations for particle interactions with different detector configurations.  
Each project demonstrates geometry definition, physics list configuration, primary generator setup, and action initialization.  
In some cases, Pythia8 is also used to generate primary events for proton-proton collisions.  

---

## 1️⃣ Geometry Definition
Implemented in **PMDetectorConstruction.cc/hh**.  
- Builds the **scintillator detector volume** (plastic or NaI depending on the project).  
- Surrounds it with **Teflon or Aluminium barriers** for optical reflection.  
- Defines material properties and optical surfaces.  
- Placement and visualization attributes are set to allow tracking of optical photons and primary particles.  

---

## 2️⃣ Physics List
Defined in **PMPhysicsList.cc/hh**.  
- Uses the **FTFP_BERT_HP** physics list, which includes:  
  - Electromagnetic processes  
  - Hadronic interactions  
  - Optical photon processes (scintillation, absorption, boundary reflections)  
- Additional physics lists can be substituted depending on the detector study.  

---

## 3️⃣ Action Initialization
Defined in **PMActionInitialization.cc/hh**.  
- Registers user-defined actions:  
  - **PMRunAction** → Simulation-level setup and run summary  
  - **PMEventAction** → Records energy deposition per event  
  - **PMSteppingAction** → Tracks optical photons step-by-step  
  - **PMSensitiveDetector** → Detects photon hits on sensitive surfaces  

---

## 4️⃣ Primary Generator Action
Defined in **PMPrimaryGenerator.cc/hh**.  
- Configures the particle gun.  
- For scintillator tests: alpha or gamma particles are generated at defined positions and energies.
- Allows selection of particle type, energy, and emission angle.  

---

## Example Projects
- **NaI-Scintillator-with-Teflon-Barrier-GEANT4** → NaI(Tl) scintillator wrapped with Teflon, optical photon yield analysis.  
- **Optic-Gamma-Scintillator-with-aluminium-GEANT4** → Gamma interactions with aluminium-covered scintillator.  
- **Plastic-Scintillator-with-Teflon-Barriers-GEANT4** → Plastic scintillator surrounded by Teflon walls.  
- **G4_Fe-Detector-with-Proton-GEANT4** → Proton interactions with Fe detector volume.  
- **pp_collision1-Pythia8313** → Proton-proton collisions generated with Pythia8, events passed to Geant4 for detector simulation.  

---

## 🛠 Requirements
- Geant4 (version 11.x recommended)  
- CMake ≥ 3.10  
- ROOT (optional, for result analysis)  
- Pythia8 (optional, for event generation)  

---

## ▶️ Build Instructions

```bash
mkdir build && cd build
cmake ..
make
./sim run.mac

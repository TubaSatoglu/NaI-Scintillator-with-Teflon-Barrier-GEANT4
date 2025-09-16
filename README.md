# Geant4 Detector Simulation

This repository contains Geant4-based simulations for particle interactions with different detector configurations.  
Each project demonstrates geometry definition, physics list configuration, primary generator setup, and action initialization.  
In some cases, Pythia8 is also used to generate primary events for proton-proton collisions.  

---

## 1. Geometry Definition
The detector geometry is implemented in **`detectorconstruction.cc`**.  
- Defines volumes such as scintillators, barriers (Teflon/Aluminium), and surrounding materials.  
- Placement and visualization attributes are set to allow tracking of optical photons and primary particles.  

## 2. Physics List
The simulation uses the **FTFP_BERT_HP** physics list, which includes:  
- Electromagnetic processes  
- Hadronic interactions  
- Optical photon processes (scintillation, absorption, reflection, boundary interactions)  

Additional physics lists can be substituted depending on the use case.  

## 3. Action Initialization
User-defined actions are managed in **`ActionInitialization.cc`**.  
- Handles run, event, stepping, and tracking actions.  
- Ensures statistics on energy deposition, optical photon counts, and detector response.  

## 4. Primary Generator Action
The initial conditions of particles are defined in **`PrimaryGeneratorAction.cc`**.  
- For scintillator tests: alpha or gamma particles are generated at defined positions and energies.  
- For proton-proton collisions: event data from Pythia8 (**`.hepmc3`**) can be read and injected into Geant4.  

---

## Example Projects
- **NaI-Scintillator-with-Teflon-Barrier-GEANT4** → NaI(Tl) scintillator wrapped with Teflon, optical photon yield analysis.  
- **Optic-Gamma-Scintillator-with-aluminium-GEANT4** → Gamma interactions with aluminium-covered scintillator.  
- **Plastic-Scintillator-with-Teflon-Barriers-GEANT4** → Plastic scintillator surrounded by Teflon walls.  
- **G4_Fe-Detector-with-Proton-GEANT4** → Proton interactions with Fe detector volume.  
- **pp_collision1-Pythia8313** → Proton-proton collisions generated with Pythia8, events passed to Geant4 for detector simulation.  

---

## Requirements
- Geant4 (version 11.x recommended)  
- CMake ≥ 3.10  
- ROOT (optional, for result analysis)  
- Pythia8 (optional, for event generation)  

---

## Build & Run

```bash
mkdir build && cd build
cmake ..
make
./exampleApp

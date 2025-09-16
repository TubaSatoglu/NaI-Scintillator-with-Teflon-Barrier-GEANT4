# NaI(Tl) Scintillator with Teflon Barrier – Geant4 Simulation

This project implements a Geant4-based simulation of a **NaI(Tl) scintillator detector wrapped with Teflon barriers**.  
It focuses on optical photon production, reflection, and detection efficiency in a reflective environment.  

---

## Geometry Definition
Implemented in **PMDetectorConstruction.cc/hh**.  
- Builds the **NaI(Tl) scintillator crystal**.  
- Surrounds it with **Teflon reflective barriers**.  
- Defines optical properties for NaI(Tl) and Teflon.  

---

## Physics List
Defined in **PMPhysicsList.cc/hh**.  
- Uses **FTFP_BERT_HP** physics list.  
- Registers optical photon processes: scintillation, absorption, reflection.  

---

## Action Initialization
Defined in **PMActionInitialization.cc/hh**.  
- Registers user actions: RunAction, EventAction, SteppingAction, SensitiveDetector.  

---

## Primary Generator Action
Defined in **PMPrimaryGenerator.cc/hh**.  
- Simulates gamma particles entering the NaI(Tl) detector.  
- Configurable energy and direction.  

---

## ▶️ Build Instructions

```bash
mkdir build && cd build
cmake ..
make
./sim run.mac

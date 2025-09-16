#ifndef PMDETECTORCONSTRUCTION_HH
#define PMDETECTORCONSTRUCTION_HH 1

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"

class G4VPhysicalVolume;

class PMDetectorConstruction : public G4VUserDetectorConstruction {
public:
    PMDetectorConstruction();
    virtual ~PMDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    void ConstructSDandField();

    private:
    G4Material* CreateScintillatorMaterial();
    G4Material* CreateTeflonMaterial();
    G4Material* CreateAluminumMaterial();

    void DefineOpticalSurfaces(G4VPhysicalVolume* scintillatorPhys, G4VPhysicalVolume* worldPhys, G4VPhysicalVolume* aluminumPhys,
                                G4VPhysicalVolume* teflonLeftPhys, G4VPhysicalVolume* teflonRightPhys,
                                G4VPhysicalVolume* teflonTopPhys, G4VPhysicalVolume* teflonBackPhys,
                                G4VPhysicalVolume* teflonFrontPhys);

    G4LogicalVolume* scintillatorLogical;
    G4LogicalVolume* aluminumLogical;
    G4VPhysicalVolume* aluminumPhys;
    G4LogicalVolume* teflonLeftLogical;
    G4LogicalVolume* teflonRightLogical;
    G4LogicalVolume* teflonFrontLogical;
    G4LogicalVolume* teflonBackLogical;
    G4LogicalVolume* teflonTopLogical;
    G4VPhysicalVolume* teflonLeftPhys;
    G4VPhysicalVolume* teflonRightPhys;
    G4VPhysicalVolume* teflonFrontPhys;
    G4VPhysicalVolume* teflonBackPhys;
    G4VPhysicalVolume* teflonTopPhys;
};

#endif

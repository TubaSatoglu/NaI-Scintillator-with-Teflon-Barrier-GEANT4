#include "PMDetectorConstruction.hh"
#include "PMSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SubtractionSolid.hh"

PMDetectorConstruction::PMDetectorConstruction() {}
PMDetectorConstruction::~PMDetectorConstruction() {}

G4Material* PMDetectorConstruction::CreateScintillatorMaterial() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* NaI = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");

    G4MaterialPropertiesTable* scintMPT = new G4MaterialPropertiesTable();

    std::vector<G4double> photonEnergies = {
        1.5 * eV, 1.8 * eV, 2.034 * eV, 2.068 * eV, 2.103 * eV, 2.139 * eV, 
        2.177 * eV, 2.216 * eV, 2.256 * eV, 2.298 * eV, 2.341 * eV, 2.386 * eV, 2.481 * eV, 3.0 * eV
    };  

    std::vector<G4double> rindex(photonEnergies.size(), 1.65);
    scintMPT->AddProperty("RINDEX", photonEnergies.data(), rindex.data(), photonEnergies.size());

    std::vector<G4double> scintil = { 
        0.02, 0.07, 0.15, 0.32, 0.57, 0.85, 1.00, 0.85, 0.57, 0.32, 0.15, 0.07 
    };
    scintMPT->AddProperty("FASTCOMPONENT", photonEnergies.data(), scintil.data(), photonEnergies.size(), true);

    std::vector<G4double> absorption(photonEnergies.size(), 800.*cm);
    scintMPT->AddProperty("ABSLENGTH", photonEnergies.data(), absorption.data(), photonEnergies.size());

    scintMPT->AddConstProperty("SCINTILLATIONYIELD", 100000. / MeV, true);
    scintMPT->AddConstProperty("RESOLUTIONSCALE", 1.5, true);
    scintMPT->AddConstProperty("FASTTIMECONSTANT", 200.*ns, true);
    scintMPT->AddConstProperty("YIELDRATIO", 1.0, true);

    NaI->SetMaterialPropertiesTable(scintMPT);
    return NaI;
}

G4Material* PMDetectorConstruction::CreateTeflonMaterial() {
    G4NistManager* nist = G4NistManager::Instance();
    return nist->FindOrBuildMaterial("G4_TEFLON");
}

G4Material* PMDetectorConstruction::CreateAluminumMaterial() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* aluminum = nist->FindOrBuildMaterial("G4_Al");

    G4MaterialPropertiesTable* alMPT = new G4MaterialPropertiesTable();
    std::vector<G4double> photonEnergies = {1.0*eV, 6.0*eV};
    std::vector<G4double> absorptionLength = {1*mm, 1*mm};
    alMPT->AddProperty("ABSLENGTH", photonEnergies.data(), absorptionLength.data(), photonEnergies.size());
    aluminum->SetMaterialPropertiesTable(alMPT);

    return aluminum;
}

void PMDetectorConstruction::DefineOpticalSurfaces(G4VPhysicalVolume* scintillatorPhys,
    G4VPhysicalVolume* worldPhys,
    G4VPhysicalVolume* aluminumPhys,
    G4VPhysicalVolume* teflonLeftPhys,
    G4VPhysicalVolume* teflonRightPhys,
    G4VPhysicalVolume* teflonTopPhys,
    G4VPhysicalVolume* teflonBackPhys,
    G4VPhysicalVolume* teflonFrontPhys)
{
    G4OpticalSurface* teflonSurface = new G4OpticalSurface("TeflonSurface");
    teflonSurface->SetType(dielectric_dielectric);
    teflonSurface->SetFinish(groundfrontpainted);
    teflonSurface->SetModel(unified);

    G4MaterialPropertiesTable* teflonMPT = new G4MaterialPropertiesTable();
    std::vector<G4double> teflonEnergy = {1.0 * eV, 6.0 * eV};
    std::vector<G4double> teflonReflectivity = {0.99, 0.99};
    std::vector<G4double> teflonEfficiency  = {0.0,  0.0};
    teflonMPT->AddProperty("REFLECTIVITY", teflonEnergy.data(), teflonReflectivity.data(), teflonEnergy.size());
    teflonMPT->AddProperty("EFFICIENCY",   teflonEnergy.data(), teflonEfficiency.data(),   teflonEnergy.size());
    teflonSurface->SetMaterialPropertiesTable(teflonMPT);

    new G4LogicalBorderSurface("TeflonLeftSurface",  scintillatorPhys, teflonLeftPhys,  teflonSurface);
    new G4LogicalBorderSurface("TeflonRightSurface", scintillatorPhys, teflonRightPhys, teflonSurface);
    new G4LogicalBorderSurface("TeflonTopSurface",   scintillatorPhys, teflonTopPhys,   teflonSurface);
    new G4LogicalBorderSurface("TeflonBackSurface",  scintillatorPhys, teflonBackPhys,  teflonSurface);
    new G4LogicalBorderSurface("TeflonFrontSurface", scintillatorPhys, teflonFrontPhys, teflonSurface);
}

G4VPhysicalVolume* PMDetectorConstruction::Construct() {
    G4double worldSize = 50.0 * cm;
    G4Box* worldBox = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);
    G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    G4LogicalVolume* worldLV = new G4LogicalVolume(worldBox, air, "World");
    G4VPhysicalVolume* worldPhys = new G4PVPlacement(nullptr, G4ThreeVector(),
                                                     worldLV, "World", nullptr, false, 0);

    G4Material* scintMaterial   = CreateScintillatorMaterial();
    G4Material* teflonMaterial  = CreateTeflonMaterial();
    G4Material* aluminumMaterial= CreateAluminumMaterial(); 

    G4double scintX = 10.0 * cm;
    G4double scintY = 10.0 * cm;
    G4double scintZ = 3.0  * cm;
    G4Box* scintBox = new G4Box("Scintillator", scintX/2, scintY/2, scintZ/2);
    scintillatorLogical = new G4LogicalVolume(scintBox, scintMaterial, "Scintillator");

    G4VPhysicalVolume* scintillatorPhys = new G4PVPlacement(
        nullptr, G4ThreeVector(0, 0, 0),
        scintillatorLogical, "ScintillatorPhys", worldLV, false, 0);

    G4double teflonThickness = 0.01 * cm;

    G4Box* teflonX   = new G4Box("TeflonX",   teflonThickness/2, scintY/2,    scintZ/2);
    G4Box* teflonY   = new G4Box("TeflonY",   scintX/2,          teflonThickness/2, scintZ/2);
    G4Box* teflonTop = new G4Box("TeflonTop", scintX/2,          scintY/2,    teflonThickness/2);

    G4double holeSize = 5.0 * mm;
    G4Box* holeBox = new G4Box("Hole", teflonThickness/2, holeSize/2, holeSize/2);

    G4ThreeVector holePosition(-(teflonThickness/2), 0, 0);
    G4SubtractionSolid* teflonLeftWithHole = new G4SubtractionSolid(
        "TeflonX_Left_Hole", teflonX, holeBox, nullptr, holePosition);

    teflonLeftLogical  = new G4LogicalVolume(teflonLeftWithHole, teflonMaterial, "TeflonX_Left");    
    teflonRightLogical = new G4LogicalVolume(teflonX, teflonMaterial, "TeflonX_Right");
    teflonFrontLogical = new G4LogicalVolume(teflonY, teflonMaterial, "TeflonY_Front");
    teflonBackLogical  = new G4LogicalVolume(teflonY, teflonMaterial, "TeflonY_Back");
    teflonTopLogical   = new G4LogicalVolume(teflonTop, teflonMaterial, "TeflonTop");

    teflonFrontPhys = new G4PVPlacement(
        nullptr, G4ThreeVector(0,  (scintY/2 + teflonThickness/2), 0),
        teflonFrontLogical, "TeflonY_Front", worldLV, false, 0);

    teflonBackPhys  = new G4PVPlacement(
        nullptr, G4ThreeVector(0, -(scintY/2 + teflonThickness/2), 0),
        teflonBackLogical, "TeflonY_Back", worldLV, false, 1);

    teflonLeftPhys  = new G4PVPlacement(
        nullptr, G4ThreeVector(-(scintX/2 + teflonThickness/2), 0, 0),
        teflonLeftLogical, "TeflonX_Left", worldLV, false, 2);

    teflonRightPhys = new G4PVPlacement(
        nullptr, G4ThreeVector((scintX/2 + teflonThickness/2), 0, 0),
        teflonRightLogical, "TeflonX_Right", worldLV, false, 3);

    teflonTopPhys   = new G4PVPlacement(
        nullptr, G4ThreeVector(0, 0, scintZ/2 + teflonThickness/2),
        teflonTopLogical, "TeflonTop", worldLV, false, 4);

    G4cout << "🔍 Debug: Creating Teflon hole with size = "
           << holeSize/mm << " mm at " << holePosition << G4endl;

    G4double aluminumThickness = teflonThickness * 1.2;
    G4Box* aluminumPlate = new G4Box("AluminumPlate",
                                     aluminumThickness/2, holeSize/2, holeSize/2);

    aluminumLogical = new G4LogicalVolume(aluminumPlate, aluminumMaterial, "AluminumPlate");

    G4ThreeVector aluminumPosition(-(scintX/2 + teflonThickness/2), 0, 0);
    aluminumPhys = new G4PVPlacement(
        nullptr, aluminumPosition,
        aluminumLogical, "AluminumPlate", worldLV, false, 5);

    G4cout << "🔍 Debug: Teflon Hole - Position: " << holePosition
           << " Size: " << holeSize/mm << " mm" << G4endl;
    G4cout << "🔍 Debug: Aluminum Position: " << aluminumPosition
           << " (Same as Hole)" << G4endl;

    DefineOpticalSurfaces(scintillatorPhys, worldPhys, aluminumPhys,
                          teflonLeftPhys, teflonRightPhys, teflonTopPhys,
                          teflonBackPhys, teflonFrontPhys);

    G4VisAttributes* scintVis = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.7));
    scintVis->SetVisibility(true);
    scintVis->SetForceSolid(true);

    G4VisAttributes* aluminumVis = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0, 0.8));
    aluminumVis->SetVisibility(true);
    aluminumVis->SetForceSolid(true);

    G4VisAttributes* teflonVis = new G4VisAttributes(G4Colour(0.85, 0.85, 0.85, 1.0));
    teflonVis->SetVisibility(true);
    teflonVis->SetForceSolid(true);

    teflonFrontLogical->SetVisAttributes(teflonVis);
    teflonBackLogical->SetVisAttributes(teflonVis);
    teflonLeftLogical->SetVisAttributes(teflonVis);
    teflonRightLogical->SetVisAttributes(teflonVis);
    teflonTopLogical->SetVisAttributes(teflonVis);
    aluminumLogical->SetVisAttributes(aluminumVis);
    scintillatorLogical->SetVisAttributes(scintVis);

    return worldPhys;
}

void PMDetectorConstruction::ConstructSDandField() {
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();

    if (!scintillatorLogical) {
        G4cerr << "🚨 ERROR: scintillatorLogical is NULL! " << G4endl;
        return;
    }
    PMSensitiveDetector* scintSD = new PMSensitiveDetector("ScintillatorSD");
    sdManager->AddNewDetector(scintSD);
    scintillatorLogical->SetSensitiveDetector(scintSD);

    if (!aluminumLogical) {
        G4cerr << "🚨 ERROR: aluminumLogical is NULL! " << G4endl;
        return;
    }
    PMSensitiveDetector* aluminumSD = new PMSensitiveDetector("AluminumSD");
    sdManager->AddNewDetector(aluminumSD);
    aluminumLogical->SetSensitiveDetector(aluminumSD);

    if (!aluminumPhys) {
        G4cerr << "🚨 ERROR: aluminumPhys is NULL! " << G4endl;
        return;
    }
    G4cout << "🔍 Debug: Checking Aluminum at "
           << aluminumPhys->GetObjectTranslation() << G4endl;
}

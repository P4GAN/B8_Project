//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B2/B2a/src/DetectorConstruction.cc
/// \brief Implementation of the B2a::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "DetectorMessenger.hh"
#include "TrackerSD.hh"

#include "G4AutoDelete.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4GeometryManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"

G4ThreadLocal G4GlobalMagFieldMessenger *DetectorConstruction::fMagFieldMessenger = nullptr;

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    // Define materials
    DefineMaterials();

    // Define volumes
    return DefineVolumes();
}

void DetectorConstruction::DefineMaterials()
{
    // Material definition
    G4NistManager *nistManager = G4NistManager::Instance();

    // Tracking cylinders made of Silicon
    nistManager->FindOrBuildMaterial("G4_AIR");
    nistManager->FindOrBuildMaterial("G4_Si");

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume *DetectorConstruction::DefineVolumes()
{
    G4Material *air = G4Material::GetMaterial("G4_AIR");
    G4Material *silicon = G4Material::GetMaterial("G4_Si");

    // Sizes of the principal geometrical components (solids)

    // World is cylinder with radius 1m and length 3m
    G4double worldRadius = 1 * m;
    G4double worldLength = 3 * m;

    G4double siliconThickness = 50 * um;
    int numTrackerBarrels = 5;
    int numTrackerDiscs = 10;

    std::vector<G4double> trackerBarrelRadii = {
        3.8 * cm, 5.0 * cm, 12.2 * cm, 27.2 * cm, 42.2 * cm
    };

    std::vector<G4double> trackerBarrelLengths = {
        27.0 * cm, 27.0 * cm, 27.0 * cm, 54.0 * cm, 80.0 * cm
    };

    std::vector<G4double> trackerDiscZPositions = {
        25.0 * cm, 45.0 * cm, 70.0 * cm, 100.0 * cm, 135.0 * cm,
        -25.0 * cm, -45.0 * cm, -65.0 * cm, -85.0 * cm, -105.0 * cm
    };

    std::vector<G4double> trackerDiscInnerRadii = {
        3.676 * cm, 3.676 * cm, 3.842 * cm, 5.443 * cm, 7.014 * cm,
        3.676 * cm, 3.676 * cm, 3.676 * cm, 4.006 * cm, 4.635 * cm
    };

    std::vector<G4double> trackerDiscOuterRadii = {
        23.0 * cm, 43.0 * cm, 43.0 * cm, 43.0 * cm, 43.0 * cm,
        23.0 * cm, 43.0 * cm, 43.0 * cm, 43.0 * cm, 43.0 * cm
    };

    // World
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

    G4cout << "Computed tolerance = "
           << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() / mm << " mm" << G4endl;

    auto worldS = new G4Tubs("world", 0., worldRadius, worldLength / 2, 0. * deg, 360. * deg);
    auto worldLV = new G4LogicalVolume(worldS, air, "World");

    //  Must place the World Physical volume unrotated at (0,0,0).
    //
    auto worldPV = new G4PVPlacement(nullptr,         // no rotation
                                     G4ThreeVector(), // at (0,0,0)
                                     worldLV,         // its logical volume
                                     "World",         // its name
                                     nullptr,         // its mother  volume
                                     false,           // no boolean operations
                                     0,               // copy number
                                     true);           // checking overlaps

    // Visualization attributes

    G4VisAttributes worldVisAtt(G4Colour::White());
    worldVisAtt.SetVisibility(true);
    worldVisAtt.SetForceWireframe(true);

    G4VisAttributes trackerVisAtt(G4Colour(1.0, 1.0, 0.0, 0.5)); // Yellow
    trackerVisAtt.SetVisibility(true);
    trackerVisAtt.SetForceSolid(true);

    worldLV->SetVisAttributes(worldVisAtt);

    // Tracker barrel segments
    for (int i = 0; i < numTrackerBarrels; i++)
    {
        auto trackerBarrelShape = new G4Tubs("TrackerBarrel_Solid",
                                             trackerBarrelRadii[i] - siliconThickness / 2,
                                             trackerBarrelRadii[i] + siliconThickness / 2,
                                             trackerBarrelLengths[i] / 2,
                                             0. * deg,
                                             360. * deg);

        auto trackerBarrelLV =
            new G4LogicalVolume(trackerBarrelShape, silicon, "TrackerBarrel_LV", nullptr, nullptr, nullptr);

        trackerBarrelLV->SetVisAttributes(trackerVisAtt);

        new G4PVPlacement(nullptr,
                          G4ThreeVector(0, 0, 0),
                          trackerBarrelLV,
                          "TrackerBarrel_PV",
                          worldLV,
                          false,
                          i,
                          true);
    }

    // Tracker disc segments
    for (int i = 0; i < numTrackerDiscs; i++)
    {
        auto trackerDiscShape = new G4Tubs("TrackerDisc_Solid",
                                           trackerDiscInnerRadii[i],
                                           trackerDiscOuterRadii[i],
                                           siliconThickness / 2,
                                           0. * deg,
                                           360. * deg);

        auto trackerDiscLV =
            new G4LogicalVolume(trackerDiscShape, silicon, "TrackerDisc_LV", nullptr, nullptr, nullptr);

        trackerDiscLV->SetVisAttributes(trackerVisAtt);

        new G4PVPlacement(nullptr,
                          G4ThreeVector(0, 0, trackerDiscZPositions[i]),
                          trackerDiscLV,
                          "TrackerDisc_PV",
                          worldLV,
                          false,
                          i,
                          true);
    }

    // Always return the physical world

    return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{
    // Sensitive detectors

    G4String trackerChamberSDname = "/TrackerChamberSD";
    auto trackerSD = new TrackerSD(trackerChamberSDname, "TrackerHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(trackerSD);
    // Setting trackerSD to all logical volumes with the same name
    // of "Chamber_LV".
    SetSensitiveDetector("TrackerBarrel_LV", trackerSD, true);
    SetSensitiveDetector("TrackerDisc_LV", trackerSD, true);

    // Create global magnetic field messenger.
    // Uniform magnetic field is then created automatically if
    // the field value is not zero.
    G4ThreeVector fieldValue = G4ThreeVector(0., 0., 1.7 * tesla);
    fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
    fMagFieldMessenger->SetVerboseLevel(1);

    // Register the field messenger for deleting
    G4AutoDelete::Register(fMagFieldMessenger);
}

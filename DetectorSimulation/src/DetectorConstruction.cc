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
#include "G4FieldManager.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4TransportationManager.hh"
#include "G4UserLimits.hh"
#include "G4UniformMagField.hh"
#include "G4VisAttributes.hh"

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    G4NistManager *nistManager = G4NistManager::Instance();

    nistManager->FindOrBuildMaterial("G4_AIR");
    nistManager->FindOrBuildMaterial("G4_Si");

    G4Material *air = G4Material::GetMaterial("G4_AIR");
    G4Material *silicon = G4Material::GetMaterial("G4_Si");

    // World is box with sides 1m and length 3m
    auto worldBox = new G4Box("World", 1.0 * m, 1.0 * m, 2.0 * m);
    auto worldLV = new G4LogicalVolume(worldBox, air, "WorldLV");
    auto worldPV =
        new G4PVPlacement(nullptr, {}, worldLV, "WorldPV", nullptr, false, 0);

    G4double siliconThickness = 50 * um;
    int numBarrels = 5;
    int numDiscs = 10;

    std::vector<G4double> barrelRadii = {3.8 * cm, 5.0 * cm, 12.2 * cm, 27.2 * cm,
                                         42.2 * cm};

    std::vector<G4double> barrelLengths = {27.0 * cm, 27.0 * cm, 27.0 * cm,
                                           54.0 * cm, 80.0 * cm};

    std::vector<G4double> discZPositions = {
        25.0 * cm, 45.0 * cm, 70.0 * cm, 100.0 * cm, 135.0 * cm,
        -25.0 * cm, -45.0 * cm, -65.0 * cm, -85.0 * cm, -105.0 * cm};

    std::vector<G4double> discInnerRadii = {
        3.676 * cm, 3.676 * cm, 3.842 * cm, 5.443 * cm, 7.014 * cm,
        3.676 * cm, 3.676 * cm, 3.676 * cm, 4.006 * cm, 4.635 * cm};

    std::vector<G4double> discOuterRadii = {
        23.0 * cm, 43.0 * cm, 43.0 * cm, 43.0 * cm, 43.0 * cm,
        23.0 * cm, 43.0 * cm, 43.0 * cm, 43.0 * cm, 43.0 * cm};

    // Visualization attributes
    G4VisAttributes worldVisAtt(G4Colour::White());
    worldVisAtt.SetVisibility(true);
    worldVisAtt.SetForceWireframe(true);
    worldLV->SetVisAttributes(worldVisAtt);

    G4VisAttributes trackerVisAtt(G4Colour(1.0, 1.0, 0.0, 0.5)); // Yellow
    trackerVisAtt.SetVisibility(true);
    trackerVisAtt.SetForceSolid(true);

    // Barrel segments
    for (int i = 0; i < numBarrels; i++)
    {
        auto name = "SVT_Barrel_" + std::to_string(i);
        auto barrelShape = new G4Tubs(name, barrelRadii[i] - siliconThickness / 2,
                                      barrelRadii[i] + siliconThickness / 2,
                                      barrelLengths[i] / 2, 0. * deg, 360. * deg);
        auto barrelLV = new G4LogicalVolume(barrelShape, silicon, name + "_LV",
                                            nullptr, nullptr, nullptr);
        barrelLV->SetVisAttributes(trackerVisAtt);
        new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), barrelLV, name + "_PV",
                          worldLV, false, i, true);
        trackerLogicalVolumes.push_back(barrelLV);
    }

    // Disc segments
    for (int i = 0; i < numDiscs; i++)
    {
        auto discShape =
            new G4Tubs("Disc_Solid", discInnerRadii[i], discOuterRadii[i],
                       siliconThickness / 2, 0. * deg, 360. * deg);
        auto discLV = new G4LogicalVolume(discShape, silicon, "Disc_LV", nullptr,
                                          nullptr, nullptr);
        discLV->SetVisAttributes(trackerVisAtt);
        new G4PVPlacement(nullptr, G4ThreeVector(0, 0, discZPositions[i]), discLV,
                          "Disc_PV", worldLV, false, numBarrels + i, true);
        trackerLogicalVolumes.push_back(discLV);
    }

    return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{
    // Set trackers as sensitive detectors
    auto trackerSD = new TrackerSD("SVT_SD", "HitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(trackerSD);

    for (auto *lv : trackerLogicalVolumes)
    {
        SetSensitiveDetector(lv, trackerSD);
    }

    // Set uniform magnetic field
    G4ThreeVector fieldValue = G4ThreeVector(0., 0., 1.7 * tesla);
    auto magField = new G4UniformMagField(fieldValue);

    auto fieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fieldManager->SetDetectorField(magField);
    fieldManager->CreateChordFinder(magField);
}

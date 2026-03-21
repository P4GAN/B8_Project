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

#ifndef B2aDetectorConstruction_h
#define B2aDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;
class G4GlobalMagFieldMessenger;

class DetectorMessenger;

/// Detector construction class to define materials, geometry
/// and global uniform magnetic field.

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

public:
    G4VPhysicalVolume *Construct() override;
    void ConstructSDandField() override;
    double GetResolution() const { return fDetectorResolution; }
    void SetResolution(G4double val) { fDetectorResolution = val; }
    void SetMaterialWidth1(G4double val) { fMaterialWidth1 = val; }
    void SetMaterialWidth2(G4double val) { fMaterialWidth2 = val; }
    void SetMaterialWidth3(G4double val) { fMaterialWidth3 = val; }

private:
    static G4ThreadLocal G4GlobalMagFieldMessenger* fMagFieldMessenger;
    std::vector<G4LogicalVolume*> trackerLogicalVolumes;
    G4double fDetectorResolution = 7 * um;
    G4double fMaterialWidth1 = 0.0007;
    G4double fMaterialWidth2 = 0.0025;
    G4double fMaterialWidth3 = 0.0055;
    DetectorMessenger* fMessenger = nullptr;  
};

#endif

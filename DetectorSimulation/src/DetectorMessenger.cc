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
/// \file B2/B2a/src/DetectorMessenger.cc
/// \brief Implementation of the B2a::DetectorMessenger class

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"

#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIdirectory.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction *det) : fDetectorConstruction(det)
{
    fDetDirectory = new G4UIdirectory("/det/");
    fDetDirectory->SetGuidance("Detector construction control");

    fResolutionCmd = new G4UIcmdWithADoubleAndUnit("/det/res", this);
    fResolutionCmd->SetGuidance("Set hit resolution");
    fResolutionCmd->SetParameterName("resolution", false);
    fResolutionCmd->SetUnitCategory("Length");
    fResolutionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fMaterialWidth1Cmd = new G4UIcmdWithADouble("/det/materialWidth1", this);
    fMaterialWidth1Cmd->SetGuidance("Set material width of IB layers");
    fMaterialWidth1Cmd->SetParameterName("materialWidth1", false);
    fMaterialWidth1Cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fMaterialWidth2Cmd = new G4UIcmdWithADouble("/det/materialWidth2", this);
    fMaterialWidth2Cmd->SetGuidance("Set material width of OB 3 layer and discs");
    fMaterialWidth2Cmd->SetParameterName("materialWidth2", false);
    fMaterialWidth2Cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fMaterialWidth3Cmd = new G4UIcmdWithADouble("/det/materialWidth3", this);
    fMaterialWidth3Cmd->SetGuidance("Set material width of OB 4 layer");
    fMaterialWidth3Cmd->SetParameterName("materialWidth3", false);
    fMaterialWidth3Cmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
    delete fResolutionCmd;
    delete fDetDirectory;
    delete fMaterialWidth1Cmd;
    delete fMaterialWidth2Cmd;
    delete fMaterialWidth3Cmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == fResolutionCmd) {
        fDetectorConstruction->SetResolution(fResolutionCmd->GetNewDoubleValue(newValue));
    }
    if (command == fMaterialWidth1Cmd) {
        fDetectorConstruction->SetMaterialWidth1(fMaterialWidth1Cmd->GetNewDoubleValue(newValue));
    }
    if (command == fMaterialWidth2Cmd) {
        fDetectorConstruction->SetMaterialWidth2(fMaterialWidth2Cmd->GetNewDoubleValue(newValue));
    }
    if (command == fMaterialWidth3Cmd) {
        fDetectorConstruction->SetMaterialWidth3(fMaterialWidth3Cmd->GetNewDoubleValue(newValue));
    }
}

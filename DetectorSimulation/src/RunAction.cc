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
/// \file B2/B2a/src/RunAction.cc
/// \brief Implementation of the B2::RunAction class

#include "RunAction.hh"

#include <chrono>
#include <format>

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction()
{
    // set printing event number per each 100 events
    G4RunManager::GetRunManager()->SetPrintProgress(1000);

    // Create analysis manager and set up ntuple for hits
    auto analysisManager = G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run *run)
{
    // inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    auto analysisManager = G4AnalysisManager::Instance();
    // Create unique file name based on system timestamp
    auto now = std::chrono::system_clock::now();
    std::string s = std::format("{:%Y%m%d%H%M%S}", now);
    std::string fileName = "output/run_" + s + ".root";
    analysisManager->OpenFile(fileName);

    // Create ntuple for hits
    analysisManager->CreateNtuple("hits", "Energy deposit and position");
    analysisManager->CreateNtupleDColumn("Energy");
    analysisManager->CreateNtupleDColumn("PositionX");
    analysisManager->CreateNtupleDColumn("PositionY");
    analysisManager->CreateNtupleDColumn("PositionZ");
    analysisManager->CreateNtupleIColumn("TrackID");
    analysisManager->FinishNtuple();

    // Create ntuple for tracks
    analysisManager->CreateNtuple("tracks", "Track parameters");
    analysisManager->CreateNtupleDColumn("VertexPositionX");
    analysisManager->CreateNtupleDColumn("VertexPositionY");
    analysisManager->CreateNtupleDColumn("VertexPositionZ");
    analysisManager->CreateNtupleDColumn("MomentumX");
    analysisManager->CreateNtupleDColumn("MomentumY");
    analysisManager->CreateNtupleDColumn("MomentumZ");
    analysisManager->CreateNtupleDColumn("Charge");
    analysisManager->CreateNtupleIColumn("ParticleID");
    analysisManager->CreateNtupleIColumn("TrackID");
    analysisManager->FinishNtuple();

}

void RunAction::EndOfRunAction(const G4Run *)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}

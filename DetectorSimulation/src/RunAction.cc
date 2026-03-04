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

#include "RunActionMessenger.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

#include <vector>

// static members defined (thread-local qualifiers must match declaration)
G4ThreadLocal std::vector<G4double> RunAction::hitPositionX;
G4ThreadLocal std::vector<G4double> RunAction::hitPositionY;
G4ThreadLocal std::vector<G4double> RunAction::hitPositionZ;

RunAction::RunAction()
{
    messenger = new RunActionMessenger(this);

    // set printing event number per each 100 events
    G4RunManager::GetRunManager()->SetPrintProgress(1000);

    // Create analysis manager and set up ntuple for tracks
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetNtupleMerging(true);
    
    // Each row contains the original particle parameters, and vector of hit positions
    analysisManager->CreateNtuple("tracks", "Track parameters");
    analysisManager->CreateNtupleDColumn("MomentumX");
    analysisManager->CreateNtupleDColumn("MomentumY");
    analysisManager->CreateNtupleDColumn("MomentumZ");
    analysisManager->CreateNtupleIColumn("ParticleID");
    analysisManager->CreateNtupleIColumn("EventID");
    analysisManager->CreateNtupleIColumn("NumHits");

    // Vector branches holding hit coordinates for each track
    analysisManager->CreateNtupleDColumn("HitPositionX", hitPositionX);
    analysisManager->CreateNtupleDColumn("HitPositionY", hitPositionY);
    analysisManager->CreateNtupleDColumn("HitPositionZ", hitPositionZ);

    analysisManager->FinishNtuple();
}

void RunAction::BeginOfRunAction(const G4Run *run)
{
    // inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    auto analysisManager = G4AnalysisManager::Instance();

    std::string fileName = "output/" + outputFileName;
    analysisManager->OpenFile(fileName);
}

void RunAction::EndOfRunAction(const G4Run *)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}

RunAction::~RunAction()
{
    delete messenger;
}
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

#include "TrackingAction.hh"

#include "G4UserTrackingAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
    if (track->GetParentID() != 0) return;

    int trackID = track->GetTrackID();
    auto pos = track->GetVertexPosition();
    auto mom = track->GetMomentum();

    // Fill track ntuple with vertex and momentum information
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->FillNtupleDColumn(1, 0, pos.x());
    analysisManager->FillNtupleDColumn(1, 1, pos.y());
    analysisManager->FillNtupleDColumn(1, 2, pos.z());
    analysisManager->FillNtupleDColumn(1, 3, mom.x());
    analysisManager->FillNtupleDColumn(1, 4, mom.y());
    analysisManager->FillNtupleDColumn(1, 5, mom.z());
    analysisManager->FillNtupleDColumn(1, 6, track->GetDynamicParticle()->GetCharge());
    analysisManager->FillNtupleIColumn(1, 7, track->GetDynamicParticle()->GetPDGcode());
    analysisManager->FillNtupleIColumn(1, 8, track->GetTrackID());
    analysisManager->AddNtupleRow(1);
}

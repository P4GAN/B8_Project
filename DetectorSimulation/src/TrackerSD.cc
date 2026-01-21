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
/// \file B2/B2a/src/TrackerSD.cc
/// \brief Implementation of the B2::TrackerSD class

#include "TrackerSD.hh"

#include "G4AnalysisManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"

TrackerSD::TrackerSD(const G4String &name, const G4String &hitsCollectionName)
    : G4VSensitiveDetector(name)
{
    collectionName.insert(hitsCollectionName);
}

void TrackerSD::Initialize(G4HCofThisEvent *hce)
{
    // Create hits collection

    fHitsCollection = new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]);

    // Add this collection in hce

    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool TrackerSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    // energy deposit
    G4double edep = step->GetTotalEnergyDeposit();

    if (edep == 0.)
        return false;

    auto newHit = new TrackerHit();

    newHit->SetTrackID(step->GetTrack()->GetTrackID());
    newHit->SetChamberNb(step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
    newHit->SetEdep(edep);
    newHit->SetPos(step->GetPostStepPoint()->GetPosition());

    fHitsCollection->insert(newHit);

    // Fill histograms and ntuple
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->FillNtupleDColumn(0, 0, edep);
    analysisManager->FillNtupleDColumn(0, 1, step->GetPostStepPoint()->GetPosition().x());
    analysisManager->FillNtupleDColumn(0, 2, step->GetPostStepPoint()->GetPosition().y());
    analysisManager->FillNtupleDColumn(0, 3, step->GetPostStepPoint()->GetPosition().z());
    analysisManager->FillNtupleDColumn(0, 4, step->GetPostStepPoint()->GetGlobalTime());
    analysisManager->FillNtupleIColumn(0, 5, step->GetTrack()->GetTrackID());
    analysisManager->FillNtupleIColumn(0, 6, step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
    analysisManager->AddNtupleRow(0);

    // newHit->Print();

    return true;
}

void TrackerSD::EndOfEvent(G4HCofThisEvent *)
{
    if (verboseLevel > 1)
    {
        std::size_t nofHits = fHitsCollection->entries();
        G4cout << G4endl << "-------->Hits Collection: in this event they are " << nofHits
               << " hits in the tracker chambers: " << G4endl;
        for (std::size_t i = 0; i < nofHits; i++)
            (*fHitsCollection)[i]->Print();
    }
}

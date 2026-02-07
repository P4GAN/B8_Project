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

#include "Randomize.hh"

#include "G4AnalysisManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

TrackerSD::TrackerSD(const G4String &name, const G4String &hitsCollectionName)
    : G4VSensitiveDetector(name)
{
    collectionName.insert(hitsCollectionName);
}

void TrackerSD::Initialize(G4HCofThisEvent *hce)
{
    fHitsCollection = new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]);
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool TrackerSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    // Simulate missed hits
    double efficiency = 0.99; 
    if (G4UniformRand() > efficiency) {
        return false;
    }

    double edep = step->GetTotalEnergyDeposit();
    double threshold = 1 * keV;
    G4cout << "Energy deposit: " << edep / keV << " keV" << G4endl;
    if (edep < threshold) {
        return false;
    }

    auto track = step->GetTrack();

    auto hit = new TrackerHit();
    hit->trackID = track->GetTrackID();
    hit->pdg = track->GetParticleDefinition()->GetPDGEncoding();
    hit->detectorID = track->GetVolume()->GetCopyNo();
    hit->time = track->GetGlobalTime();
    hit->edep = edep;
    hit->pos = step->GetPostStepPoint()->GetPosition();
    hit->momentum = step->GetPostStepPoint()->GetMomentum();

    fHitsCollection->insert(hit);

    return true;
}

void TrackerSD::EndOfEvent(G4HCofThisEvent *)
{
    auto analysisManager = G4AnalysisManager::Instance();
    std::size_t nofHits = fHitsCollection->entries();
    for (std::size_t i = 0; i < nofHits; i++) {
        auto hit = (*fHitsCollection)[i];
        G4ThreeVector smearedPos = GetSmearedPosition(*hit);
        analysisManager->FillNtupleDColumn(0, 0, hit->edep);
        analysisManager->FillNtupleDColumn(0, 1, smearedPos.x());
        analysisManager->FillNtupleDColumn(0, 2, smearedPos.y());
        analysisManager->FillNtupleDColumn(0, 3, smearedPos.z());
        analysisManager->FillNtupleIColumn(0, 4, hit->trackID);
        analysisManager->AddNtupleRow(0);

        hit->Print();

    }
}

G4ThreeVector TrackerSD::GetSmearedPosition(const TrackerHit& hit)
{
    double resolution = 7 * micrometer;
    // Barrel
    if (hit.detectorID < 5) {
        double radius = hit.pos.perp();
        double phi = hit.pos.phi();
        double smearedZ = hit.pos.z() + G4RandGauss::shoot(0, resolution);
        double smearedPhi = phi + G4RandGauss::shoot(0, resolution / radius);
        return G4ThreeVector(radius * std::cos(smearedPhi), radius * std::sin(smearedPhi), smearedZ);
    } 
    // Discs
    else { 
        double smearedX = hit.pos.x() + G4RandGauss::shoot(0, resolution);
        double smearedY = hit.pos.y() + G4RandGauss::shoot(0, resolution);
        return G4ThreeVector(smearedX, smearedY, hit.pos.z());
    }
}
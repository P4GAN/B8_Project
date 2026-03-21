#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

void SteppingAction::UserSteppingAction(const G4Step* step) {
    G4Track* track = step->GetTrack();
    // Kill particle after 10000 steps
    if (track->GetCurrentStepNumber() > 10000) {
        track->SetTrackStatus(fStopAndKill);
        return;
    }
}

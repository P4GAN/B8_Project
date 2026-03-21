#include "G4UserSteppingAction.hh"
#include "globals.hh"

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction() = default;
    virtual ~SteppingAction() = default;
    virtual void UserSteppingAction(const G4Step* step) override;
};

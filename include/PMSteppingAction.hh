#ifndef PMSTEPPINGACTION_HH
#define PMSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"

class PMEventAction;

class PMSteppingAction : public G4UserSteppingAction {
public:
    PMSteppingAction(PMEventAction* eventAction);
    virtual ~PMSteppingAction();

    virtual void UserSteppingAction(const G4Step* step) override;

private:
    PMEventAction* fEventAction;
};

#endif

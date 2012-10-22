
#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class DetectorConstruction;
class HistoManager;
class EventAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(DetectorConstruction*, HistoManager*, EventAction*);
  virtual ~SteppingAction();

  void UserSteppingAction(const G4Step*);
    
private:
  DetectorConstruction* detector;
  HistoManager*		histomanager;
  EventAction*          eventaction;  
};

#endif

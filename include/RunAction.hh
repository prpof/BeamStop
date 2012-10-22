
#ifndef RunAction_h
#define RunAction_h 1
#define NCCD 5

#include "G4UserRunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class HistoManager;
class DetectorConstruction;
class PrimaryGeneratorAction;

class RunAction : public G4UserRunAction
{
public:
  RunAction(HistoManager*, DetectorConstruction*, PrimaryGeneratorAction*);
  virtual ~RunAction();

  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
  void fillPerEvent(G4int, G4int, G4int, G4int, G4double*, G4double*);

private:
  HistoManager* histoManager;
  DetectorConstruction* detector;
  PrimaryGeneratorAction* generator;
  G4int backdoorRunLeak;
  G4int frontdoorRunLeak;
  G4int sidedoorRunLeak;
  G4int totalRunCharge;
  G4double neutronEsum[NCCD];
  G4double gammaEsum[NCCD];
};

#endif

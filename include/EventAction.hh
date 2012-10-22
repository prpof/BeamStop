
#ifndef EventAction_h
#define EventAction_h 1
#define MXPART 100
#include "G4Run.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "DetectorConstruction.hh"

class RunAction;
class HistoManager;
class DetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*, HistoManager*, DetectorConstruction*);
  virtual ~EventAction();

  void  BeginOfEventAction(const G4Event*);
  void  EndOfEventAction(const G4Event*);

  void AddCharge(G4int particleCharge) {
    totalEventCharge += particleCharge;
  }

  void AddLeakage(G4int direction) {
    switch (direction) {
      case 1:
        backdoorEventLeak++;
        break;
      case 2:
        frontdoorEventLeak++;
        break;
      case 3:
        sidedoorEventLeak++;
        break;
      default:
        break;
    }
  }

  void AddNeutronKE(G4double neutronKE_arg, G4int nCCD_arg, G4int nNeutrons_arg) {
    nNeutrons[nCCD_arg] = nNeutrons_arg;
    if ( nNeutrons_arg < MXPART ) {
      for (int i=0; i<nNeutrons[nCCD_arg]; i++) {
        neutronKE[nCCD_arg][i] = neutronKE_arg;
      }
    }
  }

  void AddNeutronEdep(G4double neutronEtot_arg, G4int nCCD_arg) {
    neutronEtot[nCCD_arg] += neutronEtot_arg;
  }

  void AddGammaKE(G4double gammaKE_arg, G4int nCCD_arg, G4int nGammas_arg) {
    nGammas[nCCD_arg] = nGammas_arg;
    if ( nGammas_arg < MXPART ) {
      for (int i=0; i<nGammas[nCCD_arg]; i++) {
        gammaKE[nCCD_arg][i] = gammaKE_arg;
      }
    }
  }

  void AddGammaEdep(G4double gammaEtot_arg, G4int nCCD_arg) {
    gammaEtot[nCCD_arg] += gammaEtot_arg;
  }

  G4Run* MyRun;

private:
  RunAction*    runAct;
  HistoManager* histoManager;
  DetectorConstruction* detector;

  G4int    RunNum;
  G4int    Evtnum;

  G4int    totalEventCharge;
  G4int    backdoorEventLeak;
  G4int    frontdoorEventLeak;
  G4int    sidedoorEventLeak;

  G4int    nCCD;

  G4int    nNeutrons[NCCD];
  G4double neutronKE[NCCD][MXPART];
  G4double neutronEtot[NCCD];

  G4int    nGammas[NCCD];
  G4double gammaKE[NCCD][MXPART];
  G4double gammaEtot[NCCD];

  G4int printModulo;
};

#endif

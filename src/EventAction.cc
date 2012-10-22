
#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* run, HistoManager* histo, DetectorConstruction* det)
:runAct(run),histoManager(histo),detector(det)
{
 printModulo = 1000; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{  
  G4int evtNb = evt->GetEventID();
  if (evtNb%printModulo == 0) 
    G4cout << "\n---> Begin of event: " << evtNb << G4endl;
  // initialisation per event
  totalEventCharge = 0;
  backdoorEventLeak = 0;
  frontdoorEventLeak = 0;
  sidedoorEventLeak = 0;
  for (int i=0; i<NCCD; i++) {
    nNeutrons[i] = 0;
    nGammas[i] = 0;
    for (int j=0; j<MXPART; j++) {
      neutronKE[i][j]=0;
      gammaKE[i][j]=0;
    }
    neutronEtot[i]=0;
    gammaEtot[i]=0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
  //accumulates statistic
  //
  runAct->fillPerEvent(backdoorEventLeak,frontdoorEventLeak,sidedoorEventLeak,
                       totalEventCharge,neutronEtot,gammaEtot);
  
  // get the current run and event numbers
  //
  MyRun = new G4Run; 
  G4int RunNum = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
  G4int evtNum = evt->GetEventID();

  //fill ntuples
//
  G4double neutronKE_0[MXPART];
  G4int nNeutrons_0 = nNeutrons[0];
  for (int i=0; i<nNeutrons[0]; i++) {
    neutronKE_0[i] = neutronKE[0][i];
  }
  G4double neutronEtot_0 = neutronEtot[0];

  G4double gammaKE_0[MXPART];
  G4int nGammas_0 = nGammas[0];
  for (int i=0; i<nGammas[0]; i++) {
    gammaKE_0[i] = gammaKE[0][i];
  }
  G4double gammaEtot_0 = gammaEtot[0];

  histoManager->FillNtuple0(RunNum, evtNum,
                            nNeutrons_0, neutronKE_0, neutronEtot_0,
                            nGammas_0, gammaKE_0, gammaEtot_0);
//
  G4double neutronKE_1[MXPART];
  G4int nNeutrons_1 = nNeutrons[1];
  for (int i=0; i<nNeutrons[1]; i++) {
    neutronKE_1[i] = neutronKE[1][i];
  }
  G4double neutronEtot_1 = neutronEtot[1];

  G4double gammaKE_1[MXPART];
  G4int nGammas_1 = nGammas[1];
  for (int i=0; i<nGammas[1]; i++) {
    gammaKE_1[i] = gammaKE[1][i];
  }
  G4double gammaEtot_1 = gammaEtot[1];

  histoManager->FillNtuple1(RunNum, evtNum,
                            nNeutrons_1, neutronKE_1, neutronEtot_1,
                            nGammas_1, gammaKE_1, gammaEtot_1);
//
  G4double neutronKE_2[MXPART];
  G4int nNeutrons_2 = nNeutrons[2];
  for (int i=0; i<nNeutrons[2]; i++) {
    neutronKE_2[i] = neutronKE[2][i];
  }
  G4double neutronEtot_2 = neutronEtot[2];

  G4double gammaKE_2[MXPART];
  G4int nGammas_2 = nGammas[2];
  for (int i=0; i<nGammas[2]; i++) {
    gammaKE_2[i] = gammaKE[2][i];
  }
  G4double gammaEtot_2 = gammaEtot[2];

  histoManager->FillNtuple2(RunNum, evtNum,
                            nNeutrons_2, neutronKE_2, neutronEtot_2,
                            nGammas_2, gammaKE_2, gammaEtot_2);
//
  G4double neutronKE_3[MXPART];
  G4int nNeutrons_3 = nNeutrons[3];
  for (int i=0; i<nNeutrons[3]; i++) {
    neutronKE_3[i] = neutronKE[3][i];
  }
  G4double neutronEtot_3 = neutronEtot[3];

  G4double gammaKE_3[MXPART];
  G4int nGammas_3 = nGammas[3];
  for (int i=0; i<nGammas[3]; i++) {
    gammaKE_3[i] = gammaKE[3][i];
  }
  G4double gammaEtot_3 = gammaEtot[3];

  histoManager->FillNtuple3(RunNum, evtNum,
                            nNeutrons_3, neutronKE_3, neutronEtot_3,
                            nGammas_3, gammaKE_3, gammaEtot_3);
//
  G4double neutronKE_4[MXPART];
  G4int nNeutrons_4 = nNeutrons[4];
  for (int i=0; i<nNeutrons[4]; i++) {
    neutronKE_4[i] = neutronKE[4][i];
  }
  G4double neutronEtot_4 = neutronEtot[4];

  G4double gammaKE_4[MXPART];
  G4int nGammas_4 = nGammas[4];
  for (int i=0; i<nGammas[4]; i++) {
    gammaKE_4[i] = gammaKE[4][i];
  }
  G4double gammaEtot_4 = gammaEtot[4];

  histoManager->FillNtuple4(RunNum, evtNum,
                            nNeutrons_4, neutronKE_4, neutronEtot_4,
                            nGammas_4, gammaKE_4, gammaEtot_4);
}  

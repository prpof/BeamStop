#include "RunAction.hh"
#include "HistoManager.hh"
#include "DetectorConstruction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include <iostream>
#include <fstream>
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(HistoManager* histo,
                     DetectorConstruction* det,
                     PrimaryGeneratorAction* gen_action)
:histoManager(histo), detector(det), generator(gen_action)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
    
  //initialize cumulative quantities
  //
  backdoorRunLeak = 0;
  frontdoorRunLeak = 0;
  sidedoorRunLeak = 0;
  totalRunCharge = 0;
  for (int i=0; i<NCCD; i++) {
    neutronEsum[i] = 0;
    gammaEsum[i] = 0;
  }
  
  //histograms
  //
  //histoManager->book();
  if (aRun->GetRunID() == 0) histoManager->book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::fillPerEvent(G4int backdoorEventLeak,
                             G4int frontdoorEventLeak,
                             G4int sidedoorEventLeak,
                             G4int totalEventCharge,
                             G4double* neutronEtot, G4double* gammaEtot)
{
  //accumulate statistic
  //

  backdoorRunLeak += backdoorEventLeak;
  frontdoorRunLeak += frontdoorEventLeak;
  sidedoorRunLeak += sidedoorEventLeak;

  totalRunCharge += totalEventCharge;

  for (int i=0; i<NCCD; i++) {
    neutronEsum[i] += neutronEtot[i];
    gammaEsum[i] += gammaEtot[i];
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;

  G4double volumeCCD = detector->GetCCDvolume();		// cm^3
  G4double densityCCD = 2.33;					// g/cm^3
  G4double massCCD = volumeCCD * densityCCD / 1000;		// kg
  G4double BeamPower = 100.0;					// 100 Watts
  G4double BeamEnergy = 1.e6 * generator->GetBeamEnergy();	// beam energy in eV
  G4double BeamCurrent = BeamPower / BeamEnergy;		// amps = watts / eV
  G4double JoulesPerMeV = 1.602e-13;
  G4double ElectronCharge = 1.602e-19;				// coulombs
  G4double ElectronsPerSecond = BeamCurrent / ElectronCharge;
  G4double hours = (NbOfEvents / ElectronsPerSecond) / 3600;	// number of hours running
  G4double GraysPerHour;					// one joule per kilogram per hour

  ofstream LOGcout;
  LOGcout.open ("BeamStop.txt");

  LOGcout << "Number of events processed is " << NbOfEvents << G4endl;

  LOGcout << "Average absolute charge per event leaking out the back is "
         << (G4double) backdoorRunLeak / (G4double) NbOfEvents << G4endl;

  LOGcout << "Average absolute charge per event leaking out the front is "
         << (G4double) frontdoorRunLeak / (G4double) NbOfEvents << G4endl;

  LOGcout << "Average absolute charge per event leaking out the side is "
         << (G4double) sidedoorRunLeak / (G4double) NbOfEvents << G4endl;

  LOGcout << "Average charge per event captured in the Faraday cup is "
         << (G4double) totalRunCharge / (G4double) NbOfEvents << G4endl;

  for (int i=0; i<NCCD; i++) {
    GraysPerHour = (neutronEsum[i] * JoulesPerMeV / massCCD) / hours;
    LOGcout << "Neutron dose in CCD " << i << " is " << GraysPerHour << " Gy per hour." << G4endl;
    GraysPerHour = (gammaEsum[i] * JoulesPerMeV / massCCD) / hours;
    LOGcout << "Gamma   dose in CCD " << i << " is " << GraysPerHour << " Gy per hour." << G4endl;
  }

  LOGcout.close();
}

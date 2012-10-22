
#include "HistoManager.hh"
#include "G4UnitsTable.hh"

#ifdef G4ANALYSIS_USE
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
:rootFile(0),ntupl0(0),ntupl1(0),ntupl2(0),ntupl3(0),ntupl4(0)
{
  // ntuple
  ntupl0 = 0;
  ntupl1 = 0;
  ntupl2 = 0;
  ntupl3 = 0;
  ntupl4 = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  if ( rootFile ) delete rootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::book()
{ 
  G4String fileName = "BeamStop.root";
  rootFile = new TFile(fileName,"RECREATE");
  if(!rootFile) {
    G4cout << " HistoManager::book :" 
           << " problem creating the ROOT TFile "
           << G4endl;
    return;
  }

  ntupl0 = new TTree("101", "CCD 0");
  ntupl0->Branch("Rnum", &Rnum, "Rnum/I");
  ntupl0->Branch("Evtnum", &Evtnum, "Evtnum/I");
  ntupl0->Branch("nNeutrons_0", &nNeutrons_0, "nNeutrons_0/I");
  ntupl0->Branch("neutronKE_0", neutronKE_0, "neutronKE_0[nNeutrons_0]/D");
  ntupl0->Branch("neutronEtot_0", &neutronEtot_0, "neutronEtot_0/D");
  ntupl0->Branch("nGammas_0", &nGammas_0, "nGammas_0/I");
  ntupl0->Branch("gammaKE_0", gammaKE_0, "gammaKE_0[nGammas_0]/D");
  ntupl0->Branch("gammaEtot_0", &gammaEtot_0, "gammaEtot_0/D");

  ntupl1 = new TTree("201", "CCD 1");
  ntupl1->Branch("Rnum", &Rnum, "Rnum/I");
  ntupl1->Branch("Evtnum", &Evtnum, "Evtnum/I");
  ntupl1->Branch("nNeutrons_1", &nNeutrons_1, "nNeutrons_1/I");
  ntupl1->Branch("neutronKE_1", neutronKE_1, "neutronKE_1[nNeutrons_1]/D");
  ntupl1->Branch("neutronEtot_1", &neutronEtot_1, "neutronEtot_1/D");
  ntupl1->Branch("nGammas_1", &nGammas_1, "nGammas_1/I");
  ntupl1->Branch("gammaKE_1", gammaKE_1, "gammaKE_1[nGammas_1]/D");
  ntupl1->Branch("gammaEtot_1", &gammaEtot_1, "gammaEtot_1/D");

  ntupl2 = new TTree("301", "CCD 2");
  ntupl2->Branch("Rnum", &Rnum, "Rnum/I");
  ntupl2->Branch("Evtnum", &Evtnum, "Evtnum/I");
  ntupl2->Branch("nNeutrons_2", &nNeutrons_2, "nNeutrons_2/I");
  ntupl2->Branch("neutronKE_2", neutronKE_2, "neutronKE_2[nNeutrons_2]/D");
  ntupl2->Branch("neutronEtot_2", &neutronEtot_2, "neutronEtot_2/D");
  ntupl2->Branch("nGammas_2", &nGammas_2, "nGammas_2/I");
  ntupl2->Branch("gammaKE_2", gammaKE_2, "gammaKE_2[nGammas_2]/D");
  ntupl2->Branch("gammaEtot_2", &gammaEtot_2, "gammaEtot_2/D");

  ntupl3 = new TTree("401", "CCD 3");
  ntupl3->Branch("Rnum", &Rnum, "Rnum/I");
  ntupl3->Branch("Evtnum", &Evtnum, "Evtnum/I");
  ntupl3->Branch("nNeutrons_3", &nNeutrons_3, "nNeutrons_3/I");
  ntupl3->Branch("neutronKE_3", neutronKE_3, "neutronKE_3[nNeutrons_3]/D");
  ntupl3->Branch("neutronEtot_3", &neutronEtot_3, "neutronEtot_3/D");
  ntupl3->Branch("nGammas_3", &nGammas_3, "nGammas_3/I");
  ntupl3->Branch("gammaKE_3", gammaKE_3, "gammaKE_3[nGammas_3]/D");
  ntupl3->Branch("gammaEtot_3", &gammaEtot_3, "gammaEtot_3/D");

  ntupl4 = new TTree("501", "CCD 4");
  ntupl4->Branch("Rnum", &Rnum, "Rnum/I");
  ntupl4->Branch("Evtnum", &Evtnum, "Evtnum/I");
  ntupl4->Branch("nNeutrons_4", &nNeutrons_4, "nNeutrons_4/I");
  ntupl4->Branch("neutronKE_4", neutronKE_4, "neutronKE_4[nNeutrons_4]/D");
  ntupl4->Branch("neutronEtot_4", &neutronEtot_4, "neutronEtot_4/D");
  ntupl4->Branch("nGammas_4", &nGammas_4, "nGammas_4/I");
  ntupl4->Branch("gammaKE_4", gammaKE_4, "gammaKE_4[nGammas_4]/D");
  ntupl4->Branch("gammaEtot_4", &gammaEtot_4, "gammaEtot_4/D");

  G4cout << "\n----> Histogram file is opened in " << fileName << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::save()
{ 
  if (rootFile) {
    rootFile->Write();        // Writing the histograms to the file
    rootFile->Close();        // and closing the tree (and the file)
    G4cout << "\n----> Histogram Tree is saved \n" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
	   << G4endl;
    return;
  }
  if  (histo[ih]) { histo[ih]->Fill(xbin, weight); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Normalize(G4int ih, G4double fac)
{
  if (ih >= MaxHisto) {
    G4cout << "---> warning from HistoManager::Normalize() : histo " << ih
           << " does not exist. (fac=" << fac << ")" << G4endl;
    return;
  }
  if (histo[ih]) histo[ih]->Scale(fac);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtuple0(G4int RunNum_arg, G4int Evtnum_arg,
                               G4int nNeutrons_arg, G4double* neutronKE_arg, G4double neutronEtot_arg,
                               G4int nGammas_arg, G4double* gammaKE_arg, G4double gammaEtot_arg)
{
  Rnum = RunNum_arg;
  Evtnum = Evtnum_arg;

  nNeutrons_0 = nNeutrons_arg;
  if ( nNeutrons_arg < MXPART ) {
    for (int i=0; i<nNeutrons_0; i++) {
      neutronKE_0[i]=neutronKE_arg[i];
    }
  }
  neutronEtot_0 = neutronEtot_arg;
 
  nGammas_0 = nGammas_arg;
  if ( nGammas_arg < MXPART ) {
    for (int i=0; i<nGammas_0; i++) {
      gammaKE_0[i]=gammaKE_arg[i];
    }
  }
  gammaEtot_0 = gammaEtot_arg;

  if (ntupl0) ntupl0->Fill();
}

void HistoManager::FillNtuple1(G4int RunNum_arg, G4int Evtnum_arg,
                               G4int nNeutrons_arg, G4double* neutronKE_arg, G4double neutronEtot_arg,
                               G4int nGammas_arg, G4double* gammaKE_arg, G4double gammaEtot_arg)
{
  Rnum = RunNum_arg;
  Evtnum = Evtnum_arg;

  nNeutrons_1 = nNeutrons_arg;
  if ( nNeutrons_arg < MXPART ) {
    for (int i=0; i<nNeutrons_1; i++) {
      neutronKE_1[i]=neutronKE_arg[i];
    }
  }
  neutronEtot_1 = neutronEtot_arg;
 
  nGammas_1 = nGammas_arg;
  if ( nGammas_arg < MXPART ) {
    for (int i=0; i<nGammas_1; i++) {
      gammaKE_0[i]=gammaKE_arg[i];
    }
  }
  gammaEtot_1 = gammaEtot_arg;

  if (ntupl1) ntupl1->Fill();
}

void HistoManager::FillNtuple2(G4int RunNum_arg, G4int Evtnum_arg,
                               G4int nNeutrons_arg, G4double* neutronKE_arg, G4double neutronEtot_arg,
                               G4int nGammas_arg, G4double* gammaKE_arg, G4double gammaEtot_arg)
{
  Rnum = RunNum_arg;
  Evtnum = Evtnum_arg;

  nNeutrons_2 = nNeutrons_arg;
  if ( nNeutrons_arg < MXPART ) {
    for (int i=0; i<nNeutrons_2; i++) {
      neutronKE_2[i]=neutronKE_arg[i];
    }
  }
  neutronEtot_2 = neutronEtot_arg;
 
  nGammas_2 = nGammas_arg;
  if ( nGammas_arg < MXPART ) {
    for (int i=0; i<nGammas_2; i++) {
      gammaKE_2[i]=gammaKE_arg[i];
    }
  }
  gammaEtot_2 = gammaEtot_arg;

  if (ntupl2) ntupl2->Fill();
}

void HistoManager::FillNtuple3(G4int RunNum_arg, G4int Evtnum_arg,
                               G4int nNeutrons_arg, G4double* neutronKE_arg, G4double neutronEtot_arg,
                               G4int nGammas_arg, G4double* gammaKE_arg, G4double gammaEtot_arg)
{
  Rnum = RunNum_arg;
  Evtnum = Evtnum_arg;

  nNeutrons_3 = nNeutrons_arg;
  if ( nNeutrons_arg < MXPART ) {
    for (int i=0; i<nNeutrons_3; i++) {
      neutronKE_3[i]=neutronKE_arg[i];
    }
  }
  neutronEtot_3 = neutronEtot_arg;
 
  nGammas_3 = nGammas_arg;
  if ( nGammas_arg < MXPART ) {
    for (int i=0; i<nGammas_3; i++) {
      gammaKE_3[i]=gammaKE_arg[i];
    }
  }
  gammaEtot_3 = gammaEtot_arg;

  if (ntupl3) ntupl3->Fill();
}

void HistoManager::FillNtuple4(G4int RunNum_arg, G4int Evtnum_arg,
                               G4int nNeutrons_arg, G4double* neutronKE_arg, G4double neutronEtot_arg,
                               G4int nGammas_arg, G4double* gammaKE_arg, G4double gammaEtot_arg)
{
  Rnum = RunNum_arg;
  Evtnum = Evtnum_arg;

  nNeutrons_4 = nNeutrons_arg;
  if ( nNeutrons_arg < MXPART ) {
    for (int i=0; i<nNeutrons_4; i++) {
      neutronKE_4[i]=neutronKE_arg[i];
    }
  }
  neutronEtot_4 = neutronEtot_arg;
 
  nGammas_4 = nGammas_arg;
  if ( nGammas_arg < MXPART ) {
    for (int i=0; i<nGammas_4; i++) {
      gammaKE_4[i]=gammaKE_arg[i];
    }
  }
  gammaEtot_4 = gammaEtot_arg;

  if (ntupl4) ntupl4->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintStatistic()
{
}


#ifndef HistoManager_h
#define HistoManager_h 1
#define MXPART 100
#define NCCD 5

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TFile;
class TTree;
class TH1D;

const G4int MaxHisto = 50;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager
{
  public:
  
    HistoManager();
   ~HistoManager();
   
    void book();
    void save();

    void FillHisto(G4int id, G4double bin, G4double weight = 1.0);
    void Normalize(G4int id, G4double fac);    

    void FillNtuple0(G4int, G4int,
                     G4int, G4double*, G4double,
                     G4int, G4double*, G4double);
    
    void FillNtuple1(G4int, G4int,
                     G4int, G4double*, G4double,
                     G4int, G4double*, G4double);

    void FillNtuple2(G4int, G4int,
                     G4int, G4double*, G4double,
                     G4int, G4double*, G4double);

    void FillNtuple3(G4int, G4int,
                     G4int, G4double*, G4double,
                     G4int, G4double*, G4double);

    void FillNtuple4(G4int, G4int,
                     G4int, G4double*, G4double,
                     G4int, G4double*, G4double);

    void PrintStatistic();
        
  private:
  
    TFile*   rootFile;
    TH1D*    histo[MaxHisto];            
    TTree*   ntupl0;    
    TTree*   ntupl1;    
    TTree*   ntupl2;    
    TTree*   ntupl3;    
    TTree*   ntupl4;    

    G4int    Rnum;
    G4int    Evtnum;

    G4int    nCCD;

    G4int    nNeutrons[NCCD];
    G4double neutronKE[NCCD][MXPART];
    G4double neutronEtot[NCCD];

    G4int    nGammas[NCCD];
    G4double gammaKE[NCCD][MXPART];
    G4double gammaEtot[NCCD];

    G4int    nNeutrons_0;
    G4double neutronKE_0[MXPART];
    G4double neutronEtot_0;

    G4int    nGammas_0;
    G4double gammaKE_0[MXPART];
    G4double gammaEtot_0;

    G4int    nNeutrons_1;
    G4double neutronKE_1[MXPART];
    G4double neutronEtot_1;

    G4int    nGammas_1;
    G4double gammaKE_1[MXPART];
    G4double gammaEtot_1;

    G4int    nNeutrons_2;
    G4double neutronKE_2[MXPART];
    G4double neutronEtot_2;

    G4int    nGammas_2;
    G4double gammaKE_2[MXPART];
    G4double gammaEtot_2;

    G4int    nNeutrons_3;
    G4double neutronKE_3[MXPART];
    G4double neutronEtot_3;

    G4int    nGammas_3;
    G4double gammaKE_3[MXPART];
    G4double gammaEtot_3;

    G4int    nNeutrons_4;
    G4double neutronKE_4[MXPART];
    G4double neutronEtot_4;

    G4int    nGammas_4;
    G4double gammaKE_4[MXPART];
    G4double gammaEtot_4;
};

#endif


#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1
#include "G4VUserPrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SPSEneDistribution.hh"
#include "globals.hh"

//class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;
class DetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction(DetectorConstruction*);    
  virtual ~PrimaryGeneratorAction();
  void GeneratePrimaries(G4Event*);
  G4double GetBeamEnergy() {return BeamEnergy;};

private:
  DetectorConstruction*    Detector;		// pointer to the geometry
  G4GeneralParticleSource* particleGun;		// pointer to G4 general particle generator class
  G4double                 BeamEnergy;		// beam energy
  G4SPSEneDistribution*    eneDist;		// pointer to generator energy distribution
  G4SPSAngDistribution*    angDist;		// pointer to generator angular distribution
  G4SPSPosDistribution*    posDist;		// pointer to generator position distribution
};

#endif


#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC)
:Detector(DC)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  particleGun = new G4GeneralParticleSource();

  eneDist = particleGun->GetCurrentSource()->GetEneDist();
  eneDist->SetEnergyDisType("User");

  angDist = particleGun->GetCurrentSource()->GetAngDist();
  angDist->SetAngDistType("iso");

  posDist = particleGun->GetCurrentSource()->GetPosDist();
  posDist->SetPosDisType("Plane");
  posDist->SetPosDisShape("Circle");

  G4double Xpos = Detector->GetParticleGunX();
  G4double Ypos = Detector->GetParticleGunY();
  G4double Zpos = Detector->GetParticleGunZ();
  posDist->SetCentreCoords(G4ThreeVector(Xpos, Ypos, Zpos));

#if CCDGUN==1
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="neutron");	// neutrons
#else
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="e-");		// primary beam
#endif

  particleGun->SetParticleDefinition(particle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  // 

#if CCDGUN==1
  BeamEnergy = 5.0*MeV;						// monoenergetic
  eneDist->UserEnergyHisto(G4ThreeVector(BeamEnergy, 1., 0.));
/*
  eneDist->UserEnergyHisto(G4ThreeVector(-0.079*MeV, 0., 0.));		// test...
  eneDist->UserEnergyHisto(G4ThreeVector(0.079*MeV, 1073., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(0.237*MeV, 750., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(0.395*MeV, 594., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(0.553*MeV, 509., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(0.711*MeV, 409., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(0.869*MeV, 315., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(1.027*MeV, 253., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(1.185*MeV, 181., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(1.343*MeV, 124., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(1.501*MeV, 128., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(1.659*MeV, 85., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(1.817*MeV, 87., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(1.975*MeV, 65., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(2.133*MeV, 67., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(2.291*MeV, 42., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(2.449*MeV, 47., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(2.607*MeV, 38., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(2.765*MeV, 42., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(2.923*MeV, 43., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(3.081*MeV, 46., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(3.239*MeV, 42., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(3.397*MeV, 36., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(3.555*MeV, 36., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(3.713*MeV, 44., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(3.871*MeV, 31., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(4.029*MeV, 32., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(4.187*MeV, 36., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(4.345*MeV, 34., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(4.503*MeV, 28., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(4.661*MeV, 31., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(4.819*MeV, 27., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(4.977*MeV, 37., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(5.135*MeV, 25., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(5.293*MeV, 24., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(5.451*MeV, 40., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(5.609*MeV, 27., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(5.767*MeV, 28., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(5.925*MeV, 34., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(6.083*MeV, 30., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(6.241*MeV, 33., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(6.399*MeV, 24., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(6.557*MeV, 25., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(6.715*MeV, 25., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(6.873*MeV, 34., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(7.031*MeV, 20., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(7.189*MeV, 25., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(7.347*MeV, 31., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(7.505*MeV, 18., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(7.663*MeV, 22., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(7.821*MeV, 12., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(7.979*MeV, 15., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(8.137*MeV, 16., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(8.295*MeV, 17., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(8.453*MeV, 13., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(8.611*MeV, 16., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(8.769*MeV, 7., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(8.927*MeV, 6., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(9.085*MeV, 4., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(9.243*MeV, 6., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(9.401*MeV, 7., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(9.559*MeV, 4., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(9.717*MeV, 5., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(9.875*MeV, 2., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(10.033*MeV, 4., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(10.191*MeV, 1., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(10.349*MeV, 2., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(10.507*MeV, 1., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(10.665*MeV, 1., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(10.823*MeV, 4., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(10.981*MeV, 3., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(11.139*MeV, 1., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(11.297*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(11.455*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(11.613*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(11.771*MeV, 1., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(11.929*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(12.087*MeV, 2., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(12.245*MeV, 1., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(12.403*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(12.561*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(12.719*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(12.877*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(13.035*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(13.193*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(13.351*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(13.509*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(13.667*MeV, 1., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(13.825*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(13.983*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(14.141*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(14.299*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(14.457*MeV, 1., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(14.615*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(14.773*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(14.931*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(15.089*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(15.247*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(15.405*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(15.563*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(15.721*MeV, 0., 0.));
  eneDist->UserEnergyHisto(G4ThreeVector(15.879*MeV, 0., 0.));
*/
  angDist->SetMinTheta(180.0*deg);				// along Y direction
  angDist->SetMaxTheta(180.0*deg);
  angDist->SetMinPhi(0.0*deg);
  angDist->SetMaxPhi(0.0*deg);
  posDist->SetPosRot1(G4ThreeVector(1,0,0));
  posDist->SetPosRot2(G4ThreeVector(0,0,1));
  posDist->SetRadius0(0.0*mm);					// circular distribution
  G4double CCDsize = Detector->GetCCDXZsize();			// CCD minimum face size
  posDist->SetRadius(0.8*CCDsize/2);				// fully contained...
#else
  BeamEnergy = 75.0*MeV;					// primary electron beam 
  eneDist->UserEnergyHisto(G4ThreeVector(BeamEnergy, 1., 0.));
  angDist->SetMinTheta(0.0*deg);				// along X direction
  angDist->SetMaxTheta(0.0*deg);
  angDist->SetMinPhi(0.0*deg);
  angDist->SetMaxPhi(0.0*deg);
  posDist->SetPosRot1(G4ThreeVector(0,1,0));
  posDist->SetPosRot2(G4ThreeVector(0,0,1));
  posDist->SetRadius0(0.0*mm);					// circular distribution
  posDist->SetRadius(0.5*mm);					// 1 mm diameter
#endif

  particleGun->GeneratePrimaryVertex(anEvent);
}

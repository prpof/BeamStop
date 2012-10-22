
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "HistoManager.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"

#define MXNT 100	// maximum number of neutron tracks in CCD

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* det,
                               HistoManager* histo,
                               EventAction* evt)
:detector(det), histomanager(histo), eventaction(evt)					 
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // initialize some variables for each new event
  static G4int oEvt = -1;
  static G4int totalEventCharge = 0;
  static G4int nNeutrons[NCCD];
  static G4int nGammas[NCCD];
  static G4int NneutronTracks[NCCD];
  static G4int neutronTrack[NCCD][MXNT];

  G4int myEvt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  if ( myEvt != oEvt ) {
    oEvt = myEvt;
    totalEventCharge = 0;
    for (int i=0; i<NCCD; i++) {
      nNeutrons[i]=0;
      nGammas[i]=0;
      NneutronTracks[i] = 0;
      for (int j=0; j<MXNT; j++) {
        neutronTrack[i][j] = -1;
      }
    }
    /*
    G4ThreeVector trackVertex = aStep->GetTrack()->GetVertexPosition();
    G4cout << "vertex = " << trackVertex.x() << ", " 
                          << trackVertex.y() << ", " 
                          << trackVertex.z() << G4endl;
    */
  }

  // get volume of the current step
  G4VPhysicalVolume* volume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
   G4String volumeName = volume->GetName();

  // keep a running sum of the charge entering and leaving the faraday cup
  if ( volumeName == "Beam Stop" ) {
    G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();
    G4int particleCharge = particle->GetPDGCharge();
    G4StepPoint* point1 = aStep->GetPreStepPoint();
    G4StepPoint* point2 = aStep->GetPostStepPoint();
    if ( particleCharge != 0 ) {
      if ( point1->GetStepStatus() == fGeomBoundary ) {		// entering the faraday cup
        eventaction->AddCharge(particleCharge);
      }
      else if ( point2->GetStepStatus() == fGeomBoundary ) {	// leaving the faraday cup
        eventaction->AddCharge(-1*particleCharge);
      }
    }
  }

  // keep track of charged particles leaving, separately for forward, backward, and transverse
  if ( volumeName == "Beam Stop" || volumeName == "Beam Stop Hole" ) {
    G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();
    G4int particleCharge = particle->GetPDGCharge();
    G4StepPoint* point2 = aStep->GetPostStepPoint();
    if ( particleCharge != 0 ) {
      if ( point2->GetStepStatus() == fGeomBoundary ) {	// leaving the faraday cup
        G4double delta = 0.1*mm;
        G4ThreeVector pos2 = point2->GetPosition();
        G4double BeamLineZ = detector->GetBeamLineZ();
        G4double BeamStopLen = detector->GetBeamStopLen();
        G4double BeamStopOD = detector->GetBeamStopOD();
        G4double pos2R = sqrt( pow(pos2.y(),2) + pow(pos2.z()-BeamLineZ,2) );
        if ( abs(pos2.x()-BeamStopLen/2) < delta && volumeName == "Beam Stop" ) {
          eventaction->AddLeakage(1);
          //G4cout << "event " << myEvt << ": leaving by the back door" << G4endl;
          //G4cout << "running charge = " << totalEventCharge << G4endl;
        }
        else if ( abs(pos2.x()+BeamStopLen/2) < delta ) {
          eventaction->AddLeakage(2);
          //G4cout << "event " << myEvt << ": leaving by the front door" << G4endl;
          //G4cout << "running charge = " << totalEventCharge << G4endl;
        }
        else if ( abs(pos2R-BeamStopOD/2) < delta && volumeName == "Beam Stop" ) {
          eventaction->AddLeakage(3);
          //G4cout << "event " << myEvt << ": leaving by the side door" << G4endl;
          //G4cout << "running charge = " << totalEventCharge << G4endl;
        }
        else {
          // must be going between the Beam Stop Hole and the Beam Stop...
          //G4cout << "where am I leaving from???" << G4endl;
          //G4cout << "beam stop len/2 = " << BeamStopLen/2 << G4endl;
          //G4cout << "beam stop radius = " << BeamStopOD/2 << G4endl;
          //G4cout << "position = " << pos2.x() << ", " << pos2.y() << ", " << pos2.z() << G4endl;
          //G4cout << "step radius = " << pos2R << G4endl;
        }
      }
    }
  }

  // check if we're in a CCD 
  if ( volumeName == "CCD" ) {

    // get the current event number
    //G4int myEvt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // get the current track and parent ID's
    G4int trackID = aStep->GetTrack()->GetTrackID();
    G4int parentID = aStep->GetTrack()->GetParentID();

    // get the vertex position of the current track
    G4ThreeVector trackVertex = aStep->GetTrack()->GetVertexPosition();

    // get the particle type
    G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();

    // get the pre step point
    G4StepPoint* point1 = aStep->GetPreStepPoint();

    // get the kinetic energy
    G4double particleKE = point1->GetKineticEnergy();

    // get the total energy deposited; includes energy lost by secondaries which have 
    // NOT been generated because each of their energies was below the cut threshold
    G4double eDeposited = aStep->GetTotalEnergyDeposit();

    // get the copy number of great-great-grandmother camera box
    G4TouchableHandle touch1 = point1->GetTouchableHandle();
    G4int nCCD = touch1->GetCopyNumber(4);

    if ( particleName == "neutron" || particleName == "proton" ) {
      if (point1->GetStepStatus() == fGeomBoundary) {	// just entered CCD
        nNeutrons[nCCD]++;
        eventaction->AddNeutronKE(particleKE,nCCD,nNeutrons[nCCD]);
      }

      // check if this neutron is in the list
      G4int newTrack = 1;
      for (int i=0; i<NneutronTracks[nCCD]; i++) {
        if ( trackID == neutronTrack[nCCD][i] ) {
          newTrack = 0;
          break;
        }
      }

      // add the track to the list if it's new
      if ( newTrack == 1 ) {
        neutronTrack[nCCD][NneutronTracks[nCCD]] = trackID;
        if (NneutronTracks[nCCD] < MXNT) NneutronTracks[nCCD]++;
      }

      // increment the neutron energy deposited
      eventaction->AddNeutronEdep(eDeposited,nCCD);

    } else {  // not a neutron or proton (but possibly a neutron/proton daughter...)

      if (point1->GetStepStatus() == fGeomBoundary) {	// just entered CCD
        nGammas[nCCD]++;
        eventaction->AddGammaKE(particleKE,nCCD,nGammas[nCCD]);
      }

      G4int newTrack = 1;
      G4int neutronDaughter = 0;
      // check if this track was created in the CCD
      if ( detector->CheckCCDVolume(trackVertex, nCCD) ) {
        // check if this is a neutron offspring
        for (int i=0; i<NneutronTracks[nCCD]; i++) {
          if ( parentID == neutronTrack[nCCD][i] ) {
            // check if the offspring track is in the list
            for (int i=0; i<NneutronTracks[nCCD]; i++) {
              if ( trackID == neutronTrack[nCCD][i] ) {
                newTrack = 0;
                break;
              }
            }
            neutronDaughter = 1;
            break;
          }
        }
      } else newTrack = 0;

      // add the track to the list if it's new
      if ( newTrack == 1 ) {
        neutronTrack[nCCD][NneutronTracks[nCCD]] = trackID;
        if (NneutronTracks[nCCD] < MXNT) NneutronTracks[nCCD]++;
      }

      // increment the appropriate counter the energy deposited
      if ( neutronDaughter == 1 ) {
        eventaction->AddNeutronEdep(eDeposited,nCCD);
      } else {
        eventaction->AddGammaEdep(eDeposited,nCCD);
        /*
        G4cout << "gamma energy " << eDeposited << " in event " << myEvt << G4endl;
        G4cout << "parent track ID = " << parentID << G4endl;
        G4cout << "track ID = " << trackID << G4endl;
        G4cout << "track vertex = "
               << trackVertex.x() << ", "
               << trackVertex.y() << ", "
               << trackVertex.z() << G4endl;
        G4cout << "particle = " << particleName << G4endl;
        G4cout << "neutronTracks = ";
        for (int i=0; i<NneutronTracks[nCCD]; i++) {
          G4cout << neutronTrack[nCCD][i] << " ";
        }
        G4cout << G4endl;
        */
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

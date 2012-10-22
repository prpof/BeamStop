
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "HistoManager.hh"

#include "G4PhysListFactory.hh"
#include "QGSP_BERT_HP.hh"	// QGSP_BERT used by ATLAS
#include "QGSP_BIC_HP.hh"	// QGSP_BIC used by CMS
#include "Shielding.hh"		// thermal neutrons in Boron seem not to be handled...

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

/*
  ToDo:

*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{     
  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  //
  DetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);

  //
  //PhysicsList* physics = new PhysicsList;
  //runManager->SetUserInitialization(physics);

  // Use the prepackaged Shielding physics list
  G4PhysListFactory *physListFactory = new G4PhysListFactory();
  G4VUserPhysicsList *physicsList = physListFactory->GetReferencePhysList("QGSP_BERT_HP");
  //G4VUserPhysicsList *physicsList = physListFactory->GetReferencePhysList("QGSP_BIC_HP");
  //G4VUserPhysicsList *physicsList = physListFactory->GetReferencePhysList("Shielding");
  runManager->SetUserInitialization(physicsList);

  // set an HistoManager
  //
  HistoManager* histo = new HistoManager();

  // set a DetectorConstruction
  //
  DetectorConstruction* det = new DetectorConstruction();
      
  // Set user action classes
  //
  PrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction(detector);
  runManager->SetUserAction(gen_action);
  //
  RunAction* run_action = new RunAction(histo,det,gen_action);  
  runManager->SetUserAction(run_action);
  //
  EventAction* event_action = new EventAction(run_action, histo, det);
  runManager->SetUserAction(event_action);
  //
  SteppingAction* stepping_action = new SteppingAction(detector, histo, event_action);
  runManager->SetUserAction(stepping_action);
  
  // Initialize G4 kernel
  //
  runManager->Initialize();
  
  // Get the pointer to the User Interface manager
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  
  if (argc!=1)   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);    
    }
  else
    {  // interactive mode : define visualization and UI terminal
#ifdef G4VIS_USE
      G4VisManager* visManager = new G4VisExecutive;
      visManager->Initialize();
#endif

#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    #ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");     
    #endif
    ui->SessionStart();
    delete ui;
#endif

#ifdef G4VIS_USE
      delete visManager;
#endif
    }
  
  // Job termination
  histo->save();		// write and close the histogram file
  delete histo;                
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

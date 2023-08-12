#include "G4Types.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "NTPhysicsList.hh"
#include "NTDetectorConstruction.hh"
#include "NTActionInitialization.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " NT [-m macro ] [-u UIsession] [-t nThreads] [-r seed] "
           << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

int main(int argc,char** argv) {
  // Evaluate arguments
  //
  if ( argc > 13 ) {
    PrintUsage();
    return 1;
  }

  G4String macro;
  G4String session;
  G4String outFileName;
  G4String dataStoreMode = "onlyHit";
#ifdef G4MULTITHREADED
  G4int nThreads = 1;
#endif

  G4long myseed = 345354;
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro   = argv[i+1];
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
    else if ( G4String(argv[i]) == "-r" ) myseed  = atoi(argv[i+1]);
    else if ( G4String(argv[i]) == "-o" ) outFileName = argv[i+1];
   // else if ( G4String(argv[i]) == "-d" ) dataStoreMode = argv[i+1];
#ifdef G4MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) {
      nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }

  // Instantiate G4UIExecutive if inetractive mode
  G4UIExecutive* ui = nullptr;
  if ( macro.size() == 0 ) {
    ui = new G4UIExecutive(argc, argv);
    outFileName = "visual_mode_run";
    dataStoreMode = "none";
  } else {
    // do nothing
  }
  
  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  if ( nThreads > 0 ) runManager->SetNumberOfThreads(nThreads);
#else
  G4RunManager * runManager = new G4RunManager;
#endif
  
  // Seed the random number generator manually
  G4Random::setTheSeed(myseed);
  
  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new NTDetectorConstruction(outFileName));
  // Physics list
  runManager->SetUserInitialization(new NTPhysicsList);
  
  // User action initialization
  // runManager->SetUserInitialization(new NTActionInitialization(pAnalysis));
  runManager->SetUserInitialization(new NTActionInitialization(outFileName,dataStoreMode));
  
  // Initialize visualization
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  
  if ( macro.size() ) {
    // Batch mode
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  } else {// Define UI session for interactive mode
    UImanager->ApplyCommand("/control/execute vis.mac");
    if (ui->IsGUI())
      UImanager->ApplyCommand("/control/execute gui.mac");
    ui->SessionStart();
    delete ui;
  }
  
  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
  
  delete visManager;
  delete runManager;
  
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

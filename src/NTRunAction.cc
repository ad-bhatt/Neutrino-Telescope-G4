#include "G4Timer.hh"

#include "NTRunAction.hh"

#include "G4Run.hh"

NTRunAction::NTRunAction(G4String fname, G4String strMode)
  : G4UserRunAction(),
    fTimer(0) {
  fTimer = new G4Timer;
  outFilePrefix = fname;
  dataStoreMode = strMode;
  saveHit = false;
  savePhoton = false;
  if(dataStoreMode == "all") {
    savePhoton = true;
    saveHit = true;
  } else if(dataStoreMode == "onlyPhoton") {
    savePhoton = true;
  } else if(dataStoreMode == "onlyHit") {
    saveHit = true;
  }
    
  mcEvt = new MCEvent(outFilePrefix);
  mcPhoton = new MCOpticalPhoton(outFilePrefix);
  
}

NTRunAction::NTRunAction()
  : G4UserRunAction(),
    fTimer(0) {
  fTimer = new G4Timer;
}

NTRunAction::~NTRunAction() {
  delete fTimer;
  delete mcEvt;
  delete mcPhoton;
}

void NTRunAction::BeginOfRunAction(const G4Run* aRun) {

  int threadID;
  
#ifdef G4MULTITHREADED
  threadID = G4Threading::G4GetThreadId();
#else
  threadID = 0;
#endif
  
  int runID = aRun->GetRunID();

  mcEvt->SetThreadID(threadID);
  mcEvt->SetRunID(runID);
  mcPhoton->SetThreadID(threadID);
  mcPhoton->SetRunID(runID);
  
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  fTimer->Start();
}

void NTRunAction::EndOfRunAction(const G4Run* aRun) {
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;
}



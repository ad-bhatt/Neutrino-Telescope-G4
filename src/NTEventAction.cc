#include "NTEventAction.hh"

NTEventAction::NTEventAction(NTRunAction* rAct)
: G4UserEventAction(), fMessenger(nullptr) {
  printInfo = 1000;
  runAct = rAct;
  mcEvt = rAct->GetMCEvent();
  mcPhoton = rAct->GetOpticalPhoton();
  // DefineCommands();
} 

NTEventAction::~NTEventAction() {
}

void NTEventAction::BeginOfEventAction(const G4Event* anEvent) {

  int evtID = anEvent->GetEventID();
  mcEvt->SetEventID(evtID);
  mcPhoton->SetEventID(evtID);
  if(runAct->isSavePhoton())
    mcPhoton->OpenNewBinaryFile();
  G4cout << "Begin of Event: " << evtID << " ---> " << G4endl;
  
}

void NTEventAction::EndOfEventAction(const G4Event* anEvent) {
  mcEvt->Print("particles");
  mcEvt->CollateAndPrintHits();
  mcEvt->WriteToBinary();
  mcEvt->Clear();
  mcPhoton->Clear();
  G4cout << "End of Event: " << anEvent->GetEventID() << G4endl;
}

void NTEventAction::DefineCommands() {
  fMessenger 
    = new G4GenericMessenger(this, 
                             "/NT/event/", 
                             "Event Action control");
  
  auto& printInfoCmd
    = fMessenger->DeclareMethod("printInfo",
				&NTEventAction::SetPrintInfo,
				"Set Print Command (int)");
  printInfoCmd.SetParameterName("printInfo", true);
  printInfoCmd.SetDefaultValue("1000");
}



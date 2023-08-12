#include "NTActionInitialization.hh"

NTActionInitialization::NTActionInitialization(G4String fname, G4String strMode)
 : G4VUserActionInitialization() {
  outFilePrefix = fname;
  dataStoreMode = strMode;
}

// NTActionInitialization::NTActionInitialization(NTSimAnalysis* panal)
//  : G4VUserActionInitialization() {
//   pAnalysis = panal;
// }

NTActionInitialization::~NTActionInitialization() {
  
}

void NTActionInitialization::BuildForMaster() const {
  SetUserAction(new NTRunAction(outFilePrefix,dataStoreMode));
}

void NTActionInitialization::Build() const {

  SetUserAction(new NTPrimaryGeneratorAction());
  NTRunAction* runAct = new NTRunAction(outFilePrefix,dataStoreMode);
  SetUserAction(runAct);
  SetUserAction(new NTEventAction(runAct));
  SetUserAction(new NTSteppingAction(runAct));
  SetUserAction(new NTStackingAction(runAct));
}

// void NTActionInitialization::BuildForMaster() const {
//   SetUserAction(new NTRunAction(pAnalysis));
// }

// void NTActionInitialization::Build() const {
//   SetUserAction(new NTPrimaryGeneratorAction(pAnalysis));
//   SetUserAction(new NTRunAction(pAnalysis));
//   SetUserAction(new NTEventAction(pAnalysis));
//   SetUserAction(new NTSteppingAction(pAnalysis));
//   SetUserAction(new NTStackingAction(pAnalysis));
// }


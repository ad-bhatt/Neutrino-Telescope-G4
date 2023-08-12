#ifndef NTRunAction_h
#define NTRunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include "MCEvent.hh"
#include "G4Threading.hh"

class G4Timer;
class G4Run;

class NTRunAction : public G4UserRunAction {
public:
  NTRunAction(G4String fname, G4String strMode);
  NTRunAction();
  virtual ~NTRunAction();
  
  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void EndOfRunAction(const G4Run* aRun);
  MCEvent* GetMCEvent() const {return mcEvt;};
  MCOpticalPhoton* GetOpticalPhoton() const {return mcPhoton;};
  G4String GetDataStoringMode() const {return dataStoreMode;};
  bool isSavePhoton() const {return savePhoton;};
  bool isSaveHit() const {return saveHit;};
  
private:
  MCEvent* mcEvt;
  MCOpticalPhoton* mcPhoton;  
  G4Timer* fTimer;
  G4String outFilePrefix;
  G4String dataStoreMode;
  bool savePhoton;
  bool saveHit;
};

#endif


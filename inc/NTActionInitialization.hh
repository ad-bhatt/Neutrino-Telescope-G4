#ifndef NTActionInitialization_h
#define NTActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "NTPrimaryGeneratorAction.hh"
#include "NTRunAction.hh"
#include "NTEventAction.hh"
#include "NTSteppingAction.hh"
#include "NTStackingAction.hh"
#include <iostream>
#include <vector>
using namespace std;

class NTActionInitialization : public G4VUserActionInitialization {
public:
  NTActionInitialization(G4String fname, G4String strMode);
  virtual ~NTActionInitialization();
  
  virtual void BuildForMaster() const;
  virtual void Build() const;

  
private:
  G4String outFilePrefix;
  G4String dataStoreMode;
};

#endif

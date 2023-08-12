#ifndef NTEventAction_h
#define NTEventAction_h 1

#include <iostream>
#include "G4UserEventAction.hh"
#include "NTRunAction.hh"
#include "MCEvent.hh"
#include "G4GenericMessenger.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"
#include "NTRunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "MCEvent.hh"

using namespace std;

class NTEventAction : public G4UserEventAction {
public:
  NTEventAction(NTRunAction* rAct);
  virtual ~NTEventAction();

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

  void SetPrintInfo(int x) {printInfo=x;};
  
private:
  NTRunAction* runAct;
  MCEvent* mcEvt;
  MCOpticalPhoton* mcPhoton;
  void DefineCommands();
  G4GenericMessenger* fMessenger;
  int printInfo;
};

#endif

    

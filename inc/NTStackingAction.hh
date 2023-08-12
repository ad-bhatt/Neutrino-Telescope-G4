#ifndef NTStackingAction_H
#define NTStackingAction_H 1

#include "globals.hh"
#include <iostream>
#include <vector>
#include "G4UserStackingAction.hh"
#include "NTRunAction.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"
#include "MCEvent.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Timer.hh"
using namespace std;

class NTStackingAction : public G4UserStackingAction {
public:
  NTStackingAction(NTRunAction* rAct);
  virtual ~NTStackingAction();
  
public:
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void NewStage();
  virtual void PrepareNewEvent();
  
private:
  NTRunAction* runAct;
  MCEvent* mcEvt;
  bool printInfo;
  G4Timer* fTimer;
  MCOpticalPhoton* mcPhoton;
  int lastTrackRecorded;
  int iphtrecord;
  int iphtrun;
};

#endif

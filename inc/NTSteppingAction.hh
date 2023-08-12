#ifndef NTSteppingAction_h
#define NTSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "NTRunAction.hh"
#include <iostream>
#include "globals.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "MCEvent.hh"
#include "G4Timer.hh"

using namespace std;
using namespace CLHEP;

class NTSteppingAction : public G4UserSteppingAction {
public:
  NTSteppingAction(NTRunAction* rAct);
  virtual ~NTSteppingAction();
  
  // method from the base class
  virtual void UserSteppingAction(const G4Step*);
  
private:
  void GetAllReplicaNumbers(const G4Step* aStep);
  void PrintTouchableHistory(const G4Step* aStep);
  void GetStepInfo(const G4Step* aStep);
  NTRunAction* runAct;
  MCEvent* mcEvt;
  G4int fPhotonCnt;
  G4int fEventNumber;
  G4int fLastTrack;
  bool printInfo;
  int fHitCnt;
  G4Timer* fTimer;

  // StepInfo
  int parentID;
  int trackID;
  int trackPDGid;
  G4String trackPDGname;
  G4String creatorProc;
  double vtxKE;
  G4ThreeVector vtxPos;
  G4ThreeVector lastPos;
  G4String logVolumeAtVtx;
  int stepnum;
  double trackKE;
  double stepLength;
  G4ThreeVector preStepPos;
  G4ThreeVector postStepPos;
  double TotalEdep;
  G4String pvName;
  float preStepTime;
  float postStepTime;

  // HitPos Info
  int secNo;
  int strNo;
  int OMid;
  G4ThreeVector OmLocalPos;
  
};

#endif

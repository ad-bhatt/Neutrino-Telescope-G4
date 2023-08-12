#ifndef NTStep_h
#define NTStep_h 1

struct partInfo {
  int parentID;
  int trackID;
  int trackPDGid;
  G4String trackPDGname;
  G4String creatorProc;
  double vtxKE;
  G4String logVolumeAtVtx;
};

struct NTStep {
  partInfo particle;
  int stepnum;
  double trackKE;
  double stepLength;
  G4ThreeVector preStepPos;
  G4ThreeVector postStepPos;
  double TotalEdep;
  G4String pvName;
};

#endif


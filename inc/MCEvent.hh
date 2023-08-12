#ifndef MCEvent_h
#define MCEvent_h 1

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "NTConstantGeomPars.hh"
using namespace std;
using namespace constpar;

class MCParticle {
public:
  MCParticle();
  virtual ~MCParticle() {};
  G4ThreeVector GetMomentum() const {return momentum;};
  G4ThreeVector GetVertex() const {return vertex;};
  G4ThreeVector GetLastPos() const {return lastPos;};
  int GetPdgID() const {return pdgID;};
  int GetTrackID() const {return trackID;};
  int GetParentID() const {return parentID;};
  G4String GetCreatorProcess() const
  {return creatorProcess;};
  
  void SetMomentum(G4ThreeVector xx) {momentum = xx;};
  void SetVertex(G4ThreeVector xx) {vertex = xx;};
  void SetLastPos(G4ThreeVector xx) {lastPos = xx;};
  void SetPdgID(int xx) {pdgID = xx;};
  void SetTrackID(int xx) {trackID = xx;};
  void SetParentID(int xx) {parentID = xx;};
  void SetCreatorProcess(G4String xx) {creatorProcess = xx;};
  void WriteToBinary(ofstream* ofmc);
  void WriteValue(int xx);
  void WriteValue(G4ThreeVector vec);
  void Print();
private:
  G4ThreeVector momentum; // in TeV
  G4ThreeVector vertex; // in m
  G4ThreeVector lastPos;
  int pdgID;
  int trackID;
  int parentID;
  G4String creatorProcess;
  ofstream* fOutput;
  
};

class MCOpticalPhoton {
public:
  MCOpticalPhoton();
  MCOpticalPhoton(G4String fname);
  virtual ~MCOpticalPhoton() {};
  void Clear();
  
  int GetThreadID() const {return threadID;};
  int GetRunID() const {return runID;};
  int GetEventID() const {return eventID;};
  float GetPhotonEnergy() const {return energy;};
  G4ThreeVector GetStartPosition() const {return startPos;};
  G4ThreeVector GetEndPosition() const {return endPos;};
  G4String GetProcessName() const {return processName;};
  int GetParentMuonID() const {return parentMuID;};
  int GetParentID() const {return parentID;};
  int GetNParents() const {return nParents;};
  int GetPhotonID() const {return photonID;};

  void SetThreadID(int xx) {threadID = xx;};
  void SetRunID(int xx) {runID = xx;};
  void SetEventID(int xx) {eventID = xx;};
  void SetPhotonEnergy(float xx) {energy = xx;};
  void SetStartPosition(G4ThreeVector xx) {startPos = xx;};
  void SetEndPosition(G4ThreeVector xx) {endPos = xx;};
  void SetProcessName(G4String xx) {processName = xx;};
  void SetParentMuonID(int xx) {parentMuID = xx;};
  void SetParentID(int xx) {parentID = xx;};
  void SetNParents(int xx) {nParents = xx;};
  void SetPhotonID(int xx) {photonID = xx;};
  void WriteToBinary();
  void WriteValue(int xx);
  void WriteValue(float xx);
  void WriteValue(G4ThreeVector vec);
  void OpenNewBinaryFile();
  
private:
  float energy; // in eV
  G4ThreeVector startPos; // in m
  G4ThreeVector endPos; // in m
  G4String processName;
  int parentMuID;
  int parentID;
  int nParents;
  int photonID;
  int threadID;
  int runID;
  int eventID;
  ofstream* binFile;
  G4String outFilePrefix;
};

class MCPhotonHit {
public:
  MCPhotonHit(int xx);
  virtual ~MCPhotonHit() {};
  int GetPhotonID() const {return photonID;};
  float GetArrivalTime() const {return arrivalTime;};
  G4ThreeVector GetOMPosLocal() const {return OMposLocal;};

  void SetArrivalTime(float xx) {arrivalTime = xx;};
  void SetOMPosLocal(G4ThreeVector xx) {OMposLocal = xx;};
  void WriteToBinary(ofstream* ofmc);
  void WriteValue(int xx);
  void WriteValue(float xx);
  void WriteValue(G4ThreeVector vec);
  
private:
  G4ThreeVector OMposLocal; // in cm
  float arrivalTime;
  int photonID;
  ofstream* fOutput;
};

class MCOmHit {
public:
  MCOmHit(int xx);
  virtual ~MCOmHit() {};
  int GetOmID() const {return omID;};
  int GetNPhotonHits() const {return fPhotonHits.size();};
  MCPhotonHit* GetPhotonHit(int xx)
  const {return fPhotonHits[xx];};

  void AddPhotonHit(MCPhotonHit* xx)
  {fPhotonHits.push_back(xx);};
  void WriteToBinary(ofstream* ofmc);
  void WriteValue(int xx);
  void Clear();
  void Print();
private:
  int omID;
  vector<MCPhotonHit*> fPhotonHits;
  ofstream* fOutput;
};

class MCSectionHit {
public:
  MCSectionHit(int xx);
  ~MCSectionHit() {};
  int GetSectionID() const {return sectionID;};
  int GetNOmHits() const {return fOmHits.size();};
  MCOmHit* GetOmHit(int xx)
  const {return fOmHits[xx];};
  
  void AddOmHit(MCOmHit* iOmHit)
  {fOmHits.push_back(iOmHit);};
  void WriteToBinary(ofstream* ofmc);
  void WriteValue(int xx);
  void Clear();
  void Print();

private:
  int sectionID;
  vector<MCOmHit*> fOmHits; 
  ofstream* fOutput;
};

class MCStringHit {
public:
  MCStringHit(int xx);
  ~MCStringHit() {};
  int GetStringID() const {return stringID;};
  int GetNSectionHits() const {return fSectionHits.size();};
  MCSectionHit* GetSectionHit(int xx)
  const {return fSectionHits[xx];};
  
  void AddSectionHit(MCSectionHit* iSectionHit)
  {fSectionHits.push_back(iSectionHit);};
  void WriteToBinary(ofstream* ofmc);
  void WriteValue(int xx);
  void Clear();
  void Print();

private:
  int stringID;
  vector<MCSectionHit*> fSectionHits; 
  ofstream* fOutput;
};

class MCEvent {
public:
  MCEvent();
  MCEvent(G4String fname);

  virtual ~MCEvent() {};

  int GetThreadID() const {return threadID;};
  int GetRunID() const {return runID;};
  int GetEventID() const {return eventID;};
  int GetNParticles() const {return fParticles.size();};
  int GetNStringHits() const {return fStringHits.size();};
  int GetNPhotons() const {return fPhotons.size();};
  int GetPhotonCnt() const {return photonCnt;};
  MCStringHit* GetStringHit(int xx)
  const {return fStringHits[xx];};
  MCParticle* GetParticle(int xx)
  const {return fParticles[xx];};
  int GetOpticalPhoton(int xx)
  const {return fPhotons[xx];};
  
  void SetThreadID(int xx) {threadID = xx;};
  void SetRunID(int xx) {runID = xx;};
  void SetEventID(int xx) {eventID = xx;};
  void AddStringHit(MCStringHit* iStringHit)
  {fStringHits.push_back(iStringHit);};
  void AddPhoton(int iPid, int iPPid)
  {fPhotons.push_back(iPid);
    fPhotonParent.push_back(iPPid);
  };
  void AddParticle(MCParticle* iParticle)
  {fParticles.push_back(iParticle);};
  void Clear();
  void WriteToBinary();
  void WriteValue(int xx);
  void AddPhotonCnt() {photonCnt++;};
  void Print(G4String verbose);
  void PrintParticles();
  void PrintStringHits();
  void CollateAndPrintHits();
  
private:
  int threadID;
  int runID;
  int eventID;
  vector<MCStringHit*> fStringHits; 
  vector<MCParticle*> fParticles;
  vector<int> fPhotons;
  vector<int> fPhotonParent;
  G4String outFilePrefix;
  ofstream* binFile;
  int photonCnt;
  int iPhtHits[nOmStrings][nOmSections][nOpticalModules];
  
};

#endif


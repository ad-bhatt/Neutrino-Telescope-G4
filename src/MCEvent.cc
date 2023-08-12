#include "MCEvent.hh"

MCEvent::MCEvent() {
  eventID = 0;
  runID = 0;
  threadID = 0;
  photonCnt = 0;
  for(int ij=0; ij<nOmStrings; ij++) {
    for(int jk=0; jk<nOmSections; jk++) {
      for(int kl=0; kl<nOpticalModules; kl++) {
	iPhtHits[ij][jk][kl] = 0;
      }
    }
  }
}

MCEvent::MCEvent(G4String fname) {
  outFilePrefix = fname;
  eventID = 0;
  runID = 0;
  threadID = 0;
  photonCnt = 0;
  binFile = new ofstream;
  for(int ij=0; ij<nOmStrings; ij++) {
    for(int jk=0; jk<nOmSections; jk++) {
      for(int kl=0; kl<nOpticalModules; kl++) {
	iPhtHits[ij][jk][kl] = 0;
      }
    }
  }
}

void MCEvent::CollateAndPrintHits() {
  // Collate
  for(int ij=0; ij<GetNStringHits(); ij++) {
    int x1 = fStringHits[ij]->GetStringID();
    for(int jk=0; jk<fStringHits[ij]->GetNSectionHits(); jk++) {
      int y2 = fStringHits[ij]->GetSectionHit(jk)->GetSectionID();
      for(int kl=0; kl<fStringHits[ij]->GetSectionHit(jk)->GetNOmHits(); kl++) {
	int z3 = fStringHits[ij]->GetSectionHit(jk)->GetOmHit(kl)->GetOmID();
	iPhtHits[x1][y2][z3] += fStringHits[ij]->GetSectionHit(jk)->GetOmHit(kl)->GetNPhotonHits();
      }
    }
  }
  
  // Print
  G4cout << "\tAll Strings Hits Display " << G4endl;
  for(int ij=0; ij<nOmStrings; ij++) {
    G4cout << setw(10) << "Str#" << ij;
  }
  G4cout << G4endl;
  for(int jk=nOmSections-1; jk>-1; jk--) {
    for(int kl=nOpticalModules-1; kl>-1; kl--) {
      for(int ij=0; ij<nOmStrings; ij++) {
	G4cout << setw(10) << iPhtHits[ij][jk][kl];
      }
      G4cout << G4endl;
    }
  }
  for(int ij=0; ij<nOmStrings; ij++) {
    G4cout << setw(10) << "Str#" << ij;
  }
  G4cout << G4endl;
}

void MCEvent::Print(G4String verbose) {
  G4cout << "Printing the output of MCEvent::" << G4endl;
  G4cout << "runID        : " << runID << G4endl;
  G4cout << "threadID     : " << threadID << G4endl;
  G4cout << "evtentID     : " << eventID << G4endl;
  G4cout << "NParticles   : " << fParticles.size() << G4endl;
  if(verbose == "all" || verbose == "particles")
    PrintParticles();
  G4cout << "NStringHits  : " << fStringHits.size() << G4endl;
  if(verbose == "all" || verbose == "stringhits")
    PrintStringHits();
  int tSectionHits = 0;
  int tOmHits = 0;
  int tPhHits = 0;
  for(int ij=0; ij<GetNStringHits(); ij++) {
    tSectionHits += fStringHits[ij]->GetNSectionHits();
    for(int jk=0; jk<fStringHits[ij]->GetNSectionHits(); jk++) {
      tOmHits += fStringHits[ij]->GetSectionHit(jk)->GetNOmHits();
      for(int kl=0; kl<fStringHits[ij]->GetSectionHit(jk)->GetNOmHits(); kl++) {
	tPhHits += fStringHits[ij]->GetSectionHit(jk)->GetOmHit(kl)->GetNPhotonHits();
      }
    }
  }
  G4cout << "NSectionHits : " << tSectionHits << G4endl;
  G4cout << "NOmHits      : " << tOmHits << G4endl;
  G4cout << "NPhotonHits  : " << tPhHits << G4endl;
  G4cout << "NPhotons     : " << fPhotons.size() << G4endl;
}

void MCEvent::PrintParticles() {
  G4cout << "Printing Particles...." << G4endl;
  for(unsigned ij=0; ij<fParticles.size(); ij++) {
    G4cout << "Particle : " << ij << " ......" << G4endl;
    fParticles[ij]->Print();
  }
}

void MCEvent::PrintStringHits() {
  G4cout << "Printing StringHits...." << G4endl;
  for(unsigned ij=0; ij<fStringHits.size(); ij++) {
    G4cout << "StringHits : " << ij << " ......" << G4endl;
    fStringHits[ij]->Print();
  }
}

  

void MCEvent::Clear() {
  eventID = -1;
  for(unsigned ij=0; ij<fStringHits.size(); ij++) {
    fStringHits[ij]->Clear();
    delete fStringHits[ij];
  }
  fStringHits.clear();
  for(unsigned ij=0; ij<fParticles.size(); ij++) {
    delete fParticles[ij];
  }
  fParticles.clear();
  fPhotons.clear();
  fPhotonParent.clear();
  photonCnt = 0;
  for(int ij=0; ij<nOmStrings; ij++) {
    for(int jk=0; jk<nOmSections; jk++) {
      for(int kl=0; kl<nOpticalModules; kl++) {
	iPhtHits[ij][jk][kl] = 0;
      }
    }
  }
}

void MCEvent::WriteToBinary() {
  char outFileName[200];
  // G4cout << outFilePrefix << G4endl;
  sprintf(outFileName,"%s_r%i_t%i_evt%i.bin",outFilePrefix.data(),runID,threadID,eventID);
  binFile->open(outFileName,std::ios::out | std::ios::binary);
  WriteValue(runID);
  WriteValue(threadID);
  WriteValue(eventID);
  int nstrhit = fStringHits.size();
  WriteValue(nstrhit);
  for(int ij=0; ij<nstrhit; ij++)
    fStringHits[ij]->WriteToBinary(binFile);
  int nparticle = fParticles.size();
  WriteValue(nparticle);
  for(int ij=0; ij<nparticle; ij++)
    fParticles[ij]->WriteToBinary(binFile);
  
  binFile->close();
}

void MCEvent::WriteValue(int xx) {
  binFile->write(reinterpret_cast<const char*>(&xx), sizeof(int));
}

MCParticle::MCParticle() {
  momentum = G4ThreeVector(0,0,0);
  vertex = G4ThreeVector(0,0,-1e10);
  pdgID = 0;
  trackID = 0;
}

void MCParticle::Print() {
  G4cout << "trackID        : " << trackID << G4endl;
  G4cout << "parentID       : " << parentID << G4endl;
  G4cout << "PdgID          : " << pdgID << G4endl;
  G4cout << "creatorProcess : " << creatorProcess << G4endl;
  G4cout << "Momentum       : " << momentum.mag() << " GeV" << G4endl;
  G4cout << "Theta          : " << momentum.theta()*180/3.1415 << "deg" << G4endl;
  G4cout << "Phi            : " << momentum.phi()*180/3.1415 << " deg" << G4endl;
  G4cout << "cos(Theta)     : " << momentum.cosTheta() << G4endl;
  G4cout << "Vertex         : " << vertex << " m " << G4endl;
  G4cout << "Last Position  : " << lastPos << " m " << G4endl;
}

void MCParticle::WriteToBinary(ofstream* ofmc) {
  fOutput = ofmc;

  WriteValue(trackID);
  WriteValue(parentID);
  WriteValue(pdgID);
  WriteValue(momentum);
  WriteValue(vertex);
}

void MCParticle::WriteValue(int xx) {
  fOutput->write(reinterpret_cast<const char*>(&xx), sizeof(int));
}

void MCParticle::WriteValue(G4ThreeVector vec) {
  float xx = vec.x(); float yy = vec.y(); float zz = vec.z();
  fOutput->write(reinterpret_cast<const char*>(&xx), sizeof(float));
  fOutput->write(reinterpret_cast<const char*>(&yy), sizeof(float));
  fOutput->write(reinterpret_cast<const char*>(&zz), sizeof(float));
}

MCOpticalPhoton::MCOpticalPhoton() {
  parentMuID = 0;
  nParents = 0;
  processName = "empty";
  startPos = G4ThreeVector(0,0,-1e10);
  endPos = G4ThreeVector(0,0,-1e10);
  energy = 0;
  photonID = 0;
}

void MCOpticalPhoton::Clear() {
  parentID = 0;
  nParents = 0;
  processName = "empty";
  startPos = G4ThreeVector(0,0,-1e10);
  endPos = G4ThreeVector(0,0,-1e10);
  energy = 0;
  photonID = 0;
  if(binFile) {
    binFile->close();
    delete binFile;
    binFile = 0;
  } else binFile = 0;
}

MCOpticalPhoton::MCOpticalPhoton(G4String fname) {
  outFilePrefix = fname;
  eventID = 0;
  runID = 0;
  threadID = 0;
  Clear();
}

void MCOpticalPhoton::OpenNewBinaryFile() {
  binFile = new ofstream;
  char outFileName[200];
  G4cout << outFilePrefix << G4endl;
  sprintf(outFileName,"%s_optphotons_r%i_t%i_evt%i.bin",outFilePrefix.data(),runID,threadID,eventID);
  binFile->open(outFileName,std::ios::out | std::ios::binary);
}

void MCOpticalPhoton::WriteToBinary() {

  WriteValue(photonID);
  WriteValue(parentMuID);
  WriteValue(nParents);
  WriteValue(energy);
  WriteValue(startPos);
  WriteValue(endPos);
  // WriteValue(processName);
}

void MCOpticalPhoton::WriteValue(int xx) {
  binFile->write(reinterpret_cast<const char*>(&xx), sizeof(int));
}

void MCOpticalPhoton::WriteValue(float xx) {
  binFile->write(reinterpret_cast<const char*>(&xx), sizeof(float));
}

void MCOpticalPhoton::WriteValue(G4ThreeVector vec) {
  float xx = vec.x(); float yy = vec.y(); float zz = vec.z();
  binFile->write(reinterpret_cast<const char*>(&xx), sizeof(float));
  binFile->write(reinterpret_cast<const char*>(&yy), sizeof(float));
  binFile->write(reinterpret_cast<const char*>(&zz), sizeof(float));
}

MCPhotonHit::MCPhotonHit(int xx) {
  arrivalTime = -1;
  photonID = xx;
  OMposLocal = G4ThreeVector(0,0,0);
}

void MCPhotonHit::WriteToBinary(ofstream* ofmc) {
  fOutput = ofmc;

  WriteValue(photonID);
  WriteValue(arrivalTime);
  WriteValue(OMposLocal);  
}

void MCPhotonHit::WriteValue(int xx) {
  fOutput->write(reinterpret_cast<const char*>(&xx), sizeof(int));
}

void MCPhotonHit::WriteValue(float xx) {
  fOutput->write(reinterpret_cast<const char*>(&xx), sizeof(float));
}

void MCPhotonHit::WriteValue(G4ThreeVector vec) {
  float xx = vec.x(); float yy = vec.y(); float zz = vec.z();
  fOutput->write(reinterpret_cast<const char*>(&xx), sizeof(float));
  fOutput->write(reinterpret_cast<const char*>(&yy), sizeof(float));
  fOutput->write(reinterpret_cast<const char*>(&zz), sizeof(float));
}

MCOmHit::MCOmHit(int xx) {
  omID = xx;
  fPhotonHits.clear();
}

void MCOmHit::Clear() {
  for(unsigned jk=0; jk<fPhotonHits.size();jk++) {
    delete fPhotonHits[jk];
 }
}

void MCOmHit::Print() {
  G4cout << "omID        : " << omID << G4endl;
  G4cout << "NPhotonHits : " << fPhotonHits.size() << G4endl;
}

void MCOmHit::WriteToBinary(ofstream* ofmc) {
  fOutput = ofmc;

  WriteValue(omID);
  int nPhotonHits = fPhotonHits.size();
  WriteValue(nPhotonHits);
  for(int ij=0; ij<nPhotonHits; ij++)
    fPhotonHits[ij]->WriteToBinary(fOutput);
}

void MCOmHit::WriteValue(int xx) {
  fOutput->write(reinterpret_cast<const char*>(&xx), sizeof(int));
}

MCSectionHit::MCSectionHit(int xx) {
  sectionID = xx;
  fOmHits.clear();
}

void MCSectionHit::Clear() {
  for(unsigned jk=0; jk<fOmHits.size();jk++) {
    fOmHits[jk]->Clear();
    delete fOmHits[jk];
 }
}

void MCSectionHit::Print() {
  G4cout << "sectionID : " << sectionID << G4endl;
  G4cout << "NOMHits   : " << fOmHits.size() << G4endl;
  for(unsigned ij=0; ij<fOmHits.size(); ij++)
    fOmHits[ij]->Print();
}

void MCSectionHit::WriteToBinary(ofstream* ofmc) {
  fOutput = ofmc;

  WriteValue(sectionID);
  int nOMHits = fOmHits.size();
  WriteValue(nOMHits);
  for(int ij=0; ij<nOMHits; ij++)
    fOmHits[ij]->WriteToBinary(fOutput);
}

void MCSectionHit::WriteValue(int xx) {
  fOutput->write(reinterpret_cast<const char*>(&xx), sizeof(int));
}

MCStringHit::MCStringHit(int xx) {
  stringID = xx;
  fSectionHits.clear();
}

void MCStringHit::Clear() {
  for(unsigned jk=0; jk<fSectionHits.size();jk++) {
    fSectionHits[jk]->Clear();
    delete fSectionHits[jk];
 }
}

void MCStringHit::Print() {
  G4cout << "sectionID    : " << stringID << G4endl;
  G4cout << "NSectionHits : " << fSectionHits.size() << G4endl;
  for(unsigned ij=0; ij<fSectionHits.size(); ij++)
    fSectionHits[ij]->Print();
}

void MCStringHit::WriteToBinary(ofstream* ofmc) {
  fOutput = ofmc;

  WriteValue(stringID);
  int NSectionHits = fSectionHits.size();
  WriteValue(NSectionHits);
  for(int ij=0; ij<NSectionHits; ij++)
    fSectionHits[ij]->WriteToBinary(fOutput);
}

void MCStringHit::WriteValue(int xx) {
  fOutput->write(reinterpret_cast<const char*>(&xx), sizeof(int));
}


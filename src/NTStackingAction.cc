#include "NTStackingAction.hh"

NTStackingAction::NTStackingAction(NTRunAction* rAct)
  : G4UserStackingAction() {
  printInfo = false;
  runAct = rAct;
  mcEvt = rAct->GetMCEvent();
  mcPhoton = rAct->GetOpticalPhoton();
  fTimer = new G4Timer;
}

NTStackingAction::~NTStackingAction() {
  delete fTimer;
}

G4ClassificationOfNewTrack
NTStackingAction::ClassifyNewTrack(const G4Track * aTrack) {

  if(aTrack->GetTrackID()<=lastTrackRecorded) {
    if(aTrack->GetParticleDefinition()->GetParticleName() == "opticalphoton" && iphtrun%1000 == 0) {
      fTimer->Stop();
      G4cout << aTrack->GetTrackID() << " " << iphtrun << " " << *fTimer << G4endl;
      fTimer->Start();
    }
    iphtrun++;
    return fUrgent;
  }
  lastTrackRecorded = aTrack->GetTrackID();
  if((aTrack->GetParticleDefinition()->GetPDGEncoding() == 11 || aTrack->GetParticleDefinition()->GetPDGEncoding() == -11)
     &&
     aTrack->GetParentID() != 0) {
    if(aTrack->GetVertexKineticEnergy() < 0.5*aTrack->GetParticleDefinition()->GetPDGMass()) {
      return fKill;
    }
  }
  
  if(aTrack->GetParticleDefinition()->GetParticleName() != "opticalphoton") {
    MCParticle* mcPart = new MCParticle();
    mcPart->SetPdgID(aTrack->GetParticleDefinition()->GetPDGEncoding());
    mcPart->SetTrackID(aTrack->GetTrackID());
    mcPart->SetVertex(aTrack->GetVertexPosition()/m);
    mcPart->SetMomentum(aTrack->GetMomentum()/TeV);
    mcPart->SetParentID(aTrack->GetParentID());
    if(aTrack->GetParentID() != 0)
      mcPart->SetCreatorProcess(aTrack->GetCreatorProcess()->GetProcessName());
    else
      mcPart->SetCreatorProcess("PrimaryTrack");
    mcEvt->AddParticle(mcPart);
    if(aTrack->GetParentID() == 0) {
      fTimer->Start(); return fUrgent;
    } else
      return fWaiting;
  } else {
    mcPhoton->SetPhotonID(aTrack->GetTrackID());
    mcPhoton->SetParentID(aTrack->GetParentID());
    mcPhoton->SetProcessName(aTrack->GetCreatorProcess()->GetProcessName());
    mcPhoton->SetStartPosition(aTrack->GetVertexPosition()/m);
    mcPhoton->SetPhotonEnergy(aTrack->GetKineticEnergy()/eV);
    if(runAct->isSavePhoton())
      mcPhoton->WriteToBinary();
    mcEvt->AddPhotonCnt();
    iphtrecord++;
    if(mcEvt->GetPhotonCnt()%10000000 == 0) {
      G4cout << "NPhotons = " << float(mcEvt->GetPhotonCnt()) << G4endl;
      fTimer->Stop();
      G4cout << *fTimer << G4endl;
      fTimer->Start();
    }
    if(runAct->isSaveHit()) return fWaiting;
    else return fKill;
  }
}

void NTStackingAction::NewStage() {
  G4cout << "void NTStackingAction::NewStage() { ..." << G4endl;
  G4cout << "Total number of optical photons recorded = " << float(mcEvt->GetPhotonCnt()) << G4endl;
  G4cout << "Total number of tracks recorded = " << mcEvt->GetNParticles() << G4endl;
  G4cout << "Last track = " << lastTrackRecorded << G4endl;
}

void NTStackingAction::PrepareNewEvent() {
  // G4cout << "void NTStackingAction::PrepareNewEvent() { ... " << G4endl;
  lastTrackRecorded = -1;
  iphtrecord = 0;
  iphtrun = 0;
}


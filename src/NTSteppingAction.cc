#include "NTSteppingAction.hh"

NTSteppingAction::NTSteppingAction(NTRunAction* rAct)
: G4UserSteppingAction() { 
  fEventNumber = -1;
  printInfo = false;
  fHitCnt = 0;
  fPhotonCnt = 0;
  fLastTrack = -1;
  runAct = rAct;
  mcEvt = rAct->GetMCEvent();
  fTimer = new G4Timer;

}

NTSteppingAction::~NTSteppingAction() {
  delete fTimer;
}

void NTSteppingAction::UserSteppingAction(const G4Step* aStep) {
  if(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() != fEventNumber) {
    fEventNumber = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    fHitCnt = 0;
    fPhotonCnt = 0;
    fLastTrack = -1;
    fTimer->Start();
  }
  
  GetStepInfo(aStep);

  for(int ij=0; ij<mcEvt->GetNParticles(); ij++) {
    if(mcEvt->GetParticle(ij)->GetTrackID() == trackID) {
      mcEvt->GetParticle(ij)->SetVertex(vtxPos/m);
      mcEvt->GetParticle(ij)->SetLastPos(postStepPos/m);
      break;
    }
  }
  
  if(trackPDGname == "opticalphoton") {
    if(fLastTrack != trackID) fPhotonCnt++;

    fLastTrack = trackID;
    if(pvName == "physiOuterOM") {
      fHitCnt++;
      GetAllReplicaNumbers(aStep);

      if(runAct->isSaveHit()) {
	MCPhotonHit* nPHit = new MCPhotonHit(trackID);
	nPHit->SetArrivalTime(postStepTime/ns);
	nPHit->SetOMPosLocal(OmLocalPos/cm);
	bool isNotRecord = true;
	
	for(int ij=0; ij<mcEvt->GetNStringHits(); ij++) {
	  if(mcEvt->GetStringHit(ij)->GetStringID() == strNo) {
	    MCStringHit* iStrHit = mcEvt->GetStringHit(ij);
	    for(int jk=0; jk<iStrHit->GetNSectionHits(); jk++) {
	      if(iStrHit->GetSectionHit(jk)->GetSectionID() == secNo) {
		MCSectionHit* iSecHit = iStrHit->GetSectionHit(jk);
		for(int kl=0; kl<iSecHit->GetNOmHits(); kl++) {
		  if(iSecHit->GetOmHit(kl)->GetOmID() == OMid) {
		    iSecHit->GetOmHit(kl)->AddPhotonHit(nPHit);
		    isNotRecord = false;
		    break;
		  }
		} // for(int kl=0; kl<iSecHit->GetNOmHits(); kl++) {
		if(isNotRecord) {
		  MCOmHit* gOmHit = new MCOmHit(OMid);
		  gOmHit->AddPhotonHit(nPHit);
		  iSecHit->AddOmHit(gOmHit);
		  isNotRecord = false;
		}
	      } // if(iStrHit->GetSectionHit(jk)->GetSectionID() == secNo) {
	      if(isNotRecord == false) break;
	    } // for(int jk=0; jk<iStrHit->GetNSectionHits(); jk++) {
	    if(isNotRecord) {
	      MCOmHit* gOmHit = new MCOmHit(OMid);
	      gOmHit->AddPhotonHit(nPHit);
	      MCSectionHit* gSecHit = new MCSectionHit(secNo);
	      gSecHit->AddOmHit(gOmHit);
	      iStrHit->AddSectionHit(gSecHit);
	      isNotRecord = false;
	    }
	  } // if(mcEvt->GetStringHit(ij)->GetStringID() == strNo) {
	  if(isNotRecord == false) break;
	} // for(int ij=0; ij<mcEvt->GetNStringHits(); ij++) {
	
	if(isNotRecord) {
	  MCOmHit* gOmHit = new MCOmHit(OMid);
	  gOmHit->AddPhotonHit(nPHit);
	  
	  MCSectionHit* gSecHit = new MCSectionHit(secNo);
	  gSecHit->AddOmHit(gOmHit);
	  
	  MCStringHit* gStrHit = new MCStringHit(strNo);
	  gStrHit->AddSectionHit(gSecHit);
	  
	  mcEvt->AddStringHit(gStrHit);
	}
      }
      if(fHitCnt%10000 == 0) {
	
	// G4cout << "-x-x-x- PhotonHit -x-x-x-" << G4endl;
	// G4cout << "preStep " << preStepPos << G4endl;
	// G4cout << "postStep " << postStepPos << G4endl;
	// G4cout << "trackID " << trackID << G4endl;
	G4cout << "OMinfo: " << strNo << " " << secNo << " " << OMid << G4endl;
	G4cout << "Volumes: " << pvName << " photonhitcnt " << fHitCnt << " " << mcEvt->GetNStringHits() << G4endl;
	fTimer->Stop();
	G4cout << "fPhotonCnt: " << float(fPhotonCnt) << " " << trackID << G4endl;
	G4cout << *fTimer << G4endl;
	fTimer->Start();
	
      }
      aStep->GetTrack()->SetTrackStatus(fStopAndKill);
    }
  }

  // if(fPhotonCnt%1000000==0) {
  //   fTimer->Stop();
  //   G4cout << "fPhotonCnt: " << float(fPhotonCnt) << " " << trackID << G4endl;
  //   G4cout << *fTimer << G4endl;
  //   fTimer->Start();
  // }

}

void NTSteppingAction::PrintTouchableHistory(const G4Step* aStep) {
  G4TouchableHistory* theTouchable = (G4TouchableHistory*)(aStep->GetPostStepPoint()->GetTouchable());

  G4cout << "theTouchable::GetHistoryDepth " << theTouchable->GetHistoryDepth() << G4endl;
  for(int ij=0; ij<theTouchable->GetHistoryDepth(); ij++) {
    G4cout << ij << " " << theTouchable->GetVolume(ij)->GetName() << " " << theTouchable->GetReplicaNumber(ij) << G4endl;
  }
  
}  

void NTSteppingAction::GetAllReplicaNumbers(const G4Step* aStep) {
  G4TouchableHistory* theTouchable = (G4TouchableHistory*)(aStep->GetPostStepPoint()->GetTouchable());
  
  OMid = theTouchable->GetReplicaNumber(0);
  secNo = theTouchable->GetReplicaNumber(1);
  strNo = theTouchable->GetReplicaNumber(2);

  OmLocalPos = theTouchable->GetHistory()->GetTopTransform().TransformPoint(postStepPos);
  
}  

void NTSteppingAction::GetStepInfo(const G4Step* aStep) {
							    
  parentID = aStep->GetTrack()->GetParentID();
  trackID = aStep->GetTrack()->GetTrackID();
  trackPDGid = aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  trackPDGname = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
  creatorProc =  (parentID>0) ?
    aStep->GetTrack()->GetCreatorProcess()->GetProcessName() : "primaryParticle";
  vtxKE = aStep->GetTrack()->GetVertexKineticEnergy();
  vtxPos = aStep->GetTrack()->GetVertexPosition();
  logVolumeAtVtx = aStep->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
  // allPart.push_back(newPart);
  
  TotalEdep = aStep->GetTotalEnergyDeposit();
  stepnum = aStep->GetTrack()->GetCurrentStepNumber();
  trackKE = aStep->GetTrack()->GetKineticEnergy();
  preStepPos = aStep->GetPreStepPoint()->GetPosition();
  postStepPos = aStep->GetPostStepPoint()->GetPosition();
  stepLength = aStep->GetTrack()->GetStepLength();
  if(aStep->GetPostStepPoint()->GetPhysicalVolume())
    pvName =  aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
  else pvName = "empty";

  preStepTime = aStep->GetPreStepPoint()->GetGlobalTime();
  postStepTime = aStep->GetPostStepPoint()->GetGlobalTime();

  //  pvName =  aStep->GetTrack()->GetVolume()->GetName();
  //  pvName1 =  aStep->GetPreStepPoint()?aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName():"empty";
  //  pvName3 =  aStep->GetTrack()?aStep->GetTrack()->GetVolume()->GetName():"empty";

  if(printInfo) {
    G4cout << "-x-x-x-x- Step #" << stepnum << " -x-x-x-x-" << G4endl;
    G4cout << " parentID : " << parentID
	 << " , trackID : " << trackID
	 << " , trackPDGid : " << trackPDGid
	 << " , trackPDGname : " << trackPDGname
	 << " , creatorProc : " << creatorProc
	 << " , pvName : " << pvName
	 << G4endl;
    G4cout << "trackKE : " << G4BestUnit(trackKE, "Energy")
      // << " , preStepPos : " << G4BestUnit(preStepPos, "Length")
      // << " , postStepPos : " << G4BestUnit(postStepPos, "Length")
	 << " , steplen : " << G4BestUnit(stepLength, "Length")
	 << " , TotalEdep : " << G4BestUnit(TotalEdep, "Energy")
	 << G4endl;


  }
}


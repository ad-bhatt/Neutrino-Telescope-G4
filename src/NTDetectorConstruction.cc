#include "NTDetectorConstruction.hh"

NTDetectorConstruction::NTDetectorConstruction(G4String fname) : G4VUserDetectorConstruction() {
  G4String outFileName = fname + "_geom.dat";
  fGeomOut.open(outFileName);
}

void NTDetectorConstruction::DefineGeometricalParameters() {
  
  oRadiusInnerOM = 20*cm;
  oRadiusOuterOM = 25*cm;
  distanceBtwnOM = 2*m;
  distToCentralString = 2*m;
  
  parOmSection[0] = 0*m;
  parOmSection[1] = oRadiusOuterOM + 1*cm;
  parOmSection[2] = ((nOpticalModules+1)*distanceBtwnOM)/2;
  
  parOmString[0] = 0*m;
  parOmString[1] = parOmSection[1] + 1*cm;
  parOmString[2] = nOmSections*parOmSection[2];
  
  parLake[0] = distToCentralString + 2*m;
  parLake[1] = distToCentralString + 2*m;
  parLake[2] = parOmString[2] + 1*m;

  parWorld[0] = parLake[0];
  parWorld[1] = parLake[1];
  parWorld[2] = parLake[2] + 1*m;
  
}

NTDetectorConstruction::~NTDetectorConstruction(){;}

void NTDetectorConstruction::WriteGeometryOutput(G4String parName, float par) {
  fGeomOut << "par " << parName << " 0 1 " << par << G4endl;
}

void NTDetectorConstruction::WriteGeometryOutput(G4String parName, float* par, int npt) {
  fGeomOut << "par " << parName << " 1 " << npt << " ";
  for(int ij=0; ij<npt; ij++)
    fGeomOut << par[ij] << " ";
  fGeomOut << G4endl;
}

void NTDetectorConstruction::WriteGeometryOutput(G4String parName, int rNum, G4ThreeVector par) {
  fGeomOut << "pos ";
  fGeomOut << parName << " " << rNum << " 3 ";
  fGeomOut << par.x() << " ";
  fGeomOut << par.y() << " ";
  fGeomOut << par.z() << G4endl;
}

void NTDetectorConstruction::PrintPar(float* xx, int npt) {
  for(int ij=0; ij<npt; ij++) {
    G4cout << G4BestUnit(xx[ij], "Length") << " ";
  }
  G4cout << G4endl;
}

void NTDetectorConstruction::PrintGeometricalPar() {
  G4cout << "parOmSection : "; PrintPar(parOmSection,3);
  G4cout << "parOmString : "; PrintPar(parOmString,3);
  G4cout << "parLake : "; PrintPar(parLake,3);
  G4cout << "parWorld : "; PrintPar(parWorld,3);
}

G4LogicalVolume* NTDetectorConstruction::ConstructSection() {
  
  G4Tubs* solidSection = new G4Tubs("solidSection",
				    parOmSection[0],
				    parOmSection[1],
				    parOmSection[2],
				    tubsPhiIni*deg,
				    tubsDPhi*deg);
  WriteGeometryOutput("OmSection",parOmSection,3);
  
  G4LogicalVolume* logicVol
    = new G4LogicalVolume(solidSection,
			  matWater,
			  "logicSection",
			  0,0,0);

  NTOpticalModule* singleOM = new NTOpticalModule(oRadiusInnerOM,oRadiusOuterOM);
  WriteGeometryOutput("oRadiusInnerOM",oRadiusInnerOM);
  WriteGeometryOutput("oRadiusOuterOM",oRadiusOuterOM);
  G4LogicalVolume* logicOuterOM = singleOM->GetLogicalVolume();
  G4cout << logicOuterOM->GetName() << G4endl;
  logicOuterOM->SetName("logicOpticalModule");
  G4cout << logicOuterOM->GetName() << G4endl;

  float xpos = 0; float ypos = 0;
  float zpos = 0;
  for(int ij=0; ij<nOpticalModules; ij++) {
    if(ij==0) {
      zpos = -parOmSection[2] + (distanceBtwnOM/2);
    } else {
      zpos += distanceBtwnOM;
    }
    G4ThreeVector pTrans(xpos,ypos,zpos);
    new G4PVPlacement(0,
		      pTrans,
		      logicOuterOM,
		      "physiOuterOM",
		      logicVol,
		      false,ij);
    WriteGeometryOutput("OmInSection",ij,pTrans);
  }
  
  return logicVol;
}

G4LogicalVolume* NTDetectorConstruction::ConstructString() {
  
  G4Tubs* solidString = new G4Tubs("solidString",
				   parOmString[0],
				   parOmString[1],
				   parOmString[2],
				   tubsPhiIni*deg,
				   tubsDPhi*deg);
  WriteGeometryOutput("OmString",parOmString,3);
  
  G4LogicalVolume* logicVol
    = new G4LogicalVolume(solidString,
			  matWater,
			  "logicString",
			  0,0,0);
  
  G4LogicalVolume* logicSection = ConstructSection();
  
  float xpos = 0; float ypos = 0;
  float zpos = 0;
  for(int ij=0; ij<nOmSections; ij++) {
    if(ij==0) {
      zpos = -parOmString[2] + parOmSection[2];
    } else {
      zpos += (2*parOmSection[2]);
    }
    G4ThreeVector pTrans(xpos,ypos,zpos);
    new G4PVPlacement(0,
		      pTrans,
		      logicSection,
		      "physiSection",
		      logicVol,
		      false,ij);
    WriteGeometryOutput("SectionInString",ij,pTrans);
  }

  return logicVol;
  
}

G4LogicalVolume* NTDetectorConstruction::ConstructLake() {

  G4Box* solidLake = new G4Box("solidLake",
			       parLake[0],
			       parLake[1],
			       parLake[2]);
  WriteGeometryOutput("Lake",parLake,3);
  
  G4LogicalVolume* logicVol
    = new G4LogicalVolume(solidLake,
			  matWater,
			  "logicLake",
			  0,0,0);
  
  G4LogicalVolume* logicString = ConstructString();
  
  float xpos, ypos, zpos;
  zpos = -parLake[2] + parOmString[2];
  float strPosPhi = 0;
  for(int ij=0; ij<nOmStrings; ij++) {
    if(ij<nOmStrings-1) {
      xpos = distToCentralString*cos(strPosPhi*deg);
      ypos = distToCentralString*sin(strPosPhi*deg);
    } else {
      xpos = 0;
      ypos = 0;
    }
    G4ThreeVector pTrans(xpos,ypos,zpos);
    new G4PVPlacement(0,pTrans,
		      logicString,"physiString",
		      logicVol,false,ij);
    strPosPhi -= (360./7.);
    WriteGeometryOutput("StringInLake",ij,pTrans);
  }
  return logicVol;
}

G4VPhysicalVolume* NTDetectorConstruction::Construct() {

  float xpos = 0;
  float ypos = 0;
  float zpos = 0;
  
  ConstructMaterials();
  DefineGeometricalParameters();
  PrintGeometricalPar();

  G4Box* solidWorld = new G4Box("solidWorld",
				parWorld[0],
				parWorld[1],
				parWorld[2]);
  
  G4LogicalVolume* logicWorld
    = new G4LogicalVolume(solidWorld,
			  matAir,
			  "logicWorld",
			  0,0,0);
  
  physiWorld = new G4PVPlacement(0,
				 G4ThreeVector(),
				 logicWorld,
				 "physiWorld",
				 0,false,0);
  
  G4LogicalVolume* logicLake = ConstructLake();
  xpos = 0; ypos = 0;
  zpos = -parWorld[2] + parLake[2];
  G4ThreeVector pTrans(xpos,ypos,zpos);
  new G4PVPlacement(0,pTrans,
		    logicLake,
		    "physiLake",
		    logicWorld,
		    false,0);

  WriteGeometryOutput("Lake",0,pTrans);

  
  WriteGeometryOutput("World",parWorld,3);
  WriteGeometryOutput("World",0,physiWorld->GetTranslation());

  fGeomOut.close();
  //always return the physical World
  return physiWorld;
}

void NTDetectorConstruction::ConstructMaterials() {

  G4double a, z, density;
  G4int nelements;

  // Water
  G4Element* elO = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
  G4Element* elH = new G4Element("Hydrogen", "H", z=1 , a=1.01*g/mole);

  matWater = new G4Material("matWater", density= 1.0*g/cm3, nelements=2);
  matWater->AddElement(elH, 2);
  matWater->AddElement(elO, 1);

  //
  // ------------ Generate & Add Material Properties Table ------------
  //
  G4double photonEnergy[] =
    { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
      2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
      2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
      2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
      2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
      3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
      3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
      3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };

  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

  //
  // Water
  //
  G4double refractiveIndex1[] =
    { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
      1.346,  1.3465, 1.347,  1.3475, 1.348,
      1.3485, 1.3492, 1.35,   1.3505, 1.351,
      1.3518, 1.3522, 1.3530, 1.3535, 1.354,
      1.3545, 1.355,  1.3555, 1.356,  1.3568,
      1.3572, 1.358,  1.3585, 1.359,  1.3595,
      1.36,   1.3608};

  assert(sizeof(refractiveIndex1) == sizeof(photonEnergy));

  G4double absorption[] =
    {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
     15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
     45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
     52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
     30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
     17.500*m, 14.500*m };

  assert(sizeof(absorption) == sizeof(photonEnergy));

  G4double scintilFast[] =
    { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00 };

  assert(sizeof(scintilFast) == sizeof(photonEnergy));

  G4double scintilSlow[] =
    { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
      7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
      3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
      4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
      7.00, 6.00, 5.00, 4.00 };

  assert(sizeof(scintilSlow) == sizeof(photonEnergy));

  G4MaterialPropertiesTable* myMPT_matWater = new G4MaterialPropertiesTable();

  myMPT_matWater->AddProperty("RINDEX",       photonEnergy, refractiveIndex1,nEntries)
    ->SetSpline(true);
  myMPT_matWater->AddProperty("ABSLENGTH",    photonEnergy, absorption,     nEntries)
    ->SetSpline(true);
  myMPT_matWater->AddProperty("FASTCOMPONENT",photonEnergy, scintilFast,     nEntries)
    ->SetSpline(true);
  myMPT_matWater->AddProperty("SLOWCOMPONENT",photonEnergy, scintilSlow,     nEntries)
    ->SetSpline(true);

  myMPT_matWater->AddConstProperty("SCINTILLATIONYIELD",50./MeV);
  myMPT_matWater->AddConstProperty("RESOLUTIONSCALE",1.0);
  myMPT_matWater->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  myMPT_matWater->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
  myMPT_matWater->AddConstProperty("YIELDRATIO",0.8);

  G4double energy_water[] = {
			     1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
			     1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
			     1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
			     1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
			     1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
			     2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
			     2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
			     2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
			     2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
			     2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
			     3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
			     3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
			     3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
			     4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV,
			     5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV
  };

  const G4int numentries_water = sizeof(energy_water)/sizeof(G4double);

  //assume 100 times larger than the rayleigh scattering for now.
  G4double mie_water[] = {
			  167024.4*m, 158726.7*m, 150742  *m,
			  143062.5*m, 135680.2*m, 128587.4*m,
			  121776.3*m, 115239.5*m, 108969.5*m,
			  102958.8*m, 97200.35*m, 91686.86*m,
			  86411.33*m, 81366.79*m, 76546.42*m,
			  71943.46*m, 67551.29*m, 63363.36*m,
			  59373.25*m, 55574.61*m, 51961.24*m,
			  48527.00*m, 45265.87*m, 42171.94*m,
			  39239.39*m, 36462.50*m, 33835.68*m,
			  31353.41*m, 29010.30*m, 26801.03*m,
			  24720.42*m, 22763.36*m, 20924.88*m,
			  19200.07*m, 17584.16*m, 16072.45*m,
			  14660.38*m, 13343.46*m, 12117.33*m,
			  10977.70*m, 9920.416*m, 8941.407*m,
			  8036.711*m, 7202.470*m, 6434.927*m,
			  5730.429*m, 5085.425*m, 4496.467*m,
			  3960.210*m, 3473.413*m, 3032.937*m,
			  2635.746*m, 2278.907*m, 1959.588*m,
			  1675.064*m, 1422.710*m, 1200.004*m,
			  1004.528*m, 833.9666*m, 686.1063*m
  };

  assert(sizeof(mie_water) == sizeof(energy_water));

  // gforward, gbackward, forward backward ratio
  G4double mie_water_const[3]={0.99,0.99,0.8};

  myMPT_matWater->AddProperty("MIEHG",energy_water,mie_water,numentries_water)
    ->SetSpline(true);
  myMPT_matWater->AddConstProperty("MIEHG_FORWARD",mie_water_const[0]);
  myMPT_matWater->AddConstProperty("MIEHG_BACKWARD",mie_water_const[1]);
  myMPT_matWater->AddConstProperty("MIEHG_FORWARD_RATIO",mie_water_const[2]);

  G4cout << "Water G4MaterialPropertiesTable" << G4endl;
  // myMPT_matWater->DumpTable();

  matWater->SetMaterialPropertiesTable(myMPT_matWater);

  // Set the Birks Constant for the Water scintillator

  matWater->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  // Air
  G4Element* elN = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
  
  matAir = new G4Material("MatAir", density=1.29*mg/cm3, nelements=2);
  matAir->AddElement(elN, 70.*perCent);
  matAir->AddElement(elO, 30.*perCent);

  G4double refractiveIndex2[] =
    { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00 };

  G4MaterialPropertiesTable* myMPT_Air = new G4MaterialPropertiesTable();
  myMPT_Air->AddProperty("RINDEX", photonEnergy, refractiveIndex2, nEntries);

  G4cout << "Air G4MaterialPropertiesTable" << G4endl;
  myMPT_Air->DumpTable();

  matAir->SetMaterialPropertiesTable(myMPT_Air);
}



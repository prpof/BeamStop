
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "globals.hh"

#define CCDGUN 0	// fire neutrons directly at CCD0
#define TUNGSTEN 1	// faraday cup is made of tungsten instead of copper

#define NCCD 5		// number of CCD's

class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction();

  public:
               
     G4VPhysicalVolume* Construct();

     void UpdateGeometry();
     
  public:
  
     void PrintCalorParameters(); 
                    
     G4double GetWorldSizeX()           {return WorldSizeX;}; 
     G4double GetWorldSizeY()           {return WorldSizeY;};
     G4double GetWorldSizeZ()           {return WorldSizeZ;};

     G4double GetParticleGunX()		{return ParticleGunX;};
     G4double GetParticleGunY()		{return ParticleGunY;};
     G4double GetParticleGunZ()		{return ParticleGunZ;};

     G4double GetBeamLineZ()		{return BeamLineZ;};

     G4double GetCCDvolume()		{return volumeCCD;};	// cm^3

     G4double GetCCDXZsize() {
       if ( CCDXlen < CCDZlen ) {
         return CCDXlen;
       } else {
         return CCDZlen;
       }
     }

     G4int CheckCCDVolume(G4ThreeVector vertex, G4int ccd) {
       if (
                vertex.x() >= CCDXlo[ccd] && vertex.x() <= CCDXhi[ccd]
             && vertex.y() >= CCDYlo[ccd] && vertex.y() <= CCDYhi[ccd]
             && vertex.z() >= CCDZlo[ccd] && vertex.z() <= CCDZhi[ccd]
          ) {
         return 1;
       } else {
         return 0;
       }
     }

    G4double GetBeamStopLen()		{return BeamStopLen;};
    G4double GetBeamStopOD()		{return BeamStopOD;};

  private:
     
     G4double		ConcreteThick;		// concrete thickness
     G4double		CeilingHeight;		// experiment hall ceiling height
     G4double		BeamLineHeight;		// beam line height above floor

     G4double		BeamCrossXlen;		// Beam Cross
     G4double		BeamCrossYlen;
     G4double		BeamCrossZlen;
     G4double		CrossThickX;
     G4double		CrossThickYZ;

     G4double		BeamStopOD;		// Beam Stop
     G4double		BeamStopLen;
     G4double		BeamStopHoleOD;
     G4double		BeamStopHoleLen;

     G4double		BeamPipeID;		// Beam Pipes
     G4double		BeamPipeOD;
     G4double		BeamPipe1Len;
     G4double		BeamPipe2Len;
     G4double		BeamPipe3Len;
     G4double		BeamPipe4Len;

     G4double		BMagnetOD;		// Bending Magnet
     G4double		BMagnetID;
     G4double		BMagnetLen;

     G4double		QMagnetOD;		// Quadrupole Magnets
     G4double		QMagnetID;
     G4double		QMagnetLen;

     G4double		CameraPipeID;		// Camera Box Pipe
     G4double		CameraPipeOD;
     G4double		CameraPipeLen;
     G4double		GlassThick;		// Glass Thickness for Camera Box Port

     G4double 		CameraBoxXlen;		// Camera Box
     G4double 		CameraBoxYlen;
     G4double 		CameraBoxZlen;
     G4double 		CameraBoxThick;

     G4double		Xlen;			// generic dimensions for building camera box
     G4double		Ylen;
     G4double		Zlen;

     G4double		Xplace;			// generic placements for building camera box
     G4double		Yplace;
     G4double		Zplace;

     G4double		PolyTopThick;		// top of camera box
     G4double		PolyBottomThick;	// bottom of camera box
     G4double		PolyFrontThick;		// front (facing beam pipe) of camera box
     G4double		PolyBackThick;		// back of camera box
     G4double		PolyUpThick;		// upstream side (facing egun) of camera box
     G4double		PolyDownThick;		// downstream side of camera box

     G4double		LeadTopThick;		// top of camera box
     G4double		LeadBottomThick;	// bottom of camera box
     G4double		LeadFrontThick;		// front (facing beam pipe) of camera box
     G4double		LeadBackThick;		// back of camera box
     G4double		LeadUpThick;		// upstream side (facing egun) of camera box
     G4double		LeadDownThick;		// downstream side of camera box

     G4double 		CCDXlen;		// CCD sizes
     G4double 		CCDYlen;
     G4double		CCDZlen;
     G4double		volumeCCD;

     G4double		CCDX;			// CCD placements
     G4double		CCDY;
     G4double		CCDZ;

     G4double		ParticleGunX;		// computed quatities
     G4double		ParticleGunY;
     G4double		ParticleGunZ;
     G4double		BeamLineZ;
     G4double		InnerCrossXlen;
     G4double		InnerCrossYlen;
     G4double		InnerCrossZlen;
     G4double		CrossPort0X;
     G4double		CrossPort1X;
     G4double		CrossPort2Y;
     G4double		BeamPipeX;
     G4double		CameraPipeY;
     G4double		CameraBoxX;
     G4double		CameraBoxY;
     G4double		CameraBoxZ;
     G4double		CameraBoxZoff;
     G4double		InnerCameraBoxXlen;
     G4double		InnerCameraBoxYlen;
     G4double		InnerCameraBoxZlen;
     G4double		CameraBoxPortOD;
     G4double		CameraBoxPortX;
     G4double		CameraBoxPortY;
     G4double		CameraBoxPortZ;
     G4double		BeamCrossX[NCCD];
     G4double		CameraPipeX;
     G4double		CCDXlo[NCCD];
     G4double		CCDXhi[NCCD];
     G4double		CCDYlo[NCCD];
     G4double		CCDYhi[NCCD];
     G4double		CCDZlo[NCCD];
     G4double		CCDZhi[NCCD];

     G4RotationMatrix	cprm0;
     G4RotationMatrix	cprm1;
     G4RotationMatrix	cprm2;
     G4RotationMatrix	bprm;
     G4RotationMatrix	bsrm;
     G4RotationMatrix   bmrm;
     G4RotationMatrix   qmrm;
     G4RotationMatrix	cprm;
     G4RotationMatrix	lcprm;
     G4RotationMatrix	pcprm;
     G4RotationMatrix	cbprm;
     
     G4Material*        defaultMaterial;
     G4double           WorldSizeX;
     G4double           WorldSizeY;
     G4double           WorldSizeZ;
            
     G4Box*             solidWorld;   
     G4LogicalVolume*   logicWorld;   
     G4VPhysicalVolume* physiWorld;   

     G4Box*             solidAir;   
     G4LogicalVolume*   logicAir;   
     G4VPhysicalVolume* physiAir;   

     G4Tubs*            solidBeamPipe1;    
     G4LogicalVolume*   logicBeamPipe1;    
     G4VPhysicalVolume* physiBeamPipe1;    

     G4Tubs*            solidInnerPipe1;    
     G4LogicalVolume*   logicInnerPipe1;    
     G4VPhysicalVolume* physiInnerPipe1;    

     G4Tubs*            solidBeamPipe2;    
     G4LogicalVolume*   logicBeamPipe2;    
     G4VPhysicalVolume* physiBeamPipe2;    

     G4Tubs*            solidInnerPipe2;    
     G4LogicalVolume*   logicInnerPipe2;    
     G4VPhysicalVolume* physiInnerPipe2;    

     G4Tubs*            solidBeamPipe3;    
     G4LogicalVolume*   logicBeamPipe3;    
     G4VPhysicalVolume* physiBeamPipe3;    

     G4Tubs*            solidInnerPipe3;    
     G4LogicalVolume*   logicInnerPipe3;    
     G4VPhysicalVolume* physiInnerPipe3;    

     G4Tubs*            solidBeamPipe4;    
     G4LogicalVolume*   logicBeamPipe4;    
     G4VPhysicalVolume* physiBeamPipe4;    

     G4Tubs*            solidInnerPipe4;    
     G4LogicalVolume*   logicInnerPipe4;    
     G4VPhysicalVolume* physiInnerPipe4;    

     G4Tubs*            solidBMagnet;    
     G4LogicalVolume*   logicBMagnet;    
     G4VPhysicalVolume* physiBMagnet;    

     G4Tubs*            solidQMagnet;    
     G4LogicalVolume*   logicQMagnet;    
     G4VPhysicalVolume* physiQMagnet;    

     G4Box*             solidCentreBeamCross;
     G4LogicalVolume*   logicCentreBeamCross;    
     G4VPhysicalVolume* physiCentreBeamCross;    

     G4Box*             solidCentreInnerCross;
     G4LogicalVolume*   logicCentreInnerCross;    
     G4VPhysicalVolume* physiCentreInnerCross;    

     G4Box*             solidBeamCross;
     G4LogicalVolume*   logicBeamCross;    
     G4VPhysicalVolume* physiBeamCross;    

     G4Box*             solidInnerCross;
     G4LogicalVolume*   logicInnerCross;    
     G4VPhysicalVolume* physiInnerCross;    

     G4Tubs*            solidCrossPort;
     G4LogicalVolume*   logicCrossPort;    
     G4VPhysicalVolume* physiCrossPort;    

     G4Tubs*            solidCrossPortGA;
     G4LogicalVolume*   logicCrossPortGA;    
     G4VPhysicalVolume* physiCrossPortGA;    

     G4Tubs*            solidCrossGlassPort;
     G4LogicalVolume*   logicCrossGlassPort;    
     G4VPhysicalVolume* physiCrossGlassPort;    

     G4Tubs*            solidBeamStop;
     G4LogicalVolume*   logicBeamStop;    
     G4VPhysicalVolume* physiBeamStop;    

     G4Tubs*            solidBeamStopHole;
     G4LogicalVolume*   logicBeamStopHole;    
     G4VPhysicalVolume* physiBeamStopHole;    

     G4Tubs*            solidCameraPipe;
     G4LogicalVolume*   logicCameraPipe;    
     G4VPhysicalVolume* physiCameraPipe;    

     G4Tubs*            solidInnerCameraPipe;
     G4LogicalVolume*   logicInnerCameraPipe;    
     G4VPhysicalVolume* physiInnerCameraPipe;    

     G4Tubs*            solidLeadCameraPipe;
     G4LogicalVolume*   logicLeadCameraPipe;    
     G4VPhysicalVolume* physiLeadCameraPipe;    

     G4Tubs*            solidInnerLeadCameraPipe;
     G4LogicalVolume*   logicInnerLeadCameraPipe;    
     G4VPhysicalVolume* physiInnerLeadCameraPipe;    

     G4Tubs*            solidPolyCameraPipe;
     G4LogicalVolume*   logicPolyCameraPipe;    
     G4VPhysicalVolume* physiPolyCameraPipe;    

     G4Tubs*            solidInnerPolyCameraPipe;
     G4LogicalVolume*   logicInnerPolyCameraPipe;    
     G4VPhysicalVolume* physiInnerPolyCameraPipe;    

     G4Box*             solidCameraBox;
     G4LogicalVolume*   logicCameraBox;    
     G4VPhysicalVolume* physiCameraBox;    

     G4Box*             solidPolyBox;
     G4LogicalVolume*   logicPolyBox;    
     G4VPhysicalVolume* physiPolyBox;    

     G4Box*             solidAluminumBox;
     G4LogicalVolume*   logicAluminumBox;    
     G4VPhysicalVolume* physiAluminumBox;    

     G4Box*             solidInnerCameraBox;
     G4LogicalVolume*   logicInnerCameraBox;    
     G4VPhysicalVolume* physiInnerCameraBox;

     G4Tubs*            solidCameraBoxPort;
     G4LogicalVolume*   logicCameraBoxPort;    
     G4VPhysicalVolume* physiCameraBoxPort;    

     G4Box*             solidCCD;
     G4LogicalVolume*   logicCCD;    
     G4VPhysicalVolume* physiCCD;    

     DetectorMessenger* detectorMessenger;  //pointer to the Messenger
      
  private:
    
     void ComputeCalorParameters();
     G4VPhysicalVolume* ConstructCalorimeter();     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DetectorConstruction::ComputeCalorParameters()
{
  // Compute derived parameters of the experimental setup

  WorldSizeZ = CeilingHeight + 2*ConcreteThick;
  BeamLineZ = ConcreteThick + BeamLineHeight - WorldSizeZ/2;
  volumeCCD = (CCDXlen * CCDYlen * CCDZlen) / 1000;	// cm^3

  for (int i=0; i<NCCD; i++) {
    CCDXlo[i] = BeamCrossX[i] - CCDXlen/2;
    CCDXhi[i] = BeamCrossX[i] + CCDXlen/2;
    CCDYlo[i] = BeamCrossYlen/2 + CameraPipeLen + CameraBoxYlen/2 - CCDYlen/2;
    CCDYhi[i] = BeamCrossYlen/2 + CameraPipeLen + CameraBoxYlen/2 + CCDYlen/2;
    CCDZlo[i] = BeamLineZ + CameraBoxZoff + CCDZ - CCDZlen/2;
    CCDZhi[i] = BeamLineZ + CameraBoxZoff + CCDZ + CCDZlen/2;
  }

#if CCDGUN==1
  ParticleGunX = 0.0*cm;
  ParticleGunY = BeamCrossYlen/2 + CameraPipeLen + CameraBoxYlen/2 - CCDYlen; // CCDYlen/2 in front
  ParticleGunZ = BeamLineZ + CameraBoxZoff + CCDZ; 
#else
  ParticleGunX = - (BeamCrossXlen + BeamPipe1Len + BeamCrossXlen + BeamPipe2Len + BeamCrossXlen/2);
  ParticleGunY = 0.0*cm;
  ParticleGunZ = BeamLineZ;
#endif
}

#endif

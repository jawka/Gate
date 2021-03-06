/**
 *  @copyright Copyright 2017 The J-PET Gate Authors. All rights reserved.
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  @file GateJPETSingleGammaMode.cc
 */

#include "GateJPETSingleGammaModel.hh"
#include "TLorentzVector.h"
#include "G4Electron.hh"
#include "G4PhysicalConstants.hh"
#include "GateConstants.hh"
#include <cmath>

GateJPETSingleGammaModel* GateJPETSingleGammaModel::ptrJPETSingleGammaModel=0;
GateJPETSingleGammaModel::GateJPETSingleGammaModel()
{
	G4cout <<"GateJPETSingleGammaModel initialization.\n";
	SetParticlesNumber(1);
	GateJPETSourceManager::GetInstance()->AddGammaSourceModel(this);
}

GateJPETSingleGammaModel::~GateJPETSingleGammaModel()
{
}

void GateJPETSingleGammaModel::GetGammaParticles(std::vector<G4PrimaryParticle*>& particles)
{
 Double_t x = 0, y = 0, z = 0;
 GetRandomGenerator()->Sphere( x, y, z, 1.0 );

 G4ThreeVector momentum_direction( x, y, z );
 G4double kinetic_energy = GetPromptGammaEnergy();
 G4ThreeVector gamma_polarization = GetPolarization( momentum_direction );

 particles[ 0 ]->SetMomentumDirection( momentum_direction );
 particles[ 0 ]->SetMass( 0.0 );
 particles[ 0 ]->SetKineticEnergy( kinetic_energy );
 particles[ 0 ]->SetPolarization( gamma_polarization );
}

G4String GateJPETSingleGammaModel::GetModelName()
{
	return "SGJPET";
}

GateJPETSingleGammaModel* GateJPETSingleGammaModel::GetInstance()
{
	if(!ptrJPETSingleGammaModel){
		ptrJPETSingleGammaModel = new GateJPETSingleGammaModel;
	}

	return ptrJPETSingleGammaModel;
}


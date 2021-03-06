/**
 *  @copyright Copyright 2018 The J-PET Gate Authors. All rights reserved.
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
 *  @file GateJPETFourGammasDecayModel.cc
 */

#include "GateJPETFourGammasDecayModel.hh"
#include "TLorentzVector.h"
#include "G4Electron.hh"
#include "G4PhysicalConstants.hh"
#include "GateConstants.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

GateJPETFourGammasDecayModel* GateJPETFourGammasDecayModel::ptrJPETFourGammasDecayModel=nullptr;

GateJPETFourGammasDecayModel::GateJPETFourGammasDecayModel()
{
 G4cout <<"GateJPETFourGammasDecayModel initialization.\n";
 SetParticlesNumber(4);
 GateJPETSourceManager::GetInstance()->AddGammaSourceModel(this);
 fRandomGen.SetSeed(0);
}

GateJPETFourGammasDecayModel::~GateJPETFourGammasDecayModel()
{
}

void GateJPETFourGammasDecayModel::GetGammaParticles(std::vector<G4PrimaryParticle*>& particles)
{
 Double_t mass_e = 0.511 * MeV / 1000.0;//GeV - because TGenPhaseSpace work with GeV
 TLorentzVector positronium( 0.0, 0.0, 0.0, 2.0 * mass_e );
 Double_t mass_secondaries[ 4 ] = { 0.0, 0.0, 0.0, 0.0 };

 TGenPhaseSpace four_body_decay;
 four_body_decay.SetDecay( positronium, 4, mass_secondaries ); 

 double uniform_weight, gen_weigth;

 do
 {
  uniform_weight = fRandomGen.Uniform( 0.12 * 1.1 );
  gen_weigth = four_body_decay.Generate();
 }
 while ( uniform_weight > gen_weigth );

 TLorentzVector gamma_momentum;
 G4ThreeVector gamma_polarization;

 int particles_number = GetParticlesNumber();
 for ( int i = 0; i < particles_number; ++i )
 {
  gamma_momentum = *four_body_decay.GetDecay( i );
  particles[ i ]->SetMomentum( gamma_momentum.Px() * 1000.0, gamma_momentum.Py() * 1000.0, gamma_momentum.Pz() * 1000.0 );
  gamma_polarization = GetPolarization( particles[ i ]->GetMomentumDirection() );
  particles[ i ]->SetPolarization( gamma_polarization );
 }

}

G4String GateJPETFourGammasDecayModel::GetModelName()
{
 return "FourGammasJPET";
}

GateJPETFourGammasDecayModel* GateJPETFourGammasDecayModel::GetInstance()
{
 if(!ptrJPETFourGammasDecayModel)
  ptrJPETFourGammasDecayModel = new GateJPETFourGammasDecayModel;
 return ptrJPETFourGammasDecayModel;
}

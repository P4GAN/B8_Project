//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B2/B2a/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the B2::PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "Randomize.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4ios.hh"
#include "G4RandomDirection.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(const std::string &hepmcFile)
    : hepMCReader(hepmcFile)
{
    if (hepMCReader.failed())
    {
        G4Exception("PrimaryGeneratorAction",
                    "HEPMC_READER_FAIL",
                    FatalException,
                    ("Cannot open HepMC file " + hepmcFile).c_str());
    }

    fPossibleMomenta = {
        0.1 * GeV, 0.2 * GeV, 0.3 * GeV, 0.5 * GeV, 0.7 * GeV, 1.0 * GeV,
        2.0 * GeV, 3.0 * GeV, 5.0 * GeV, 7.0 * GeV, 10.0 * GeV, 14.0 * GeV, 20.0 * GeV
    };

    // For testing with single pion gun
    G4int nofParticles = 1;
    fParticleGun = new G4ParticleGun(nofParticles);

    G4ParticleDefinition *particleDefinition =
        G4ParticleTable::GetParticleTable()->FindParticle("pi+");

    fParticleGun->SetParticleDefinition(particleDefinition);

}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
    G4double momentum = fPossibleMomenta[std::rand() % fPossibleMomenta.size()];
    fParticleGun->SetParticleMomentum(momentum);
    fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
    fParticleGun->GeneratePrimaryVertex(event);
    return;


    /* Emit particles from HepMC file
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    std::string hepmcFileName = "../electron_proton.hepmc";
    HepMC3::ReaderAscii hepMCReader("events.hepmc");

    while (!hepMCReader.failed())
    {
        HepMC3::GenEvent hepmcEvent;
        hepMCReader.read_event(hepmcEvent);

        for (auto vertex : hepmcEvent.vertices())
        {
            auto position = vertex->position();

            auto g4Vertex = new G4PrimaryVertex(
                position.x() * mm,
                position.y() * mm,
                position.z() * mm,
                position.t() * ns);

            for (auto particle : vertex->particles_out())
            {
                // HepMC3 status convention: 1 is usually final state
                if (particle->status() != 1)
                    continue;

                G4ParticleDefinition *particleDef = particleTable->FindParticle(particle->pid());
                G4cout << "Emitted " << particleDef->GetParticleName() << " with energy " << particle->momentum().e() << " GeV" << G4endl;

                auto momentum = particle->momentum();
                auto primary = new G4PrimaryParticle(
                    particle->pid(),
                    momentum.px() * GeV,
                    momentum.py() * GeV,
                    momentum.pz() * GeV);

                g4Vertex->SetPrimary(primary);
            }

            // Only keep vertices that actually have primaries
            if (g4Vertex->GetNumberOfParticle() > 0)
            {
                event->AddPrimaryVertex(g4Vertex);
            }
            else
            {
                delete g4Vertex;
            }
        }
    }
    */  
}
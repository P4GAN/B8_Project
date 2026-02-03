// https://github.com/eic/LambdaGen

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC3.h"

using namespace Pythia8;

int main() {
    int nEvents = 1;
    Pythia8::Pythia8ToHepMC toHepMC("../electron_proton.hepmc");

    Pythia pythia;
    pythia.readString("Beams:idA = 2212");      // Proton
    pythia.readString("Beams:idB = 11");        // Electron
    pythia.readString("Beams:frameType = 2");
    pythia.readString("Beams:eA = 275");        // Proton beam energy
    pythia.readString("Beams:eB = 18");         // Electron beam energy
    pythia.readString("WeakBosonExchange:ff2ff(t:gmZ) = on");
    pythia.readString("SpaceShower:dipoleRecoil = on");
    pythia.readString("PhaseSpace:Q2min = 100");
    pythia.init();

    int numAbort = 0;

    for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
        if (!pythia.next()) {
            numAbort++;
        }
        toHepMC.writeNextEvent(pythia);
    }

    pythia.stat();

    std::cout << "Number of aborted events: " << numAbort << std::endl;

    return 0;

}
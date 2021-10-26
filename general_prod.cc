#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

using namespace Pythia8;



int main(const int argc, const char *num[]) {

	std::cout << "This is the process " << num[1] << std::endl;
	
	std::string name = "/home/sergey/Root/Projects/lambda1520/data/pythiaCuAu200_";
	name += num[1];
	name.append(".root");
	
	std::cout << "Output file is " << name << std::endl;	
	
	std::string set_seed("Random:seed = ");
	set_seed += num[1];
	
	Pythia pythia;
	pythia.readString("Beams:eCM = 200.");
	pythia.readString("Beams:idA = 1000290630");
	pythia.readString("Beams:idA = 1000791970");
	pythia.readString("HardQCD:all = on");
	pythia.readString("Random:setSeed = on");
	pythia.readString(set_seed.c_str());
	
	pythia.init();
	
	double nEvents = 1000000;
	
	int id;
	long long int iEvent;
	
	double e, px, py, pz;
	
	
	
	for (iEvent = 0; iEvent < nEvents; ++iEvent) {
	
	if (static_cast<long long int>(iEvent) % 10000 == 0) cout << "The process is ready for " << iEvent/nEvents*100 << " percent" << endl;
	
		if (!pythia.next()) continue;
		
		for (int ipart = 0; ipart < pythia.event.size(); ++ipart) {
		
			if (pythia.event[ipart].isFinal() && pythia.event[ipart].isCharged()) {
				
				id = pythia.event[ipart].id();
				
				//if (abs(id) != 211 && abs(id) != 321 && abs(id) != 2212) continue;
				if (abs(id) > 3000) continue;
				
				e = pythia.event[ipart].e();
				px = pythia.event[ipart].px();
				py = pythia.event[ipart].py();
				pz = pythia.event[ipart].pz();
				 
				Part->Fill();
				
				
			}
		}
	}
	
	TFile *outf = new TFile(name.c_str(), "RECREATE");
	
	Part->Write();
	
	delete Part, outf;
	
	pythia.stat();
	
	return 0;
	
}

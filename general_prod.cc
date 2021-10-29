#include "/home/sergey/Root/Projects/lib/Particles.h"
#include "/home/sergey/Root/Projects/lib/CComby.h"

using namespace Pythia8;



int main(const int argc, const char *num[]) {

	std::cout << "This is the process " << num[1] << std::endl;
	
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
	
	long unsigned int nEvents = 100000;
	const int mix_num = 2;
	const int cc_size = 10000;
	bool check{true};
	
	std::cout << "Output files are located in /home/sergey/Root/Projects/lambda1520/data/pythia_production" << std::endl;
	
	CComby *CC;
	
	for (unsigned long int iEvent = 0; iEvent < nEvents; ++iEvent) {
	
		if(check) {
			
			CC = new CComby();
			check = false;
			
			std::string name = "/home/sergey/Root/Projects/lambda1520/data/pythia_production/pythiaCuAu200_";
			name += num[1];
			name.append("_");
			name.append(to_string(static_cast<double>(iEvent)/cc_size));
			name.append(".root");
			
			CC->AddChannel(5);
			
			CC->SetOutput(name.c_str());
				
		};
		
		
		if (!pythia.next()) continue;
		
		for (int ipart = 0; ipart < pythia.event.size(); ++ipart) {
		
			if (pythia.event[ipart].isFinal() && pythia.event[ipart].isCharged()) {
			
				int id;
				double e, px, py, pz;
				
				id = pythia.event[ipart].id();
				
				if (abs(id) != 211 && abs(id) != 321 && abs(id) != 2212) continue;
				
				e = pythia.event[ipart].e();
				px = pythia.event[ipart].px();
				py = pythia.event[ipart].py();
				pz = pythia.event[ipart].pz();
				
				if (id < 0 && id != 2212) {id = -321};
				
				CC->AddParticle(id, iEvent, e, px, py, pz);
				
			}
		}
		
		if (iEvent % mix_num == 0 && iEvent != 0 || iEvent == nEvents - 1) {
			
			CC->ProcessEvents();			
			
			if (iEvent % cc_size == 0) {
			
				cout << "The process is ready for " << static_cast<double>(iEvent)/nEvents*100 << " percent" << endl;
				CC->Write();
				
				delete CC;
				check = true;
			
			}
			
		}
	}
	
	pythia.stat();
	
	return 0;
	
}

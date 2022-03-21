#include "/home/sergey/Root/Projects/lib/Particles.h"
#include "/home/sergey/Root/Projects/lib/CComby.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"

using namespace Pythia8;

int main(const unsigned int seed_num) {

	std::cout << "This is the process " << seed_num << std::endl;
	
	std::string set_seed("Random:seed = ");
	set_seed.append(to_string(seed_num));
	
	Pythia pythia;
	pythia.readString("Beams:eCM = 200.");
	//pythia.readString("HardQCD:all = on");
	//pythia.readString("PhaseSpace:pTHatMin = 20.");
	
	//pythia.readString("Beams:idA = 2212"); //pp
	//pythia.readString("Beams:idB = 2212");
	
	pythia.readString("Beams:idA = 1000290630");//Cu
	pythia.readString("Beams:idB = 1000791970");//Au
	
	pythia.readString("HeavyIon:SigFitDefPar = 9.82,1.69,0.29,0.0,0.0,0.0,0.0,0.0"); //CuAu200 parameters
	pythia.readString("HeavyIon:SigFitNGen = 0");
	
	pythia.readString("Random:setSeed = on");
	pythia.readString(set_seed.c_str());
	
	pythia.init();
	
	long unsigned int nEvents = 1e6;	//number of events
	const int mix_num = 10;			//number of events to mix
	const int cc_size = 5e4;		//number of events to write after mixing
	bool check{true};
	
	std::cout << "Output files are located in /home/sergey/Root/Projects/data/pythia_production" << std::endl;
	
	CComby *CC;
	
	unsigned int out_number = 0;
	
	for (unsigned long int iEvent = 0; iEvent < nEvents; ++iEvent) {
	
		if(check) {
			
			CC = new CComby();
			check = false;
			
			std::string name = "/home/sergey/Root/Projects/data/pythia_production/pythiaCuAu200_";
			name += to_string(seed_num);
			name.append("_");
			name.append(to_string(out_number));
			name.append(".root");
			
			CC->AddChannel(211, 211);
			CC->AddChannel(321, 211);
			CC->AddChannel(321, 321);
			CC->AddChannel(2212, 211);
			CC->AddChannel(2212, 321);
			CC->AddChannel(2212, 2212);

			CC->AddChannel(211, 11);
			CC->AddChannel(321, 11);
			CC->AddChannel(2212, 11);

			CC->AddChannel(11, 11);
			CC->AddChannel(22, 22);
			CC->AddChannel(22, 11);
			
			CC->SetOutput(name.c_str());
			
			out_number++;
				
		};
		
		if (!pythia.next()) continue;
		
		unsigned int ncharged = 0;
		
		for (int ipart = 0; ipart < pythia.event.size(); ++ipart) {
			
			if (pythia.event[ipart].isFinal()) {
			
				double eta = pythia.event[ipart].eta();
				
				if (eta < 3.1 || eta > 4) continue;
				
				ncharged++;
				
				int id, centr;
				double e, px, py, pz;
				
				id = pythia.event[ipart].id();
				
				if (abs(id) != 211 && abs(id) != 321 && abs(id) != 2212 && abs(id) != 11 && id != 22) continue; //pi, K, P, e
				
				e = pythia.event[ipart].e();
				px = pythia.event[ipart].px();
				py = pythia.event[ipart].py();
				pz = pythia.event[ipart].pz();
				
				CC->AddParticle(id, iEvent, e, px, py, pz);
				
			}
		}
		
		const int centr = CC->SetCentrality(iEvent, ncharged);
		
		if (iEvent % mix_num == 0 && iEvent != 0 || iEvent == nEvents - 1) {
			
			CC->ProcessEvents();
			
			if (iEvent % cc_size == 0) {
				
				cout << "The process is ready for " << static_cast<double>(iEvent)/nEvents*100 << " percent" << endl;
				CC->Write(cc_size);
				
				delete CC;
				check = true;
				
			}
			
		}
	}
	
	pythia.stat();
	
	return 0;
	
}

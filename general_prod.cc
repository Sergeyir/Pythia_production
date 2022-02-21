#include "/home/sergey/Root/Projects/lib/Particles.h"
#include "/home/sergey/Root/Projects/lib/CComby.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"

using namespace Pythia8;

int main(const unsigned int argc, const char *num[]) {

	std::cout << "This is the process " << num[1] << std::endl;
	
	std::string set_seed("Random:seed = ");
	set_seed += num[1];
	
	Pythia pythia;
	pythia.readString("Beams:eCM = 200.");
	pythia.readString("HardQCD:all = on");
	pythia.readString("PhaseSpace:pTHatMin = 20.");
	
	//pythia.readString("Beams:idA = 2212"); //pp
	//pythia.readString("Beams:idB = 2212");
	
	pythia.readString("Beams:idA = 1000290630");//Cu
	pythia.readString("Beams:idB = 1000791970");//Au
	
	//pythia.readString("HeavyIon:SigFitDefPar = 9.82,1.69,0.29,0.0,0.0,0.0,0.0,0.0"); \\CuAu200 parameters
	//pythia.readString("HeavyIon:SigFitNGen = 0");
	
	pythia.readString("Random:setSeed = on");
	pythia.readString(set_seed.c_str());
	
	pythia.init();
	
	long unsigned int nEvents = 250000;	//number of events
	const int mix_num = 2;			//number of events to mix
	const int cc_size = 10000;		//number of events to write after mixing
	bool check{true};
	
	std::cout << "Output files are located in /home/sergey/Root/Projects/data/pythia_production" << std::endl;
	
	CComby *CC;
	
	unsigned int out_number = 0;
	
	std::array<const float, 13> pt_range = {0., 1.4, 1.7, 1.9, 2.1, 2.3, 2.6, 2.9, 3.4, 4.0, 4.5, 5.0, 6.5};
	
	std::array<TGraph*, 5> kstar_mult;
	
	for (int count = 0; count < kstar_mult.size(); count++) {
	
		kstar_mult[count] = new TGraph();
		kstar_mult[count]->SetMarkerStyle(21);
		kstar_mult[count]->SetMarkerColor(count);
		
	}
	
	kstar_mult[0]->SetName("0-20");
	kstar_mult[1]->SetName("20-40");
	kstar_mult[2]->SetName("40-60");
	kstar_mult[3]->SetName("60-80");
	kstar_mult[4]->SetName("80-93");
	
	std::array<std::array<unsigned int, 12>, 5> nkstars_centr;
	
	for (unsigned long int iEvent = 0; iEvent < nEvents; ++iEvent) {
	
		if(check) {
			
			CC = new CComby();
			check = false;
			
			std::string name = "/home/sergey/Root/Projects/data/pythia_production/pythiaPP200_";
			name += num[1];
			name.append("_");
			name.append(to_string(out_number));
			name.append(".root");
			
			CC->AddChannel(211, -321);
			CC->AddChannel(321, -211);
			
			CC->SetOutput(name.c_str());
			
			out_number++;
				
		};
		
		
		if (!pythia.next()) continue;
		std::array<unsigned int, 12> nkstars;
		unsigned int ncharged = 0;
		
		for (int ipart = 0; ipart < pythia.event.size(); ++ipart) {
		
		if (abs(pythia.event[ipart].id()) == 313) {
		
			for (int count = 0; count < pt_range.size() - 1; count++) {
				
				float pt = pythia.event[ipart].pT();
				
				if (pt > pt_range[count] && pt < pt_range[count+1])  nkstars[count]++;
			
			}
		
		}
		
			if (pythia.event[ipart].isFinal() && pythia.event[ipart].isCharged()) {
			
				double eta = pythia.event[ipart].eta();
				
				if (eta < 3.1 && eta > 4) continue;
				
				ncharged++;
			
				int id, centr;
				double e, px, py, pz;
				
				id = pythia.event[ipart].id();
				
				if (abs(id) != 211 && abs(id) != 321 && abs(id) != 2212) continue; //pi, K, P
				
				e = pythia.event[ipart].e();
				px = pythia.event[ipart].px();
				py = pythia.event[ipart].py();
				pz = pythia.event[ipart].pz();
				
				CC->AddParticle(id, iEvent, e, px, py, pz);
				
			}
		}
		
		const int centr = CC->SetCentrality(iEvent, ncharged);
		
		for (int pt_count = 0; pt_count < pt_range.size() - 1; pt_count++) {nkstars_centr[centr][pt_count] += nkstars[pt_count];}
		
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
	
	for (int count = 0; count < kstar_mult.size(); count++) {
	
		for (int pt_count = 0; pt_count < pt_range.size()-1; pt_count++) {
		
		kstar_mult[count]->AddPoint(pt_range[pt_count+1], nkstars_centr[count][pt_count]);
		
		}
	
	}
	
	TCanvas *c = new TCanvas("kstar_mult", "kstar_mult");
	
	for (int count = 0; count < kstar_mult.size(); count++) {kstar_mult[count]->Draw("EP+");};	
	
	c->Write();
	
	pythia.stat();
	
	return 0;
	
}


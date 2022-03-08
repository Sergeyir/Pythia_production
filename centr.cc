#include "Pythia8/Pythia.h"
//#include "Pythia8/HeavyIons.h"

#include <iostream>
#include "TH1.h"
#include "TFile.h"
#include "TGraph.h"

using namespace Pythia8;

int main(const unsigned int argc, const char *num[]) {

	std::array <const unsigned int, 6> centr_det = {200, 85, 41, 17, 5, 2}; //centality ranges for CuAu200	
	std::array <const float, 14> pt_min = {0.0, 0.0, 1.4, 1.7, 1.9, 2.1, 2.3, 2.6, 2.9, 3.4, 4.0, 4.5, 5.0, 6.5};
	std::array <const float, 14> pt_max = {8.0, 1.4, 1.7, 1.9, 2.1, 2.3, 2.6, 2.9, 3.4, 4.0, 4.5, 5.0, 6.5, 8.0};
	std::array <double, 14> nkstars, nantikstars;

	TH1F *nkstars_hist = new TH1F("nkstars", "nkstars", 14, 0, 14);
	TH1F *nantikstars_hist = new TH1F("nantikstars", "nantikstars", 14, 0, 14);

	const long long unsigned int nEvents = 1e7;

		
	for (int count = 0; count < pt_min.size(); count ++) {
		nkstars[count] = 0;
		nantikstars[count] = 0;
	}

	TH1D *ncharged_per_event_hist, *ncharged_per_event_hist_no_weight_i_said_you_bro_no_weight_at_all_believe_me_please_i_am_not_joking_can_you_trust_me_at_least_one_time_bro_please_can_you_believe_i_ve_got_ligma;
	TFile *output;

	std::cout << "This is the process " << num[1] << std::endl;
	
	std::string set_seed("Random:seed = ");
	set_seed += num[1];
	
	Pythia pythia;
	
	//pythia.readString("Beams:idA = 1000290630");
	//pythia.readString("Beams:idB = 1000791970");
	
	pythia.readString("Beams:eCM = 200.");
	//pythia.readString("PhaseSpace:pTHatMin = 0.");
	pythia.readString("HardQCD:all = on");
	
	//pythia.readString("HeavyIon:SigFitDefPar = 9.82,1.69,0.29,0.0,0.0,0.0,0.0,0.0");
	//pythia.readString("HeavyIon:SigFitNGen = 0");
	
	pythia.readString("Random:setSeed = on");
	pythia.readString(set_seed.c_str());
	
	pythia.init();
	
	unsigned int nbins = 200;
	
	ncharged_per_event_hist = new TH1D("ncharged", "ncharged", nbins, 0, nbins);
	ncharged_per_event_hist_no_weight_i_said_you_bro_no_weight_at_all_believe_me_please_i_am_not_joking_can_you_trust_me_at_least_one_time_bro_please_can_you_believe_i_ve_got_ligma = new TH1D("nchargednw", "nchargednw", nbins, 0, nbins);
	
	for (unsigned long int iEvent = 0; iEvent < nEvents; ++iEvent) {
	
		if (iEvent % 10000 == 0) {
			cout << "Events has passed: " << iEvent << endl;
			//system("sensors");
		}
		if (!pythia.next()) continue;
		
		unsigned int ncharged = 0;
		double weight = 0;

	for (int ipart = 0; ipart < pythia.event.size(); ++ipart) {
		
		int d1 = pythia.event[ipart].daughter1();
		int d2 = pythia.event[ipart].daughter2();
		
		if (abs(pythia.event[ipart].id()) == 313 && (((abs(pythia.event[d1].id()) == 211 && abs(pythia.event[d2].id()) == 321)) || (abs(pythia.event[d1].id()) == 321 && abs(pythia.event[d2].id())))) {

			//cout << pythia.event[ipart].id() << " " << pythia.event[d1].id() << " " << pythia.event[d2].id() << endl;

			for (int count = 0; count < pt_min.size(); count++) {
			
				if (pythia.event[ipart].pT() > pt_min[count] && pythia.event[ipart].pT() < pt_max[count] && pythia.event[ipart].id() > 0) {
					nkstars[count] += 1;
					nkstars_hist->Fill(count, weight*1e6/nEvents);	
					}
				else if (pythia.event[ipart].pT() > pt_min[count] && pythia.event[ipart].pT() < pt_max[count] && pythia.event[ipart].id() < 0) {
					nantikstars[count] += 1; 
					nantikstars_hist->Fill(count, weight*1e6/nEvents);	
				}

			}

		} 
		
			if (pythia.event[ipart].isFinal() && pythia.event[ipart].isCharged()) {
			
				double eta = pythia.event[ipart].eta();
				if (eta < 3.1 && eta > 4) continue;
				ncharged++;
				
				
			}
			
		weight = pythia.info.weight();
		
		ncharged_per_event_hist->Fill(ncharged);
		ncharged_per_event_hist_no_weight_i_said_you_bro_no_weight_at_all_believe_me_please_i_am_not_joking_can_you_trust_me_at_least_one_time_bro_please_can_you_believe_i_ve_got_ligma->Fill(ncharged, weight);
		
		}

	}
	
	std::cout << "kstars: {";
		
	for (int count = 0; count < pt_min.size(); count ++) {
		
		nkstars[count] = nkstars[count]*1e6/nEvents;

		std::cout << nkstars[count] << ", "; 

	}

	std::cout << "}" << endl;

	std::cout << "anti kstars: {";
		
	for (int count = 0; count < pt_min.size(); count ++) {
		
		nantikstars[count] = nantikstars[count]*1e6/nEvents;

		std::cout << nantikstars[count] << ", "; 

	}

	std::cout << "}" << endl;

	std::cout << "Output file is located at /home/sergey/Root/Projects/pythia_production/output" << std::endl;
	
	std::string name = "/home/sergey/Root/Projects/pythia_production/output/pythiaPP200_centr_";
	name += num[1];
	name.append("_");
	name.append(".root");
	
	output = new TFile(name.c_str(), "RECREATE");
	ncharged_per_event_hist->Write();
	ncharged_per_event_hist_no_weight_i_said_you_bro_no_weight_at_all_believe_me_please_i_am_not_joking_can_you_trust_me_at_least_one_time_bro_please_can_you_believe_i_ve_got_ligma->Write();
	nkstars_hist->Write();
	nantikstars_hist->Write();
	
	pythia.stat();
	delete output, ncharged_per_event_hist;
	
	return 0;
	
}

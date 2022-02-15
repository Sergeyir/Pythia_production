#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"

#include <iostream>
#include "TH1.h"
#include "TFile.h"

using namespace Pythia8;

int main(const unsigned int argc, const char *num[]) {

	TH1D *ncharged_per_event_hist, *ncharged_per_event_hist_no_weight_i_said_you_bro_no_weight_at_all_believe_me_please_i_am_not_joking_can_you_trust_me_at_least_one_time_bro_please_can_you_believe_i_ve_got_ligma;
	TFile *output;

	std::cout << "This is the process " << num[1] << std::endl;
	
	std::string set_seed("Random:seed = ");
	set_seed += num[1];
	
	Pythia pythia;
	
	pythia.readString("Beams:idA = 1000290630"); //Cu
	pythia.readString("Beams:idB = 1000791970"); //Au
	
	pythia.readString("Beams:eCM = 200.");
	//pythia.readString("PhaseSpace:pTHatMin = 0.");
	//pythia.readString("HardQCD:all = on");
	
	pythia.readString("HeavyIon:SigFitDefPar = 9.82,1.69,0.29,0.0,0.0,0.0,0.0,0.0");
	pythia.readString("HeavyIon:SigFitNGen = 0");
	
	pythia.readString("Random:setSeed = on");
	pythia.readString(set_seed.c_str());
	
	pythia.init();
	
	const long long unsigned int nEvents = 100000;
	
	unsigned int nbins = 200;
	
	ncharged_per_event_hist = new TH1D("ncharged", "ncharged", nbins, 0, nbins);
	ncharged_per_event_hist_no_weight_i_said_you_bro_no_weight_at_all_believe_me_please_i_am_not_joking_can_you_trust_me_at_least_one_time_bro_please_can_you_believe_i_ve_got_ligma = new TH1D("nchargednw", "nchargednw", nbins, 0, nbins);
	
	for (unsigned long int iEvent = 0; iEvent < nEvents; ++iEvent) {
	
		if (iEvent % 100 == 0) cout << "Events has passed: " << iEvent << endl;
		
		unsigned int ncharged = 0;
		
		if (!pythia.next()) continue;
		double weight = 0;
		
		for (int ipart = 0; ipart < pythia.event.size(); ++ipart) {
		
			if (pythia.event[ipart].isFinal() && pythia.event[ipart].isCharged()) {
			
				double eta = pythia.event[ipart].eta();
				if (eta > 3.1 && eta < 4) ncharged++;
				//ncharged++;
			}
			
		weight = pythia.info.weight();
		
		}
	
	ncharged_per_event_hist->Fill(ncharged);
	ncharged_per_event_hist_no_weight_i_said_you_bro_no_weight_at_all_believe_me_please_i_am_not_joking_can_you_trust_me_at_least_one_time_bro_please_can_you_believe_i_ve_got_ligma->Fill(ncharged, weight);
		
	}
	
	std::cout << "Output file is located at /home/sergey/pythis/pythia8306/pythia_production/output" << std::endl;
	
	std::string name = "/home/sergey/pythia/pythia8306/pythia_production/output/pythiaCuAu200_centr_";
	name += num[1];
	name.append("_");
	name.append(".root");
	
	output = new TFile(name.c_str(), "RECREATE");
	ncharged_per_event_hist->Write();
	ncharged_per_event_hist_no_weight_i_said_you_bro_no_weight_at_all_believe_me_please_i_am_not_joking_can_you_trust_me_at_least_one_time_bro_please_can_you_believe_i_ve_got_ligma->Write();
	
	pythia.stat();
	delete output, ncharged_per_event_hist;
	
	return 0;
	
}

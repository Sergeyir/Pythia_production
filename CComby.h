#pragma once

#include <array>
#include <cmath>
#include "TFile.h"
#include "TH2.h"
#include "Pythia8/Pythia.h"

class Particles;

class CComby: public Particles {
	
	private:
	
		static const unsigned int ch_number{9}; //number of decay channels
		
		bool check{true}; 
	
		TFile *output;
		
		std::vector <unsigned int> channels;
	
		std::array <const int, ch_number> pos_part_id{211, 211, 321, 321, 321, 2212, 2212, 211, 2212};
		std::array <const int, ch_number> neg_part_id{-211, -321, -211, -321, -2212, -321, -2212, -2212, -211};
	
		std::array <const std::string, ch_number> decay_channels{"PiPibar", "PiKbar", "KPibar", "KKbar", "KPbar", "PKbar", "PPbar", "PiPbar", "PPibar"};
	
		std::array <const std::string, ch_number> bg_hists_names{"PiPibar_BG", "PiKbar_BG", "KPibar_BG", "KKbar_BG", "KPbar_BG", "PKbar_BG", "PPbar_BG", "PiPbar_BG", "PPibar_BG"};
		std::array <const std::string, ch_number> fg_hists_names{"PiPibar_FG", "PiKbar_FG", "KPibar_FG", "KKbar_FG", "KPbar_FG", "PKbar_FG", "PPbar_FG", "PiPbar_FG", "PPibar_FG"};
	
		std::array <TH2F*, ch_number> fg_hists;
		std::array <TH2F*, ch_number> bg_hists;
		
		void ClearParticles() {
		
			p_id.clear();
			p_iEvent.clear();
			p_e.clear();
			p_px.clear();
			p_py.clear();
			p_pz.clear();
			
			pbar_id.clear();
			pbar_iEvent.clear();
			pbar_e.clear();
			pbar_px.clear();
			pbar_py.clear();
			pbar_pz.clear();
		
		}
		
		int GetChannelNum(std::string ch_name) {
			
			bool check = false;
			
			for (int ch_num = 0; ch_num < ch_number; ch_num++) {
			
				if (decay_channels[ch_num] == ch_name) {
				
					continue;
					check = true;
				
				};
			
			}
			
			
			if (check) return ch_number;
			
			else {
		
				std::cerr << "ERROR in CComby::GetChannelNum: There is no channel named " << ch_name << std::endl;
				exit(0);
			
			}
		}
	
	public:
	
		CComby() {	}
	
		int GetNumberOfChannels() {
		
			return ch_number;
	
		}
		
		void SetOutput(std::string output_name, std::string option = "RECREATE") {
		
			if (check) {
		
				output = new TFile(output_name.c_str(), option.c_str());
				check = false;
		
			}
		
			else {
		
				delete output;
				output = new TFile(output_name.c_str(), option.c_str());
			
			}	
			
		
		}
		
		void AddChannel(int ch_num) {
			
			for (int num = 0; num < channels.size(); num++) {
			
				if (channels[num] == ch_num) {
				
					std::cout << "Channel " << decay_channels[num] << "has already been addet to CComby object. This attempt of adding a new channel will be ignored" << std::endl;
					return;
					
				}
			
			}
			
			channels.push_back(ch_num);
			
			fg_hists[ch_num] = new TH2F(fg_hists_names[ch_num].c_str(), fg_hists_names[ch_num].c_str(), 80, 0, 8, 4000, 0, 4);
			bg_hists[ch_num] = new TH2F(bg_hists_names[ch_num].c_str(), bg_hists_names[ch_num].c_str(), 80, 0, 8, 4000, 0, 4);
		
		}
	
		void ProcessEvents() {
			
			for (int num; num < channels.size(); num ++) {
				
				const int p_size = p_id.size();
				const int pbar_size = pbar_id.size();
			
				for (int p_num = 0; p_num < p_size; p_num++) {
		
					for (int pbar_num = 0; pbar_num < pbar_size; pbar_num++) {
					
						if (p_id[p_num] != pos_part_id[channels[num]] || pbar_id[pbar_num] != neg_part_id[channels[num]]) continue;
						
						const double e2 = pow(p_e[p_num] + pbar_e[pbar_num], 2);
						const double p2 = pow(p_px[p_num] + pbar_px[pbar_num], 2) + pow(p_py[p_num] + pbar_py[pbar_num], 2) + pow(p_pz[p_num] + pbar_pz[pbar_num], 2);
						const double m = sqrt(e2 - p2);
						
						bg_hists[channels[num]]->Fill(sqrt(p2), m);
						
						if (p_iEvent[p_num] == pbar_iEvent[pbar_num]) {fg_hists[channels[num]]->Fill(sqrt(p2), m);}
					
					}
		
				}
		
			}
		
			ClearParticles();	
	
		}
		
		void Write() {
			
			for (int num = 0; num < channels.size(); num++) {
			
				fg_hists[channels[num]]->Write();
				bg_hists[channels[num]]->Write();
			
			}
		}
	
		~CComby() {
			
			
			
			for (int num = 0; num < channels.size(); num++) {
			
				delete fg_hists[channels[num]], bg_hists[channels[num]];
			
			}
			
			output->Close();
			
			delete output;
	
		}
		
};

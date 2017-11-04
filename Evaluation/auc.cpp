#include <iostream> 
#include <fstream> // std:ifstream
#include <sstream> // std:stringstream
#include <vector> // std:vector
#include <string> //std:stod

using namespace std;

struct label_score
{
	string label;
	double score;

	label_score(string _label, double _score): label(_label), score(_score){};
	bool operator<(const label_score& obj) const{
		return score < obj.score;
	}
	bool operator>(const label_score& obj) const{
		return score > obj.score;
	}
};

inline void split(const string &str, vector<string> &rst, const char clim=';'){
	rst.clear();
	stringstream ss(str);
	string tmp;
	while(getline(ss, tmp, clim)){
		rst.push_back(move(tmp));
	}
}


int main(){
   ifstream ifs("./sample_data/label_score.txt");
   vector<label_score> label_scores;

   int pos;
   string line;
   vector<string> splited;
   while(getline(ifs, line)){
   	split(line, splited, ',');
   	label_score ls(splited[0], stod(splited[1])	);
   	label_scores.push_back(ls);
   }

   sort(label_scores.begin(), label_scores.end(), [](label_score x, label_score y)->bool{return x>y;});

   int P, N; // Positive/Negative sample count
   double prev= -1; // used to compare 
   int rank = label_scores.size();
   double positive_rank; // summation of positve samples' rank  

   int prev_seq_len; // length of previous repeating sequence
   int last_positive_num; // number of positive sample of previous repeating sequence
   bool prev_score_same=0; // flag indicates whether previous scores sequence is repeating or not
   double repeating_rank_sums; // summation of rank for previous repeating sequence
   label_scores.push_back(label_score("dummy", -1)); //dummy node at the end 

   
   for(auto sample:label_scores){
      // update positive_rank
   	if(sample.score==prev){
   		prev_score_same = 1;
   		repeating_rank_sums += rank;
   	}else{
   		if(prev_score_same==1){
   			positive_rank += repeating_rank_sums*last_positive_num/prev_seq_len;
   		}else{
   			if(last_positive_num){
   				positive_rank += repeating_rank_sums;
   			}
   		}
		   repeating_rank_sums = rank;
   		prev_seq_len = 0;
   		prev_score_same = 0;
   		last_positive_num = 0;
   	}

   	// update P and N
   	prev_seq_len++;
   	if(sample.label == "1" || sample.label == "+1"){
   		P++;
   		last_positive_num++;
   	}else if(sample.label == "0" || sample.label == "-1"){
   		N++;
   	}else if(sample.label == "dummy"){}
   	else
   	{
   		cout << "label format error!" << endl;
   		exit(0);
   	}

   	// update prev and rank
   	prev = sample.score;
   	rank--;
   }

   double auc = (positive_rank-(1+P)*P/2.0)/(P*N);
   cout << "AUC:" << auc << endl;

}

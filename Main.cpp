//a fully working turing machine
//beginning state is always 0
//this program holds the logic and requires a text file with:
//head starting location
//a tape
//a finite table of instructions
//there are 4 files in the repository for 4 different situations
//1 runs and completes the program
//2 and 3 run the program until they try going out of tape's bounds and then are forced to stop
//4 is an endless loop that can be stopped by pressing the "esc" button

//this is a project I did as a university assignment
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <Windows.h>
using namespace std;

//error catching
class juosta_range {};
class no_match {};    
class manual_esc {};


//this function reads the text files into the program's variables
void read_file(string& choice, int& head, string& juosta, int& busena, vector <int>& cb, vector <int>& nb, vector <char>& cs, vector <char>& ns, vector <char>& kr) {
	cb.clear();         // vektoriai laikys po stulpeli is taisykliu
	cs.clear();         //tam kad ne recompile is naujo, reikia clear visus
	ns.clear();
	kr.clear();
	nb.clear();
	int ti1, ti2;
	char tc1, tc2, tc3;                             //skaitysiu faila i 5 atskyrus vektorius nes kitaip pats nesugalvojau
	busena = 0;                                     //pradine busena visada = 0
	string filename = choice + ".txt";                 // pasirinkimas 1-4, pridedu .txt kad iskart atidarytu
	ifstream reader(filename);                        // atidaro txt file
	reader >> head;                               // pirmas simbolis galvutes orig padetis
	head = head - 1;             //skaitysiu stringa kurio pirmas indexas yra 0
	cout << head << " - Head\n";
	reader >> ws;                        //ignoruoja tarpa
	getline(reader, juosta);                     // iskaito 2 linija i stringa juosta
	cout << "Juosta " << juosta << endl;
	while (reader >> ti1 >> tc1 >> tc2 >> tc3 >> ti2) {
		cb.push_back(ti1);          // esama busena
		cs.push_back(tc1);          // esamas simb
		ns.push_back(tc2);          // naujas simb
		kr.push_back(tc3);          // kryprtis
		nb.push_back(ti2);          // nauja busena
	}//close reader
}

//prints out the table of instructions
void print_prog_rules(vector <int>& cb, vector <int>& nb, vector <char>& cs, vector <char>& ns, vector <char>& kr) {
	for (int i = 0; i < cb.size(); ++i) {                                 // visu vectoriu dydis vienodas tai loopui tinka bet kokio size()
		cout << cb[i] << " " << cs[i] << " " << ns[i] << " " << kr[i] << " " << nb[i] << " \n";
	}
}

//where the turing magic happens

void turing(int& head, string& juosta, int& busena, vector <int>& curr_bus, vector <int>& new_bus, vector <char>& curr_sim, vector <char>& new_sim, vector <char>& kryptis) {
	//exiting turing function if the head goes out of tape's bounds OR
	//if there are no current state and current simbol combination in the instructions (meaning program ran it's course normally) OR
	//by pressing esc key

	bool matching_pair = 1;   // kiekviena loopa tikrins ar yra esamos busenos ir esamo simb pora taisyklese
	bool esc_check = 0;
	while (true) {
		//tiesiog norejau pazaist su throw ir catch =D
		if (head >= juosta.size() || head < 0) throw juosta_range{}; // jeigu galvute iseina uz juostos rybas
		if (matching_pair == 0) throw no_match{};  // jeigu taisyklese nerasta esamos busenos ir juosta[head] kombinacijos
		if (esc_check == 1) throw manual_esc{};  // jei paspaudziam esc vietoj to kad laukt kol programa baigs veikti pati
		else {
			matching_pair = 0;   // resetina po praeito rato
			for (int i = 0; i < curr_bus.size(); ++i) {          // nera skyrtumo kokio is 5 vektoriu size() naudosime loopui
				if (curr_bus[i] == busena && curr_sim[i] == juosta[head]) { //iesko esamos busenos ir juosta[head] kombinacijos taisyklese  

					juosta[head] = new_sim[i];

					busena = new_bus[i];

					if (kryptis[i] == 'R') ++head;
					else --head;

					cout << juosta << endl;
					matching_pair = 1;

					if (GetAsyncKeyState(VK_ESCAPE)) esc_check = 1;  //jeigu paspaustas 1 flagas uzsidega
				}
			}
		}
	}
}

//program holds instruction columns in 5 different vectors, tape in a string and head in an int

int main() {
	vector <int> curr_bus, new_bus;         //busenos yra intai
	vector <char> curr_sim, new_sim, kryptis;       //simboliai ir kriptys yra charai

	int head, busena;                       //head- juostos skaitomas symbolis, busena- programos busena
	string juosta, choice;                   // juosta laikys visa juosta, choice laikys koki faila norite atidaryti
	cout << "Select the file you want to work with (1-4): ";
	while (cin >> choice) {
		if (choice == "1" || choice == "2" || choice == "3" || choice == "4") {
			read_file(choice, head, juosta, busena, curr_bus, new_bus, curr_sim, new_sim, kryptis); // skaito info is failo
			print_prog_rules(curr_bus, new_bus, curr_sim, new_sim, kryptis);
			try {
				turing(head, juosta, busena, curr_bus, new_bus, curr_sim, new_sim, kryptis);
			}
			catch (manual_esc) { //jeigu yra aktyvuotas vienas is exceptionu, turingas baigtas
				cout << "User stopped the loop!\n";
				cout << "Turing machine halted!" << " If you want to continue, select a file you want to work with (1-4). If u want to exit press ctrl+z\n";
			}

			catch (...) { //jeigu yra aktyvuotas vienas is exceptionu, turingas baigtas
				cout << "Turing machine halted!" << " If you want to continue, select a file you want to work with (1-4). If u want to exit press ctrl+z\n";
			}
		}
		else cout << "No such file exists!\n" << "If you want to continue, select a file you want to work with (1-4). If u want to exit press ctrl+z\n";

	}
}


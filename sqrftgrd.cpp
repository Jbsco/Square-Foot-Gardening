/*******************************************************************
Instructions
Create a program that helps with managing a square foot garden.

Refactor 4 - Jacob B. Seman
*******************************************************************/
#include<iostream>
#include<fstream>

using namespace std;

void printDisplay(); // clears a predetermined area of the terminal

const int DISPHEIGHT=20; // number of lines to use for terminal "UI"

// individual class Plant(s), setup by user
class Plant{
	public:
		int size;
		string name;
		Plant(){
			size=0;
			name="";
		}
		friend istream & operator >> (istream &in,Plant &other){
			in >> other.name >> other.size;
			return in;
		}
};

// individual class Planter(s), containing type Plant(s), setup by user
const int MAXPLANTS=10;
class Planter{
	int sizeX,sizeY,pNum;
	Plant plantType[MAXPLANTS];
	public:
		Planter(){
			sizeX=0;
			sizeY=0;
			pNum=-1;
		}
		friend istream & operator >> (istream &in,Planter &other){
			other.pNum++;
			in >> other.sizeX >> other.sizeY >> other.plantType[other.pNum];
			cout << "\033[A\33[2K";
			return in;
		}
		void print(){
			int fitX=0,fitY=0,fitT=0;
			fitY=sizeY/plantType[pNum].size;
			fitX=(fitY*(sizeX/plantType[pNum].size));
			cout << "\r\n"; // correct positioning
			printf("\033[%iA",DISPHEIGHT-1); // move to top of display area
			for(int i=0;i<DISPHEIGHT-1;i++){ // print planter and count
				if(i<sizeY){
					cout << '|';
					for(int j=0;j<sizeX;j++){
						if(fitX>0&&(j%plantType[pNum].size)==0
								 &&(j+plantType[pNum].size)<=sizeX
								 &&(i%plantType[pNum].size)==0
								 &&(i+plantType[pNum].size)<=sizeY){
							cout << plantType[pNum].name.at(0) << '|';
							fitX--; // maintain accuracy
							fitT++; // keep a count
						}
						else cout << "_|"; // empty cell
					}
				}
				if(i==DISPHEIGHT-3) cout << "You can fit " << fitT << ' ' << plantType[pNum].name;
				cout << "\n";
			}
		}
};

// class containing Planter(s), setup by user, may save multiple Garden(s) using Data class
const int MAXPLANTERS=10;
class Garden{
	int pNum;
	Planter planters[MAXPLANTERS];
	public:
		Garden(){
			pNum=0;
		}
		void update(const Planter &newPlanter){
			planters[pNum]=newPlanter;
			pNum++;
		}
};

// helper class for data management of user Garden/Planter/Plant configurations
class Data{
	ofstream fout;
	ifstream fin;
	public:
		Data(){
		}
		int countLines(){  //  Subroutine
			fin.open("data.txt");
			int i = 0;
			string temp;
			while(getline(fin, temp)){
				i++;
			}
			fin.close();
			return i;
		}

		void getData(ifstream &inf, int n, string* d){  //  Subroutine
		//  prints contents of "data.txt" line-by-line
		//  fin is the ifstream object
		//  n is the numbers of lines to read in
		//  d is a dynamic array with enough memory to hold the data
			fin.open("data.txt");
			for(int i = 0; i < n; i++){
				getline(fin, d[i]);
				cout << d[i] << endl;
			}
			fin.close();
		}
};

int main(){
	printDisplay();
	string select;
	while(select!="quit"){
		if(select=="change"){
			Planter frontYard;
			cin >> frontYard;
			cout << "\033[A\33[2K";
			printDisplay();
			frontYard.print();
		}
		cin >> select;
	}
	return 0;
}

void printDisplay(){ // clear display area per DISPHEIGHT and print static lines
	printf("\033[%dA",DISPHEIGHT);
	for(int i=0;i<DISPHEIGHT;i++){
			cout << "\33[2K";
			if(i==0) cout << "Sqare Foot Gardening v0.4 - by Jacob Seman";
			if(i==DISPHEIGHT-1) cout << "Please enter: save, load, change, add, quit: ";
			if(i!=DISPHEIGHT-1) cout << "\n";
	}
}
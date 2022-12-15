/*******************************************************************
Instructions
Create a program that helps with managing a square foot garden.

Must handle plants that are 1x1, 2x2, 3x3 and 4x4
Must use multiple classes
Must have an explanation so it can be used by Karl to test
Menus/ASCII ART/ are optional but not required
Your application could be demonstrated to the class chosen at random
Empty hands only on this project.  No sharing of code or designs.
Just discuss and go your own way.
https://squarefootgardening.org/about-us/history/

+-+-+-+-+
| | | | |
+-+-+-+-+
| | | | |
+-+-+-+-+
| | | | |
+-+-+-+-+
| | | | |
+-+-+-+-+

"Plants"
"Small"
"Medium"
"large"
Multiple classes

Produce a reasonable report
-Could be fancy
- 2x2 and 3x3 cannot be in same box

Potential Sample input
4 onions
2 dwarve peachs
3 tomatoes
1 marigold

Potential Sample output
You need 4 boxes
Box 1 1 dwarve peach
Box 2 dwarve peach
Box 3 4 onions and 3 tomatoes
Box 4 marigold
*******************************************************************/
#include<iostream>
#include<fstream>

using namespace std;

void clearDisp(); // clears a predetermined area of the terminal

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
		void update(int newSize, string newName){ // initialize
			size=newSize;
			name=newName;
		}
		friend istream & operator >> (istream &in,Plant &other){
			in >> other.name >> other.size;
			return in;
		}
};

// individual class Planter(s), containing type Plant(s), setup by user
// take in dimensions, assign plants to planter, calculate number of plants that fit, etc
const int MAXPLANTS=10;
class Planter{ 
	int sizeX,sizeY,/*pQty[MAXPLANTS],*/pNum;
	Plant plantType[MAXPLANTS];
	public:
		Planter(){
			sizeX=0;
			sizeY=0;
			pNum=-1;
			plantType[0].update(0,"");
			//pQty[0]=0;
		}
		/*
		void update(int newSizeX,int newSizeY,const Plant &newPlantType){
			sizeX=newSizeX;
			sizeY=newSizeY;
			plantType[pNum]=newPlantType;
			pQty[pNum]=(sizeX/plantType[pNum].size)*(sizeY/plantType[pNum].size);
			pNum++;
		}
		*/
		friend istream & operator >> (istream &in,Planter &other){
			other.pNum++;
			in >> other.sizeX >> other.sizeY >> other.plantType[other.pNum];
			// other.updateQ(other);
			cout << "\033[A\33[2K";
			return in;
		}
		/*
		void updateQ(Planter &other){
			other.pQty[other.pNum]=(other.sizeX/other.plantType[other.pNum].size)*(other.sizeY/other.plantType[other.pNum].size);
			other.pNum++;
		}
		int quantity(){
			int i=0,qty=0;
			while(pQty[i]!=0){
				qty+=pQty[i];
				i++;
			}
			return (qty);
		}
		*/
		void print(){
			int fitX=0, fitY=0;
			fitY=sizeY/plantType[pNum].size;
			fitX=(fitY*(sizeX/plantType[pNum].size));
			cout << "\r\n";
			for(int i=0;i<DISPHEIGHT-1;i++){
				cout << "\033[A";
			}
			for(int i=0;i<DISPHEIGHT-1;i++){
				if(i<sizeY){
					cout << '|';
					for(int j=0;j<sizeX;j++){
						if(fitX>0&&(j%plantType[pNum].size)==0
								 &&(j+plantType[pNum].size)<=sizeX
								 &&(i%plantType[pNum].size)==0
								 &&(i+plantType[pNum].size)<=sizeY){
							cout << plantType[pNum].name.at(0) << '|';
							fitX--;
						}
						else cout << "_|";
					}
				}
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

// helper class for easy display of Garden/Planter/Plant setup to CLI
class Display{

	public:
		// constructor
		// display to CLI
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
	// display basic function and possible inputs "Menu"

		// select data, save, load, modify

	// display selected data configuration

		// update CLI display with changes
	clearDisp();
	string select;
	while(select!="quit"){
		if(select=="change"){
			Planter frontYard;
			cin >> frontYard;
			cout << "\033[A\33[2K";
			clearDisp();
			frontYard.print();
		}
		cin >> select;
	}
	return 0;
}

void clearDisp(){
	for(int i=0;i<2;i++){
		for(int j=0;j<DISPHEIGHT;j++){
			if(i==0) cout << "\033[A";
			else{
				cout << "\33[2K";
				if(j==0) cout << "Sqare Foot Gardening v0.4 - by Jacob Seman";
				if(j==DISPHEIGHT-1) cout << "Please enter: save, load, change, add, quit: ";
				if(j!=DISPHEIGHT-1) cout << "\n";
			}
		}
	}
}
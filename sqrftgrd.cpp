// consider a programming problem that takes in 'carrot' 'sage' 'dwarf peach' etc
// and outputs some solution for organizing this proposed garden by square footage
// carrots = 1x1
// sage = 2x2
// dwarf peach = 4x4
// etc
#include<iostream>
#include<cstring>

using namespace std;

class Plant{
	private:
		int side;
		int qty;
		string name;
	public:
		Plant(){
			side=0;
			qty=0;
			name="";
		}
		void update(int newSide, int quantity, string newName){
			side=newSide;
			qty=quantity;
			name=newName;
		}
		int area(){
			return (side*side);
		}
		int totalArea(){
			return (qty*(side*side));
		}
		friend istream & operator >> (istream &in,Plant &other){
			int s,q;
			string n;
			in >> s >> q >> n;
			other.update(s,q,n);
			return in;
		}
		friend ostream & operator << (ostream &out,const Plant &other){
			out << other.name << ":" << endl;
			out << "\tQuantity: " << other.qty << endl;
			out << "\tTotal area: " << (other.side*other.side) << endl;
			return out;
		}
};

class Planter{
	private:
		int sideX;
		int sideY;
	public:
		Planter(int newSideX, int newSideY){
			sideX=newSideX;
			sideY=newSideY;
		}
		int area(){
			return sideX*sideY;
		}
};

int main(){
	// input
	int index=0;
	cout << "Please input the number of plant types followed by the individual plant type, size required (single side of square area), and quantity\n";
	cout << "Ex: \"3\"; \"carrots 1 2\"; \"sage 2 3\"; \"dwarf-peach 4 1\":\n";
	cin >> index;
	string name[index];
	Plant plants[index];
	int amount[index],size[index],planter[2];
	for(int i=0;i<index;i++){
		cin >> name[i] >> size[i] >> amount[i];
		plants[i].update(size[i],amount[i],name[i]);
	}
	cout << "Please input the size of the planter in \'x y\' format:\n";
	cin >> planter[0] >> planter[1];
	Planter Planter(planter[0],planter[1]);

	// calculation
	int counter[index]={0};
	for(int i=0;i<index;i++){
		while(Planter.area()>counter[i]){
			counter[i]+=(Planter.area()/plants[i].area()); // to-do fix math
		}
	}

	// output
	for(int i=0;i<index;i++){
		cout << "You can fit " << counter[i] << " " << name[i];
		if(i<index-1) cout << ", or\n";
	}
	cout << endl;

	return 0;
}
/*******************************************************************************
Instructions
Create a program that helps with managing a square foot garden.

Refactor 5 - Jacob B. Seman
*******************************************************************************/

/*******************************************************************************
 Order of Operations:
 clear display area, ~20 lines or so
 print prompt/instructions/information
 take input from user - load,new,quit

 load:
 getlines from text file, prompt selections available
 take data from text file per selection
 populate and print saved garden/planters/plants
 take input from user - save,load,new,add,change,remove,quit

 new:
 create new empty garden
 take input from user - garden name
 take input from user - save,load,new,add,change,remove,quit

 save:
 push current garden/planters/plants configuration to data file
 leave current garden state unchanged
 take input from user - save,load,new,add,change,remove,quit

 add:
 add a planter or plant to the current garden
 take input from user - planter:
                          sizeX,sizeY
                        plant:
                          name,size,quantity
 compute garden arrangement after new planter or plant
 print updated garden
 take input from user - save,load,new,add,change,remove,quit

 change:
 change a planter or plant currently in the garden
 take input from user - change planter,change plant
 take input from user - planter:
                          sizeX,sizeY
                        plant:
                          size,name,quantity
 compute garden arrangement after changed planter or plant
 print updated garden
 take input from user - save,load,new,add,change,remove,quit

 remove:
 remove a planter or plant from the garden
 take input from user - planter,plant
 compute garden arrangement after removed planter or plant
 print updated garden
 take input from user - planter number,plant name

 quit:
 exit program, prompt to save
 take input from user - yes,no
 save if yes, exit program
 ******************************************************************************/

#include<bits/stdc++.h> // break out into required libraries after completion

using namespace std;

const int DISPHEIGHT=20; // number of lines to use for terminal "UI"
bool flag=0; // flag for help input

class Plant{ // Todo: fix encapsulation
// to be placed in planters by name, by garden class
// has size and quantity attributes that affect placement
    protected:
    public:
        string name;
        int size,quantity;
        Plant(){
            name="";
            size=0;
            quantity=0;
        }
        Plant(string newName,int newSize,int newQuantity){
            name=newName;
            size=newSize;
            quantity=newQuantity;
        }
        void update(string newName,int newSize,int newQuantity){
            name=newName;
            size=newSize;
            quantity=newQuantity;
        }
        bool operator ==(const string &other)const{
            return(name==other);
        }
        bool operator >(const Plant &other)const{
            return(size>other.size);
        }
		friend istream & operator >> (istream &in,Plant &other){
			in >> other.name >> other.size >> other.quantity;
			return in;
		}
};

class Planter{ // Todo: fix encapsulation
// to be placed in garden by number, by garden class
// has size in (x,y) attribute that affects plants able to be placed
    protected:
    public:
        int number,sizeX,sizeY;
        Planter(){
            number=0;
            sizeX=0;
            sizeY=0;
        }
        Planter(int newNumber,int newSizeX,int newSizeY){
            number=newNumber;
            sizeX=newSizeX;
            sizeY=newSizeY;
        }
        void update(int newSizeX,int newSizeY){
            sizeX=newSizeX;
            sizeY=newSizeY;
        }
        bool operator ==(const int &other)const{
            return(number==other);
        }
		friend istream & operator >> (istream &in,Planter &other){
			in >> other.sizeX >> other.sizeY;
			return in;
		}
};

class Garden{ // Todo: fix encapsulation, save, print
// container for multiple planters, plants - provision for user data
    protected:
    public:
        string dataName; // name to be used when saving/loading data
        vector<Planter> userPlanters; // planters in garden
        vector<Plant> userPlants; // plants in garden, assigned to planters
        Garden(){
            dataName="";
        }
        Garden(string newName){
            dataName=newName;
        }
        friend istream & operator >>(istream &in,Garden &other){
            other.reset(); // clear current garden's name and vectors
            in >> other.dataName;
            return in;
        }
        void add(const Planter newPlanter){ // add a planter to vector
            userPlanters.push_back(newPlanter);
            userPlanters.back().number=userPlanters.size();
        }
        void add(const Plant newPlant){ // add a plant to vector
            userPlants.push_back(newPlant);
        }
        int countLines(ifstream &inf){  // subroutine for getData
            inf.open("sfg-v5.txt");
            int i = 0;
            string temp;
            while(getline(inf, temp)){
                i++;
            }
            inf.close();
            return i;
        }
        void getData(ifstream &inf, int n, string* d){  // subroutine for data management
        //  gets contents of "sfg-v5.txt" line-by-line
        //  inf is the ifstream object
        //  n is the number of lines to read in
        //  d is a dynamic array with enough memory to hold the data
            inf.open("sfg-v5.txt");
            for(int i = 0; i < n; i++){
                getline(inf, d[i]);
                // cout << d[i] << endl;
            }
            inf.close();
        }
        void parseIn(){ // Todo: parse ifstream lines for dataName and delimit data
            // DATA SCHEME:
            // dataName ';' planter number, size x, y [loop all] ';' plant name, size, quantity [loop all] "\n"
            // Ex:
            // dataName;userPlanters[i].number,userPlanters[i].sizeX,userPlanters[i].sizeY;userPlants[j].name,userPlants[j].size,userPlants[j].quantity
            // Eg:
            // "karl;1,4,4,2,16,16,3,5,3;marigold,1,20,peach tree,4,2,tomatoes,3,10"
            ifstream fin;
            int lineNumber=countLines(fin);
            string newName[lineNumber];
            int i=0;
            while(!fin.eof()){
                getline(fin,newName[i],';');
                cout << newName[i] << endl;
                i++;
            }
        }
        string parseOut(){ // parse ofstream lines for current garden as delimited lines
            // DATA SCHEME:
            // dataName ';' planter number, size x, y [loop all] ';' plant name, size, quantity [loop all] "\n"
            // Ex:
            // dataName;userPlanters[i].number,userPlanters[i].sizeX,userPlanters[i].sizeY;userPlants[j].name,userPlants[j].size,userPlants[j].quantity
            // Eg:
            // "karl;1,4,4,2,16,16,3,5,3;marigold,1,20,peach_tree,4,2,tomato,3,10"
            string out=dataName;
            out.append(";");
            for(int i=0;i<(int)userPlanters.size();i++){
                out.append(to_string(userPlanters[i].number));
                out.append(",");
                out.append(to_string(userPlanters[i].sizeX));
                out.append(",");
                out.append(to_string(userPlanters[i].sizeY));
                if(i!=(int)userPlanters.size()-1) out.append(",");
                else out.append(";");
            }
            for(int i=0;i<(int)userPlants.size();i++){
                out.append(userPlants[i].name);
                out.append(",");
                out.append(to_string(userPlants[i].size));
                out.append(",");
                out.append(to_string(userPlants[i].quantity));
                if(i!=(int)userPlants.size()-1) out.append(",");
            }
            return out;
        }
        void load(){ // Todo: parse() subroutine
            ifstream fin;                       
            int lineNumber = countLines(fin);
            string * data = new string[lineNumber];
            getData(fin, lineNumber, data);
            // call parse() to get requested data
            // initialize garden with data
        }
        void save(){ // Todo: parse existing data and check for overwrite
            ifstream fin;                       
            int lineNumber = countLines(fin);
            string * data = new string[lineNumber];
            getData(fin, lineNumber, data);
            // check for dataName and overwrite
            for(int i=0;i<lineNumber;i++){
                // check data[i] for "NAME:xxxx"==dataName
                
            }
            // else write new line
            ofstream fout("sfg-v5.txt");
            for(int i = 0; i < lineNumber; i++){
                fout << data[i] << endl;
            }

            fout << parseOut() << endl;
            fout.close();

        }
        void print(bool flag){ // Todo print current state and note planter number, plant fitment, issues (if any)
            if(dataName==""||flag){ // if dataName is empty (ex. after new>reset) or help flag is toggled
			    printf("\033[%iA",DISPHEIGHT-1); // move to top of display area
                for(int i=0;i<DISPHEIGHT;i++){ // print instructions/usage information
                    cout << "\33[2K";
			        if(i==0) cout << "\rSquare Foot Gardening v0.5 - by Jacob Seman"; // so THAT'S who's responsible for all this kludge
                    if(i==2) cout << "Welcome to Square Foot Gardening!";
                    if(i==3) cout << "You may save your Garden by name,";
                    if(i==4) cout << "and restore it by using the 'save'/'load' functions.";
                    if(i==6) cout << "A Garden may have multiple Planters of different size.";
                    if(i==7) cout << "You may add Plants of any size and name.";
                    if(i==9) cout << "The program will attempt to place the largest Plants first,";
                    if(i==10) cout << "and will alert you if any Plants will not fit in available Planter space.";
                    if(i==12) cout << "This message may be accessed at any time by typing \"help\",";
                    if(i==13) cout << "type help again to return to the Garden display.";
			        if(i!=DISPHEIGHT-1) cout << "\n"; // newline for all but last
			        if(i==DISPHEIGHT-1) cout << "Please enter: new, save, load, change, add, remove, help, quit: ";
                }
            }
            else{
                // Todo place plants according to size/fitment and quantity
                // print available planters in order by number
                // print available plants in planters by size, placed amount should not exceed quantity per plant
                // sort userPlants by userPlants[i].size and place largest first
			    printf("\033[%iA",DISPHEIGHT-1); // move to top of display area
                int scanline=1;
                for(int i=0;i<DISPHEIGHT;i++){
                    cout << "\33[2K";
			        if(i==0) cout << "\rSquare Foot Gardening v0.5 - by Jacob Seman"; // so THAT'S who's responsible for all this kludge
                    /* test output - just planters and plants w/attributes
                    if(i==2){
                        // prints planters and attributes by number
                        for(int j=0;j<(int)userPlanters.size();j++){
                            cout << "Planter #: " << userPlanters[j].number << ", " << userPlanters[j].sizeX << "x" << userPlanters[j].sizeY;
                            if(j!=(int)userPlanters.size()-1) cout << "; ";
                        }                        
                    }
                    if(i==4){
                        // prints plants and attributes by name
                        for(int j=0;j<(int)userPlants.size();j++){
                            cout << "Plant: " << userPlants[j].name << ", size: " << userPlants[j].size << ", qty:" << userPlants[j].quantity;
                            if(j!=(int)userPlants.size()-1) cout << "; ";
                        }                        
                    }
                    */
                    if(i==scanline){ // testing variable line checking
                        if(i==1){ // print corresponding number headers above each planter
                            for(int j=0;j<(int)userPlanters.size();j++){
                                for(int k=0;k<userPlanters[j].sizeX*2+1;k++){
                                    if(k==0) cout << userPlanters[j].number; // print planter number on upper left
                                    else if(k==userPlanters[j].sizeX*2) cout << ' '; // prevent overhanging upper right corner
                                    else cout << '_'; // print upper border
                                }
                                cout << ' '; // correct spacing between planters
                            }
                        }
                        if(i>1&&i<DISPHEIGHT-1){ // print lines for planters
                            for(int j=0;j<(int)userPlanters.size();j++){ // print cells per number of planters
                                for(int k=0;k<userPlanters[j].sizeX;k++){ // print cells per size of planter[j]
                                    if(scanline-2<userPlanters[j].sizeY&&k==0) cout << '|';
                                    if(scanline-2<userPlanters[j].sizeY) cout << "_|"; // print if within sizeY
                                    else{
                                        if(k==0) cout << ' ';
                                        cout << "  "; // double space if sizeY exceeded
                                    }
                                }
                                cout << ' ';
                            }
                        }
                        if(scanline<DISPHEIGHT-1) scanline++;
                    }
			        if(i!=DISPHEIGHT-1) cout << "\n"; // newline for all but last
                    // print footer
			        if(i==DISPHEIGHT-1) cout << "Please enter: new, save, load, change, add, remove, help, quit: ";
                }
                // maybe call a Plant insertion function here?
                // function would need a lot of escape movement and to return to the same point...
                subPrint();
            }
        }
        void subPrint(){
            /*
            - Move the cursor up N lines:
            \033[<N>A
            - Move the cursor down N lines:
            \033[<N>B
            - Move the cursor forward N columns:
            \033[<N>C
            - Move the cursor backward N columns:
            \033[<N>D
            */
            cout << "\033[s"; // save cursor pos

            sort(userPlants.begin(),userPlants.end(),greater<Plant>()); // sort Plant vector by Plant.size, descending
            printf("\033[%iA\r",DISPHEIGHT-3); // move to top left of planter display area
            int fitX[(int)userPlanters.size()]={0}; // fitY per planter
            int fitY[(int)userPlanters.size()]={0}; // fitX per planter
            int qty[(int)userPlants.size()]={0}; // qty per plant
            for(int i=0;i<(int)userPlants.size();i++){ // per plant
                qty[i]=userPlants[i].quantity; // set current plant[i] quantity
                for(int j=0;j<(int)userPlanters.size();j++){ // per planter
                    fitY[i]=userPlanters[j].sizeY/userPlants[i].size; // set current plant[i] fitY for current planter[j].sizeY
                    fitX[i]=fitY[i]*(userPlanters[j].sizeX/userPlants[i].size); // set current plant[i] fitX for current planter[j]/sizeX
                    for(int k=0;k<userPlanters[j].sizeY;k++){ // per planter[j].sizeY
                        printf("\033[1C"); // move to the first empty planter cell
                        for(int l=0;l<userPlanters[j].sizeX;l++){ // per planter[j].sizeX
                            if(qty[i]>0){ // while a qty exists
                                if(fitX[i]>0&&(l%userPlants[i].size)==0 // while fitX remains
                                        &&(l+userPlants[i].size)<=userPlanters[j].sizeX
                                        &&(k%userPlants[i].size)==0
                                        &&(k+userPlants[i].size)<=userPlanters[j].sizeY){
                                    cout << userPlants[i].name.at(0) << "\033[1C";
                                    fitX[i]--;
                                    qty[i]--;
                                        }
                                else printf("\033[2C");
                            }
                        }
                        cout << "\n";
                    }
                    printf("\033[%iA\033[%iC",userPlanters[j].sizeY,userPlanters[j].sizeX*2+2); // move cursor to next planter upper left
                }
            }

            cout << "\033[u"; // restore cursor pos
        }
        void change(const int &changePlanter){ // change a specific planter
            cout << "input your planter dimensions in 'x' 'y': ";
            for(int i=0;i<(int)userPlanters.size();i++){
                if(userPlanters[i].number==changePlanter){
                    cin >> userPlanters[i];
                    printf("\033[A\33[2K"); // move cursor up, clear line
                }
            }
        }
        void change(const string &changePlant){ // change a specific plant
            cout << "input your plant name, size, and quantity: ";
            for(int i=0;i<(int)userPlants.size();i++){
                if(userPlants[i]==changePlant){
                    cin >> userPlants[i];
                    printf("\033[A\33[2K"); // move cursor up, clear line
                }
            }
        }
        void reset(){ // clear all - name and vectors
            dataName="";
            userPlanters.clear();
            userPlants.clear();
        }
        void remove(int &remPlanter){ // remove planter from vector
            for(int i=0;i<(int)userPlanters.size();i++){
                if(userPlanters[i].number==remPlanter)
                    userPlanters.erase(userPlanters.begin()+i);
            }
        }
        void remove(string &remPlant){ // remove plant from vector
            for(int i=0;i<(int)userPlants.size();i++){
                if(userPlants[i].name==remPlant)
                    userPlants.erase(userPlants.begin()+i);
            }
        }
};

/* *this function from refactor 4 only clears the UI area* */
/* not needed as of v5
void initDisplay(){ // clear display area per DISPHEIGHT and print static lines
	printf("\033[%dA",DISPHEIGHT); // move cursor up to top of UI space
	for(int i=0;i<DISPHEIGHT;i++){
			cout << "\33[2K"; // clear current line in loop
			if(i==0) cout << "Square Foot Gardening v0.5 - by Jacob Seman";
			if(i!=DISPHEIGHT-1) cout << "\n"; // newline for all but last
			if(i==DISPHEIGHT-1) cout << "Please enter: new, save, load, change, add, remove, quit: ";
	}
}
*/

void inputFcn(string &input,Garden &current){ // Todo: fix load, save
    current.print(flag);
    cin >> input;
    if(input=="help"){ // Todo function: show initial functions screen again
        printf("\033[A\33[2K"); // move cursor up, clear line
        flag=!flag;
    }
    else if(input=="load"){ // Todo function: load()
        current.parseIn();
        printf("\033[A\33[2K"); // move cursor up, clear line
    }
    else if(input=="save"){ // Todo current.save()
        current.save();
        printf("\033[A\33[2K"); // move cursor up, clear line
    }
    else if(input=="new"){ // clears current Garden vectors and updates name
        printf("\033[A\33[2K"); // move cursor up, clear line
        cout << "Input your new garden's name, for save/load purposes: ";
        cin >> current;
        printf("\033[A\33[2K"); // move cursor up, clear line
    }
    else if(input=="add"){ // adds planter or plant to current garden
        string choice;
        printf("\033[A\33[2K"); // move cursor up, clear line
        cout << "Add a planter or plant? ";
        cin >> choice; // determine planter or plant
        printf("\033[A\33[2K"); // move cursor up, clear line
        if(choice=="planter"){
            Planter pNew;
            cout << "input your planter dimensions in 'x' 'y': ";
            cin >> pNew;
            printf("\033[A\33[2K"); // move cursor up, clear line
            current.add(pNew);
        }
        else if(choice=="plant"){
            Plant pNew;
            cout << "input your plant name, size, and quantity: ";
            cin >> pNew;
            printf("\033[A\33[2K"); // move cursor up, clear line
            current.add(pNew);
        }
        else{
            printf("\033[A\33[2K");
            inputFcn(input,current);
        }
    }
    else if(input=="change"){ // changes planter or plant in current garden
        string choice;
        printf("\033[A\33[2K"); // move cursor up, clear line
        cout << "Change planter or plant? ";
        cin >> choice; // determine planter or plant
        printf("\033[A\33[2K"); // move cursor up, clear line
        if(choice=="planter"){
            int pNum;
            cout << "Input planter number to change: ";
            cin >> pNum;
            printf("\033[A\33[2K"); // move cursor up, clear line
            current.change(pNum);
        }
        else if(choice=="plant"){
            string pName;
            cout << "Input plant name to change: ";
            cin >> pName;
            printf("\033[A\33[2K"); // move cursor up, clear line
            current.change(pName);
        }
        else{
            printf("\033[A\33[2K");
            inputFcn(input,current);
        }
    }
    else if(input=="remove"){ // removes planter or plant from current garden
        string choice;
        printf("\033[A\33[2K"); // move cursor up, clear line
        cout << "Remove planter or plant? ";
        cin >> choice; // determine planter or plant
        printf("\033[A\33[2K"); // move cursor up, clear line
        if(choice=="planter"){
            int pNum;
            cout << "Input planter number to remove: ";
            cin >> pNum;
            printf("\033[A\33[2K"); // move cursor up, clear line
            current.remove(pNum);
        }
        else if(choice=="plant"){
            string pName;
            cout << "Input plant name to remove: ";
            cin >> pName;
            printf("\033[A\33[2K"); // move cursor up, clear line
            current.remove(pName);
        }
        else{
            printf("\033[A\33[2K");
            inputFcn(input,current);
        }
    }
    else if(input=="quit") return;
    else{
        printf("\033[A\33[2K");
        inputFcn(input,current);
    }
}

/* not currently used in v5
void load(){ // Todo: function to load garden/planters/plants from file

}
*/

int main(){
    string input;
    Garden current;
    while(input!="quit"){
        /* moved inside function for input control
        current.print(flag);
        current.parseIn();
        cin >> input;
        printf("\033[A\33[2K"); // move cursor up, clear line
        */
        inputFcn(input,current);
    }
	return 0;
}
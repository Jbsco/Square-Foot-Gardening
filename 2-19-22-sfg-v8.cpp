/*******************************************************************************
 *  INSTRUCTIONS:
 * Create a program that helps with managing a square foot garden.
 * 
 * Refactor 8 - Jacob B. Seman
*******************************************************************************/

/*******************************************************************************
 *  IDEA:
 * clear display area, ~20 lines, add more if planter exceeds height
 * print prompt/instructions/information
 * take input from user - new,save,load,add,change,remove,quit
 * 
 *  NEW:
 * create new empty garden
 * take input from user - garden name
 * this name is used to identify data
 *
 *  SAVE:
 * push current garden/planters/plants configuration to data file
 * leave current garden state unchanged
 *
 *  LOAD:
 * getlines from text file, prompt selections available
 * take data from text file per selection
 * populate and print saved garden/planters/plants
 * take input from user - save,load,new,add,change,remove,quit
 *
 *  ADD:
 * add a planter or plant to the current garden
 * take input from user - planter:
 *                          sizeX,sizeY
 *                        plant:
 *                          name,size,quantity
 * compute garden arrangement after new planter or plant
 * print updated garden
 * if a new planter sizeY exceeds 16, display height will be increased
 * accordingly
 *
 *  CHANGE:
 * change a planter or plant currently in the garden
 * take input from user - change planter,change plant
 * take input from user - planter number:
 *                          sizeX,sizeY
 *                        plant name:
 *                          size,name,quantity
 * compute garden arrangement after changing planter or plant
 * print updated garden
 *
 *  REMOVE:
 * remove a planter or plant from the garden
 * take input from user - planter,plant
 * compute garden arrangement after removing planter or plant
 * print updated garden
 * 
 *  HELP:
 * toggles the introductory help screen
 * 
 *  OPTIONS:
 * configure the display height unless a larger planter is overriding this
 * choose a display character for plant fill
 *      lower case plant.name[0]
 *      '-'
 *      '~'
 *      'x'
 *      '.'
 *
 *  QUIT:
 * exit program, prompt to save
 * take input from user - yes,no,cancel
 * save if yes, exit program unless cancel
*******************************************************************************/

/*******************************************************************************
 *  TODO: LIST v8
 * -fix encapsulation
 * -loading of data yet
 * -verbose output of allocated and excess plant quantities
*******************************************************************************/

#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<algorithm>

using namespace std;

int DISPHEIGHT=20; // number of lines to use for terminal "UI", change in options
char FILL='_'; // default character to use when filling space, change in options
bool flag=1; // flag for help input

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
        bool operator ==(const string &other)const{ // comparator overload for name equivalence
            return(name==other);
        }
        bool operator >(const Plant &other)const{ // comparator overload for sort(greater<Plant>())
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
            if(other.sizeY>16) DISPHEIGHT=other.sizeY+4; // allow for increasing display height for larger planters
			return in;
		}
};

class Garden{ // Todo: fix encapsulation, load function
// container for multiple planters, plants - provision for display and user data
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
            inf.open("sfg-v8.txt");
            int i = 0;
            string temp;
            while(getline(inf, temp)){
                i++;
            }
            inf.close();
            return i;
        }
        void getData(ifstream &inf, int n, string* d){  // subroutine for data management
        //  gets contents of "sfg-v7.txt" line-by-line
        //  inf is the ifstream object
        //  n is the number of lines to read in
        //  d is a dynamic array with enough memory to hold the data
            inf.open("sfg-v8.txt");
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
            ofstream fout("sfg-v8.txt");
            for(int i = 0; i < lineNumber; i++){
                fout << data[i] << endl;
            }

            fout << parseOut() << endl;
            fout.close();

        }
        void newPrint(bool flag){ // Todo print plant fitment quantities, issues (if any)
            if(flag){ // if flag is toggled
			    printf("\033[%iA\r",DISPHEIGHT-1); // move to top of display area
                for(int i=0;i<DISPHEIGHT;i++){ // print instructions/usage information
                    cout << "\33[2K";
			        if(i==0) cout << "Square Foot Gardening v0.8 - by Jacob Seman"; // so THAT'S who's responsible for all this kludge
                    if(i==2) cout << "Welcome to Square Foot Gardening!";
                    if(i==3) cout << "You may save your Garden by name,";
                    if(i==4) cout << "and restore it by using the 'save'/'load' functions.";
                    if(i==6) cout << "A Garden may have multiple Planters of different size.";
                    if(i==7) cout << "You may add Plants of any size and name.";
                    if(i==9) cout << "The program will place the largest Plants first,";
                    if(i==10) cout << "and will alert you if any Plants will not fit in available Planter space.";
                    if(i==12) cout << "This message may be accessed at any time by typing \"help\",";
                    if(i==13) cout << "type help again to toggle the Garden display.";
			        if(i!=DISPHEIGHT-1) cout << "\n"; // newline for all but last
			        if(i==DISPHEIGHT-1) cout << "Please enter: new, save, load, add, change, remove, help, options, quit: ";
                }
            }
            else{
                // printing all planters
                sort(userPlants.begin(),userPlants.end(),greater<Plant>()); // sort Plant vector by Plant.size, descending
                char cells[(int)userPlanters.size()][50][50]; // initialize char array cells[planter qty][x][y]
                memset(cells,'_',sizeof(cells));
                int qty[(int)userPlants.size()];
                for(int j=0;j<(int)userPlants.size();j++) qty[j]=userPlants[j].quantity;
                // *big breath* populate array for planter fill
                for(int j=0;j<(int)userPlanters.size();j++){ // per planter
                    for(int k=0;k<userPlanters[j].sizeY;k++){ // per planter sizeY
                        for(int l=0;l<userPlanters[j].sizeX;l++){ // per planter sizeX
                            for(int m=0;m<(int)userPlants.size();m++){ // per plant
                                // check clearance to right and below current pos
                                bool check=1;
                                for(int cI=0;cI<userPlants[m].size;cI++){
                                    for(int tI=0,tJ=0;tJ<userPlants[m].size;tI++){
                                        if(cells[j][l+tI][k+tJ]!='_') check=0;
                                        if(tI+1==userPlants[m].size) tI=-1,tJ++;
                                    }
                                }
                                if(cells[j][l][k]=='_'
                                    &&check
                                    &&userPlants[m].size<=userPlanters[j].sizeX-l
                                    &&userPlants[m].size<=userPlanters[j].sizeY-k
                                    &&qty[m]>0){ // if array space unnallocated, clearance and plant.qty available
                                    for(int tI=0,tJ=0;tJ<userPlants[m].size;tI++){ // offsets based on plant[m].size
                                        if(tI==0&&tJ==0){
                                            cells[j][l][k]=toupper(userPlants[m].name.at(0)); // upper left cell -> make upper case
                                            qty[m]--;
                                        }
                                        else{
                                            if(FILL=='_') cells[j][l+tI][k+tJ]=tolower(userPlants[m].name.at(0)); // all other cells in plant size range -> make lower case
                                            else cells[j][l+tI][k+tJ]=FILL; // if FILL is set by user use this char instead
                                        }
                                        if(tI+1==userPlants[m].size) tI=-1,tJ++; // reset x pos, increment y pos
                                    }
                                }
                            }
                        }
                    }
                }
                // cell array should be populated, print array
                printf("\033[%iA",DISPHEIGHT-1);
                for(int i=0;i<DISPHEIGHT;i++){
                    cout << "\33[2K";
                    if(i==0) cout << "Square Foot Gardening v0.8 - by Jacob Seman";
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
                    if(i>1&&i<DISPHEIGHT-2){
                        for(int j=0;j<(int)userPlanters.size();j++){
                            for(int k=0;k<userPlanters[j].sizeX+1;k++){
                                if(i-2<userPlanters[j].sizeY){
                                    // print while in planter[j] range
                                    if(k==0) cout << '|'; // leftmost edge
                                    else cout << cells[j][k-1][i-2] << '|';
                                }
                                else{
                                    // print while out of planter[j] range
                                    if(k==0) cout << ' ';
                                    else cout << "  ";
                                }
                            }
                            cout << ' ';
                        }
                    }
			        if(i!=DISPHEIGHT-1) cout << "\n"; // newline for all but last
                    // print footer
			        if(i==DISPHEIGHT-1) cout << "Please enter: new, save, load, change, add, remove, help, options, quit: ";
                }
            }
        }
        void change(const int &changePlanter){ // change a specific planter
            cout << "This planter has dimensions: " << userPlanters[changePlanter].sizeX << " by " << userPlanters[changePlanter].sizeY << ", input new dimensions in 'x' 'y': ";
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
        void dispPlant(){
            for(int i=0;i<(int)userPlants.size();i++){
                cout << userPlants[i].name;
                if(i+1<(int)userPlants.size()) cout << ", ";
                else cout << ". ";
            }
        }
};

void inputFcn(string &input,Garden &current){ // Todo: fix load
    current.newPrint(flag);
    cin >> input;
    if(input=="help"){ // Todo function: show initial functions screen again
        printf("\033[A\33[2K"); // move cursor up, clear line
        flag=!flag;
    }
    else if(input=="load"){ // Todo function: load()
        flag=0;
        current.parseIn();
        printf("\033[A\33[2K"); // move cursor up, clear line
    }
    else if(input=="save"){ // Todo current.save()
        flag=0;
        if(current.dataName=="") current.dataName="default";
        current.save();
        printf("\033[A\33[2K"); // move cursor up, clear line
    }
    else if(input=="new"){ // clears current Garden vectors and updates name
        flag=0;
        printf("\033[A\33[2K"); // move cursor up, clear line
        cout << "Input your new garden's name, for save/load purposes: ";
        cin >> current;
        printf("\033[A\33[2K"); // move cursor up, clear line
    }
    else if(input=="add"){ // adds planter or plant to current garden
        flag=0;
        if(current.dataName=="") current.dataName="default";
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
        flag=0;
        if(current.dataName=="") current.dataName="default";
        string choice;
        printf("\033[A\33[2K"); // move cursor up, clear line
        cout << "Change garden, planter, or plant? ";
        cin >> choice; // determine planter or plant
        printf("\033[A\33[2K"); // move cursor up, clear line
        if(choice=="garden"){
            cout << "Current name: " << current.dataName << ", input a new name for this garden: ";
            cin >> current.dataName;
            printf("\033[A\33[2K"); // move cursor up, clear line
        }
        if(choice=="planter"){
            int pNum;
            cout << "Input planter number to change: ";
            cin >> pNum;
            printf("\033[A\33[2K"); // move cursor up, clear line
            current.change(pNum);
        }
        else if(choice=="plant"){
            string pName;
            cout << "Current plants are: ";
            current.dispPlant();
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
        flag=0;
        if(current.dataName=="") current.dataName="default";
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
            cout << "Current plants are: ";
            current.dispPlant();
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
    else if(input=="options"){
        printf("\033[A\33[2K");
        cout << "Change DISPHEIGHT or default plant space fill character? "
             << "Input display or character: ";
        string option;
        cin >> option;
        printf("\033[A\33[2K"); // move cursor up, clear line
        if(option=="display"){
            cout << "Input a new DISPHEIGHT (default is 20): ";
            cin >> DISPHEIGHT;
            printf("\033[A\33[2K"); // move cursor up, clear line
        }
        else if(option=="character"){
            cout << "Input a new ASCII character to use for plant space fill,"
                 << " or type default to use lower case per plant name: ";
            string ASCII;
            cin >> ASCII;
            if(ASCII=="default") FILL='_';
            else FILL=(char)ASCII.at(0);
            printf("\033[A\33[2K"); // move cursor up, clear line
        }

    }
    else if(input=="quit"){
        printf("\033[A\33[2K");
        cout << "Do you want to save? Input yes, no, or cancel: ";
        string quitter;
        cin >> quitter;
        printf("\033[A\33[2K"); // move cursor up, clear line
        if(quitter=="yes"){
            current.save();
            return;
        }
        else if(quitter=="cancel"){
            printf("\033[A\33[2K");
            inputFcn(input,current);
        }
        else if(quitter=="no") return;
        else{
            printf("\033[A\33[2K");
            inputFcn(input,current);
        }
    }
    else{
        printf("\033[A\33[2K");
        inputFcn(input,current);
    }
}

int main(){ // expansive things come in minimal mains...
    string input;
    Garden current;
    while(input!="quit"){
        inputFcn(input,current);
    }
	return 0;
}
/*******************************************************************************
 *  SECRET:
 * make a secret randomize function that randomly sizes 4-5 planters and loads
 * plants from the all the save data that it can and places them randomly to
 * showcase the functionality and dymanic handling.
 * 
*******************************************************************************/

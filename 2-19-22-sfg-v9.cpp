/*******************************************************************************
 *  INSTRUCTIONS:
 * Create a program that helps with managing a square foot garden.
 * 
 * Version 9 - Jacob B. Seman
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
 *  TODO: LIST v9
 * -fix encapsulation
 * -verbose output of allocated and excess plant quantities
*******************************************************************************/

#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<algorithm>
#include<sstream>
#include<unistd.h>

using namespace std;

int DISPHEIGHT=20; // number of lines to use for terminal "UI", change in options
char FILL='_'; // default character to use when filling space, change in options
bool flag=1; // flag for help input
const string dataFile="sfg-v9.txt"; // filename used across data management functions

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
        Plant(string &newName,int &newSize,int &newQuantity){
            name=newName;
            size=newSize;
            quantity=newQuantity;
        }
        void update(string &newName,int &newSize,int &newQuantity){
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
        Planter(int &newNumber,int &newSizeX,int &newSizeY){
            if(newSizeY>16) DISPHEIGHT=newSizeY+4; // allow for increasing display height for larger planters
            number=newNumber;
            sizeX=newSizeX;
            sizeY=newSizeY;
        }
        void update(int &newSizeX,int &newSizeY){
            if(newSizeY>16) DISPHEIGHT=newSizeY+4; // allow for increasing display height for larger planters
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

class Garden{ // Todo: fix encapsulation, verbose allocation results
// container for multiple planters, plants - provision for display and user data
    protected:
    public:
        string dataName; // name to be used when saving/loading data
        vector<Planter> userPlanters; // planters in garden
        vector<Plant> userPlants; // plants in garden, assigned to planters
        Garden(){
            dataName="";
        }
        Garden(string &newName){
            dataName=newName;
        }
        friend istream & operator >>(istream &in,Garden &other){
            other.reset(); // clear current garden's name and vectors
            in >> other.dataName;
            return in;
        }
        void add(const Planter &newPlanter){ // add a planter to vector
            userPlanters.push_back(newPlanter);
            userPlanters.back().number=userPlanters.size();
        }
        void add(const Plant &newPlant){ // add a plant to vector
            userPlants.push_back(newPlant);
        }
        int countLines(ifstream &inf){  // subroutine for getData
            inf.open(dataFile);
            int i=0;
            string temp;
            while(getline(inf,temp)){
                i++;
            }
            inf.close();
            return i;
        }
        void getData(ifstream &inf, int n, string* d){  // subroutine for data management
        //  gets contents of dataFile line-by-line
        //  inf is the ifstream object
        //  n is the number of lines to read in
        //  d is a dynamic array with enough memory to hold the data
            inf.open(dataFile);
            for(int i=0;i<n;i++){
                getline(inf,d[i]);
                // cout << d[i] << endl;
            }
            inf.close();
        }
        void parseIn(string &data){ // parse ifstream lines for dataName and delimit data
            // DATA SCHEME:
            // dataName ';' planter number, size x, y [loop all] ';' plant name, size, quantity [loop all] "\n"
            // Ex:
            // dataName;userPlanters[i].number,userPlanters[i].sizeX,userPlanters[i].sizeY;userPlants[j].name,userPlants[j].size,userPlants[j].quantity
            // Eg:
            // "karl;1,4,4,2,16,16,3,5,3;marigold,1,20,peach tree,4,2,tomatoes,3,10"
            stringstream ss(data);
            string chunk[4];
             // first chunk, load the dataName
            int i=0;
            while(!ss.eof()){
                if(i<3) getline(ss,chunk[i],';');
                else getline(ss,chunk[i]);
                i++;
            }
            dataName=chunk[0];
            // second chunk, load planters
            i=0;
            stringstream pltrs(chunk[1]);
            string subChunk;
            int pN,pX,pY;
            while(!pltrs.eof()){
                getline(pltrs,subChunk,',');
                if(i==0){
                    pN=stoi(subChunk);
                    i++;
                }
                else if(i==1){
                    pX=stoi(subChunk);
                    i++;
                }
                else if(i>=2){
                    pY=stoi(subChunk);
                    Planter pltrN(pN,pX,pY);
                    add(pltrN);
                    i=0;
                }
            }
            // third chunk, load plants
            stringstream plts(chunk[2]);
            string subPChunk, pName;
            i=0;
            while(!plts.eof()){
                getline(plts,subPChunk,',');
                if(i==0){
                    pName=subPChunk;
                    i++;
                }
                else if(i==1){
                    pX=stoi(subPChunk);
                    i++;
                }
                else if(i>=2){
                    pY=stoi(subPChunk);
                    Plant pltN(pName,pX,pY);
                    add(pltN);
                    i=0;
                }
            }
            // final chunk! get FILL preference
            FILL=chunk[3].at(0);
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
            out.append(";");
            out.push_back(FILL);
            return out;
        }
        void load(string &loadName){ // load data into string, call parseIn
            ifstream fin;
            int lineNumber=countLines(fin);
            string * data=new string[lineNumber];
            getData(fin, lineNumber, data);
            // call parse() to get requested data
            // initialize garden with data
            loadName.append(";"); // resolve finding requested name inside of longer name ex: steve, steven
            for(int i=0;i<lineNumber;i++){
                if(data[i].find(loadName)!=string::npos){
                    reset();
                    parseIn(data[i]);
                    return;
                }
            }
        }
        void save(){ // parse existing data and check for overwrite, call parseOut
            ifstream fin;                       
            int lineNumber=countLines(fin);
            string * data=new string[lineNumber];
            getData(fin, lineNumber, data);
            ofstream fout(dataFile);
            string tempName=dataName; // resolve finding requested name inside of longer name ex: steve, steven
            tempName.append(";");
            bool dataFlag=0; // check for cases where string occurs inside of another name ex: steve, steven
            for(int i=0;i<lineNumber;i++){ // check for dataName and overwrite
                if(data[i].find(tempName)!=string::npos&&!dataFlag){
                    fout << parseOut() << endl; // if dataName already exists
                    dataFlag=1;
                }
                else fout << data[i] << endl; // else preserve previous data
            }
            if(!dataFlag) fout << parseOut() << endl;
            fout.close();

        }
        void newPrint(bool &flag){ // print plant fitment quantities, TODO: issues (if any)
            if(flag){ // if flag is toggled
			    printf("\033[%iA\r",DISPHEIGHT-1); // move to top of display area
                for(int i=0;i<DISPHEIGHT;i++){ // print instructions/usage information
                    cout << "\33[2K";
			        if(i==0) cout << "Square Foot Gardening v0.9 - by Jacob Seman"; // so THAT'S who's responsible for all this kludge
                    if(i==2) cout << "Welcome to Square Foot Gardening!";
                    if(i==3) cout << "You may save your Garden by name,";
                    if(i==4) cout << "and restore it by using the 'save'/'load' functions.";
                    if(i==6) cout << "A Garden may have multiple Planters of different size (up to 50x50).";
                    if(i==7) cout << "You may add Plants of any size and name.";
                    if(i==9) cout << "The program will place the largest Plants first,";
                    if(i==10) cout << "and will continue placing plants until no more can be allocated.";
                    if(i==11) cout << "";
                    if(i==12) cout << "";
                    if(i==14) cout << "This message may be accessed at any time by typing \"help\",";
                    if(i==15) cout << "type help again to toggle the Garden display.";
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
                    if(i==0) cout << "Square Foot Gardening v0.9 - by Jacob Seman";
                    if(i==1){ // print corresponding number headers above each planter
                        for(int j=0;j<(int)userPlanters.size();j++){
                            bool spaceFlag=1; // only for skipping a char if planter.number is double digits
                            for(int k=0;k<userPlanters[j].sizeX*2+1;k++){
                                if(k==0) cout << userPlanters[j].number; // print planter number on upper left
                                else if(k==userPlanters[j].sizeX*2) cout << ' '; // prevent overhanging upper right corner
                                else if(userPlanters[j].number>9&&spaceFlag) spaceFlag=0; // double digits, skip a char
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
			        if(i==DISPHEIGHT-1) cout << "Please enter: new, save, load, add, change, remove, help, options, quit: ";
                }
            }
        }
        void change(const int &changePlanter){ // change a specific planter
            cout << "This planter has dimensions: " << userPlanters[changePlanter-1].sizeX << " by " << userPlanters[changePlanter-1].sizeY << ", input new dimensions in 'x' 'y': ";
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

void inputFcn(string &input,Garden &current){ // Todo: verbose allocation results
    current.newPrint(flag);
    cin >> input;
    if(input=="help"){ // show initial functions screen again
        printf("\033[A\33[2K"); // move cursor up, clear line
        flag=!flag;
    }
    else if(input=="load"){ // load data from file
        flag=0;
        printf("\033[A\33[2K"); // move cursor up, clear line
        cout << "Input name to load: ";
        string loadName;
        cin >> loadName;
        printf("\033[A\33[2K"); // move cursor up, clear line
        current.load(loadName);
    }
    else if(input=="save"){ // save current data
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
        else{ // invalid input, recursive catchall
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
        else{ // invalid input, recursive catchall
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
        else{ // invalid input, recursive catchall
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
    else if(input=="random"){
        flag=0;
        printf("\033[A\33[2K"); // move cursor up, clear line
        srand(time(NULL));
        for(int i=0;i<rand()%5+3;i++){ // for 3-8 iterations
            int pNum=i+1; // planter number i+1
            char rName=rand()%26+97; // 97-123
            string pName(1,rName); // convert char to string
            srand(time(NULL)*(i+1)); // new seed
            int pX=rand()%16+1; // planter sizeX from 1-17
            int pS=rand()%6+1; // plant size from 1-7
            srand(time(NULL)/(i+1)); // new seed
            int pY=rand()%16+1; // planter sizeY from 1-17
            int pQ=rand()%14+1; // plant quantity from 1-15
            Planter pltrN(pNum,pX,pY);
            Plant pltN(pName,pS,pQ);
            current.add(pltrN); // add planter
            current.add(pltN); // add plant
            current.dataName.append(pName); // append random char to name
        }
        int rChar=rand()%32+33; // ASCII 33-47, 58-64, 91-96, 123-126
        if(rChar>48&&rChar<56) FILL=rChar+9; // 44-51 -> 58-64, offset up 13
        else if(rChar>55&&rChar<62) FILL=rChar+35; // 52-57 -> 91-96, offset up 39
        else if(rChar>61) FILL=rChar+61; // 62-65 -> 123-126, offset up 65
        else FILL=rChar-1; // random fill char!!!
    }
    else{ // invalid input, recursive catchall
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
 * an input of "random" calls a function that randomly sizes 3-8 planters and
 * plants with random starting letters, generates a garden dataName from those
 * letters, and also chooses a random fill character!
 * 
*******************************************************************************/
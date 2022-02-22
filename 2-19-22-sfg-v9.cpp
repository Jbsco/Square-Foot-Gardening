/*******************************************************************************
 *  INSTRUCTIONS:
 * Create a program that helps with managing a square foot garden.
 * 
 * Version 10 - Jacob B. Seman
*******************************************************************************/

/*******************************************************************************
 *  IDEA:
 * clear display area, 21 lines, add more if planter exceeds height
 * print prompt/instructions/information
 * print verbose allocation results
 * take input from user - new,save,load,add,change,remove,help,options,quit
 * 
 *  NEW:
 * create new empty garden
 * take input from user - garden name
 * this name is used to identify data
 *
 *  SAVE:
 * push current garden/planters/plants configuration to data file
 * leave current garden state unchanged
 * don't try to save an empty garden!
 *
 *  LOAD:
 * getlines from text file, prompt selections available
 * take data from text file per selection
 * populate and print saved garden/planters/plants
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
 * automatically
 *
 *  CHANGE:
 * change a planter or plant currently in the garden
 * take input from user - change garden, planter, plant
 * take input from user - garden name
 *                      - planter number -> new x y size
 *                      - plant name -> new name, size, quantity
 * update garden arrangement after changes
 *
 *  REMOVE:
 * remove a planter or plant from the garden
 * take input from user - planter number
 *                      - plant name
 * update garden arrangement after removals
 * 
 *  HELP:
 * toggles the introductory help screen
 * 
 *  OPTIONS:
 * configure the display height manually
 * choose a display character for plant fill
 *      default: lower case plant.name[0]
 *               '-'
 *               '~'
 *               'x'
 *               '.'
 *
 *  QUIT:
 * exit program, prompt to save
 * take input from user - yes,no,cancel
 * save if yes, exit program, return if cancel
*******************************************************************************/

/*******************************************************************************
 *  TODO: LIST v10
 * Looking good!
 * Some fringe bugs here and there but all intended use-cases are working!
*******************************************************************************/

#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<algorithm>
#include<sstream>
#include<ctime>

using namespace std;

int DISPHEIGHT=21; // number of lines to use for terminal "UI", change in options
bool ORIDE=0; // override for manually setting the display height
char FILL='_'; // default character to use when filling space, change in options
bool flag=1; // flag for help input
const string dataFile="sfg-v10.txt"; // filename used across data management functions

void clearTop(bool retC){ // to improve readability
                          //move to top of display area, argument for return carriage
	if(retC) printf("\033[%iA\r",DISPHEIGHT-1);
    else printf("\033[%iA",DISPHEIGHT-1);
}

void clearUp(){ // to improve readability
                // move cursor up, clear line
    printf("\033[A\33[2K");
}

class Plant{ // to be placed in planters by name, by garden class
             // has size and quantity attributes that affect placement
    private:
        string name;
        int size,quantity;
    public:
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
        bool operator <(const int &other)const{
            return(size<other);
        }
        bool operator >(const int &other)const{
            return(size>other);
        }
        bool operator <=(const int &other)const{
            return(size<=other);
        }
        bool operator ==(const string &other)const{ // comparator overload for name equivalence
            return(name==other);
        }
        bool operator ==(const int &other)const{ // comparator overload for int equivalence
            return(size==other);
        }
        bool operator >(const Plant &other)const{ // comparator overload for sort(greater<Plant>())
            return(size>other.size);
        }
		friend istream & operator >> (istream &in,Plant &other){
			in >> other.name >> other.size >> other.quantity;
			return in;
		}
		friend ostream & operator << (ostream &out,const Plant &other){
			out << other.name;
			return out;
		}
        string pName(){
            return name;
        }
        int pSize(){
            return size;
        }
        int pQty(){
            return quantity;
        }
};

class Planter{ // to be placed in garden by number, by garden class
               // has size in (x,y) attribute that affects plants able to be placed
    private:
        int number,sizeX,sizeY;
    public:
        Planter(){
            number=0;
            sizeX=0;
            sizeY=0;
        }
        Planter(int &newNumber,int &newSizeX,int &newSizeY){
            number=newNumber;
            sizeX=newSizeX;
            sizeY=newSizeY;
        }
        void update(int &newSizeX,int &newSizeY){
            sizeX=newSizeX;
            sizeY=newSizeY;
        }
        bool operator ==(const int &other)const{
            return(number==other);
        }
        bool operator >(const int &other)const{
            return(number>other);
        }
        void operator =(const int &other){
            number=other;
        }
		friend istream & operator >> (istream &in,Planter &other){
			in >> other.sizeX >> other.sizeY;
            return in;
		}
		friend ostream & operator << (ostream &out,const Planter &other){
			out << other.sizeX << " by " << other.sizeY;
			return out;
		}
        int getX(){
            return sizeX;
        }
        int getY(){
            return sizeY;
        }
        int getN(){
            return number;
        }
};

class Garden{ // Todo: verbose allocation results
// container for multiple planters, plants - provision for display and user data
    private:
        string dataName; // name to be used when saving/loading data
        vector<Plant> userPlants; // plants in garden, assigned to planters
        vector<Planter> userPlanters; // planters in garden
    public:
        Garden(){
            dataName="";
        }
        Garden(string &newName){
            dataName=newName;
        }
        friend istream & operator >>(istream &in,Garden &other){
            in >> other.dataName;
            return in;
        }
        void add(const Planter &newPlanter){ // add a planter to vector
            userPlanters.push_back(newPlanter);
            userPlanters.back()=userPlanters.size();
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
            // dataName ';' planter number, size x, y [loop all] ';' plant name, size, quantity [loop all] ';' FILL "\n"
            // Ex:
            // dataName;userPlanters[i].number,userPlanters[i].sizeX,userPlanters[i].sizeY;userPlants[j].name,userPlants[j].size,userPlants[j].quantity;FILL
            // Eg:
            // "karl;1,4,4,2,16,16,3,5,3;marigold,1,20,peach_tree,4,2,tomato,3,10;~"
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
            // dataName ';' planter number, size x, y [loop all] ';' plant name, size, quantity [loop all] ';' FILL "\n"
            // Ex:
            // dataName;userPlanters[i].number,userPlanters[i].sizeX,userPlanters[i].sizeY;userPlants[j].name,userPlants[j].size,userPlants[j].quantity;FILL
            // Eg:
            // "karl;1,4,4,2,16,16,3,5,3;marigold,1,20,peach_tree,4,2,tomato,3,10;~"
            string out=dataName;
            out.append(";");
            for(int i=0;i<(int)userPlanters.size();i++){
                out.append(to_string(userPlanters[i].getN()));
                out.append(",");
                out.append(to_string(userPlanters[i].getX()));
                out.append(",");
                out.append(to_string(userPlanters[i].getY()));
                if(i!=(int)userPlanters.size()-1) out.append(",");
                else out.append(";");
            }
            for(int i=0;i<(int)userPlants.size();i++){
                out.append(userPlants[i].pName());
                out.append(",");
                out.append(to_string(userPlants[i].pSize()));
                out.append(",");
                out.append(to_string(userPlants[i].pQty())); // get plant.quantity
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
            pltrScan(); // update display based on current planters
            if(flag){ // if flag is toggled
			    clearTop(1); // move to top of display area
                for(int i=0;i<DISPHEIGHT;i++){ // print instructions/usage information
                    cout << "\33[2K";
			        if(i==0)
                        cout << "\033[1;37mSquare Foot Gardening v1.0 - by Jacob Seman\033[0m"; // so THAT'S who's responsible for all this kludge
                    if(i==2)
                        cout << "Welcome to Square Foot Gardening!";
                    if(i==3)
                        cout << "Type 'new' to create a new garden. You may save your Garden by name,";
                    if(i==4)
                        cout << "and restore it by using the 'save'/'load' functions.";
                    if(i==6)
                        cout << "A Garden may have multiple Planters of different width and height.";
                    if(i==7)
                        cout << "You may add Plants of any name, size, and quantity. Plants are always square.";
                    if(i==9)
                        cout << "The program will place the largest Plants first,";
                    if(i==10)
                        cout << "and will continue placing plants until no more can be allocated.";
                    if(i==12)
                        cout << "It is recommended to avoid overrunning the edge of the screen!";
                    if(i==14)
                        cout << "This message may be accessed at any time by typing \"help\",";
                    if(i==15)
                        cout << "type help again to toggle the Garden display.";
                    if(i==17)
                        cout << "Thank you for using this program, I hope you find it interesting!";
			        if(i!=DISPHEIGHT-1)
                        cout << "\n"; // newline for all but last
			        if(i==DISPHEIGHT-1)
                        cout << "Please enter: new, save, load, add, change, remove, help, options, quit: ";
                }
            }
            else{
                // printing all planters
                sort(userPlants.begin(),userPlants.end(),greater<Plant>()); // sort Plant vector by Plant.size, descending
                char cells[(int)userPlanters.size()][50][50]; // initialize char array cells[planter qty][x][y]
                memset(cells,'_',sizeof(cells));
                int qty[(int)userPlants.size()]; // don't place more than the qty
                int pPrint[(int)userPlants.size()]; // to print allocations
                memset(pPrint,0,sizeof(pPrint));
                for(int j=0;j<(int)userPlants.size();j++)
                    qty[j]=userPlants[j].pQty(); // get plant.quantity
                // *big breath* populate array for planter fill
                for(int j=0;j<(int)userPlanters.size();j++){ // per planter
                    for(int k=0;k<userPlanters[j].getY();k++){ // per planter sizeY
                        for(int l=0;l<userPlanters[j].getX();l++){ // per planter sizeX
                            for(int m=0;m<(int)userPlants.size();m++){ // per plant
                                // check clearance to right and below current pos
                                bool check=1;
                                for(int cI=0;userPlants[m]>cI;cI++){ // comp overload
                                    for(int tI=0,tJ=0;userPlants[m]>tJ;tI++){ // comp overload
                                        if(cells[j][l+tI][k+tJ]!='_')
                                            check=0;
                                        if(userPlants[m]==tI+1)
                                            tI=-1,tJ++;
                                    }
                                }
                                // if array space unnallocated
                                // and if clearance and plant.qty available
                                if(cells[j][l][k]=='_'
                                    &&check
                                    &&userPlants[m]<=userPlanters[j].getX()-l
                                    &&userPlants[m]<=userPlanters[j].getY()-k
                                    &&qty[m]>0){
                                    for(int tI=0,tJ=0;userPlants[m]>tJ;tI++){ // offsets based on plant[m].size, comp overload
                                        if(tI==0&&tJ==0){
                                            cells[j][l][k]=toupper(userPlants[m].pName().at(0)); // upper left cell -> make upper case
                                            qty[m]--;
                                            pPrint[m]++; // allocated amount
                                        }
                                        else{
                                            if(FILL=='_') // fill cells in plant size range -> make lower case
                                                cells[j][l+tI][k+tJ]=tolower(userPlants[m].pName().at(0));
                                            else // if FILL is set by user use this char instead
                                                cells[j][l+tI][k+tJ]=FILL;
                                        }
                                        if(userPlants[m]==tI+1) // reset x pos, increment y pos
                                            tI=-1,tJ++;
                                    }
                                }
                            }
                        }
                    }
                }
                // cell array should now be populated, print array
                clearTop(0);
                for(int i=0;i<DISPHEIGHT;i++){
                    cout << "\33[2K";
                    if(i==0)
                        cout << "\033[1;37mSquare Foot Gardening v1.0 - by Jacob Seman\033[0m";
                    if(i==1){ // print corresponding number headers above each planter
                        for(int j=0;j<(int)userPlanters.size();j++){
                            bool spaceFlag=1; // if planter.number is double digits
                            for(int k=0;k<userPlanters[j].getX()*2+1;k++){
                                if(k==0) // print planter number on upper left
                                    cout << userPlanters[j].getN();
                                else if(k==userPlanters[j].getX()*2)
                                    cout << ' '; // prevent overhanging upper right corner
                                else if(userPlanters[j]>9&&spaceFlag)
                                    spaceFlag=0; // double digits, skip a char
                                else // print upper border
                                    cout << '_';
                            }
                            cout << ' '; // correct spacing between planters
                        }
                    }
                    if(i>1&&i<DISPHEIGHT-2){
                        for(int j=0;j<(int)userPlanters.size();j++){
                            for(int k=0;k<userPlanters[j].getX()+1;k++){
                                if(i-2<userPlanters[j].getY()){
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
                    if((int)userPlants.size()>0){
                        if(i==DISPHEIGHT-3){ // print allocated plants
                            cout << "\rAllocated: ";
                            for(int j=0;j<(int)userPlants.size();j++){
                                cout << pPrint[j] << ' ' << userPlants[j];
                                if(j<(int)userPlants.size()-1) cout << "; ";
                            }
                        }
                        if(i==DISPHEIGHT-2){ // print unallocated status
                            bool remFlag=0;
                            for(int j=0;j<(int)userPlants.size();j++){
                                if(qty[j]>0){
                                    remFlag=1;
                                    
                                }
                            }
                            if(remFlag){
                                cout << "Remaining: ";
                                for(int j=0;j<(int)userPlants.size();j++){
                                    if(qty[j]>0){
                                        cout << qty[j] << ' ' << userPlants[j];
                                        if(j<(int)userPlants.size()-1) cout << "; ";
                                    }
                                }
                            }
                            else cout << "All plants successfully allocated! \033[1;35mGood Jorb!\033[0m";
                        }
                    }
			        if(i!=DISPHEIGHT-1) cout << "\n"; // newline, all but last
                    // print footer
			        if(i==DISPHEIGHT-1){
                        cout << "Please enter: new, save, load, add, change, ";
                        cout << "remove, help, options, quit: ";
                    }
                }
            }
        }
        void change(const int &changePlanter){ // change a specific planter
            cout << "This planter has dimensions: "
                 << userPlanters[changePlanter-1]
                 << ". Input new dimensions in 'x' 'y': ";
            for(int i=0;i<(int)userPlanters.size();i++){
                if(userPlanters[i]==changePlanter){
                    cin >> userPlanters[i];
                    clearUp();
                }
            }
        }
        void pltrScan(){ // scan for largest sizeY, update DISPHEIGHT if !ORIDE
            if(!ORIDE){
                int temp=16,clr=DISPHEIGHT; // temp vars
                for(int i=0;i<(int)userPlanters.size();i++){ // find largest sizeY
                    if(userPlanters[i].getY()>temp) temp=userPlanters[i].getY();
                }
                temp+=5; // correct for non-display lines
                if(temp>21) DISPHEIGHT=temp; // update
                else DISPHEIGHT=21; // return to default
                if(clr>DISPHEIGHT){ // if display is smaller clean space above
                    printf("\033[%iA",clr); // move up to previous height
                    for(int i=0;i<clr;i++){
                        printf("\033[2K\n"); // clear lines
                    }
                }
            }
        }
        void change(const string &changePlant){ // change a specific plant
            for(int i=0;i<(int)userPlants.size();i++){
                if(userPlants[i]==changePlant){
                    cout << "Selected: "
                         << userPlants[i]
                         << ", size: "
                         << userPlants[i].pSize()
                         << ", qty: "
                         << userPlants[i].pQty()
                         << ". Input your plant's name, size, and quantity: ";
                    cin >> userPlants[i];
                    clearUp();
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
                if(userPlanters[i]==remPlanter)
                    userPlanters.erase(userPlanters.begin()+i);
            }
        }
        void remove(string &remPlant){ // remove plant from vector
            for(int i=0;i<(int)userPlants.size();i++){
                if(userPlants[i]==remPlant)
                    userPlants.erase(userPlants.begin()+i);
            }
        }
        void dispPlant(){
            for(int i=0;i<(int)userPlants.size();i++){
                cout << userPlants[i];
                if(i+1<(int)userPlants.size())
                    cout << ", ";
                else
                    cout << ". ";
            }
        }
        bool operator ==(const string &other)const{
            return(dataName==other);
        }
        void operator =(const string &other){
            dataName=other;
        }
        string gName(){
            return dataName;
        }
};

void inputFcn(string &input,Garden &current){ // Todo: verbose allocation results
    current.newPrint(flag);
    cin >> input;
    if(input=="help"){ // show initial functions screen again
        clearUp();
        flag=!flag;
    }
    else if(input=="load"){ // load data from file
        flag=0;
        clearUp();
        cout << "Input name to load: ";
        string loadName;
        cin >> loadName;
        clearUp();
        current.load(loadName);
    }
    else if(input=="save"){ // save current data
        flag=0;
        if(current=="") current="default";
        current.save();
        clearUp();
    }
    else if(input=="new"){ // clears current Garden vectors and updates name
        flag=0;
        clearUp();
        cout << "Input your new garden's name, for save/load purposes: ";
        current.reset(); // clear current garden's name and vectors
        cin >> current;
        clearUp();
    }
    else if(input=="add"){ // adds planter or plant to current garden
        flag=0;
        if(current=="") current="default";
        string choice;
        clearUp();
        cout << "Add a planter or plant? ";
        cin >> choice; // determine planter or plant
        clearUp();
        if(choice=="planter"){
            Planter pNew;
            cout << "input your planter dimensions in 'x' 'y': ";
            cin >> pNew;
            clearUp();
            current.add(pNew);
        }
        else if(choice=="plant"){
            Plant pNew;
            cout << "input your plant name, size, and quantity: ";
            cin >> pNew;
            clearUp();
            current.add(pNew);
        }
        else{ // invalid input, recursive catchall
            clearUp();
            inputFcn(input,current);
        }
    }
    else if(input=="change"){ // changes planter or plant in current garden
        flag=0;
        if(current=="") current="default";
        string choice;
        clearUp();
        cout << "Change garden, planter, or plant? ";
        cin >> choice; // determine planter or plant
        clearUp();
        if(choice=="garden"){
            cout << "Current name: " << current.gName() << ". Input a new name for this garden: ";
            cin >> current;
            clearUp();
        }
        if(choice=="planter"){
            int pNum;
            cout << "Input planter number to change: ";
            cin >> pNum;
            clearUp();
            current.change(pNum);
        }
        else if(choice=="plant"){
            string pName;
            cout << "Current plants are: ";
            current.dispPlant();
            cout << "Input plant name to change: ";
            cin >> pName;
            clearUp();
            current.change(pName);
        }
        else{ // invalid input, recursive catchall
            clearUp();
            inputFcn(input,current);
        }
    }
    else if(input=="remove"){ // removes planter or plant from current garden
        flag=0;
        if(current=="") current="default";
        string choice;
        clearUp();
        cout << "Remove planter or plant? ";
        cin >> choice; // determine planter or plant
        clearUp();
        if(choice=="planter"){
            int pNum;
            cout << "Input planter number to remove: ";
            cin >> pNum;
            clearUp();
            current.remove(pNum);
        }
        else if(choice=="plant"){
            cout << "Current plants are: ";
            current.dispPlant();
            string pName;
            cout << "Input plant name to remove: ";
            cin >> pName;
            clearUp();
            current.remove(pName);
        }
        else{ // invalid input, recursive catchall
            clearUp();
            inputFcn(input,current);
        }
    }
    else if(input=="options"){
        clearUp();
        cout << "Change DISPHEIGHT or default plant space fill character? "
             << "Input display or character: ";
        string option;
        cin >> option;
        clearUp();
        if(option=="display"){
            int clr=DISPHEIGHT; // temp var for clearing after downsizing
            cout << "Input a new DISPHEIGHT. Current is: "
                 << DISPHEIGHT;
            if(ORIDE) cout << ", override is set!";
            else cout << '.';
            cout << " Default is 21, setting this value clears any override: ";
            cin >> DISPHEIGHT;
            if(DISPHEIGHT==21) ORIDE=0;
            else ORIDE=1;
            clearUp();
            if(clr>DISPHEIGHT){ // if display is smaller clean space above
                printf("\033[%iA",clr); // move up to previous height
                for(int i=0;i<clr;i++){
                    printf("\033[2K\n"); // clear lines
                }
            }
        }
        else if(option=="character"){
            cout << "Input a new ASCII character to use for plant space fill,"
                 << " or type default to use lower case per plant name: ";
            string ASCII;
            cin >> ASCII;
            if(ASCII=="default") FILL='_';
            else FILL=(char)ASCII.at(0);
            clearUp();
        }

    }
    else if(input=="quit"){
        clearUp();
        cout << "Do you want to save? Input yes, no, or cancel: ";
        string quitter;
        cin >> quitter;
        clearUp();
        if(quitter=="yes"){
            current.save();
            return;
        }
        else if(quitter=="cancel"){
            clearUp();
            inputFcn(input,current);
        }
        else if(quitter=="no") return;
        else{
            clearUp();
            inputFcn(input,current);
        }
    }
    else if(input=="random"){
        flag=0;
        clearUp();
        srand(time(NULL));
        stringstream randName(current.gName(),
                              ios_base::ate|
                              ios_base::in|
                              ios_base::out); // make the stream able to be appended
        for(int i=0;i<rand()%5+3;i++){ // for 3-7 iterations
            int pNum=i+1; // planter number i+1
            char rName=rand()%26+97; // 97-123
            string pName(1,rName); // convert char to string
            srand(time(NULL)*(i+1)); // new seed
            int pX=rand()%16+1; // planter sizeX from 1-16
            int pS=rand()%5+1; // plant size from 1-5
            srand(time(NULL)/(i+1)); // new seed
            int pY=rand()%16+1; // planter sizeY from 1-16
            int pQ=rand()%20+1; // plant quantity from 1-20
            Planter pltrN(pNum,pX,pY);
            Plant pltN(pName,pS,pQ);
            current.add(pltrN); // add planter
            current.add(pltN); // add plant
            randName << pName; // send random char to name
        }
        randName >> current;
        int rChar=rand()%33+33; // ASCII 33-47, 58-64, 91-96, 123-126
        if(rChar>48&&rChar<56) // 44-51 -> 58-64, offset up 13
            FILL=rChar+9;
        else if(rChar>55&&rChar<62) // 52-57 -> 91-96, offset up 39
            FILL=rChar+35;
        else if(rChar>61) // 62-65 -> 123-126, offset up 65
            FILL=rChar+61;
        else // random fill char!!!
            FILL=rChar-1;
    }
    else{ // invalid input, recursive catchall
        clearUp();
        inputFcn(input,current);
    }
}

int main(){ // expansive things come in minimal mains...
    string input;
    Garden current;
    while(input!="quit") inputFcn(input,current);
	return 0;
}

/*******************************************************************************
 *  SECRET:
 * an input of "random" calls a function that randomly sizes 3-8 planters and
 * plants with random starting letters, generates a garden dataName from those
 * letters, and also chooses a random fill character!
*******************************************************************************/
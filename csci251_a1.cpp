#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

//declaring global variables
int userChoice = 0;
int option = 0;
int number;
int row, column, total;
int min_x, max_x;
int min_y, max_y;
string isEmpty;
string cityFile, cloudFile, pressureFile;

//Lab task 4
struct Map
{
	int x, y, value;
	string name;
};


Map * city;
Map * cloud;
Map * pressure;

int ** cityArray = nullptr;
int ** cloudArray = nullptr;
int ** pressureArray = nullptr;

//General function prototypes
void mainMenu();
int listMenu();
void returnMenu();
void readConfigFile();
void readTextFile(string fileName);
void rowColumn();
void safelydeallocateMemory(int ** mapArray);
//City function prototypes
int cityCounter();
void readCityFile();
void createCityArray();
void displayCityArray();
//Cloud function prototypes
void readCloudFile();
void createCloudArray();
void displayCloudArray();
void displayCloudLMH();
//Pressure function prototypes
void readPressureFile();
void createPressureArray();
void displayPressureArray();
void displayPressureLMH();
//Display function prototypes
void displaySummaryReport();
void displayAverage(float totalCloudArea, float totalPressureArea, int counter);


int main(){

	mainMenu();
	return (0);
}

void mainMenu(){
	bool quit = false;
	while(!quit){
		switch(listMenu()){
			//Reading of configuration file
			case 1:
				cout << endl;
				readConfigFile();
				rowColumn();
				createCityArray();
				createCloudArray();
				createPressureArray();
				readCityFile();
				readCloudFile();
				readPressureFile();
				returnMenu();
				cin.clear();
				break;
			//Display city map
			case 2:
				cout << endl;
				displayCityArray();
				returnMenu();
				cin.clear();
				break;
			//Display cloud coverage map (cloudiness index)
			case 3:
				cout << endl;
				displayCloudArray();
				returnMenu();
				cin.clear();
				break;
			//Display cloud coverage map (LMH symbols)
			case 4:
				cout << endl;
				displayCloudLMH();
				returnMenu();
				cin.clear();
				break;
			//Display atmospheric pressure map (pressure index)
			case 5:
				cout << endl;
				displayPressureArray();
				returnMenu();
				cin.clear();
				break;
			//Display atmospheric pressure map (LMH symbols)
			case 6:
				cout << endl;
				displayPressureLMH();
				returnMenu();
				cin.clear();
				break;
			//Show weather forecast summary report
			case 7:
				cout << endl;
				displaySummaryReport();
				returnMenu();
				cin.clear();
				break;
			//Quit
			case 8:
				cout << endl;
				cout << "Exiting program now.." << endl;
				quit = true;
				break;
			default:
				cout << endl;
				cout << "Please enter a valid choice: " << endl;
		}
	}
	safelydeallocateMemory(cityArray);
	safelydeallocateMemory(cloudArray);
	safelydeallocateMemory(pressureArray);

}

//Display Mainmenu with options
int listMenu(){
	cout << "Student ID	:7432665" << endl;
	cout << "Student Name :Sulynn Teo Ji Ai" << endl;
	cout << "--------------------------------------" << endl;
	cout << "Welcome to Weather Information Processing Sysytem!" << endl << endl;
	cout << "1)	Read in and process a configuration file" << endl;
	cout << "2)	Display city map" << endl;
	cout << "3)	Display cloud coverage map (cloudiness index)" << endl;
	cout << "4)	Display cloud coverage map (LMH symbols)" << endl;
	cout << "5)	Display atmospheric pressure map (pressure index)" << endl;
	cout << "6)	Display atmospheric pressure map (LMH symbols)" << endl;
	cout << "7)	Show weather forecast summary report" << endl;
	cout << "8)	Quit" << endl << endl;
	cout << "Please enter your choice: ";
	//Take in userinput (INT)
	cin >> userChoice;
	return userChoice;
}
	
void readConfigFile(){
	string inputFileName;
	//system prompt for user input and put userinput into 'filename'
	cout << "[ Read in and process a configuration file ]" << endl;
	cout << "Please enter config filename: ";
	cin >> inputFileName;
	cout << endl;
	
	//fstream handles both ifstream and ofstream functions
	//declare myfile as fstream function the
	fstream inputFile(inputFileName.c_str(), fstream::in);

	string oneLine;
	string delimiter = "=";
	string token;
	char dash;

	if (inputFile.is_open()){
		while(getline(inputFile, oneLine)){
			if (!(oneLine[0] == '/' && oneLine[1] == '/')){
				if ((oneLine.find("GridX_IdxRange")) != string::npos){
					size_t posit = oneLine.find(delimiter);
					if (posit != string::npos){
						token = oneLine.substr(posit + delimiter.length());
					}
					stringstream ss(token);
					ss >> min_x >> dash >> max_x;
					cout << "Reading in GridX_IdxRange : " << min_x << dash << max_x << "...done!"<< endl;
				}
				
				if ((oneLine.find("GridY_IdxRange")) != string::npos){
					size_t posit = oneLine.find(delimiter);
					if (posit != string::npos){
						token = oneLine.substr(posit + delimiter.length());
					}
					stringstream ss(token);
					ss >> min_y >> dash >> max_y;
					cout << "Reading in GridY_IdxRange : " << min_y << dash << max_y << "...done!"<< endl;
					cout << endl;
				}
				size_t posit = oneLine.find(".txt");
				if (posit != string::npos){
					readTextFile(oneLine);
					cout << oneLine << " ...done!" << endl;
				}
			}
		}
		cout << endl;
		cout << "All records successfully stored." << endl;
		cout << endl;
	}else{
		cout << inputFileName << " have failed to open." << endl;
		cout << "Please enter a valid filename: ";
		cin >> inputFileName;
		inputFile.open(inputFileName);
	}
}

//find and read txt.file
void readTextFile(string fileName){
	if (cityFile == ""){
		cout << "Data stored successfully from input file : " << endl;
		cityFile = fileName;
	}
	else if (cloudFile == ""){
		cloudFile = fileName;
	}else if (pressureFile == ""){
		pressureFile = fileName;
	}
	option = userChoice;
}

int cityCounter(){
	fstream inputFile(cityFile.c_str(), fstream::in);
	
	string delimiter = "[";
	size_t posit = 0;
	number = 0;
	while (getline(inputFile, cityFile)){
		if ((posit = cityFile.find(delimiter)) != string::npos){
			number++; //count number of lines in citylocation.txt for number of struct to create for storing the data
		}
	}
	return number;
}

void rowColumn(){
	column = max_x - min_x + 1;	
	
	row = max_y - min_y + 1;
	
	total = row * column;
}

void readCityFile(){
	fstream inputFile(cityFile.c_str(), fstream::in);

	string delimiter_1 = "[";
	string delimiter_2 = "-";
	string token;
	char comma;
	char dash;
	number = cityCounter();
	size_t posit = 0;
	int i = 0;
	city = new Map[number];
	
	while (getline(inputFile, cityFile)){
		if ((posit = cityFile.find(delimiter_1)) != string::npos){
			token = cityFile.substr(posit + delimiter_1.length());
			stringstream ss1(token);
			ss1 >> city[i].x >> comma >> city[i].y;
		}

		if ((posit = cityFile.find(delimiter_2)) != string::npos){
			token = cityFile.substr(posit + delimiter_2.length());
			stringstream ss2(token);

			ss2 >> city[i].value >> dash >> city[i].name;
			int f = row - city[i].y - 1;
			cityArray[f + min_y][city[i].x - min_x] = city[i].value;
		}
		i++;
	}
}

void readCloudFile(){
	fstream inputFile(cloudFile.c_str(), fstream::in);

	string delimiter_1 = "[";
	string delimiter_2 = "-";
	string token;
	char comma;
	char dash;
	
	size_t posit = 0;
	int i = 0;
	cloud = new Map[total];
	
	while (getline(inputFile, cloudFile)){
		if ((posit = cloudFile.find(delimiter_1)) != string::npos){
			token = cloudFile.substr(posit + delimiter_1.length());
			stringstream ss1(token);
			ss1 >> cloud[i].x >> comma >> cloud[i].y;
		}

		if ((posit = cloudFile.find(delimiter_2)) != string::npos){
			token = cloudFile.substr(posit + delimiter_2.length());
			stringstream ss2(token);
			ss2 >> cloud[i].value;
			int f = row - cloud[i].y - 1;
			cloudArray[f + min_y][cloud[i].x - min_x] = cloud[i].value;
		}
		i++;
	}
}

void readPressureFile(){
	fstream inputFile(pressureFile.c_str(), fstream::in);
	string delimiter_1 = "[";
	string delimiter_2 = "-";
	string token;
	char comma;
	char dash;

	size_t posit = 0;
	int i = 0;
	pressure = new Map[total];
	
	while (getline(inputFile, pressureFile)){
		if ((posit = pressureFile.find(delimiter_1)) != string::npos){
			token = pressureFile.substr(posit + delimiter_1.length());
			stringstream ss1(token);
			ss1 >> pressure[i].x >> comma >> pressure[i].y;
		}

		if ((posit = pressureFile.find(delimiter_2)) != string::npos){
			token = pressureFile.substr(posit + delimiter_2.length());
			stringstream ss2(token);
			ss2 >> pressure[i].value;
			int f = row - pressure[i].y - 1;
			pressureArray[f + min_y][pressure[i].x - min_x] = pressure[i].value;
		}
		i++;
	}
}

void createCityArray(){
	cityArray = new int * [row];
	for (int i = 0; i < row; i++){
		cityArray[i] = new int[column];
		for (int f = 0; f < column; f++){
			cityArray[i][f] = 0;
		}
	}
}

void createCloudArray(){
	cloudArray = new int *[row];
	for (int i = 0; i < row; i++){
		cloudArray[i] = new int[column];
		for (int f = 0; f < column; f++){
			cloudArray[i][f] = 0;
		}
	}
}

void createPressureArray(){
	pressureArray = new int * [row];
	for (int i = 0; i < row; i++){
		pressureArray[i] = new int[column];
		for (int f = 0; f < column; f++){
			pressureArray[i][f] = 0;
		}
	}
}

void displayCityArray(){
	string hash;
	int amtOfHash = column + 2;
	hash.assign(amtOfHash, '#');
	cout << "   ";
	for (int i= 0; i < amtOfHash; i++){
		cout << " " << hash[i] << " ";
	}
	cout << endl;
	int a = max_y;
	
	for (int i = 0; i < row; i++){
		if (a >= 10){
			cout << " " << a << " ";
		}else{
			cout << " " << a << "  ";
		}
		cout << "# ";
		a--;
		for (int f = 0; f < column; f++){
			if (cityArray[i][f] == 0){
				cout << "   ";
			}else{
				cout << " " << cityArray[i][f]  << " ";
			}
		}
		cout << " #";
		cout << endl;
	}

	cout << "   ";
	for (int i= 0; i < amtOfHash; i++){
		cout << " " << hash[i] << " ";
	}
	cout << endl;
	cout << "      ";

	for (int i = min_x; i < column + min_x; i++){
		if (i >= 10){
			cout << " " << i;
		}else{
			cout << " " << i << " ";
		}
	}
	cout << endl;
}

void displayCloudArray(){
	string hash;
	int amtOfHash = column + 2;
	hash.assign(amtOfHash, '#');
	cout << "   ";
	for (int i=0; i<amtOfHash; i++){
		cout << " " << hash[i] << " ";
	}
	cout << endl;
	int a = max_y;
	
	for (int i=0; i<row; i++){
		if (a>=10){
			cout << " " << a << " ";
		}else{
			cout << " " << a << "  ";
		}
		cout << "# ";
		a--;
		for (int f=0; f<column; f++){
			if (cloudArray[i][f] >= 0 && cloudArray[i][f] < 10){
				cout << " " << "0" << " ";
			}else if (cloudArray[i][f] >= 10 && cloudArray[i][f] < 20){
				cout << " " << "1" << " ";
			}else if (cloudArray[i][f] >= 20 && cloudArray[i][f] < 30){
				cout << " " << "2" << " ";
			}else if (cloudArray[i][f] >= 30 && cloudArray[i][f] < 40){
				cout << " " << "3" << " ";
			}else if (cloudArray[i][f] >= 40 && cloudArray[i][f] < 50){
				cout << " " << "4" << " ";
			}else if (cloudArray[i][f] >= 50 && cloudArray[i][f] < 60){
				cout << " " << "5" << " ";
			}else if (cloudArray[i][f] >= 60 && cloudArray[i][f] < 70){
				cout << " " << "6" << " ";
			}else if (cloudArray[i][f] >= 70 && cloudArray[i][f] < 80){
				cout << " " << "7" << " ";
			}else if (cloudArray[i][f] >= 80 && cloudArray[i][f] < 90){
				cout << " " << "8" << " ";
			}else if (cloudArray[i][f] >= 90 && cloudArray[i][f] < 100){
				cout << " " << "9" << " ";
			}
		}
		cout << " #";
		cout << endl;
	}
	cout << "   ";
	for (int i= 0; i < amtOfHash; i++){
		cout << " " << hash[i] << " ";
	}
	cout << endl;
	cout << "      ";
	for (int i = min_x; i < column + min_x; i++){
		if (i >= 10){
			cout << " " << i;
		}
		else{
			cout << " " << i << " ";
		}
	}
	cout << endl;
}

void displayCloudLMH(){
	string hash;
	int amtOfHash = column + 2;
	hash.assign(amtOfHash, '#');
	cout << "   ";
	for (int i= 0; i < amtOfHash; i++)
	{
		cout << " " << hash[i] << " ";
	}
	cout << endl;
	int a = max_y;
	
	for (int i = 0; i < row; i++)
	{
		if (a >= 10)
		{
			cout << " " << a << " ";
		}
		else
		{
			cout << " " << a << "  ";
		}
		cout << "# ";
		a--;
		for (int f = 0; f < column; f++)
		{
			if (cloudArray[i][f] >= 0 &&  cloudArray[i][f] < 35)
			{
				cout << " " << "L" << " ";
			}
			else if (cloudArray[i][f] >= 35 &&  cloudArray[i][f] < 65)
			{
				cout << " " << "M"  << " ";
			}
			else if (cloudArray[i][f] >= 65 &&  cloudArray[i][f] < 100)
			{
				cout << " " << "H"  << " ";
			}
			
		}
		cout << " #";
		cout << endl;
	}

	cout << "   ";
	for (int i= 0; i < amtOfHash; i++)
	{
		cout << " " << hash[i] << " ";
	}
	cout << endl;
	cout << "      ";

	for (int i = min_x; i < column + min_x; i++)
	{
		if (i >= 10)
		{
			cout << " " << i;
		}
		else
		{
			cout << " " << i << " ";
		}
	}
	cout << endl;
}

void displayPressureArray(){
	string hash;
	int amtOfHash = column + 2;
	hash.assign(amtOfHash, '#');
	cout << "   ";
	for (int i= 0; i < amtOfHash; i++){
		cout << " " << hash[i] << " ";
	}
	cout << endl;
	int a = max_y;
	
	for (int i = 0; i < row; i++){
		if (a >= 10){
			cout << " " << a << " ";
		}
		else{
			cout << " " << a << "  ";
		}
		cout << "# ";
		a--;
		for (int f = 0; f < column; f++){
			if (pressureArray[i][f] >= 0 &&  pressureArray[i][f] < 10){
				cout << " " << "0" << " ";
			}
			else if (pressureArray[i][f] >= 10 &&  pressureArray[i][f] < 20){
				cout << " " << "1"  << " ";
			}
			else if (pressureArray[i][f] >= 20 &&  pressureArray[i][f] < 30){
				cout << " " << "2"  << " ";
			}
			else if (pressureArray[i][f] >= 30 &&  pressureArray[i][f] < 40){
				cout << " " << "3"  << " ";
			}
			else if (pressureArray[i][f] >= 40 &&  pressureArray[i][f] < 50){
				cout << " " << "4"  << " ";
			}
			else if (pressureArray[i][f] >= 50 &&  pressureArray[i][f] < 60){
				cout << " " << "5"  << " ";
			}
			else if (pressureArray[i][f] >= 60 &&  pressureArray[i][f] < 70){
				cout << " " << "6"  << " ";
			}
			else if (pressureArray[i][f] >= 70 &&  pressureArray[i][f] < 80){
				cout << " " << "7"  << " ";
			}
			else if (pressureArray[i][f] >= 80 &&  pressureArray[i][f] < 90){
				cout << " " << "8"  << " ";
			}
			else if (pressureArray[i][f] >= 90 &&  pressureArray[i][f] < 100){
				cout << " " << "9"  << " ";
			}
		}
		cout << " #";
		cout << endl;
	}

	cout << "   ";
	for (int i= 0; i < amtOfHash; i++){
		cout << " " << hash[i] << " ";
	}
	cout << endl;
	cout << "      ";

	for (int i = min_x; i < column + min_x; i++){
		if (i >= 10){
			cout << " " << i;
		}
		else{
			cout << " " << i << " ";
		}
	}
	cout << endl;
}

void displayPressureLMH(){
	string hash;
	int amtOfHash = column + 2;
	hash.assign(amtOfHash, '#');
	cout << "   ";
	for (int i= 0; i < amtOfHash; i++){
		cout << " " << hash[i] << " ";
	}
	cout << endl;
	int a = max_y;
	
	for (int i = 0; i < row; i++){
		if (a >= 10){
			cout << " " << a << " ";
		}else{
			cout << " " << a << "  ";
		}
		cout << "# ";
		a--;
		for (int f = 0; f < column; f++){
			if (pressureArray[i][f] >= 0 &&  pressureArray[i][f] < 35){
				cout << " " << "L" << " ";
			}else if (pressureArray[i][f] >= 35 &&  pressureArray[i][f] < 65){
				cout << " " << "M"  << " ";
			}else if (pressureArray[i][f] >= 65 &&  pressureArray[i][f] < 100){
				cout << " " << "H"  << " ";
			}
			
		}
		cout << " #";
		cout << endl;
	}

	cout << "   ";
	for (int i= 0; i < amtOfHash; i++){
		cout << " " << hash[i] << " ";
	}
	cout << endl;
	cout << "      ";

	for (int i = min_x; i < column + min_x; i++){
		if (i >= 10){
			cout << " " << i;
		}else{
			cout << " " << i << " ";
		}
	}
	cout << endl;
}

void displaySummaryReport(){
	cout << "Weather Forecast Summary Report" << endl;
	cout << "-------------------------------" << endl;

	int * checkPosArray = new int[number]; //create a array to check for avg cloud and pressure

	
	for (int i = 0; i < number; i++){
		checkPosArray[i] = city[i].value; //allocate city ID to respective location

	}
	for (int i = 0; i < number; i++){
		if (checkPosArray[i] != 0){
			float totalCloudArea = 0.00;
			float totalPressureArea = 0.00;
			int counter = 0;
			
			cout << "City Name : " << city[i].name << endl;
			cout << "City ID   : " << city[i].value << endl;
			
			for (int a = 0; a < row; a++){
				for (int b = 0; b < column; b++){
					if (cityArray[a][b] == 0){
						bool outerArea = false;
						for (int c = 0; c < 3; c++){
							if(!((a+1-c) < 0) && !((a+1-c) > (row - 1))) // check the above and bottom of the current row
{
								for (int d = 0; d < 3; d++)
								{
									if (!((b+1-d) < 0) && !((b+1-d) > (column - 1))) // check the left and right of the current column
									{
										if (cityArray[(a+1-c)][(b+1-d)] == city[i].value) //check with the city array if that position is part of the outer area of the city
										{
											outerArea = true;
										}
									}
								}
							}
						}
						if (outerArea == true){
							totalCloudArea = totalCloudArea + cloudArray[a][b];
							totalPressureArea = totalPressureArea + pressureArray[a][b];
							counter++;
						}
					}
				}
			}

			for (int f = 0; f < number; f++){
				if (checkPosArray[f] == city[i].value){
					for (int k = 0; k < total; k++){
						if ((cloud[k].x == city[f].x) && (cloud[k].y == city[f].y)){
							totalCloudArea = totalCloudArea + cloud[k].value;
						}
						
						if ((pressure[k].x == city[f].x) && (pressure[k].y == city[f].y)){
							totalPressureArea = totalPressureArea + pressure[k].value;
						}
					}
					counter++;
					checkPosArray[f] = 0;
				}
			}

			displayAverage(totalCloudArea, totalPressureArea, counter);
			cout << endl;
		}
	}
	delete[] checkPosArray;
}

void displayAverage(float totalCloudArea, float totalPressureArea, int counter){
	cout << setprecision(2) << fixed;
	string cloudLMH;
	string pressureLMH;

	if (((totalCloudArea/counter) >= 0) && ((totalCloudArea/counter) < 35)){
		cloudLMH = "(L)";
	}else if (((totalCloudArea/counter) >= 35) && ((totalCloudArea/counter) < 65)){
		cloudLMH = "(M)";
	}else if (((totalCloudArea/counter) >= 65) && ((totalCloudArea/counter) < 100)){
		cloudLMH = "(H)";
	}

	cout << left << setfill(' ') << setw(26) << "Ave. Cloud Cover  (ACC)" << " : " << totalCloudArea/counter;
	cout << " " << cloudLMH << endl;

	if (((totalPressureArea/counter) >= 0) && ((totalPressureArea/counter) < 35)){
		pressureLMH = "(L)";
	}else if (((totalPressureArea/counter) >= 35) && ((totalPressureArea/counter) < 65)){
		pressureLMH = "(M)";
	}else if (((totalPressureArea/counter) >= 65) && ((totalPressureArea/counter) < 100)){
		pressureLMH = "(H)";
	}

	cout << left << setfill(' ') << setw(26) << "Ave. Pressure  (AP)" << " : " << totalPressureArea/counter;
	cout << " " << pressureLMH << endl;

	if (pressureLMH == "(L)"){
		if (cloudLMH == "(H)"){
			cout <<"Probability of Rain (%)" << " : ";
			cout << " 90.00" << endl;
			cout << "~~~~" << endl;
			cout << "~~~~~" << endl;
			cout << "\\\\\\\\\\" << endl;
		}else if (cloudLMH == "(M)"){
			cout <<"Probability of Rain (%)" << " : ";
			cout << " 80.00" << endl;
			cout << "~~~~" << endl;
			cout << "~~~~~" << endl;
			cout << " " << "\\\\\\" << endl;
		}else if (cloudLMH == "(L)"){
			cout <<"Probability of Rain (%)" << " : ";
			cout << " 70.00" << endl;
			cout << "~~~~" << endl;
			cout << "~~~~~" << endl;
			cout << "  " << "\\\\\\" << endl;
		}
		
	}else if (pressureLMH == "(M)"){
		if (cloudLMH == "(H)"){
			cout <<"Probability of Rain (%)" << " : ";
			cout << " 60.00" << endl;
			cout << "~~~~" << endl;
			cout << "~~~~~" << endl;
			cout << "   " << "\\\\" << endl;
		}else if (cloudLMH == "(M)"){
			cout <<"Probability of Rain (%)" << " : ";
			cout << " 50.00" << endl;
			cout << "~~~~" << endl;
			cout << "~~~~~" << endl;
			cout << "    " << "\\" << endl;
		}else if (cloudLMH == "(L)"){
			cout << "Probability of Rain (%)" << " : ";
			cout << " 40.00" << endl;
			cout << "~~~~" << endl;
			cout << "~~~~~" << endl;
			cout << "     " << endl;
		}
		
	}else if (pressureLMH == "(H)"){
		if (cloudLMH == "(H)"){
			cout <<"Probability of Rain (%)" << " : ";
			cout << " 30.00" << endl;
			cout << "~~~" << endl;
			cout << "~~~~" << endl;
		}else if (cloudLMH == "(M)"){
			cout <<"Probability of Rain (%)" << " : ";
			cout << " 20.00" << endl;
			cout << "~~" << endl;
			cout << "~~~" << endl;
		}else if (cloudLMH == "(L)"){
			cout << "Probability of Rain (%)" << " : ";
			cout << " 10.00" << endl;
			cout << "~" << endl;
			cout << "~~" << endl;
		}
	}
}

//Lab task 2; deallocate memory = free RAM of finished process
void safelydeallocateMemory(int ** mapArray){
	if (column <= 0 ){
		return;
	}for (int i = 0; i < row; i++){
		delete[] mapArray[i];
	}
	delete[] mapArray;
}

void returnMenu(){
	cout << endl;
	cout << "Press the key (Enter) to return to the main menu";
	cin.ignore();
	cin.get();
	cout << endl;
}

//Lab 5: Call Analysis II
//Main program
//Vanesssa Mastrosimone
//CSC 2430
//February 9, 2018

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

#include "CallDb2.h"

void OutputHeading();
void ReformattedOutput(CALL& call, int& totalDuration);

int main(int argc, const char * argv[])
{
	ifstream fin;
	string fileName;
	string timeStarted, phoneNum, callLength;
	cout << "File name: ";
	cin >> fileName;
	fin.open(fileName);

	CALL call;
	CALLDB database;

	//Error message
	if (fin.fail())
	{
		cerr << "Can't open '" << fileName << "'" << endl;
		exit(1);
	}


	int recordsDropped = database.Load(fin);     //Load records from file into call DB
	fin.close();                                        //Close file 

	int recordsAdded = database.GetCountCalls();
	string successMessage = "Log successfully read into database, ";

	if (recordsAdded == 0 && recordsDropped == 0)   //Empty file scenario
		cout << successMessage << "0 records added" << endl;
	else if (recordsAdded > 0)                      //File with records added
		cout << successMessage << recordsAdded << " records added" << endl;
	else if (recordsDropped > 0)                    //File with records dropped
		cout << successMessage << recordsAdded << " records added, " << recordsDropped << " dropped" << endl;

	//Print out records in call database
	cout << '\n' << "Contents of Call Database" << endl;
	if (recordsAdded > 0)
	{
		OutputHeading();
		int totalDuration = 0;
		for (int i = 0; i < recordsAdded; i++)
		{
			database.GetCall(i, call);
			ReformattedOutput(call, totalDuration);
		}

		int minutes = totalDuration / 60;
		int seconds = totalDuration % 60;

		if (seconds < 10)
			 cout << "Total duration of all calls: " << minutes << ":"  << '0' << seconds << endl;
		else
			cout << "Total duration of all calls: " << minutes << ":" << seconds << endl;
	}
	else
		cout << "No records" << endl;



	//Loop: prompt user to enter E164 prefix, print out records that match
	bool keepGoing = true;
	string prefix;
	cin.ignore(1); //Buffer
	while (keepGoing)
	{
		cout << '\n' << "E164 prefix for query:";   //Prompt user
		getline(cin, prefix);
		if (prefix == "")   //If the user hits ENTER to specify query string, program terminates
		{
			keepGoing = false;
			return 0;
		}


		int totalDuration = 0;

		if (database.GetCountCalls() > 0)
		{
			int index = database.FindByE164Prefix(0, prefix);
			if (index == -1)
				cout << "No records" << endl << endl;
			else
			{
				OutputHeading();
				for (int i = index; i < database.GetCountCalls(); i++)
				{
					database.GetCall(index, call);
					if (index == -1) {
						break;
					}
					ReformattedOutput(call, totalDuration);
					index = database.FindByE164Prefix(index + 1, prefix);
				}
				int minutes = totalDuration / 60;
				int seconds = totalDuration % 60;
				if (seconds < 10)
					cout << "Total duration of all calls: " << minutes << ":" << '0' << seconds << endl;

				else
					cout << "Total duration of all calls: " << minutes << ":" << seconds << endl;
			}
		}
	}

	return 0;
}


void OutputHeading()
{
	cout << left << setw(25) << "Time";
	cout << left << setw(10) << "Country";
	cout << left << setw(20) << "Phone Number";
	cout << left << setw(20) << "Duration" << endl;
}

//Outputs correctly formatted start time, country code, phone number, and duration for each call
void ReformattedOutput(CALL& call, int& totalDuration) {
	//Correctly formatted fields
	cout << left << setw(25) << call.GetFormattedStart();
	cout << left << setw(10) << call.GetCountryCode();
	cout << left << setw(20) << call.GetPhoneNumber();
	int reformatDuration = call.GetDuration();

	int minutes = reformatDuration / 60;
	int seconds = reformatDuration % 60;
	if (seconds < 10)
		cout << minutes << ":" << '0' << seconds << endl;
	else
		cout << minutes << ":" << seconds << endl;

	totalDuration += call.GetDuration();
}

//Lab 5: Call Analysis II
//Defines the CALL & CALLDB classes
//Vanesssa Mastrosimone
//CSC 2430
//February 9, 2018

#include <iostream>
#include <string>
using namespace std;

class CALL //Constructor with no parameters: default
{
	friend class CALLDB; //Declare CALLDB to be a friend class
public:
	CALL();
	//Getters - should be const
	string GetStart() const;
	string GetE164() const;
	int GetDuration() const;

	//Formatted Getters - should be const
	string GetFormattedStart() const;
	string GetCountryCode() const;
	string GetPhoneNumber() const;

private:
	//"Setter" methods for setting the start, E164, and duration as strings to be used by the friend class
	void SetStart(string timeStarted);
	void SetE164(string phoneNum);
	void SetDuration(int callLength);

	//Private member variables
	string m_start;
	string m_e164;
	int m_duration;

};

const int MAXCALLS = 15;

class CALLDB //constructor with no parameters, initialize # of records to zero
{

public:
	CALLDB();
	//Load records from file into database. Return # of records that can't be added
	unsigned int Load(istream& fin);
	//Get count of call records
	unsigned int GetCountCalls() const;
	//Retrieve call record
	bool GetCall(unsigned int index, CALL& call) const;
	//Return first index >= of call with matching E164 prefix, or -1
	int FindByE164Prefix(unsigned int startIndex, string prefix) const;
private:
	CALL m_callLog[MAXCALLS];     //Stores calls in database
	unsigned int m_numCalls;      //Number of calls stored
};

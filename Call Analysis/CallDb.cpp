//Lab 5: Call Analysis II
//Implements classes
//Vanesssa Mastrosimone
//CSC 2430
//February 9, 2018

#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>


using namespace std;


#include "CallDb2.h"

//Validates start time by checking that the string is exactly 14 characters
bool ValidStartTime(const string& timeStarted)
{
	if (timeStarted.length() == 14)
		return true;
	else
		return false;
}

//Validates phone number by checking that the string is exactly 12 characters and begins with one of the 3 prefixes
bool ValidPhoneNumber(const string& phoneNum)
{
	string northAmerica("+1");
	string france("+33");
	string hongKong("+852");
	if (phoneNum.length() != 12)
		return false;
	if (phoneNum.find(northAmerica, 0) || phoneNum.find(france, 0) || phoneNum.find(hongKong, 0))
		return true;
	else
		return false;
}

//Validates call duration by checking length & checking that string is all integers
bool ValidCallDuration(const string& callLength)
{
	if (callLength.length() > 0 && callLength.length() < 10 && callLength.find_first_not_of("0123456789") == string::npos)
		return true;
	else
		return false;
}

CALL::CALL()
{
}

//Constructor to initialize the number of records to 0
CALLDB::CALLDB()
{
	m_numCalls = 0;
}

//Getter method for start time
string CALL::GetStart() const
{
	return m_start;
}

//Getter method for E164
string CALL::GetE164() const
{
	return m_e164;
}

//Getter method for duration
int CALL::GetDuration() const
{
	return m_duration;
}


//Formatted getter method to return the reformatted start time
string CALL::GetFormattedStart() const
{
	string startTime = GetStart().substr(4, 2) + "-" + GetStart().substr(6, 2) + "-" + GetStart().substr(0, 4) + " " + GetStart().substr(8, 2) + ":" + GetStart().substr(10, 2) + ":" + GetStart().substr(12, 2);
	return startTime;
}

//Formatted getter method to return the country code
string CALL::GetCountryCode() const
{
	string countryCode;
	string phone = GetE164();
	if (GetE164().substr(0, 2) == "+1")
		countryCode = "1";
	else if (GetE164().substr(0, 3) == "+33")
		countryCode = "33";
	else if (GetE164().substr(0, 4) == "+852")
		countryCode = "852";
	else
		countryCode = "ERROR";
	return countryCode;
}

//Formatted getter method to return the reformatted phone number
string CALL::GetPhoneNumber() const
{
	string localPhoneNumber = "";
	if (GetCountryCode() == "1")
	{
		string temp = GetE164().substr(2, 10);
		localPhoneNumber = "(" + temp.substr(0, 3) + ")-" + temp.substr(3, 3) + "-" + temp.substr(6, 4);
	}
	else if (GetCountryCode() == "33")
	{
		string temp = GetE164().substr(3, 9);
		localPhoneNumber += "0" + temp.substr(0, 1) + " " + temp.substr(1, 2) + " " + temp.substr(3, 2) + " " + temp.substr(5, 2) + " " + temp.substr(7, 2);
	}
	else if (GetCountryCode() == "852")
	{
		string temp = GetE164().substr(4, 8);
		localPhoneNumber = temp.substr(0, 4) + " " + temp.substr(4, 4);
	}
	return localPhoneNumber;
}



//Private setter method for start to be used by the friend class
void CALL::SetStart(string timeStarted)
{
	assert(ValidStartTime(timeStarted));       //Assert that start time being used is valid
	m_start = timeStarted;

}

//Private setter method for E164 to be used by the friend class
void CALL::SetE164(string phoneNum)
{
	assert(ValidPhoneNumber(phoneNum));     //Assert that phone number being used is valid 
	m_e164 = phoneNum;
}

//Private setter method for duration to be used by the friend class
void CALL::SetDuration(int callLength)
{
	assert(ValidCallDuration(to_string(callLength)));    //Assert that duration being used is valid
	m_duration = callLength;
}

//Loads records from file into database. Return # of records that can't be added
unsigned int CALLDB::Load(istream& fin)
{
	int failed = 0;
	int index = m_numCalls;
	string startTime, phoneNumber, duration;


	fin >> startTime >> phoneNumber >> duration;
	while (!fin.eof())
	{
		//If call information is valid & there is room in the array
		if (ValidStartTime(startTime) && ValidPhoneNumber(phoneNumber) && ValidCallDuration(duration) && index < MAXCALLS)
		{
			m_callLog[index].SetStart(startTime);
			m_callLog[index].SetE164(phoneNumber);
			m_callLog[index].SetDuration(stoi(duration));

			index++;
		}
		else
			failed++;
		fin >> startTime >> phoneNumber >> duration;
	}
	m_numCalls = index;    //Set number of calls equal to the number of elements in the callLog array
	return failed;
}

//Get count of call records
unsigned int CALLDB::GetCountCalls() const
{
	unsigned int numberOfCalls = m_numCalls;
	return numberOfCalls;
}

//Retrieve call record
bool CALLDB::GetCall(unsigned int index, CALL& call) const
{
	if (index < m_numCalls)
	{
		call = m_callLog[index]; //Set the call reference parameter = to the CALL record at the specified index in the callLog array
		return true;
	}
	else //Not in range
		return false;
}

//Return first index >= of call with matching E164 prefix, or -1
int CALLDB::FindByE164Prefix(unsigned int startIndex, string prefix) const
{
	for (unsigned int index = startIndex; index < m_numCalls; index++)
	{
		if (m_callLog[index].GetE164().substr(0, prefix.length()) == prefix)
			return index;
	}
	return -1;	//Cannot find in database of call records

}


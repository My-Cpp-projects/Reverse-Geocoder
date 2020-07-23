



#include "application_controller.h"
#include "request_handler.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

////////////////////////////////////////////////////////

struct ApplicationData
{
	std::string m_key{ "N/A" };
	std::string m_pathToCoordinatesFile{ "N/A" };
	std::string m_pathToOutputFile{ "N/A" };
};

struct CoordinateEntry
{
	double m_latitude{ 0 };
	double m_longitude{ 0 };
	bool shouldProcessEntry{ true };
};

////////////////////////////////////////////////////////

ApplicationController::ApplicationController()
	: m_data(std::make_unique<ApplicationData>())
	, m_requestHandler(std::make_unique<RequestHandler>())
{}

ApplicationController::~ApplicationController()
{}

void ApplicationController::run()
{
	getDataFromUser();
	
	std::ifstream coordinateFile(m_data->m_pathToCoordinatesFile);
	std::string currentLine;

	while(std::getline(coordinateFile, currentLine))
	{
		if(currentLine.empty())
		{
			std::cerr << "EMPTY LINE RECEIVED" << std::endl;
			continue;
		}

		CoordinateEntry entry;
		readLineIntoEntry(currentLine, entry);

		std::stringstream response;
		getResponseForEntry(entry, response);

		std::stringstream processedResponse;
		processResponse(response, processedResponse);
		outputResponseIntoFile(processedResponse);
	}
}

void ApplicationController::getDataFromUser()
{
	std::cout << "Enter your mapquest.com key" << std::endl;
	//std::getline(std::cin, m_data->m_key);

	std::cout << "Enter path to the file, where the coordinates are located (Example: D:/User files/aaa.txt)" << std::endl;
	//std::getline(std::cin, m_data->m_pathToCoordinatesFile);

	std::cout << "Enter path to the output file, where results will be stored (NOTE: the new data will be appended)" << std::endl;
	//std::getline(std::cin, m_data->m_pathToOutputFile);

	m_data->m_key = "MVcIFxUDslEOM69nnpe1JX50JhlXIYH6";
	m_data->m_pathToCoordinatesFile = "D:/Gor/Projects/Projects C++/Visual Studio/ReverseGeocoder/testDataInput.txt";
	m_data->m_pathToOutputFile = "D:/Gor/Projects/Projects C++/Visual Studio/ReverseGeocoder/testDataOutput.txt";
}

void ApplicationController::readLineIntoEntry(const std::string& currentLine, CoordinateEntry& entry)
{
	if(currentLine.empty())
	{
		std::cerr << "EMPTY LINE RECEIVED" << std::endl;
		entry.shouldProcessEntry = false;
		return;
	}

	std::istringstream iss(currentLine);
	if(!(iss >> entry.m_latitude >> entry.m_longitude))
	{
		std::cerr << "CAN'T READ LINE FROM FILE: " << m_data->m_pathToCoordinatesFile << std::endl;;
		entry.shouldProcessEntry = false;
	}
}

void ApplicationController::getResponseForEntry(const CoordinateEntry& entry, std::stringstream& response)
{
	if(not entry.shouldProcessEntry)
		return;

	std::string url = composeRequestUrl(entry);
	m_requestHandler->handleRequest(url, response);
}

void ApplicationController::outputResponseIntoFile(const std::stringstream& response)
{
	std::ofstream outFile(m_data->m_pathToOutputFile, std::ios_base::app);
	if(outFile.is_open())
	{
		outFile << response.rdbuf();
	}
	outFile.close();
}

void ApplicationController::processResponse(const std::stringstream& response, std::stringstream& processedResponse)
{
	if(response.str().empty())
	{
		std::cerr << "Empty Response received" << std::endl;
		return;
	}

	std::vector<std::pair<std::string, std::string>> interestingTags
	{
		{"<lat>", "</lat>"},
		{"<lng>", "</lng>"},
		{"<street>", "</street>"}
	};

	auto data = response.str();

	for(auto tagPair : interestingTags)
	{
		processedResponse << extractTagValue(data, tagPair.first, tagPair.second) << " ";
	}
	processedResponse << "\n";
}

std::string ApplicationController::extractTagValue(const std::string& data, const std::string& openingTag, const std::string& closingTag)
{
	std::string retVal {};

	auto findOpening = data.find(openingTag);
	findOpening += openingTag.length();
	
	// TODO: add better error handling
	if(findOpening == std::string::npos)
	{
		std::cout << "CAN'T FIND OPENING TAG" << std::endl;
		return retVal;
	}

	auto findClosing = data.find(closingTag);
	if(findClosing == std::string::npos)
	{
		std::cout << "CAN'T FIND CLOSING TAG" << std::endl;
		return retVal;
	}

	auto length = findClosing - findOpening;

	retVal = data.substr(findOpening, length);
	return retVal;
}

std::string ApplicationController::composeRequestUrl(const CoordinateEntry& entry)
{
	std::string retVal = "http://www.mapquestapi.com/geocoding/v1/reverse?key="
		+ m_data->m_key
		+ "&location=" + std::to_string(entry.m_latitude)
		+ "," + std::to_string(entry.m_longitude)
		+ "&outFormat=xml&thumbMaps=false";

	return retVal;
}

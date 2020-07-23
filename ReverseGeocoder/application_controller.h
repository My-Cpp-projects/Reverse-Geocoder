



#ifndef APPLICATION_CONTROLLER
#define APPLICATION_CONTROLLER

#include <string>
#include <memory>

class RequestHandler;
struct ApplicationData;
struct CoordinateEntry;

class ApplicationController
{
public:
	ApplicationController();
	virtual ~ApplicationController();

	void run();

private:
	void getDataFromUser();
	void readLineIntoEntry(const std::string& currentLine, CoordinateEntry& entry);
	void getResponseForEntry(const CoordinateEntry& entry, std::stringstream& response);
	void outputResponseIntoFile(const std::stringstream& response);
	void processResponse(const std::stringstream& response, std::stringstream& processedResponse);
	std::string extractTagValue(const std::string& data, const std::string& openingTag, const std::string& closingTag);
	std::string composeRequestUrl(const CoordinateEntry& entry);

	std::unique_ptr<ApplicationData> m_data;
	std::unique_ptr<RequestHandler> m_requestHandler;
};

#endif // !APPLICATION_CONTROLLER


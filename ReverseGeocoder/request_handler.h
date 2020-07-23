



#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER

#include <sstream>

class RequestHandler
{
public:
	RequestHandler() = default;
	virtual ~RequestHandler() = default;

	void handleRequest(const std::string& requestUrl, std::stringstream& response);

private:
};

#endif // !REQUEST_HANDLER


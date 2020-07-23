



#include "request_handler.h"
#include "curl_easy.h"
#include "curl_form.h"
#include "curl_ios.h"
#include "curl_exception.h"

void RequestHandler::handleRequest(const std::string& requestUrl, std::stringstream& response)
{
	try
	{
		curl::curl_ios<std::stringstream> writer(response);
		curl::curl_easy easy(writer);

		easy.add<CURLOPT_URL>(requestUrl.data());
		easy.add<CURLOPT_FOLLOWLOCATION>(1L);

		easy.perform();
	}
	catch(curl::curl_easy_exception const& error)
	{
		auto errors = error.get_traceback();

		for(const auto& err : errors)
		{
			response << "\"ERROR:\"\"\"message\"" + err.first + "\"";
		}
		error.clear_traceback();
	}
}

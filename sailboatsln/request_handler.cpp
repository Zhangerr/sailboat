#include "request_handler.hpp"

Response getResponse(Request req)
{
	return Response(req.getUri());
}
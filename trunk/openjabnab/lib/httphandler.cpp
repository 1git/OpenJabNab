#include "apimanager.h"
#include "bunny.h"
#include "bunnymanager.h"
#include "httphandler.h"
#include "httprequest.h"
#include "openjabnab.h"
#include "log.h"
#include "settings.h"

HttpHandler::HttpHandler(QTcpSocket * s, PluginManager * p, ApiManager * a, bool api, bool violet)
{
	incomingHttpSocket = s;
	pluginManager = p;
	apiManager = a;
	httpApi = api;
	httpViolet = violet;
	bytesToReceive = 0;
	connect(s, SIGNAL(readyRead()), this, SLOT(ReceiveData()));
}

HttpHandler::~HttpHandler() {}

void HttpHandler::ReceiveData()
{
	receivedData += incomingHttpSocket->readAll();
	if(bytesToReceive == 0 && (receivedData.size() >= 4))
		bytesToReceive = *(int *)receivedData.left(4).constData();

	if(bytesToReceive != 0 && (receivedData.size() == bytesToReceive))
		HandleBunnyHTTPRequest();
}

void HttpHandler::HandleBunnyHTTPRequest()
{
	HTTPRequest request(receivedData);
	
	if (request.GetURI().startsWith("/ojn_api/"))
	{
		if(httpApi)
		{
			ApiManager::ApiAnswer * answer = apiManager->ProcessApiCall(request.GetURI().mid(9));
			incomingHttpSocket->write(answer->GetData());
			delete answer;
		}
		else
			incomingHttpSocket->write("Api is disabled");
	}
	else if(httpViolet)
	{
		if (request.GetURI().startsWith("/vl/rfid.jsp"))
		{
			QString serialnumber = request.GetArg("sn");
			QString tagId = request.GetArg("t");

			Bunny * b = BunnyManager::GetBunny(serialnumber.toAscii());
			b->SetGlobalSetting("Last RFID Tag", tagId);
		
			if (!pluginManager->OnRFID(b, QByteArray::fromHex(tagId.toAscii())))
			{
				// Forward it to Violet's servers
				request.reply = request.ForwardTo(GlobalSettings::GetString("DefaultVioletServers/PingServer"));
				incomingHttpSocket->write(request.reply);
			}
		}
		else
		{
			pluginManager->HttpRequestBefore(request);
			// If none can answer, try to forward it directly to Violet's servers
			if (!pluginManager->HttpRequestHandle(request))
			{
				if (request.GetURI().startsWith("/vl/sendMailXMPP.jsp"))
				{
					Log::Warning("Problem with the bunny, he's calling sendMailXMPP.jsp !");
					request.reply = "Not Allowed !";
				}
				else if (request.GetURI().startsWith("/vl/"))
					request.reply = request.ForwardTo(GlobalSettings::GetString("DefaultVioletServers/PingServer"));
				else if (request.GetURI().startsWith("/broad/"))
					request.reply = request.ForwardTo(GlobalSettings::GetString("DefaultVioletServers/BroadServer"));
				else
				{
					Log::Error(QString("Unable to handle HTTP Request : ") + request.GetURI());
					request.reply = "404 Not Found\n";
				}
			}
			pluginManager->HttpRequestAfter(request);
			incomingHttpSocket->write(request.reply);
		}
	}
	else
	{
		incomingHttpSocket->write("Bunny's message parsing is disabled <br />");
		incomingHttpSocket->write("Request was : <br />");
		incomingHttpSocket->write(request.toString().toAscii());
	}
	incomingHttpSocket->close();
	delete this;
}
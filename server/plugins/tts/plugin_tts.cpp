#include <QDir>
#include <QCryptographicHash>
#include <memory>
#include "plugin_tts.h"
#include "bunny.h"
#include "bunnymanager.h"
#include "httprequest.h"
#include "messagepacket.h"
#include "settings.h"
#include "ttsmanager.h"
#include "../tts/acapela/tts_acapela.h"

Q_EXPORT_PLUGIN2(plugin_tts, PluginTTS)

PluginTTS::PluginTTS():PluginInterface("tts", "TTS Plugin, Send Text to Bunny",BunnyZtampPlugin)
{
}

/*******
 * API *
 *******/

void PluginTTS::InitApiCalls()
{
	DECLARE_PLUGIN_BUNNY_API_CALL("say(text,voice)", PluginTTS, Api_Say);
	DECLARE_PLUGIN_BUNNY_API_CALL("acapelaList()", PluginTTS, Api_AcapelaList);
}

PLUGIN_BUNNY_API_CALL(PluginTTS::Api_Say)
{
	Q_UNUSED(account);

	if(!bunny->IsConnected())
		return new ApiManager::ApiError(QString("Bunny '%1' is not connected").arg(QString(bunny->GetID())));

	QByteArray fileName = TTSManager::CreateNewSound(hRequest.GetArg("text"), hRequest.GetArg("voice"));

	bunny->SendPacket(MessagePacket("MU " + fileName + "\nMW\n"));
	return new ApiManager::ApiOk(QString("Sending '%1' to bunny '%2'").arg(hRequest.GetArg("text"), QString(bunny->GetID())));
}

PLUGIN_BUNNY_API_CALL(PluginTTS::Api_AcapelaList)
{
	Q_UNUSED(account);
	Q_UNUSED(bunny);
	Q_UNUSED(hRequest);

	QStringList voices = TTSManager::ListVoices();
	return new ApiManager::ApiList(QStringList(voices));
}

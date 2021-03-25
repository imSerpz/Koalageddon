#include "pch.h"
#include "Config.h"
#include "util.h"

// Source: https://stackoverflow.com/a/54394658/3805929
#define GET(j, key) this->key = j[#key].get<decltype(key)>()

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Platform, enabled, process, replicate, ignore, blacklist)

void from_json(const json& j, SteamPlatform& p)
{
	from_json(j, (Platform&) p);
	j["unlock_shared_library"].get_to(p.unlock_shared_library);
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Platforms, Steam, EpicGames, Origin, UplayR1)

Config::Config()
{
	auto fullPath = getWorkingDirPath() / CONFIG_NAME;

	std::ifstream ifs(fullPath, std::ios::in);

	if(!ifs.good())
	{
		MessageBox(NULL, fullPath.c_str(), L"Config not found at: ", MB_ICONERROR);
		exit(1);
	}

	try
	{
		auto j = json::parse(ifs, nullptr, true, true);

		GET(j, log_level);
		GET(j, platforms);
		GET(j, ignore);
		GET(j, terminate);

		platformRefs = j["platforms"].get<Platforms>();
	} catch(json::exception e)
	{
		MessageBoxA(NULL, e.what(), "Error parsing config file", MB_ICONERROR);
		exit(1);
	}
}

void Config::init()
{
	if(config != nullptr)
		return;

	config = new Config();
}

// Every app must call the config constructor first.
Config* config = nullptr;

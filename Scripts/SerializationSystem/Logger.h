#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <Utils\Singleton.h>
#include <iostream>
#include "Core\AppTime.h"

constexpr std::string_view DefaultPath = "Log.txt";

class Logger
{
public:

	std::filesystem::path Path;
	std::fstream File;

	Logger() : Logger(DefaultPath, true) {};

	Logger(std::filesystem::path path, bool rewrite = true): Path(std::move(path))
	{
		if (rewrite)
		{
			File = std::fstream(Path, std::ios_base::out | std::ios_base::trunc);
		}
		else
		{
			File = std::fstream(Path, std::ios_base::out | std::ios_base::ate);
		}

		if (!File.is_open())
		{
			throw std::exception("Log file doesn't open");
		}

		File << "-------------------" << AppTime::GetTime() << "-------------------" << std::endl;
	}

	std::fstream& GetFile()
	{
		return File;
	}

	template<typename T>
	void Log(T const& arg)
	{
		File << arg << std::endl;
	}

	template<typename T, typename... Ts>
	void Log(T const& firstArg, Ts const& ... args)
	{
		File << firstArg;
		Log(args...);
	}

};

template<>
void SingletonFill<Logger>(Singleton<Logger>& a);
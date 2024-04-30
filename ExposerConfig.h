#pragma once

enum class BuildType
{
	DEBUG, RELEASE
};

enum class LogLevel
{
	DEBUG, INFO
};

static BuildType buildType = BuildType::DEBUG;
static LogLevel logLevel = LogLevel::INFO;
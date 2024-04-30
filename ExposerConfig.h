#pragma once

enum class BuildType
{
	DEBUG, RELEASE
};

enum class LogLevel
{
	DEBUG, INFO
};

static BuildType buildType = BuildType::RELEASE;
static LogLevel logLevel = LogLevel::INFO;
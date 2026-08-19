#pragma once
#include "TelemetryMessage.h"
#include <string>

class TelemetryParser {
public:
	// TCP로 받은 JSON 문자열을 검사해서 정상적인 TelemetryMessage 객체로 변환
	bool parse(const std::string& jsonText,
		TelemetryMessage& message) const;

	// C++ TelemetryMessage를 TCP로 보낼 JSON 문자열로 바꾼다.
	std::string toJson(const TelemetryMessage& message) const;
};


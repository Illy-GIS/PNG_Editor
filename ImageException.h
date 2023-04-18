#pragma once
#include <exception>
#include <string>

class ImageException : public std::exception {
public:
	ImageException(std::string message) : errorMessage(message) {}

	~ImageException() = default;

	const char* what() const noexcept override {
		return errorMessage.c_str();
	}

private:
	std::string errorMessage;
};

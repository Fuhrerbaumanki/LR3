#pragma once

#include <exception>
#include <string>

class BadIteratorException : public std::exception {
public:
  const char *what() const override { return errorMessage.c_str(); }

private:
  std::string errorMessage = "Iterator is not in container";
};

#pragma once

#include <exception>
#include "common/common.hpp"

namespace novatel::edie
{
class DecoderException : public std::exception
{
public:
   DecoderException(STATUS status);

   const char* what() const noexcept override;

private:
   STATUS status;
   std::string msg;
};
}
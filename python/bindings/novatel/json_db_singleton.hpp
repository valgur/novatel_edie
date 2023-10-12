#pragma once

#include "common/jsonreader.hpp"
#include <memory>

class JsonDbSingleton
{
public:
   static JsonReader::Ptr& get();

private:
   JsonDbSingleton() = default;
};

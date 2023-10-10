#pragma once

#include "common/jsonreader.hpp"
#include <memory>

class JsonDbSingleton
{
public:
   static JsonReader* get();

private:
   JsonDbSingleton() = default;
};

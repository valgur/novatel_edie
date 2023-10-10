#pragma once

#include <memory>

#include "common/jsonreader.hpp"

class JsonDbSingleton
{
  public:
    static JsonReader* get();

  private:
    JsonDbSingleton() = default;
};

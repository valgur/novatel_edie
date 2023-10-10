#pragma once

#include <memory>

#include "edie/decoders/common/json_reader.hpp"

class JsonDbSingleton
{
  public:
    static JsonReader* get();

  private:
    JsonDbSingleton() = default;
};

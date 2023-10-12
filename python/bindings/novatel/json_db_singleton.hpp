#pragma once

#include <memory>

#include "edie/decoders/common/json_reader.hpp"

class JsonDbSingleton
{
  public:
    static JsonReader::Ptr& get();

  private:
    JsonDbSingleton() = default;
};

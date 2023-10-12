#pragma once

#include <memory>

#include "common/jsonreader.hpp"

class JsonDbSingleton
{
  public:
    static JsonReader::Ptr& get();

  private:
    JsonDbSingleton() = default;
};

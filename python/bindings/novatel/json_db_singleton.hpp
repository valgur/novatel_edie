#pragma once

#include <memory>

#include "novatel/edie/common/jsonreader.hpp"

class JsonDbSingleton
{
  public:
    static JsonReader::Ptr& get();

  private:
    JsonDbSingleton() = default;
};

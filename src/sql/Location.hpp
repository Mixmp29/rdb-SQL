#pragma once

#include <cstddef>

namespace rdb::sql {
  
struct Location {
  Location(size_t row, size_t col, size_t offset) : row_(row), col_(col), offset_(offset) {}
  size_t row_;
  size_t col_;
  size_t offset_;
};

}

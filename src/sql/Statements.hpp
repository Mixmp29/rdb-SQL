#pragma once

#include <memory>
#include <string_view>
#include <variant>
#include <vector>

namespace rdb::sql {

class Statement {
 public:
  virtual ~Statement() = 0;
};

using StatementPtr = std::unique_ptr<const Statement>;

class DropTableStatement: public Statement {
  public:
    explicit DropTableStatement(std::string_view table_name) 
      : table_name_(table_name) {}

    std::string_view table_name() const { return table_name_; }

  private:
    std::string_view table_name_;
};

using DropTableStatementPtr = std::unique_ptr<const DropTableStatement>;

using Value = std::variant<long int, double, std::string_view>;

class InsertStatement: public Statement {
  public:
    InsertStatement(
      std::string_view table_name,
      const std::vector<std::string_view>& column_names,
      const std::vector<Value>& values)
        : table_name_(table_name), column_names_(column_names), values_(values) {}
        
    std::string_view table_name() const { return table_name_; }

    const std::vector<std::string_view>& column_names() const {
      return column_names_;
    }

    const std::vector<Value>& values() const { return values_; }

  private:
    std::string_view table_name_;
    std::vector<std::string_view> column_names_;
    std::vector<Value> values_;
}; 

using InsertStatementPtr = std::unique_ptr<const InsertStatement>;

} // namespace rdb::sql
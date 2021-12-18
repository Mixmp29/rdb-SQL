#include <sql/Statements.hpp>

#include <iostream>

namespace rdb::sql {

Statement::~Statement() = default;

std::ostream& CreateTableStatement::print(std::ostream& os) const
{
    os << "CreateTableStatement: TABLE - " << table_name() << "; ColumnDefs -";
    for (const auto& ColumnDefs : column_defs()) {
        os << " " << ColumnDefs;
    }
    return os;
}

std::ostream& SelectStatement::print(std::ostream& os) const
{
    os << "SelectStatement: TABLE - " << table_name() << "; ColumnList -";
    for (const auto& ColumnList : column_list()) {
        os << " " << ColumnList;
    }
    if (!expression().has_value()) {
        return os;
    }
    os << "; Expression - " << expression().value().left_ << " "
       << expression().value().operation_ << " " << expression().value().right_;
    return os;
}

std::ostream& DropTableStatement::print(std::ostream& os) const
{
    os << "DropTableStatement: TABLE - " << table_name();
    return os;
}

std::ostream& DeleteStatement::print(std::ostream& os) const
{
    os << "DeleteStatement: TABLE - " << table_name();
    if (!expression().has_value()) {
        return os;
    }
    os << "; Expression - " << expression().value().left_ << " "
       << expression().value().operation_ << " " << expression().value().right_;
    return os;
}

std::ostream& InsertStatement::print(std::ostream& os) const
{
    os << "InsertStatement: TABLE - " << table_name() << "; Column Names -";
    for (const auto& ColumnNames : column_names()) {
        os << " " << ColumnNames;
    }
    os << "; Values -";
    for (const auto& Values : values()) {
        os << " " << Values;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Value& value)
{
    if (const long int* int_value = std::get_if<long int>(&value)) {
        os << *int_value;
    } else if (const double* double_value = std::get_if<double>(&value)) {
        os << *double_value;
    } else if (
            const std::string_view* string_value
            = std::get_if<std::string_view>(&value)) {
        os << *string_value;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Operand& operand)
{
    if (const Value* value = std::get_if<Value>(&operand)) {
        os << *value;
    } else if (
            const std::string_view* string_value
            = std::get_if<std::string_view>(&operand)) {
        os << *string_value;
    }
    return os;
}

} // namespace rdb::sql
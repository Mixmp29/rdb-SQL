#include <sql/Statements.hpp>

#include <iostream>

namespace rdb::sql {

Statement::~Statement() = default;

std::ostream& CreateTableStatement::print(std::ostream& os) const
{
    os << "CreateTableStatement: Table - " << table_name() << "; ColumnDefs -";
    for (const auto& i : column_defs()) {
        os << " " << i;
    }
    os << "\n";
    return os;
}

std::ostream& SelectStatement::print(std::ostream& os) const
{
    os << "SelectStatement: Table - " << table_name() << "; ColumnList -";
    for (const auto& i : column_list()) {
        os << " " << i;
    }
    if (!expression().has_value()) {
        os << "\n";
        return os;
    }
    os << "; Expression - " << expression().value().left_ << " "
       << expression().value().operation_ << " " << expression().value().right_;
    os << "\n";
    return os;
}

std::ostream& DropTableStatement::print(std::ostream& os) const
{
    os << "DropTableStatement: Table - " << table_name() << "\n";
    return os;
}

std::ostream& DeleteStatement::print(std::ostream& os) const
{
    os << "DeleteStatement: Table - " << table_name();
    if (!expression().has_value()) {
        os << "\n";
        return os;
    }
    os << "; Expression - " << expression().value().left_ << " "
       << expression().value().operation_ << " " << expression().value().right_;
    os << "\n";
    return os;
}

std::ostream& InsertStatement::print(std::ostream& os) const
{
    os << "InsertStatement: Table - " << table_name() << "; Column Names -";
    for (const auto& i : column_names()) {
        os << " " << i;
    }
    os << "; Values -";
    for (const auto& i : values()) {
        os << " " << i;
    }
    os << "\n";
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

/* std::ostream& operator<<(std::ostream& os, Expression::OpType type)
{
    if (Expression::OpType Lt = type == Expression::OpType::Lt) {
        os << Lt;
    } else if (
            Expression::OpType Lte = type == Expression::OpType::Lte) {
        os << Lte;
    }
    return os;
} */

} // namespace rdb::sql
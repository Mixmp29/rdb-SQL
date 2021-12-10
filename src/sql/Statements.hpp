#pragma once

#include <memory>
#include <optional>
#include <string_view>
#include <variant>
#include <vector>

namespace rdb::sql {

class Statement {
public:
    virtual ~Statement() = 0;
};

using StatementPtr = std::unique_ptr<const Statement>;
using Value = std::variant<long int, double, std::string_view>;
using Operand = std::variant<std::string_view, Value>;

class Expression {
public:
    enum class OpType { Lt, Lte };
    Expression(Operand left, OpType operation, Operand right)
        : left_(left), operation_(operation), right_(right)
    {
    }

    Operand left() const
    {
        return left_;
    }

    OpType operation() const
    {
        return operation_;
    }

    Operand right() const
    {
        return right_;
    }

private:
    Operand left_;
    OpType operation_;
    Operand right_;
};

// using ExpressionPtr = std::unique_ptr<const Expression>;

class CreateTableStatement : public Statement {
public:
    CreateTableStatement(
            std::string_view table_name,
            const std::vector<std::string_view>& column_names)
        : table_name_(table_name), column_names_(column_names)
    {
    }

    std::string_view table_name() const
    {
        return table_name_;
    }

    const std::vector<std::string_view>& column_names() const
    {
        return column_names_;
    }

private:
    std::string_view table_name_;
    std::vector<std::string_view> column_names_;
};

using CreateTableStatementPtr = std::unique_ptr<const CreateTableStatement>;

class SelectStatement : public Statement {
public:
    SelectStatement(
            std::string_view table_name,
            const std::vector<std::string_view>& column_list,
            std::optional<Expression> expression)
        : table_name_(table_name),
          column_list_(column_list),
          expression_(expression)
    {
    }

    std::string_view table_name() const
    {
        return table_name_;
    }
    const std::vector<std::string_view>& column_list() const
    {
        return column_list_;
    }
    const std::optional<Expression>& exptression() const
    {
        return expression_;
    }

private:
    std::string_view table_name_;
    const std::vector<std::string_view> column_list_;
    std::optional<Expression> expression_;
};

using SelectStatementPtr = std::unique_ptr<const SelectStatement>;

class DropTableStatement : public Statement {
public:
    explicit DropTableStatement(std::string_view table_name)
        : table_name_(table_name)
    {
    }

    std::string_view table_name() const
    {
        return table_name_;
    }

private:
    std::string_view table_name_;
};

class DeleteStatement : public Statement {
public:
    DeleteStatement(
            std::string_view table_name, std::optional<Expression> expression)
        : table_name_(table_name), expression_(expression)
    {
    }

    std::string_view table_name() const
    {
        return table_name_;
    }
    const std::optional<Expression>& exptression() const
    {
        return expression_;
    }

private:
    std::string_view table_name_;
    std::optional<Expression> expression_;
};

using DeleteStatementPtr = std::unique_ptr<const DeleteStatement>;

using DropTableStatementPtr = std::unique_ptr<const DropTableStatement>;

class InsertStatement : public Statement {
public:
    InsertStatement(
            std::string_view table_name,
            const std::vector<std::string_view>& column_names,
            const std::vector<Value>& values)
        : table_name_(table_name), column_names_(column_names), values_(values)
    {
    }

    std::string_view table_name() const
    {
        return table_name_;
    }

    const std::vector<std::string_view>& column_names() const
    {
        return column_names_;
    }

    const std::vector<Value>& values() const
    {
        return values_;
    }

private:
    std::string_view table_name_;
    std::vector<std::string_view> column_names_;
    std::vector<Value> values_;
};

using InsertStatementPtr = std::unique_ptr<const InsertStatement>;

} // namespace rdb::sql
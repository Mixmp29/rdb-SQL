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
    virtual std::ostream& print(std::ostream& os) const
    {
        return os;
    };
    friend std::ostream&
    operator<<(std::ostream& os, const Statement& statement)
    {
        return statement.print(os);
    }
};

using StatementPtr = std::unique_ptr<const Statement>;
using Value = std::variant<long int, double, std::string_view>;
using Operand = std::variant<std::string_view, Value>;

class Expression {
public:
    Expression(Operand left, std::string_view operation, Operand right)
        : left_(left), operation_(operation), right_(right)
    {
    }

    Operand left() const
    {
        return left_;
    }

    std::string_view operation() const
    {
        return operation_;
    }

    Operand right() const
    {
        return right_;
    }

    Operand left_;
    std::string_view operation_;
    Operand right_;
};

// using ExpressionPtr = std::unique_ptr<const Expression>;

class CreateTableStatement : public Statement {
public:
    CreateTableStatement(
            std::string_view table_name,
            const std::vector<std::string_view>& column_defs)
        : table_name_(table_name), column_defs_(column_defs)
    {
    }

    std::string_view table_name() const
    {
        return table_name_;
    }

    const std::vector<std::string_view>& column_defs() const
    {
        return column_defs_;
    }

    std::ostream& print(std::ostream& os) const override;

private:
    std::string_view table_name_;
    std::vector<std::string_view> column_defs_;
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
    const std::optional<Expression>& expression() const
    {
        return expression_;
    }

    std::ostream& print(std::ostream& os) const override;

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

    std::ostream& print(std::ostream& os) const override;

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
    const std::optional<Expression>& expression() const
    {
        return expression_;
    }

    std::ostream& print(std::ostream& os) const override;

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

    std::ostream& print(std::ostream& os) const override;

private:
    std::string_view table_name_;
    std::vector<std::string_view> column_names_;
    std::vector<Value> values_;
};

using InsertStatementPtr = std::unique_ptr<const InsertStatement>;

std::ostream& operator<<(std::ostream& os, const Value& value);
std::ostream& operator<<(std::ostream& os, const Operand& operand);

} // namespace rdb::sql
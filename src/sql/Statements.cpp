#include <sql/Statements.hpp>

namespace rdb::sql {

Statement::~Statement() = default;

/* std::ostream& operator<<(std::ostream& os, const Statement& statement) {
  os << kind_to_str(token.kind()) << " '" << token.text() << "' "
     << "Loc=" << token.location().row_ << ":" << token.location().col_;
  return os;
} */

} // namespace rdb::sql
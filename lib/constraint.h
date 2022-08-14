#pragma once

#include "counter.h"
#include "variable.h"

#include <map>
#include <string>

namespace jsolve
{
class Constraint : public Counter<Constraint>
{
  public:
    enum class Type
    {
        LESS,
        GREAT,
        EQUAL
    };

    explicit Constraint(Type type, const std::string &name);

    Type type() const;
    Type &type();

    double rhs() const;
    double &rhs();

    void add_to_lhs(double coeff, Variable *var);
    void add_to_rhs(double coeff, Variable *var);

    const std::map<Variable *, double, Variable::CompareNames> &get_entries() const;
    std::map<Variable *, double, Variable::CompareNames> &get_entries();

    std::string to_string() const;

    friend std::ostream &operator<<(std::ostream &os, const Constraint &c);

  private:
    Type m_type{Type::LESS};
    double m_rhs{0.0};
    std::map<Variable *, double, Variable::CompareNames> m_entries;
};

std::ostream &operator<<(std::ostream &os, const Constraint &c);
} // namespace jsolve

#include "mps.h"

#include "tools.h"

#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace
{
void process_rows_data_record(jsolve::Model& model, const std::vector<std::string>& words)
{
    const auto& constraint_type = words.at(0);
    const auto& constraint_name = words.at(1);

    if (constraint_type == "N")
    {
        model.objective_name() = constraint_name;
    }
    else if (constraint_type == "G")
    {
        model.make_constraint(jsolve::Constraint::Type::GREAT, constraint_name);
    }
    else if (constraint_type == "L")
    {
        model.make_constraint(jsolve::Constraint::Type::LESS, constraint_name);
    }
    else if (constraint_type == "E")
    {
        model.make_constraint(jsolve::Constraint::Type::EQUAL, constraint_name);
    }
    else
    {
        throw jsolve::MPSError(fmt::format("Unknown ROWS type: {}", constraint_type));
    }
}

void process_cols_data_record(jsolve::Model& model, const std::vector<std::string>& words)
{
    const auto& variable_name = words.at(0);

    auto* variable = model.get_variable(variable_name);

    if (!variable)
    {
        variable = model.make_variable(jsolve::Variable::Type::LINEAR, variable_name);
    }

    auto it = std::cbegin(words);
    auto it_end = std::cend(words) - 1;

    for (; it != it_end; ++it)
    {
        it++;
        auto constraint_name = *it;
        auto constraint_value = std::stod(*(it + 1));

        if (model.objective_name() == constraint_name)
        {
            // Pair is specifying the objective coefficient of this variable
            variable->cost() = constraint_value;
        }
        else
        {
            // Pair is specifying the entries of a constraint
            auto* constraint = model.get_constraint(constraint_name);

            if (constraint)
            {
                constraint->add_to_lhs(constraint_value, variable);
            }
            else
            {
                throw jsolve::MPSError(fmt::format("Constraint not found: {}", constraint_value));
            }
        }
    }
}

void process_rhs_data_record(jsolve::Model& model, const std::vector<std::string>& words)
{
    auto it = (words.size() % 2) ? std::cbegin(words) : std::next(std::cbegin(words));

    auto it_end = std::cend(words) - 1;

    for (; it != it_end; ++it)
    {
        it++;
        auto constraint_name = *it;
        auto rhs = std::stod(*(it + 1));

        auto* constraint = model.get_constraint(constraint_name);

        if (constraint)
        {
            constraint->rhs() = rhs;
        }
        else
        {
            throw jsolve::MPSError(fmt::format("Constraint not found: {}", constraint_name));
        }
    }
}

void process_bounds_data_record(jsolve::Model& model, const std::vector<std::string>& words)
{
    // Bounds record could have 4 entries:
    // LO INTBOU    GRDTIMN1         -105
    // or two types of 3 entries:
    // UP           C03609             14
    // FR BND       x2

    const auto& bound_type = words.at(0);

    if (bound_type == "LO")
    {
        // Lower bound
        auto* variable = model.get_variable(words.size() == 3 ? words.at(1) : words.at(2));
        const auto& bound_value = std::stod(words.size() == 3 ? words.at(2) : words.at(3));

        variable->lower_bound() = bound_value;
    }
    else if (bound_type == "UP")
    {
        // Upper bound
        auto* variable = model.get_variable(words.size() == 3 ? words.at(1) : words.at(2));
        const auto& bound_value = std::stod(words.size() == 3 ? words.at(2) : words.at(3));

        if (bound_value < 0.0)
        {
            // Can't handle negative variables with current simplex implementation.
            throw jsolve::MPSError(fmt::format("Unhandled MPS upper bounds < 0 for variable: {}", words.at(2)));
        }
        else
        {
            variable->upper_bound() = bound_value;
        }
    }
    else if (bound_type == "FX")
    {
        // Fixed value
        auto* variable = model.get_variable(words.size() == 3 ? words.at(1) : words.at(2));
        const auto& bound_value = std::stod(words.size() == 3 ? words.at(2) : words.at(3));

        if (bound_value < 0.0)
        {
            // Can't handle negative variables with current simplex implementation.
            throw jsolve::MPSError(fmt::format("Unhandled MPS fixed values < 0 for variable: {}", words.at(2)));
        }
        else
        {
            variable->lower_bound() = bound_value;
            variable->upper_bound() = bound_value;
        }
    }
    else if (bound_type == "FR")
    {
        auto* variable = model.get_variable(words.at(2));
        variable->lower_bound() = -std::numeric_limits<double>::infinity();
        variable->upper_bound() = std::numeric_limits<double>::infinity();
    }
    else
    {
        throw jsolve::MPSError(fmt::format("Unhandled MPS BOUNDS type: {}", bound_type));
    }
}
} // namespace

namespace jsolve
{
MPSError::MPSError(const std::string& message)
    : std::runtime_error(message)
{
}

MPSError::~MPSError() = default;

enum class section
{
    NONE,
    NAME,
    OBJSENSE,
    OBJNAME,
    ROWS,
    COLUMNS,
    RHS,
    BOUNDS,
    RANGES,
    END
};

section record_to_section(std::string_view line)
{
    if (line == "NAME")
    {
        return section::NAME;
    }
    else if (line == "OBJSENSE")
    {
        return section::OBJSENSE;
    }
    else if (line == "OBJNAME")
    {
        return section::OBJNAME;
    }
    else if (line == "ROWS")
    {
        return section::ROWS;
    }
    else if (line == "COLUMNS")
    {
        return section::COLUMNS;
    }
    else if (line == "RHS")
    {
        return section::RHS;
    }
    else if (line == "BOUNDS")
    {
        return section::BOUNDS;
    }
    else if (line == "ENDATA")
    {
        return section::END;
    }
    else if (line == "RANGES")
    {
        return section::RANGES;
    }
    else
    {
        throw jsolve::MPSError(fmt::format("Unknown header: {}", line));
    }
}

bool is_indicator_record(std::string_view line)
{
    if (line.empty())
    {
        return false;
    }
    else
    {
        return std::isalpha(line.front());
    }
}

std::vector<std::string> split_string(const std::string& line)
{
    std::istringstream iss{line};
    std::vector<std::string> result{std::istream_iterator<std::string>{iss}, {}};
    return result;
}

void process_indicator_record(
    std::optional<jsolve::Model>& model, const section& section, const std::vector<std::string>& words
)
{
    if (section == section::NAME)
    {
        model.emplace(Model::Sense::MIN, words.size() > 1 ? words.at(1) : "Unnamed");
    }
}

void process_data_record(
    std::optional<jsolve::Model>& model, const section& section, const std::vector<std::string>& words
)
{
    if (section == section::ROWS)
    {
        process_rows_data_record(model.value(), words);
    }
    else if (section == section::COLUMNS)
    {
        process_cols_data_record(model.value(), words);
    }
    else if (section == section::RHS)
    {
        process_rhs_data_record(model.value(), words);
    }
    else if (section == section::BOUNDS)
    {
        process_bounds_data_record(model.value(), words);
    }
    else if (section == section::RANGES)
    {
        throw MPSError("Unhandled MPS section RANGES");
    }
}

void process_record(std::optional<jsolve::Model>& model, section& section, const std::string& line)
{
    auto words = split_string(line);

    if (is_indicator_record(line))
    {
        section = record_to_section(words.front());
        process_indicator_record(model, section, words);
    }
    else
    {
        process_data_record(model, section, words);
    }
}

std::size_t count_lines(std::filesystem::path path)
{
    std::ifstream file{path, std::ios::in | std::ios::binary};

    std::size_t line_count = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');

    return line_count;
}

jsolve::Model read_mps(std::filesystem::path path)
{
    Timer timer{info_logger(), "Reading MPS file {}", path};

    std::optional<jsolve::Model> model;

    if (!std::filesystem::exists(path))
    {
        throw MPSError(fmt::format("File does not exist: {}", path));
    }

    std::ifstream file{path, std::ios::in | std::ios::binary};

    if (file.is_open())
    {
        auto line_count = count_lines(path);
        log()->debug("File has {} lines", line_count);

        auto current_section = section::NONE;

        const std::size_t print_percent = 10;
        std::size_t step = line_count / (100 / print_percent);
        std::size_t next_print = step;
        std::size_t current_line = 0;

        std::string line;
        while (getline(file, line))
        {
            log()->debug("|{}", line);
            process_record(model, current_section, line);

            current_line++;
            if (current_line >= next_print)
            {
                auto percent = (100 * current_line) / line_count;
                next_print += step;
                log()->debug("{}%", percent);
            }
        }
    }
    else
    {
        throw MPSError(fmt::format("File could not be opened: {}", path));
    }

    if (!model)
    {
        throw MPSError("No model created");
    }

    return std::move(model.value());
}
} // namespace jsolve

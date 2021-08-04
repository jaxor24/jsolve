#include "mps.h"

#include "logging.h"

#include <optional>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace jsolve
{
	template <typename... Args>
	MPSError::MPSError(Args&&... args)
		:
		std::runtime_error(fmt::format(std::forward<Args>(args)...))
	{}
	
	MPSError::~MPSError() throw () 
	{}
}


namespace jsolve
{
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

	section line_to_section(std::string_view line)
	{
		if (line == "NAME") { return section::NAME; }
		else if (line == "OBJSENSE") { return section::OBJSENSE; }
		else if (line == "OBJNAME") { return section::OBJNAME; }
		else if (line == "ROWS") { return section::ROWS; }
		else if (line == "COLUMNS") { return section::COLUMNS; }
		else if (line == "RHS") { return section::RHS; }
		else if (line == "BOUNDS") { return section::BOUNDS; }
		else if (line == "ENDATA") { return section::END; }
		else if (line == "RANGES") { return section::RANGES; }
		else { throw jsolve::MPSError("Unknown header: {}", line); }
	}

	bool is_header(std::string_view line)
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
		std::istringstream iss{ line };
		std::vector<std::string> result { std::istream_iterator<std::string>{ iss }, {} };
		return result;
	}

	void process_header_row(std::optional<jsolve::Model>& model, const section& section, const std::vector<std::string>& words)
	{
		if (section == section::NAME)
		{
			model.emplace(Model::Sense::MIN, words.at(1));
		}
	}

	void process_section_row(std::optional<jsolve::Model>& model, const section& section, const std::vector<std::string>& words)
	{
		if (section == section::ROWS)
		{
			const auto& constraint_type = words.at(0);
			const auto& constraint_name = words.at(1);

			if (constraint_type == "N")
			{
				model->objective_name() = constraint_name;
			}
			else if (constraint_type == "G")
			{
				model->make_constraint(jsolve::Constraint::Type::GREAT, constraint_name);
			}
			else if (constraint_type == "L")
			{
				model->make_constraint(jsolve::Constraint::Type::LESS, constraint_name);
			}
			else if (constraint_type == "E")
			{
				model->make_constraint(jsolve::Constraint::Type::EQUAL, constraint_name);
			}
			else
			{
				throw MPSError("Unknown ROWS type: {}", constraint_type);
			}
		}
		else if (section == section::COLUMNS)
		{
			const auto& variable_name = words.at(0);

			auto* variable = model->get_variable(variable_name);

			if (!variable)
			{
				variable = model->make_variable(jsolve::Variable::Type::LINEAR, variable_name);
			}

			auto it = std::cbegin(words);
			auto it_end = std::cend(words) - 1;
			
			for (; it != it_end; ++it)
			{
				it++;
				auto constraint_name = *it;
				auto constraint_value = std::stod(*(it + 1));

				if (model->objective_name() == constraint_name)
				{
					// Pair is specifying the objective coefficient of this variable
					variable->cost() = constraint_value;
				}
				else
				{
					// Pair is specifying the entries of a constraint
					auto* constraint = model->get_constraint(constraint_name);

					if (constraint) 
					{
						constraint->add_to_lhs(constraint_value, variable);
					}
					else
					{
						throw MPSError("Constraint not found: {}", constraint_value);
					}
				}
			}
		}
		else if (section == section::RHS)
		{
			auto it = std::cbegin(words);
			auto it_end = std::cend(words) - 1;

			for (; it != it_end; ++it)
			{
				it++;
				auto constraint_name = *it;
				auto rhs = std::stod(*(it + 1));

				auto* constraint = model->get_constraint(constraint_name);

				if (constraint)
				{
					constraint->rhs() = rhs;
				}
				else
				{
					throw MPSError("Constraint not found: {}", constraint_name);
				}
			}
		}
		else if (section == section::BOUNDS)
		{
			const auto& bound_type = words.at(0);
			// const auto& bound_name = words.at(1);

			auto* variable = model->get_variable(words.at(2));
			const auto& bound_value = std::stod(words.at(3));

			if (bound_type == "LO")
			{
				// Lower bound
				if (bound_value < 0.0)
				{
					// Can't handle negative variables with current simplex implementation.
					throw MPSError("Unhandled MPS lower bounds < 0 for variable: {}", words.at(2));
				}
				else
				{
					auto* constraint = model->make_constraint(jsolve::Constraint::Type::GREAT, fmt::format("BND_{}_GEQ_{}", words.at(2), bound_value));
					constraint->rhs() = bound_value;
					constraint->add_to_lhs(1.0, variable);
				}
			}
			else if (bound_type == "UP")
			{
				// Upper bound
				if (bound_value < 0.0)
				{
					// Can't handle negative variables with current simplex implementation.
					throw MPSError("Unhandled MPS upper bounds < 0 for variable: {}", words.at(2));
				}
				else
				{
					auto* constraint = model->make_constraint(jsolve::Constraint::Type::LESS, fmt::format("BND_{}_LEQ_{}", words.at(2), bound_value));
					constraint->rhs() = bound_value;
					constraint->add_to_lhs(1.0, variable);
				}
			}
			else if (bound_type == "FX")
			{
				// Fixed value
				if (bound_value < 0.0)
				{
					// Can't handle negative variables with current simplex implementation.
					throw MPSError("Unhandled MPS fixed values < 0 for variable: {}", words.at(2));
				}
				else
				{
					// Create upper and lower bound constraints
					auto* lb_constraint = model->make_constraint(jsolve::Constraint::Type::GREAT, fmt::format("BND_{}_GEQ_{}", words.at(2), bound_value));
					lb_constraint->rhs() = bound_value;
					lb_constraint->add_to_lhs(1.0, variable);

					auto* ub_constraint = model->make_constraint(jsolve::Constraint::Type::LESS, fmt::format("BND_{}_LEQ_{}", words.at(2), bound_value));
					ub_constraint->rhs() = bound_value;
					ub_constraint->add_to_lhs(1.0, variable);
				}
			}
			else
			{
				throw MPSError("Unhandled MPS BOUNDS type: {}", bound_type);
			}
		}
		else if (section == section::RANGES)
		{
			log()->warn("Unprocessed mps section: RANGES");
		}
	}

	void process_row(std::optional<jsolve::Model>& model, section& section, const std::string& line)
	{
		auto words = split_string(line);

		if (is_header(line))
		{
			section = line_to_section(words.front());
			process_header_row(model, section, words);
		}
		else
		{
			process_section_row(model, section, words);
		}
	}

	std::size_t count_lines(std::filesystem::path path)
	{
		std::ifstream file{ path, std::ios::in | std::ios::binary };

		std::size_t line_count = std::count(
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>(), 
			'\n'
		);

		return line_count;
	}

	jsolve::Model read_mps(std::filesystem::path path)
	{
		log()->info("Reading MPS file: {}", path);

		std::optional<jsolve::Model> model;

		if (!std::filesystem::exists(path))
		{
			throw MPSError("File does not exist: {}", path);
		}

		std::ifstream file{ path, std::ios::in | std::ios::binary };

		if (file.is_open())
		{
			auto line_count = count_lines(path);
			log()->info("File has {} lines", line_count);

			auto current_section = section::NONE;

			const std::size_t print_percent = 10;
			std::size_t step = line_count / (100 / print_percent);
			std::size_t next_print = step;
			std::size_t current_line = 0;

			std::string line;
			while (getline(file, line))
			{
				log()->debug("|{}", line);
				process_row(model, current_section, line);

				current_line++;
				if (current_line >= next_print)
				{
					auto percent = (100 * current_line) / line_count;
					next_print += step;
					log()->info("{}%", percent);
				}
			}
		}
		else
		{
			throw MPSError("File could not be opened: {}", path);
		}

		log()->info("Parsed model:");
		//log()->info(model->to_string());
		return std::move(model.value());
	}
}

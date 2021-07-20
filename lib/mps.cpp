#include "mps.h"

#include "logging.h"

#include "model.h"

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

			auto it_end = std::cend(words) - 1;
			auto it = std::cbegin(words);

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
		}
		else if (section == section::BOUNDS)
		{
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


	void read_mps(std::filesystem::path path)
	{
		if (!std::filesystem::exists(path))
		{
			log()->warn("File {} does not exist.", path);
			return;
		}

		std::ifstream file{ path, std::ios::in | std::ios::binary };

		if (file.is_open())
		{
			std::optional<jsolve::Model> model;
			auto current_section = section::NONE;

			std::string line;
			while (getline(file, line))
			{
				log()->debug("|{}", line);
				process_row(model, current_section, line);
			}

			log()->debug("{}", model.value());
		}
		else
		{
			log()->warn("File {} could not be opened.", path);
		}	
	}
}

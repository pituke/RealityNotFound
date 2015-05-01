
#include "Importer/Parsing/Java/ParserlibBase.h"

namespace Importer
{
	namespace Parsing
	{
		using parserlib::error_list;
		using parserlib::input;

        ast_node* ParserAlgorithms::ParseInternal(const string& sourceCode, rule& rootRule, rule& whitespaceRule, vector<ParseError>& errorList)
		{
			error_list errors;
			ast_node* astNode = parse(input(sourceCode.begin(), sourceCode.end()), rootRule, whitespaceRule, errors);
			if (!astNode)
			{
				errors.sort();
				errorList.clear();
				errorList.reserve(errors.size());
                for (auto it = errors.begin(); it != errors.end(); ++it)
				{
                    const auto& err = *it;
                    ParseError pe;
                    pe.line = err.m_begin.m_line;
                    pe.column = err.m_begin.m_col;
                    errorList.push_back(pe);
				}
			}
			return astNode;
		}
	}
}

#pragma once

#include <QString>
#include <sstream>
#include <vector>
#include <parserlib.hpp>

#define CONNECT_RULE_AST(ClassName, RuleName) parserlib::ast<ClassName> _##RuleName(RuleName)

namespace Importer
{
	namespace Parsing
	{
		using std::vector;
		using std::list;
		using parserlib::ast_ptr;
		using parserlib::ast_list;
		using parserlib::ast_node;
		using parserlib::ast_stack;
		using parserlib::ast_container;
		using parserlib::rule;

		typedef unsigned int uint;

		template <typename T, bool _OPTIONAL = false>
		class AstValue : public ast_ptr <T, _OPTIONAL>
		{
		public:
			AstValue(T *obj = 0)
				: ast_ptr<T, _OPTIONAL>(obj)
			{
				}

			AstValue(const ast_ptr<T, _OPTIONAL>& src)
				: ast_ptr<T, _OPTIONAL>(src)
			{
				}
		};

		template <typename T>
		class AstList : public ast_list <T>
		{
		public:
			AstList()
				: ast_list<T>()
			{
				}

			AstList(const ast_list<T>& src)
				: ast_list<T>(src)
			{
				}

			const container& GetItems() const
			{
				return objects();
			}

			vector<T*> ToStdVector() const
			{
				return vector<T*>(objects().begin(), objects().end());
			}

			const T* At(uint index) const
			{
				auto iter = objects().begin();
				advance(iter, index);
				return *iter;
			}

			const T* First() const
			{
				return objects().front();
			}

			uint GetCount() const
			{
				return objects().size();
			}

			typename list<T*>::const_iterator begin() const
			{
				return objects().begin();
			}

			typename list<T*>::const_iterator end() const
			{
				return objects().end();
			}

			bool Empty() const
			{
				return objects().empty();
			}
		};

		class NodeTerm : public ast_node
		{
			template <class T> friend class ast;

		private:
			QString value;

		public:
			NodeTerm()
				: ast_node()
			{
			}

			NodeTerm(const ast_node& n)
				: ast_node(n)
			{
			}

			NodeTerm(const QString& val)
				: ast_node(), value(val)
			{
			}

			virtual ~NodeTerm()
			{
			}

            void construct(ast_stack &st)
            {
				value = QString::fromStdString(std::string(m_begin.m_it, m_end.m_it));
            }


			virtual QString GetValue() const
			{
				return value;
			}

			void SetValue(const QString& str)
			{
				value = str;
			}
		};

		class NodeContainer : public ast_container
		{
		public:
			NodeContainer()
				: ast_container()
			{
			}

			NodeContainer(const ast_container& src)
				: ast_container(src)
			{
			}
		};

        struct ParseError
		{
			int line;
			int column;
		};

		class ParserAlgorithms abstract
		{
			template<typename _ROOT_AST_NODE_TYPE> friend class FFParser;

		private:
			static ast_node* ParseInternal(const QString& sourceCode, rule& rootRule, rule& whitespaceRule, QVector<ParseError>& errorList);
		};

		template <typename _ROOT_AST_NODE_TYPE>
        class Parser
		{
		private:
			rule& rootRule;
			rule& whitespaceRule;
            QVector<ParseError> errorList;

		public:
            Parser(rule& rootRule, rule& whitespaceRule)
				: rootRule(rootRule), whitespaceRule(whitespaceRule)
			{
			}

            ~Parser()
			{
			}

			_ROOT_AST_NODE_TYPE* Parse(const QString& sourceCode)
			{
				return static_cast<_ROOT_AST_NODE_TYPE*>(FFParserAlgorithms::ParseInternal(sourceCode, rootRule, whitespaceRule, errorList));
			}

			QString GetErrorMessage() const
			{
				ostringstream oss;
				oss << "found " << errorList.size() << " " << (errorList.size() > 1 ? "errors" : "error") << ":\n";
				for (const auto& error : errorList)
				{
					oss << "    line " << error.line << ", col " << error.column << ": " << endl;
				}
				return QString::fromStdString(oss.str());
			}
		};

	}
}

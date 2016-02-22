#pragma once

#include <vector>
#include <string>
#include <map>
#include <QString>

#define TABS(_count) QString(_count, '\t').toStdString()

namespace Importer
{
    namespace Parsing
    {
        using std::vector;
        using std::string;

		class Modifier
		{
		private:
			Modifier();

		public:
			enum ModifierEnum { UNKNOWN, PUBLIC, PROTECTED, PRIVATE };
			static string ToString(ModifierEnum modifier)
			{
				static const std::map<ModifierEnum, string> enumToStr
				{
					{ PUBLIC, "public" },
					{ PROTECTED, "protected" },
					{ PRIVATE, "private" }
				};
				auto found = enumToStr.find(modifier);
				return found != enumToStr.end() ? found->second : string();
			}
		};

        struct Attribute
        {
			string type;
            string name;

            string ToString(int tabs) const
            {
                return TABS(tabs) + type + " " + name + '\n';
            }
        };

		struct Parameter
		{
			string type;
			string name;

			string ToString(int tabs) const
			{
				return TABS(tabs) + type + " " + name + '\n';
			}
		};

        struct Method
        {
			Modifier::ModifierEnum modifier;
			string returnType;
			string name;
			vector<Parameter> parameters;

			string ToString(int tabs) const
			{
				string s;
				for (const auto& p : parameters)
					s += (s.empty() ? "" : ", ") + p.ToString(0);
				return TABS(tabs) + Modifier::ToString(modifier) + " " + returnType + " " + name + "(" + s + ")";
			}
        };

        struct Class
        {
			Modifier::ModifierEnum modifier;
            string name;
            vector<Attribute> attributes;
            vector<Method> methods;

			Class()
				: modifier(Modifier::UNKNOWN)
			{
			}

            string ToString(int tabs) const
            {
                string s = TABS(tabs) + "class " + name + '\n';
                s += TABS(tabs) + "attributes:\n";
                for (const auto& attribute : attributes)
					s += attribute.ToString(tabs + 1) + '\n';
                s += TABS(tabs) + "methods:\n";
				for (const auto& method : methods)
					s += method.ToString(tabs + 1) + '\n';
                return s;
            }
        };

		struct EnumConstant
		{
			string name;

			string ToString(int tabs) const
			{
				return TABS(tabs) + name;
			}
		};

		struct Enum
		{
			Modifier::ModifierEnum modifier;
			string name;
			vector<EnumConstant> enums;
			vector<Attribute> attributes;
			vector<Method> methods;

			Enum()
				: modifier(Modifier::UNKNOWN)
			{
			}

			string ToString(int tabs) const
			{
				string s = TABS(tabs) + "enum " + name + '\n';
				s += TABS(tabs) + "enums:\n";
				for (const auto& e : enums)
					s += e.ToString(tabs + 1) + '\n';
				s += TABS(tabs) + "attributes:\n";
				for (const auto& attribute : attributes)
					s += attribute.ToString(tabs + 1) + '\n';
				s += TABS(tabs) + "methods:\n";
				for (const auto& method : methods)
					s += method.ToString(tabs + 1) + '\n';
				return s;
			}
		};

        struct Namespace
        {
            string name;
            vector<Class> classes;
			vector<Enum> enums;

            string ToString(int tabs) const
            {
                string s = "namespace " + name + "{\n";
                for (const auto& c : classes)
                    s += c.ToString(tabs+1) + '\n';
				for (const auto& e : enums)
					s += e.ToString(tabs + 1) + '\n';
                s += "}\n";
                return s;
            }
        };

        struct SoftTree
        {
            vector<Namespace> namespaces;

            string ToString() const
            {
                string s;
                foreach (const Namespace& _namespace, namespaces)
                {
                    s += _namespace.ToString(0) + '\n';
                }
                return s;
            }
        };
    }
}

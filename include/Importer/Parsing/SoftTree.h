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
			Modifier::ModifierEnum modifier;
			string type;
            string name;

            string ToString(int tabs) const
            {
				auto modifierStr = Modifier::ToString(modifier);
				return TABS(tabs) + (modifierStr.empty() ? "" : modifierStr + " ") + type + " " + name;
            }
        };

		struct Parameter
		{
			string type;
			string name;

			string ToString(int tabs) const
			{
				return TABS(tabs) + type + " " + name;
			}
		};

        struct Method
        {
			Modifier::ModifierEnum modifier;
			string returnType;
			string name;
			vector<Parameter> parameters;
			string content;

			string ToString(int tabs) const
			{
				string params;
				for (const auto& p : parameters)
					params += (params.empty() ? "" : ", ") + p.ToString(0);
				auto modifierStr = Modifier::ToString(modifier);
				return TABS(tabs) + (modifierStr.empty() ? "" : modifierStr + " ") + (IsConstructor() ? "" : returnType + " ") + name + "(" + params + ")" + (IsInterface() ? "" : "\n" + TABS(tabs) + content);
			}
			bool IsConstructor() const
			{
				return returnType.empty();
			}
			bool IsInterface() const
			{
				return content.empty();
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
                string s = TABS(tabs) + "class " + name + "\n" + TABS(tabs) + "{\n";
				for (const auto& attribute : attributes)
					s += attribute.ToString(tabs + 1) + ";\n";
				if (!attributes.empty()) s += '\n';
				bool mFirst = true;
				for (const auto& method : methods)
				{
					if (!mFirst) s += "\n\n";
					s += method.ToString(tabs + 1) + (method.IsInterface() ? ";" : "");
					mFirst = false;
				}
				s += "\n" + TABS(tabs) + "}";
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
				string s = TABS(tabs) + "enum " + name + "\n" + TABS(tabs) + "{\n";
				bool eFirst = true;
				for (const auto& e : enums)
				{
					if (!eFirst) s += ",\n";
					s += e.ToString(tabs + 1);
					eFirst = false;
				}
				if (!enums.empty()) s += ";\n\n";
				for (const auto& attribute : attributes)
					s += attribute.ToString(tabs + 1) + ";\n";
				if (!attributes.empty()) s += '\n';
				bool mFirst = true;
				for (const auto& method : methods)
				{
					if (!mFirst) s += "\n\n";
					s += method.ToString(tabs + 1) + (method.IsInterface() ? ";" : "");
					mFirst = false;
				}
				s += "\n" + TABS(tabs) + "}";
				return s;
			}
		};

		struct Interface
		{
			Modifier::ModifierEnum modifier;
			string name;
			vector<Attribute> attributes;
			vector<Method> methods;

			Interface()
				: modifier(Modifier::UNKNOWN)
			{
			}

			string ToString(int tabs) const
			{
				string s = TABS(tabs) + "interface " + name + "\n" + TABS(tabs) + "{\n";
				for (const auto& attribute : attributes)
					s += attribute.ToString(tabs + 1) + ";\n";
				if (!attributes.empty()) s += '\n';
				bool mFirst = true;
				for (const auto& method : methods)
				{
					if (!mFirst) s += "\n\n";
					s += method.ToString(tabs + 1) + (method.IsInterface() ? ";" : "");
					mFirst = false;
				}
				s += "\n" + TABS(tabs) + "}";
				return s;
			}
		};

        struct Namespace
        {
            string name;
            vector<Class> classes;
			vector<Enum> enums;
			vector<Interface> interfaces;

            string ToString(int tabs) const
            {
                string s = "namespace " + (name.empty() ? "(default)" : name) + " --> \n\n";
                for (const auto& c : classes)
                    s += c.ToString(tabs) + "\n\n";
				for (const auto& e : enums)
					s += e.ToString(tabs) + "\n\n";
				for (const auto& i : interfaces)
					s += i.ToString(tabs) + "\n\n";
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

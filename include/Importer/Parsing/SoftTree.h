#pragma once

#include <vector>
#include <string>
#include <QString>

#define TABS(_count) QString(_count, '\t').toStdString()

namespace Importer
{
    namespace Parsing
    {
        using std::vector;
        using std::string;

        struct Attribute
        {
            string name;

            string ToString(int tabs) const
            {
                return TABS(tabs) + name + '\n';
            }
        };

        struct Method
        {
            string name;

            string ToString(int tabs) const
            {
                return TABS(tabs) + name + '\n';
            }
        };

        struct Class
        {
            string name;
            vector<Attribute> attributes;
            vector<Method> methods;

            string ToString(int tabs) const
            {
                string s = TABS(tabs) + "class " + name + '\n';
                s += TABS(tabs) + "attributes:\n";
                foreach (const Attribute& attribute, attributes)
                {
                    s += attribute.ToString(tabs+1);
                }
                s += TABS(tabs) + "methods:\n";
                foreach (const Method& method, methods)
                {
                    s += method.ToString(tabs+1);
                }
                return s;
            }
        };

        struct Namespace
        {
            string name;
            vector<Class> classes;

            string ToString(int tabs) const
            {
                string s = "namespace " + name + "{\n";
                foreach (const Class& _class, classes)
                {
                    s += TABS(tabs) + _class.ToString(tabs+1) + '\n';
                }
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

#pragma once

#include <vector>

namespace Importer
{
    namespace Parsing
    {
        using std::vector;

        struct Attribute
        {
        };

        struct Method
        {
        };

        struct Class
        {
            vector<Attribute> attributes;
            vector<Method> methods;
        };

        struct Namespace
        {
            vector<Class> classes;
        };

        struct SoftTree
        {
            vector<Namespace> namespaces;
        };
    }
}

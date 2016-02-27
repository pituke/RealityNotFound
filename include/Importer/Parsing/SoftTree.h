#pragma once

#include <QVector>
#include <QString>

namespace Importer
{
    namespace Parsing
    {
		class Modifier
		{
		private:
			Modifier();

		public:
			enum ModifierEnum { UNKNOWN, PUBLIC, PROTECTED, PRIVATE };
			static QString ToString(ModifierEnum modifier);
		};

        struct Attribute
        {
			Modifier::ModifierEnum modifier;
			QString type;
            QString name;

			QString ToString(int tabs) const;
        };

		struct Parameter
		{
			QString type;
			QString name;

			QString ToString(int tabs) const;
		};

        struct Method
        {
			Modifier::ModifierEnum modifier;
			QString returnType;
			QString name;
			QVector<Parameter> parameters;
			QString content;

			QString ToString(int tabs) const;
			bool IsConstructor() const;
			bool IsInterface() const;
        };

        struct Class
        {
			Modifier::ModifierEnum modifier;
            QString name;
            QVector<Attribute> attributes;
            QVector<Method> methods;

			Class();
			QString ToString(int tabs) const;
        };

		struct EnumConstant
		{
			QString name;

			QString ToString(int tabs) const;
		};

		struct Enum
		{
			Modifier::ModifierEnum modifier;
			QString name;
			QVector<EnumConstant> enums;
			QVector<Attribute> attributes;
			QVector<Method> methods;

			Enum();
			QString ToString(int tabs) const;
		};

		struct Interface
		{
			Modifier::ModifierEnum modifier;
			QString name;
			QVector<Attribute> attributes;
			QVector<Method> methods;

			Interface();
			QString ToString(int tabs) const;
		};

        struct Namespace
        {
            QString name;
            QVector<Class> classes;
			QVector<Enum> enums;
			QVector<Interface> interfaces;

			QString ToString(int tabs) const;
        };

        struct SoftTree
        {
            QVector<Namespace> namespaces;

			QString ToString() const;
        };
    }
}

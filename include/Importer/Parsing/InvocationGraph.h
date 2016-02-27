#pragma once

#include "SoftTree.h"
#include <QVector>
#include <QMap>

namespace Importer
{
	namespace Parsing
	{
		typedef unsigned int uint;

		struct GetterSetterDirectAttrAccess
		{
			uint callingMethodIndex;
			uint accessedAttributeIndex;
		};

		struct MethodInvocation
		{
			uint callingMethodIndex;
			QVector<uint> calledMethodsIndexes;
			QVector<uint> calledGettersSettersIndexes;
			QVector<uint> calledDirectAttributesIndexes;
		};

		struct InvocationGraph
		{
			QVector<GetterSetterDirectAttrAccess> gettersSetters;
			QVector<MethodInvocation> internalMethods;
			QVector<MethodInvocation> interfaceMethods;

			static InvocationGraph AnalyzeClass(const Class& class1)
			{
				InvocationGraph res;

				QMap<QString, GetterSetterDirectAttrAccess> gettersSettersMap;
				QMap<QString, MethodInvocation> internalMethodsMap;
				QMap<QString, MethodInvocation> interfaceMethodsMap;
				QMap<QString, uint> allMethods;

				for (uint i = 0; i < class1.methods.count(); ++i)
					allMethods.insert(class1.methods[i].name, i);

				return res;
			}
		};
	}
}

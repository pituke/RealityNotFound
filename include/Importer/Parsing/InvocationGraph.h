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
			QVector<MethodInvocation> constructors;

			static InvocationGraph AnalyzeClass(const Class& class1)
			{
				InvocationGraph res;

				QMap<QString, GetterSetterDirectAttrAccess> gettersSettersMap;
				QMap<QString, MethodInvocation> internalMethodsMap;
				QMap<QString, MethodInvocation> interfaceMethodsMap;
				QMap<QString, MethodInvocation> constructorsMap;
				QMap<const Method&, uint> allMethods;

				for (uint i = 0; i < class1.methods.count(); ++i)
					allMethods.insert(class1.methods[i], i);

				QMapIterator<const Method&, uint> m(allMethods);
				while (m.hasNext())
				{
					// cekni ci je to getter setter - nazov get/set, vnutri return atribut - spoj ho z atributom s ktorym pracuje
					const auto& method = m.key();
					if (method.name.toLower().startsWith("get") || method.name.toLower().startsWith("set"))
					{
						QRegExp re("return\\w+;");
						int pos = 0;
						while ((pos = re.indexIn(method.content, pos)) != -1)
						{
							
							pos += re.matchedLength();
						}
					}

					// cekni ci je to constructor, public, private/protected metoda a podla toho zarad
					// cekni volania metod, get/setterov, atributov a podla toho napln asociacie - cekuj relativne k zoznamom znamych metod v mapach
				}

				return res;
			}
		};
	}
}

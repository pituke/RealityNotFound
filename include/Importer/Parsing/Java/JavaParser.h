#pragma once

#include "JavaAst.h"
#include "../SoftTree.h"

namespace Importer
{
	namespace Parsing
	{
		class JavaParser
		{
		public:
			JavaParser();
            bool Parse(const string& javaProjectDirectory, SoftTree& softTree, string& errorMessage = string());
		};
	}
}

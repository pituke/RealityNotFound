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
			void Parse(const string& javaProjectDirectory);
            bool ParseUnit(const string& javaFilePath, SourceFileCompilationUnit& javaRootAst, string& errorMessage = string());
            SoftTree ProcessParseResult(const SourceFileCompilationUnit& rootAst);
		};
	}
}

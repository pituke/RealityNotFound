
#include "Importer/Parsing/Java/JavaGrammar.h"
#include "Importer/Parsing/Java/JavaParser.h"
#include <fstream>
#include <sstream>
#include <QDirIterator>
#include <QTextStream>

namespace Importer
{
	namespace Parsing
	{
		using namespace std;
		using namespace parserlib;

        struct CompileResult
        {
            string file;
            string errorMessage;
            SourceFileCompilationUnit* astRoot;
            CompileResult() : astRoot(NULL)
            {
            }
        };

        void ProcessParseResult(const vector<CompileResult>& compileResults, SoftTree& softTree);

		JavaParser::JavaParser()
		{
			#pragma region connect rules <-> ast

			CONNECT_RULE_AST(NodeExpressionBracket, expressionBracket);
			CONNECT_RULE_AST(NodeExpressionRound, expressionRound);
			CONNECT_RULE_AST(NodeExpressionString, expressionString);
			CONNECT_RULE_AST(NodeExpressionPostfix, expressionPostfix);
			CONNECT_RULE_AST(NodeExpression, expression);
			CONNECT_RULE_AST(NodeExpressionString2, expressionString2);
			CONNECT_RULE_AST(NodeExpression2, expression2);
			CONNECT_RULE_AST(NodeExpression3, expression3);
			CONNECT_RULE_AST(NodeExpressionList, expressionList);
			CONNECT_RULE_AST(NodeIdentifier, identifier);
			CONNECT_RULE_AST(NodeQualifiedName, qualifiedName);
			CONNECT_RULE_AST(NodeAnnotationIdentifier, annotationIdentifier);
			CONNECT_RULE_AST(NodeAnnotationName, annotationName);
			CONNECT_RULE_AST(NodeAnnotationSequenceBefore, annotationSequenceBefore);
			CONNECT_RULE_AST(NodeAnnotationSequenceAfter, annotationSequenceAfter);
			CONNECT_RULE_AST(NodeAnnotationSequence, annotationSequence);
			CONNECT_RULE_AST(NodeAnnotation, annotation);
			CONNECT_RULE_AST(NodePackageDeclaration, packageDeclaration);
			CONNECT_RULE_AST(NodeImportDeclaration, importDeclaration);
			CONNECT_RULE_AST(NodeClassOrInterfaceModifierBasic, classOrInterfaceModifierBasic);
			CONNECT_RULE_AST(NodeClassOrInterfaceModifier, classOrInterfaceModifier);
			CONNECT_RULE_AST(NodeTypeArgumentsBefore, typeArgumentsBefore);
			CONNECT_RULE_AST(NodeTypeArgumentsAfter, typeArgumentsAfter);
			CONNECT_RULE_AST(NodeTypeArguments, typeArguments);
			CONNECT_RULE_AST(NodeClassOrInterfaceTypePart, classOrInterfaceTypePart);
			CONNECT_RULE_AST(NodeClassOrInterfaceType, classOrInterfaceType);
			CONNECT_RULE_AST(NodePrimitiveType, primitiveType);
			CONNECT_RULE_AST(NodeType, type);
			CONNECT_RULE_AST(NodeTypeBound, typeBound);
			CONNECT_RULE_AST(NodeTypeParameter, typeParameter);
			CONNECT_RULE_AST(NodeTypeParameters, typeParameters);
			CONNECT_RULE_AST(NodeClassDeclarationExtend, classDeclarationExtend);
			CONNECT_RULE_AST(NodeTypeList, typeList);
			CONNECT_RULE_AST(NodeClassDeclarationImplement, classDeclarationImplement);
			CONNECT_RULE_AST(NodeBlockTextBefore, blockTextBefore);
			CONNECT_RULE_AST(NodeBlockTextAfter, blockTextAfter);
			CONNECT_RULE_AST(NodeBlockWithTextAfter, blockWithTextAfter);
			CONNECT_RULE_AST(NodeBlock, block);
			CONNECT_RULE_AST(NodeModifier, modifier);
			CONNECT_RULE_AST(NodeVariableModifier, variableModifier);
			CONNECT_RULE_AST(NodeVariableDeclaratorId, variableDeclaratorId);
			CONNECT_RULE_AST(NodeFormalParameter, formalParameter);
			CONNECT_RULE_AST(NodeLastFormalParameter, lastFormalParameter);
			CONNECT_RULE_AST(NodeFormalParameterList, formalParameterList);
			CONNECT_RULE_AST(NodeFormalParameters, formalParameters);
			CONNECT_RULE_AST(NodeQualifiedNameList, qualifiedNameList);
			CONNECT_RULE_AST(NodeMethodBody, methodBody);
			CONNECT_RULE_AST(NodeMethodDeclaration, methodDeclaration);
			CONNECT_RULE_AST(NodeGenericMethodDeclaration, genericMethodDeclaration);
			CONNECT_RULE_AST(NodeArrayInitializer, arrayInitializer);
			CONNECT_RULE_AST(NodeVariableInitializer, variableInitializer);
			CONNECT_RULE_AST(NodeVariableDeclarator, variableDeclarator);
			CONNECT_RULE_AST(NodeVariableDeclarators, variableDeclarators);
			CONNECT_RULE_AST(NodeFieldDeclaration, fieldDeclaration);
			CONNECT_RULE_AST(NodeConstructorBody, constructorBody);
			CONNECT_RULE_AST(NodeConstructorDeclaration, constructorDeclaration);
			CONNECT_RULE_AST(NodeGenericConstructorDeclaration, genericConstructorDeclaration);
			CONNECT_RULE_AST(NodeConstantDeclarator, constantDeclarator);
			CONNECT_RULE_AST(NodeConstDeclaration, constDeclaration);
			CONNECT_RULE_AST(NodeInterfaceMethodDeclaration, interfaceMethodDeclaration);
			CONNECT_RULE_AST(NodeGenericInterfaceMethodDeclaration, genericInterfaceMethodDeclaration);
			CONNECT_RULE_AST(NodeInterfaceBodyInner, interfaceBodyInner);
			CONNECT_RULE_AST(NodeInterfaceDeclarationInner, interfaceDeclarationInner);
			CONNECT_RULE_AST(NodeAnnotationTypeBody, annotationTypeBody);
			CONNECT_RULE_AST(NodeAnnotationTypeDeclarationInner, annotationTypeDeclarationInner);
			CONNECT_RULE_AST(NodeClassBodyInner, classBodyInner);
			CONNECT_RULE_AST(NodeClassDeclarationInner, classDeclarationInner);
			CONNECT_RULE_AST(NodeEnumConstantsAndBodyDeclarationsInner, enumConstantsAndBodyDeclarationsInner);
			CONNECT_RULE_AST(NodeEnumDeclarationInner, enumDeclarationInner);
			CONNECT_RULE_AST(NodeMemberDeclaration, memberDeclaration);
			CONNECT_RULE_AST(NodeClassBodyDeclaration, classBodyDeclaration);
			CONNECT_RULE_AST(NodeClassBody, classBody);
			CONNECT_RULE_AST(NodeClassDeclaration, classDeclaration);
			CONNECT_RULE_AST(NodeArguments, arguments);
			CONNECT_RULE_AST(NodeEnumConstant, enumConstant);
			CONNECT_RULE_AST(NodeEnumConstants, enumConstants);
			CONNECT_RULE_AST(NodeEnumBodyDeclarations, enumBodyDeclarations);
			CONNECT_RULE_AST(NodeEnumDeclaration, enumDeclaration);
			CONNECT_RULE_AST(NodeInterfaceMemberDeclaration, interfaceMemberDeclaration);
			CONNECT_RULE_AST(NodeInterfaceBodyDeclaration, interfaceBodyDeclaration);
			CONNECT_RULE_AST(NodeInterfaceBody, interfaceBody);
			CONNECT_RULE_AST(NodeInterfaceDeclaration, interfaceDeclaration);
			CONNECT_RULE_AST(NodeAnnotationTypeDeclaration, annotationTypeDeclaration);
			CONNECT_RULE_AST(NodeTypeDeclaration, typeDeclaration);
			CONNECT_RULE_AST(SourceFileCompilationUnit, compilationUnit);

			#pragma endregion
		}

        bool JavaParser::Parse(const string& javaProjectDirectory, SoftTree& softTree, string& errorMessage)
		{
            QDirIterator dirIt(QString::fromStdString(javaProjectDirectory), QStringList("*.java"), QDir::Files, QDirIterator::Subdirectories);
            vector<CompileResult> compileResults;
            while (dirIt.hasNext())
            {
                QString filePath = dirIt.next();

                CompileResult cr;
                cr.file = filePath.toStdString();

                QFile f(filePath);
                if (!f.open(QFile::ReadOnly | QFile::Text))
                {
                    cr.errorMessage = "Cannot open file";
                    continue;
                }
                string fileContent = QTextStream(&f).readAll().toStdString();
                input input(fileContent.begin(), fileContent.end());
                bool pass = false;
                error_list errors;
                try
                {
                    pass = parse(input, compilationUnit, whitespace, errors, cr.astRoot);
                }
                catch (exception& ex)
                {
                    cr.errorMessage = ex.what();
                    compileResults.push_back(cr);
                    continue;
                }
                if (!pass)
                {
                    ostringstream oss;
                    oss << "    found " << errors.size() << " " << (errors.size() > 1 ? "errors" : "error") << ": ";
                    errors.sort();
                    for (auto it = errors.begin(); it != errors.end(); ++it)
                    {
                        const auto& e = *it;
                        oss << "line " << e.m_begin.m_line << ", col " << e.m_begin.m_col << ": ";
                        oss << endl;
                    }
                    cr.errorMessage = oss.str();
                }
                compileResults.push_back(cr);
            }

            foreach (const CompileResult& cr, compileResults)
            {
                if (!cr.errorMessage.empty())
                {
                    if (!errorMessage.empty())
                        errorMessage += "\n";
                    errorMessage += cr.file + ":\n" + cr.errorMessage;
                }
            }

            if (errorMessage.empty())
            {
                ProcessParseResult(compileResults, softTree);
                return true;
            }
            else
                return false;
        }

		string GetQualifiedName(const NodeQualifiedName* qualifiedName)
		{
			string s;
			for (const auto& i : qualifiedName->identifier)
				s += (s.empty() ? "" : ".") + i->GetValue();
			return s;
		}

		string GetBlockContent(const NodeBlock* block)
		{
			string s;
			s += "{";
			s += block->blockTextBefore->GetValue();
			for (const auto& b : block->blockWithTextAfter)
			{
				s += GetBlockContent(b->block);
				s += b->blockTextAfter->GetValue();
			}
			s += "}";
			return s;
		}

        void ProcessParseResult(const vector<CompileResult>& compileResults, SoftTree& softTree)
        {
			/*const auto& root = compileResults.front().astRoot;
			string package;
			if (!root->packageDeclaration.Empty())
				package = GetQualifiedName(root->packageDeclaration.First()->qualifiedName);

			vector<string> imports;
			for (const auto& i : root->importDeclaration)
				imports.push_back(GetQualifiedName(i->qualifiedName));

			return;*/

            /*QMap<string, Namespace> javaPackages;

            foreach (const CompileResult& cr, compileResults)
            {
                SourceFileCompilationUnit* javaAstRoot = cr.astRoot;

                auto javaClassList = javaAstRoot->GetClassList();
                foreach (const NodeClassDeclaration* javaClass, javaClassList)
                {
                    Class _class;
                    _class.name = javaClass->GetClassName();

                    auto javaClassBody = javaClass->GetClassBody();
                    auto javaMethodList = javaClassBody->GetMethodList();
                    auto javaGenericMethodList = javaClassBody->GetGenericMethodList();
                    auto javaConstructorList = javaClassBody->GetConstructorList();
                    auto javaGenericConstructorList = javaClassBody->GetGenericConstructorList();
                    auto javaFieldList = javaClassBody->GetFieldList();

                    foreach (const NodeMethodDeclaration* javaMethod, javaMethodList)
                    {
                        Method method;
                        method.name = javaMethod->GetMethodName();
                        _class.methods.push_back(method);
                    }
                    foreach (const NodeGenericMethodDeclaration* javaGenericMethod, javaGenericMethodList)
                    {
                        Method method;
                        method.name = javaGenericMethod->GetGenericMethodName();
                        _class.methods.push_back(method);
                    }
                    foreach (const NodeConstructorDeclaration* javaConstructor, javaConstructorList)
                    {
                        Method method;
                        method.name = javaConstructor->GetConstructorName();
                        _class.methods.push_back(method);
                    }
                    foreach (const NodeGenericConstructorDeclaration* javaGenericConstructor, javaGenericConstructorList)
                    {
                        Method method;
                        method.name = javaGenericConstructor->GetGenericConstructorName();
                        _class.methods.push_back(method);
                    }

                    foreach (const NodeFieldDeclaration* javaField, javaFieldList)
                    {
                        Attribute attribute;
                        attribute.name = javaField->GetFieldName();
                        _class.attributes.push_back(attribute);
                    }

                    auto packageName = javaAstRoot->GetPackageName();
                    if (!javaPackages.contains(packageName))
                    {
                        Namespace javaPackage;
                        javaPackage.name = packageName;
                        javaPackages.insert(packageName, javaPackage);
                    }
                    javaPackages[packageName].classes.push_back(_class);
                }
            }

            Q_FOREACH(auto javaPackage, javaPackages)
            {
                softTree.namespaces.push_back(javaPackage);
            }*/
        }
	}
}

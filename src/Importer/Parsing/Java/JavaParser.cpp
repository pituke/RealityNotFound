
#include "Importer/Parsing/Java/JavaGrammar.h"
#include "Importer/Parsing/Java/JavaParser.h"
#include "Importer/Parsing/CollectionHelper.h"
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

			CONNECT_RULE_AST(SourceFileCompilationUnit, compilationUnit);
			CONNECT_RULE_AST(NodePackageDeclaration, packageDeclaration);
			CONNECT_RULE_AST(NodeImportDeclaration, importDeclaration);
			CONNECT_RULE_AST(NodeTypeDeclaration, typeDeclaration);
			CONNECT_RULE_AST(NodeClassOrInterfaceModifier, classOrInterfaceModifier);
			CONNECT_RULE_AST(NodeTypeBound, typeBound);
			CONNECT_RULE_AST(NodeTypeParameter, typeParameter);
			CONNECT_RULE_AST(NodeTypeParameters, typeParameters);
			CONNECT_RULE_AST(NodeClassDeclaration, classDeclaration);
			CONNECT_RULE_AST(NodeClassDeclarationExtend, classDeclarationExtend);
			CONNECT_RULE_AST(NodeClassDeclarationImplement, classDeclarationImplement);
            CONNECT_RULE_AST(NodeClassBody, classBody);
            CONNECT_RULE_AST(NodeClassBodyDeclaration, classBodyDeclaration);
			CONNECT_RULE_AST(NodeMemberDeclaration, memberDeclaration);
			CONNECT_RULE_AST(NodeMethodDeclaration, methodDeclaration);
			CONNECT_RULE_AST(NodeMethodBody, methodBody);
			CONNECT_RULE_AST(NodeGenericMethodDeclaration, genericMethodDeclaration);
			CONNECT_RULE_AST(NodeFieldDeclaration, fieldDeclaration);
			CONNECT_RULE_AST(NodeConstructorDeclaration, constructorDeclaration);
			CONNECT_RULE_AST(NodeConstructorBody, constructorBody);
			CONNECT_RULE_AST(NodeGenericConstructorDeclaration, genericConstructorDeclaration);
			CONNECT_RULE_AST(NodeEnumDeclaration, enumDeclaration);
			CONNECT_RULE_AST(NodeEnumConstants, enumConstants);
			CONNECT_RULE_AST(NodeEnumConstant, enumConstant);
			CONNECT_RULE_AST(NodeArguments, arguments);
            CONNECT_RULE_AST(NodeEnumBodyDeclarations, enumBodyDeclarations);
			CONNECT_RULE_AST(NodeInterfaceDeclaration, interfaceDeclaration);
			CONNECT_RULE_AST(NodeInterfaceBody, interfaceBody);
			CONNECT_RULE_AST(NodeInterfaceBodyDeclaration, interfaceBodyDeclaration);
			CONNECT_RULE_AST(NodeInterfaceMemberDeclaration, interfaceMemberDeclaration);
			CONNECT_RULE_AST(NodeConstDeclaration, constDeclaration);
			CONNECT_RULE_AST(NodeConstantDeclarator, constantDeclarator);
			CONNECT_RULE_AST(NodeVariableInitializer, variableInitializer);
			CONNECT_RULE_AST(NodeArrayInitializer, arrayInitializer);
			CONNECT_RULE_AST(NodeInterfaceMethodDeclaration, interfaceMethodDeclaration);
			CONNECT_RULE_AST(NodeGenericInterfaceMethodDeclaration, genericInterfaceMethodDeclaration);
			CONNECT_RULE_AST(NodeFormalParameter, formalParameter);
			CONNECT_RULE_AST(NodeFormalParameterList, formalParameterList);
			CONNECT_RULE_AST(NodeLastFormalParameter, lastFormalParameter);
			CONNECT_RULE_AST(NodeFormalParameters, formalParameters);
			//CONNECT_RULE_AST(NodeAnnotationTypeDeclaration, annotationTypeDeclaration);
			//CONNECT_RULE_AST(NodeAnnotationTypeBody, annotationTypeBody);
			//CONNECT_RULE_AST(NodeAnnotationTypeElementDeclaration, annotationTypeElementDeclaration);
			//CONNECT_RULE_AST(NodeAnnotationTypeElementRest, annotationTypeElementRest);
			//CONNECT_RULE_AST(NodeAnnotationMethodOrConstantRest, annotationMethodOrConstantRest);
			//CONNECT_RULE_AST(NodeAnnotationMethodRest, annotationMethodRest);
			//CONNECT_RULE_AST(NodeAnnotationConstantRest, annotationConstantRest);
			//CONNECT_RULE_AST(NodeAnnotationIdentifier, annotationIdentifier);
			//CONNECT_RULE_AST(NodeAnnotationName, annotationName);
			//CONNECT_RULE_AST(NodeAnnotationSequence, annotationSequence);
			//CONNECT_RULE_AST(NodeAnnotation, annotation);
			CONNECT_RULE_AST(NodeModifier, modifier);
			//CONNECT_RULE_AST(NodeDefaultValue, defaultValue);
			//CONNECT_RULE_AST(NodeElementValue, elementValue);
			//CONNECT_RULE_AST(NodeElementValueArrayInitializer, elementValueArrayInitializer);
			//CONNECT_RULE_AST(NodeElementValuePair, elementValuePair);
			//CONNECT_RULE_AST(NodeElementValuePairs, elementValuePairs);
			CONNECT_RULE_AST(NodeBlock, block);
            CONNECT_RULE_AST(NodeBlockStatement, blockStatement);
			CONNECT_RULE_AST(NodeLocalVariableDeclarationStatement, localVariableDeclarationStatement);
			CONNECT_RULE_AST(NodeLocalVariableDeclaration, localVariableDeclaration);
			CONNECT_RULE_AST(NodeVariableDeclaratorId, variableDeclaratorId);
			CONNECT_RULE_AST(NodeVariableDeclarator, variableDeclarator);
			CONNECT_RULE_AST(NodeVariableDeclarators, variableDeclarators);
			CONNECT_RULE_AST(NodeVariableModifier, variableModifier);
			CONNECT_RULE_AST(NodeType, type);
			CONNECT_RULE_AST(NodeTypeList, typeList);
			CONNECT_RULE_AST(NodeClassOrInterfaceType, classOrInterfaceType);
			CONNECT_RULE_AST(NodeClassOrInterfaceTypePart, classOrInterfaceTypePart);
			CONNECT_RULE_AST(NodeTypeArguments, typeArguments);
			CONNECT_RULE_AST(NodePrimitiveType, primitiveType);
			CONNECT_RULE_AST(NodeStatement, statement);
			CONNECT_RULE_AST(NodeAssertStatement, assertStatement);
			CONNECT_RULE_AST(NodeIfStatement, ifStatement);
			CONNECT_RULE_AST(NodeElseStatement, elseStatement);
			CONNECT_RULE_AST(NodeForStatement, forStatement);
			CONNECT_RULE_AST(NodeWhileStatement, whileStatement);
			CONNECT_RULE_AST(NodeDoStatement, doStatement);
			CONNECT_RULE_AST(NodeTryStatement, tryStatement);
			CONNECT_RULE_AST(NodeTryBlock, tryBlock);
			CONNECT_RULE_AST(NodeTryResSpec, tryResSpec);
			CONNECT_RULE_AST(NodeSwitchStatement, switchStatement);
			CONNECT_RULE_AST(NodeSynchronizedStatement, synchronizedStatement);
			CONNECT_RULE_AST(NodeReturnStatement, returnStatement);
			CONNECT_RULE_AST(NodeThrowStatement, throwStatement);
			CONNECT_RULE_AST(NodeBreakStatement, breakStatement);
			CONNECT_RULE_AST(NodeContinueStatement, continueStatement);
			CONNECT_RULE_AST(NodeParExpression, parExpression);
			CONNECT_RULE_AST(NodeForControl, forControl);
			CONNECT_RULE_AST(NodeForInit, forInit);
			CONNECT_RULE_AST(NodeForUpdate, forUpdate);
			CONNECT_RULE_AST(NodeEnhancedForControl, enhancedForControl);
			CONNECT_RULE_AST(NodeRegularForControl, regularForControl);
			CONNECT_RULE_AST(NodeCatchClause, catchClause);
			CONNECT_RULE_AST(NodeCatchType, catchType);
			CONNECT_RULE_AST(NodeFinallyBlock, finallyBlock);
			CONNECT_RULE_AST(NodeResourceSpecification, resourceSpecification);
			CONNECT_RULE_AST(NodeResources, resources);
			CONNECT_RULE_AST(NodeResource, resource);
			CONNECT_RULE_AST(NodeSwitchBlockStatementGroup, switchBlockStatementGroup);
			CONNECT_RULE_AST(NodeSwitchLabel, switchLabel);
			CONNECT_RULE_AST(NodeConstantExpression, constantExpression);
			CONNECT_RULE_AST(NodeEnumConstantName, enumConstantName);
			CONNECT_RULE_AST(NodeStatementExpression, statementExpression);
			CONNECT_RULE_AST(NodeQualifiedName, qualifiedName);
			CONNECT_RULE_AST(NodeQualifiedNameList, qualifiedNameList);
            CONNECT_RULE_AST(NodeExpression, expression);
            CONNECT_RULE_AST(NodeExpression2, expression2);
            CONNECT_RULE_AST(NodeExpression3, expression3);
            CONNECT_RULE_AST(NodeExpressionString, expressionString);
            CONNECT_RULE_AST(NodeExpressionString2, expressionString2);
            CONNECT_RULE_AST(NodeExpressionRound, expressionRound);
            CONNECT_RULE_AST(NodeExpressionBracket, expressionBracket);
            CONNECT_RULE_AST(NodeExpressionPostfix, expressionPostfix);
            CONNECT_RULE_AST(NodeExpressionList, expressionList);
            CONNECT_RULE_AST(NodeIdentifier, identifier);

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

            FOREACH (cr, compileResults)
            {
                if (!cr->errorMessage.empty())
                {
                    if (!errorMessage.empty())
                        errorMessage += "\n";
                    errorMessage += cr->file + ":\n" + cr->errorMessage;
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

        void ProcessParseResult(const vector<CompileResult>& compileResults, SoftTree& softTree)
        {
            // TODO: process java ast to soft tree
        }
	}
}

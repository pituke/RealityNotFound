
#include "Importer/Parsing/Java/JavaGrammar.h"
#include "Importer/Parsing/Java/JavaParser.h"
#include <fstream>
#include <sstream>
#include <QDirIterator>
#include <QTextStream>

// #define FOREACH(_it, _collection) for (auto _it = _collection.begin(); _it != _collection.end(); ++_it)

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
			/*CONNECT_RULE_AST(NodeAnnotationIdentifier, annotationIdentifier);
			CONNECT_RULE_AST(NodeAnnotationName, annotationName);
			CONNECT_RULE_AST(NodeAnnotationSequenceBefore, annotationSequenceBefore);
			CONNECT_RULE_AST(NodeAnnotationSequenceAfter, annotationSequenceAfter);
			CONNECT_RULE_AST(NodeAnnotationSequence, annotationSequence);
			CONNECT_RULE_AST(NodeAnnotation, annotation);*/
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
			CONNECT_RULE_AST(NodeTypeArray, typeArray);
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

		Modifier::ModifierEnum GetClassOrInterfaceModifier(const NodeClassOrInterfaceModifier* classOrInterfaceModifier)
		{
			string coim = classOrInterfaceModifier->classOrInterfaceModifierBasic->GetValue();
			QString upperCoim = QString::fromStdString(coim).toUpper();
			if (upperCoim == "PUBLIC") return Modifier::PUBLIC;
			else if (upperCoim == "PROTECTED") return Modifier::PROTECTED;
			else if (upperCoim == "PRIVATE") return Modifier::PRIVATE;
			else return Modifier::UNKNOWN;
		}

		string GetTypeArguments(const NodeTypeArguments* typeArguments)
		{
			string s;
			s += "<";
			s += typeArguments->typeArgumentsBefore->GetValue();
			if (typeArguments->typeArguments != nullptr)
				s += GetTypeArguments(typeArguments->typeArguments);
			s += typeArguments->typeArgumentsAfter->GetValue();
			s += ">";
			return s;
		}

		string GenTypeArray(uint count)
		{
			string s;
			for (uint i = 0; i < count; ++i)
				s += "[]";
			return s;
		}

		string GetType(const NodeType* type)
		{
			string s;
			if (type)
			{
				if (type->classOrInterfaceType != nullptr)
				{
					for (const NodeClassOrInterfaceTypePart* coitp : type->classOrInterfaceType->classOrInterfaceTypePart)
					{
						if (!s.empty()) s += '.';
						s += coitp->identifier->GetValue();
						if (coitp->typeArguments != nullptr)
							s += GetTypeArguments(coitp->typeArguments);
					}
				}
				else if (type->primitiveType != nullptr)
					s += type->primitiveType->GetValue();
				else
					return s;
				s += GenTypeArray(type->typeArray.GetCount());
			}
			else
				s = "void";
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

		Modifier::ModifierEnum GetModifier(const NodeModifier* modifier)
		{
			string coim = modifier->GetValue();
			QString upperCoim = QString::fromStdString(coim).toUpper();
			if (upperCoim == "PUBLIC") return Modifier::PUBLIC;
			else if (upperCoim == "PROTECTED") return Modifier::PROTECTED;
			else if (upperCoim == "PRIVATE") return Modifier::PRIVATE;
			else return Modifier::UNKNOWN;
		}

		vector<Parameter> GetParametersFromFormalParameterList(const NodeFormalParameterList* fpl)
		{
			vector<Parameter> parameters;
			for (const NodeFormalParameter* fp : fpl->formalParameter)
			{
				Parameter parameter;
				parameter.type = GetType(fp->type) + GenTypeArray(fp->variableDeclaratorId->typeArray.GetCount());
				parameter.name = fp->variableDeclaratorId->identifier->GetValue();
				parameters.push_back(parameter);
			}
			return parameters;
		}

		Method GetMethodFromMethodDeclaration(const NodeMethodDeclaration* methodDeclaration, Modifier::ModifierEnum modifier)
		{
			Method method;
			method.modifier = modifier;
			method.returnType = GetType(methodDeclaration->type);
			method.name = methodDeclaration->identifier->GetValue();
			if (methodDeclaration->formalParameters->formalParameterList != nullptr)
				method.parameters = GetParametersFromFormalParameterList(methodDeclaration->formalParameters->formalParameterList);
			return method;
		}

		Attribute GetAttributeFromFieldDeclaration(const NodeFieldDeclaration* fieldDeclaration)
		{
			Attribute attribute;
			auto type = GetType(fieldDeclaration->type);
			for (const NodeVariableDeclarator* vdi : fieldDeclaration->variableDeclarators->variableDeclarator)
			{
				auto typeArray = GenTypeArray(vdi->variableDeclaratorId->typeArray.GetCount());
				attribute.type = type + typeArray;
				attribute.name = vdi->variableDeclaratorId->identifier->GetValue();
			}
			return attribute;
		}

		Method GetConstructorFromConstructorDeclaration(const NodeConstructorDeclaration* constructorDeclaration, Modifier::ModifierEnum modifier)
		{
			Method method;
			method.modifier = modifier;
			method.name = constructorDeclaration->identifier->GetValue();
			if (constructorDeclaration->formalParameters->formalParameterList != nullptr)
				method.parameters = GetParametersFromFormalParameterList(constructorDeclaration->formalParameters->formalParameterList);
			return method;
		}

        void ProcessParseResult(const vector<CompileResult>& compileResults, SoftTree& softTree)
        {
            QMap<string, Namespace> javaPackages;

			for (const auto& cr : compileResults)
            {
				const SourceFileCompilationUnit* javaAstRoot = cr.astRoot;

				string packageName;
				if (javaAstRoot->packageDeclaration.Empty())
					packageName = GetQualifiedName(javaAstRoot->packageDeclaration.First()->qualifiedName);

				for (const NodeTypeDeclaration* typeDecl : javaAstRoot->typeDeclaration)
				{
					Modifier::ModifierEnum modifierForTypeDecl;
					for (const NodeClassOrInterfaceModifier* coim : typeDecl->classOrInterfaceModifier)
					{
						modifierForTypeDecl = GetClassOrInterfaceModifier(coim);
						if (modifierForTypeDecl != Modifier::UNKNOWN)
							break;
					}
					if (typeDecl->classDeclaration != nullptr)
					{
						Class class1;
						class1.modifier = modifierForTypeDecl;
						class1.name = typeDecl->classDeclaration->identifier->GetValue();
						for (const NodeClassBodyDeclaration* cbd : typeDecl->classDeclaration->classBody->classBodyDeclaration)
						{
							if (cbd->memberDeclaration == nullptr)
								continue;

							Modifier::ModifierEnum modifierForClassMember;
							for (const NodeModifier* m : cbd->memberDeclaration->modifier)
							{
								modifierForClassMember = GetModifier(m);
								if (modifierForClassMember != Modifier::UNKNOWN)
									break;
							}

							if (cbd->memberDeclaration->methodDeclaration != nullptr)
							{
								class1.methods.push_back(GetMethodFromMethodDeclaration(cbd->memberDeclaration->methodDeclaration, modifierForClassMember));
							}
							else if (cbd->memberDeclaration->genericMethodDeclaration != nullptr)
							{
								class1.methods.push_back(GetMethodFromMethodDeclaration(cbd->memberDeclaration->genericMethodDeclaration->methodDeclaration, modifierForClassMember));
							}
							else if (cbd->memberDeclaration->fieldDeclaration != nullptr)
							{
								class1.attributes.push_back(GetAttributeFromFieldDeclaration(cbd->memberDeclaration->fieldDeclaration));
							}
							else if (cbd->memberDeclaration->constructorDeclaration != nullptr)
							{
								class1.methods.push_back(GetConstructorFromConstructorDeclaration(cbd->memberDeclaration->constructorDeclaration, modifierForClassMember));
							}
							else if (cbd->memberDeclaration->genericConstructorDeclaration != nullptr)
							{
								class1.methods.push_back(GetConstructorFromConstructorDeclaration(cbd->memberDeclaration->genericConstructorDeclaration->constructorDeclaration, modifierForClassMember));
							}
						}
						javaPackages[packageName].classes.push_back(class1);
					}
					else if (typeDecl->enumDeclaration != nullptr)
					{
						Enum enum1;
						enum1.modifier = modifierForTypeDecl;
						enum1.name = typeDecl->enumDeclaration->identifier->GetValue();
						if (typeDecl->enumDeclaration->enumConstants != nullptr)
						{
							for (const NodeEnumConstant* ec : typeDecl->enumDeclaration->enumConstants->enumConstant)
							{
								EnumConstant enumConstant;
								enumConstant.name = ec->identifier->GetValue();
								enum1.enums.push_back(enumConstant);
							}
						}
						for (const NodeClassBodyDeclaration* cbd : typeDecl->enumDeclaration->enumBodyDeclarations->classBodyDeclarations)
						{
							if (cbd->memberDeclaration == nullptr)
								continue;

							Modifier::ModifierEnum modifierForClassMember;
							for (const NodeModifier* m : cbd->memberDeclaration->modifier)
							{
								modifierForClassMember = GetModifier(m);
								if (modifierForClassMember != Modifier::UNKNOWN)
									break;
							}

							if (cbd->memberDeclaration->methodDeclaration != nullptr)
							{
								enum1.methods.push_back(GetMethodFromMethodDeclaration(cbd->memberDeclaration->methodDeclaration, modifierForClassMember));
							}
							else if (cbd->memberDeclaration->genericMethodDeclaration != nullptr)
							{
								enum1.methods.push_back(GetMethodFromMethodDeclaration(cbd->memberDeclaration->genericMethodDeclaration->methodDeclaration, modifierForClassMember));
							}
							else if (cbd->memberDeclaration->fieldDeclaration != nullptr)
							{
								enum1.attributes.push_back(GetAttributeFromFieldDeclaration(cbd->memberDeclaration->fieldDeclaration));
							}
							else if (cbd->memberDeclaration->constructorDeclaration != nullptr)
							{
								enum1.methods.push_back(GetConstructorFromConstructorDeclaration(cbd->memberDeclaration->constructorDeclaration, modifierForClassMember));
							}
							else if (cbd->memberDeclaration->genericConstructorDeclaration != nullptr)
							{
								enum1.methods.push_back(GetConstructorFromConstructorDeclaration(cbd->memberDeclaration->genericConstructorDeclaration->constructorDeclaration, modifierForClassMember));
							}
						}
						javaPackages[packageName].enums.push_back(enum1);
					}
					else if (typeDecl->interfaceDeclaration != nullptr)
					{
					}
					else if (typeDecl->annotationTypeDeclaration != nullptr)
					{
					}
				}
            }

			for (const auto& javaPackage : javaPackages)
                softTree.namespaces.push_back(javaPackage);
        }
	}
}

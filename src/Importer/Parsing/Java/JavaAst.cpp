
#include "Importer/Parsing/Java/JavaAst.h"
#include "Importer/Parsing/Java/StringHelper.h"
#include <string>
#include <algorithm>
#include <list>

#define FOREACH(_it, _collection) for (auto _it = _collection.begin(); _it != _collection.end(); ++_it)

namespace Importer
{
	namespace Parsing
	{
		// SourceFileCompilationUnit
        astList(NodePackageDeclaration) SourceFileCompilationUnit::GetPackageList() const
		{
            return packageDeclarationList;
		}

        string SourceFileCompilationUnit::GetPackageName() const
        {
            return packageDeclarationList.Empty() ? "" : packageDeclarationList.At(0)->GetPackageName();
        }

		astList(NodeImportDeclaration) SourceFileCompilationUnit::GetImportList() const
		{
			return importDeclarationList;
		}

		astList(NodeTypeDeclaration) SourceFileCompilationUnit::GetTypeDeclarationList() const
		{
			return typeDeclarationList;
		}

		vector<const NodeClassDeclaration*> SourceFileCompilationUnit::GetClassList() const
		{
            vector<const NodeClassDeclaration*> typeList;
            FOREACH (typeDecl, typeDeclarationList)
            {
                if ((*typeDecl)->GetTypeCategory() == NodeTypeDeclaration::CLASS)
                    typeList.push_back((*typeDecl)->GetClass());
			}
			return typeList;
		}

		vector<const NodeEnumDeclaration*> SourceFileCompilationUnit::GetEnumList() const
		{
			vector<const NodeEnumDeclaration*> typeList;
            FOREACH (typeDecl, typeDeclarationList)
			{
                if ((*typeDecl)->GetTypeCategory() == NodeTypeDeclaration::ENUM)
                    typeList.push_back((*typeDecl)->GetEnum());
			}
			return typeList;
		}

		vector<const NodeInterfaceDeclaration*> SourceFileCompilationUnit::GetInterfaceList() const
		{
			vector<const NodeInterfaceDeclaration*> typeList;
            FOREACH (typeDecl, typeDeclarationList)
			{
                if ((*typeDecl)->GetTypeCategory() == NodeTypeDeclaration::INTERFACE)
                    typeList.push_back((*typeDecl)->GetInterface());
			}
			return typeList;
		}

		// NodePackageDeclaration
		string NodePackageDeclaration::GetPackageName() const
		{
			return qualifiedName->GetValue();
		}

		// NodeImportDeclaration
		string NodeImportDeclaration::GetImportPackageName() const
		{
			return qualifiedName->GetValue();
		}

		// NodeTypeDeclaration
		vector<string> NodeTypeDeclaration::GetModifiersNames()
		{
			vector<string> modifiers;
            FOREACH(coim, classOrInterfaceModifierList)
                modifiers.push_back((*coim)->GetValue());
			return modifiers;
		}

		NodeTypeDeclaration::NodeTypeDeclarationCategory NodeTypeDeclaration::GetTypeCategory() const
		{
			if (classDeclaration != NULL)
				return CLASS;
			else if (enumDeclaration != NULL)
				return ENUM;
			else if (interfaceDeclaration != NULL)
				return INTERFACE;
			else
				return UNKNOWN;
		}

		astVal(NodeClassDeclaration) NodeTypeDeclaration::GetClass() const
		{
			return classDeclaration;
		}

		astVal(NodeEnumDeclaration) NodeTypeDeclaration::GetEnum() const
		{
			return enumDeclaration;
		}

		astVal(NodeInterfaceDeclaration) NodeTypeDeclaration::GetInterface() const
		{
			return interfaceDeclaration;
		}

		// NodeClassOrInterfaceModifier

		// NodeTypeBound

		// NodeTypeParameter

		// NodeTypeParameters

		// NodeClassDeclaration
		string NodeClassDeclaration::GetClassName() const
		{
			return identifier->GetValue();
		}

		astVal(NodeClassDeclarationExtend) NodeClassDeclaration::GetClassExtend() const
		{
			return classDeclarationExtend;
		}

		astVal(NodeClassDeclarationImplement) NodeClassDeclaration::GetClassImplement() const
		{
			return classDeclarationImplement;
		}

		astVal(NodeClassBody) NodeClassDeclaration::GetClassBody() const
		{
			return classBody;
		}

		// NodeClassDeclarationExtend
		string NodeClassDeclarationExtend::GetExtendsTypeName() const
		{
			return type->GetTypeName();
		}

		// NodeClassDeclarationImplement
		vector<string> NodeClassDeclarationImplement::GetImplementsTypeNames() const
		{
			return typeList->GetTypeNames();
		}

		// NodeClassBody
		vector<const NodeBlock*> NodeClassBody::GetBlockList() const
		{
			/*vector<const NodeBlock*> typeList;
			for (auto typeDecl : classBodyDeclarationList->g)
			{
			typeList.push_back(typeDecl->_method);
			}
			return typeList;*/
			return vector<const NodeBlock*>();
		}

        vector<const NodeMemberDeclaration*> NodeClassBody::GetMembers() const
        {
            vector<const NodeMemberDeclaration*> members;
            FOREACH (member, classBodyDeclarationList)
            {
                auto innerMember = (*member)->GetMember();
                if (innerMember != NULL)
                    members.push_back(innerMember);
            }
            return members;
        }

		vector<const NodeMethodDeclaration*> NodeClassBody::GetMethodList() const
		{
			vector<const NodeMethodDeclaration*> memberList;
            FOREACH(typeDecl, classBodyDeclarationList)
			{
                auto member = (*typeDecl)->GetMember();
				if (member != NULL)
				{
					if (member->GetMemberCategory() == NodeMemberDeclaration::METHOD)
						memberList.push_back(member->GetMethod());
				}
			}
			return memberList;
		}

		vector<const NodeGenericMethodDeclaration*> NodeClassBody::GetGenericMethodList() const
		{
			vector<const NodeGenericMethodDeclaration*> memberList;
            FOREACH(typeDecl, classBodyDeclarationList)
			{
                auto member = (*typeDecl)->GetMember();
				if (member != NULL)
				{
					if (member->GetMemberCategory() == NodeMemberDeclaration::GENERIC_METHOD)
						memberList.push_back(member->GetGenericMethod());
				}
			}
			return memberList;
		}

		vector<const NodeFieldDeclaration*> NodeClassBody::GetFieldList() const
		{
			vector<const NodeFieldDeclaration*> memberList;
            FOREACH (typeDecl, classBodyDeclarationList)
			{
                auto member = (*typeDecl)->GetMember();
				if (member != NULL)
				{
					if (member->GetMemberCategory() == NodeMemberDeclaration::FIELD)
						memberList.push_back(member->GetField());
				}
			}
			return memberList;
		}

		vector<const NodeConstructorDeclaration*> NodeClassBody::GetConstructorList() const
		{
			vector<const NodeConstructorDeclaration*> memberList;
            FOREACH (typeDecl, classBodyDeclarationList)
			{
                auto member = (*typeDecl)->GetMember();
				if (member != NULL)
				{
					if (member->GetMemberCategory() == NodeMemberDeclaration::CONSTRUCTOR)
						memberList.push_back(member->GetConstructor());
				}
			}
			return memberList;
		}

		vector<const NodeGenericConstructorDeclaration*> NodeClassBody::GetGenericConstructorList() const
		{
			vector<const NodeGenericConstructorDeclaration*> memberList;
            FOREACH(typeDecl, classBodyDeclarationList)
			{
                auto member = (*typeDecl)->GetMember();
				if (member != NULL)
				{
					if (member->GetMemberCategory() == NodeMemberDeclaration::GENERIC_CONSTRUCTOR)
						memberList.push_back(member->GetGenericConstructor());
				}
			}
			return memberList;
		}

		vector<const NodeInterfaceDeclaration*> NodeClassBody::GetInterfaceList() const
		{
			vector<const NodeInterfaceDeclaration*> memberList;
            FOREACH(typeDecl, classBodyDeclarationList)
			{
                auto member = (*typeDecl)->GetMember();
				if (member != NULL)
				{
					if (member->GetMemberCategory() == NodeMemberDeclaration::INTERFACE)
						memberList.push_back(member->GetInterface());
				}
			}
			return memberList;
		}

		vector<const NodeClassDeclaration*> NodeClassBody::GetClassList() const
		{
			vector<const NodeClassDeclaration*> memberList;
            FOREACH(typeDecl, classBodyDeclarationList)
			{
                auto member = (*typeDecl)->GetMember();
				if (member != NULL)
				{
					if (member->GetMemberCategory() == NodeMemberDeclaration::CLASS)
						memberList.push_back(member->GetClass());
				}
			}
			return memberList;
		}

		vector<const NodeEnumDeclaration*> NodeClassBody::GetEnumList() const
		{
			vector<const NodeEnumDeclaration*> memberList;
            FOREACH(typeDecl, classBodyDeclarationList)
			{
                auto member = (*typeDecl)->GetMember();
				if (member != NULL)
				{
					if (member->GetMemberCategory() == NodeMemberDeclaration::ENUM)
						memberList.push_back(member->GetEnum());
				}
			}
			return memberList;
		}

		// NodeClassBodyDeclaration
		astVal(NodeBlock) NodeClassBodyDeclaration::GetBlock() const
		{
			return block;
		}

		astVal(NodeMemberDeclaration) NodeClassBodyDeclaration::GetMember() const
		{
			return memberDeclaration;
		}

		// NodeMemberDeclaration
		vector<string> NodeMemberDeclaration::GetModifiersNames()
		{
			vector<string> modifiers;
            FOREACH(coim, modifierList)
                modifiers.push_back((*coim)->GetValue());
			return modifiers;
		}

		NodeMemberDeclaration::NodeMemberDeclarationCategory NodeMemberDeclaration::GetMemberCategory() const
		{
			if (methodDeclaration != NULL)
				return METHOD;
			else if (genericMethodDeclaration != NULL)
				return GENERIC_METHOD;
			else if (fieldDeclaration != NULL)
				return FIELD;
			else if (constructorDeclaration != NULL)
				return CONSTRUCTOR;
			else if (genericConstructorDeclaration != NULL)
				return GENERIC_CONSTRUCTOR;
			else if (interfaceDeclaration != NULL)
				return INTERFACE;
			else if (classDeclaration != NULL)
				return CLASS;
			else if (enumDeclaration != NULL)
				return ENUM;
			else
				return UNKNOWN;
		}

		astVal(NodeMethodDeclaration) NodeMemberDeclaration::GetMethod() const
		{
			return methodDeclaration;
		}

		astVal(NodeGenericMethodDeclaration) NodeMemberDeclaration::GetGenericMethod() const
		{
			return genericMethodDeclaration;
		}

		astVal(NodeFieldDeclaration) NodeMemberDeclaration::GetField() const
		{
			return fieldDeclaration;
		}

		astVal(NodeConstructorDeclaration) NodeMemberDeclaration::GetConstructor() const
		{
			return constructorDeclaration;
		}

		astVal(NodeGenericConstructorDeclaration) NodeMemberDeclaration::GetGenericConstructor() const
		{
			return genericConstructorDeclaration;
		}

		astVal(NodeInterfaceDeclaration) NodeMemberDeclaration::GetInterface() const
		{
			return interfaceDeclaration;
		}

		astVal(NodeClassDeclaration) NodeMemberDeclaration::GetClass() const
		{
			return classDeclaration;
		}

		astVal(NodeEnumDeclaration) NodeMemberDeclaration::GetEnum() const
		{
			return enumDeclaration;
		}

		// NodeMethodDeclaration
		string NodeMethodDeclaration::GetReturnTypeName() const
		{
			return type != NULL ? type->GetTypeName() : string();
		}

		string NodeMethodDeclaration::GetMethodName() const
		{
			return identifier->GetValue();
		}

		// NodeMethodBody

		// NodeGenericMethodDeclaration
		string NodeGenericMethodDeclaration::GetGenericMethodName() const
		{
			return methodDeclaration->GetMethodName();
		}

		// NodeFieldDeclaration
        string NodeFieldDeclaration::GetFieldName() const
        {
            return "FieldName";
        }

		// NodeConstructorDeclaration
		string NodeConstructorDeclaration::GetConstructorName() const
		{
			return identifier->GetValue();
		}

		// NodeConstructorBody

		// NodeGenericConstructorDeclaration
		string NodeGenericConstructorDeclaration::GetGenericConstructorName() const
		{
			return constructorDeclaration->GetConstructorName();
		}

		// NodeEnumDeclaration
		string NodeEnumDeclaration::GetEnumName() const
		{
			return identifier->GetValue();
		}

		// NodeEnumConstants

		// NodeEnumConstant

		// NodeArguments

		// NodeEnumBodyDeclarations

		// NodeInterfaceDeclaration
		string NodeInterfaceDeclaration::GetInterfaceName() const
		{
			return identifier->GetValue();
		}

		// NodeInterfaceBody

		// NodeInterfaceBodyDeclaration

		// NodeInterfaceMemberDeclaration

		// NodeConstDeclaration

		// NodeConstantDeclarator

		// NodeVariableInitializer

		// NodeArrayInitializer

		// NodeInterfaceMethodDeclaration

		// NodeGenericInterfaceMethodDeclaration

		// NodeFormalParameter

		// NodeFormalParameterList

		// NodeLastFormalParameter

		// NodeFormalParameters

		// NodeAnnotationTypeDeclaration

		// NodeAnnotationTypeBody

		// NodeAnnotationTypeElementDeclaration

		// NodeAnnotationTypeElementRest

		// NodeAnnotationMethodOrConstantRest

		// NodeAnnotationMethodRest

		// NodeAnnotationConstantRest

		// NodeAnnotationIdentifier

		// NodeAnnotationName

		// NodeAnnotationSequence

		// NodeAnnotation

		// NodeModifier

		// NodeDefaultValue

		// NodeElementValue

		// NodeElementValueArrayInitializer

		// NodeElementValuePair

		// NodeElementValuePairs

		// NodeBlock

		// NodeBlockStatement

		// NodeLocalVariableDeclarationStatement

		// NodeLocalVariableDeclaration

		// NodeVariableDeclaratorId
		string NodeVariableDeclaratorId::GetVariableName() const
		{
			return identifier->GetValue();
		}

		// NodeVariableDeclarator
		astVal(NodeVariableDeclaratorId) NodeVariableDeclarator::GetVariableDeclaratorId() const
		{
			return variableDeclaratorId;
		}

		// NodeVariableDeclarators

		// NodeVariableModifier

		// NodeType
		string NodeType::GetTypeName() const
		{
			if (primitiveType != NULL)
				return primitiveType->GetValue();
			else if (classOrInterfaceType != NULL)
				return classOrInterfaceType->GetTypeName();
			else
				return string();
		}

		// NodeTypeList
		vector<string> NodeTypeList::GetTypeNames() const
		{
			vector<string> typenames;
            FOREACH(nt, typeList)
                typenames.push_back((*nt)->GetTypeName());
			return typenames;
		}

		// NodeClassOrInterfaceType
		string NodeClassOrInterfaceType::GetTypeName() const
		{
			vector<string> typenameParts;
            FOREACH(tnp, classOrInterfaceTypePartList)
                typenameParts.push_back((*tnp)->GetTypeNamePart());
			return StringHelper::Join(typenameParts, ".");
		}

		// NodeClassOrInterfaceTypePart
		string NodeClassOrInterfaceTypePart::GetTypeNamePart() const
		{
			return identifier->GetValue() + (typeArguments != NULL ? typeArguments->GetValue() : "");
		}

		// NodeTypeArguments

		// NodePrimitiveType

		// NodeStatement

		// NodeAssertStatement

		// NodeIfStatement

		// NodeForStatement

		// NodeWhileStatement

		// NodeDoStatement

		// NodeTryStatement

		// NodeTryBlock

		// NodeTryResSpec

		// NodeSwitchStatement

		// NodeSynchronizedStatement

		// NodeReturnStatement

		// NodeThrowStatement

		// NodeBreakStatement

		// NodeContinueStatement

		// NodeParExpression

		// NodeForControl

		// NodeForInit

		// NodeForUpdate

		// NodeEnhancedForControl

		// NodeCatchClause

		// NodeCatchType

		// NodeFinallyBlock

		// NodeResourceSpecification

		// Noderesources

		// NodeResource

		// NodeSwitchBlockStatementGroup

		// NodeSwitchLabel

		// NodeConstantExpression

		// NodeEnumConstantName

		// NodeStatementExpression

		// NodeQualifiedName
		string NodeQualifiedName::GetValue() const
		{
			vector<string> names;
            FOREACH(n, identifierList)
                names.push_back((*n)->GetValue());
			return StringHelper::Join(names, ".");
		}

		// NodeQualifiedNameList

		// NodeExpression

		// NodeExpressionSequence

		// NodeExpressionPostfix

		// NodeExpressionInner

		// NodeExpressionSequenceInner

		// NodeExpressionList

		// NodeIdentifier
	}
}

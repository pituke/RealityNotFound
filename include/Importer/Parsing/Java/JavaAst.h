#pragma once

#include "ParserlibBase.h"

namespace Importer
{
	namespace Parsing
	{
		#pragma region forward declaration

        struct SourceFileCompilationUnit;
        struct NodePackageDeclaration;
        struct NodeImportDeclaration;
        struct NodeTypeDeclaration;
        struct NodeClassOrInterfaceModifier;
        struct NodeTypeBound;
        struct NodeTypeParameter;
        struct NodeTypeParameters;
        struct NodeClassDeclaration;
        struct NodeClassDeclarationExtend;
        struct NodeClassDeclarationImplement;
        struct NodeClassBody;
        struct NodeClassBodyDeclaration;
        struct NodeMemberDeclaration;
        struct NodeMethodDeclaration;
        struct NodeMethodBody;
        struct NodeGenericMethodDeclaration;
        struct NodeFieldDeclaration;
        struct NodeConstructorDeclaration;
        struct NodeConstructorBody;
        struct NodeGenericConstructorDeclaration;
        struct NodeEnumDeclaration;
        struct NodeEnumConstants;
        struct NodeEnumConstant;
        struct NodeArguments;
        struct NodeEnumBodyDeclarations;
        struct NodeInterfaceDeclaration;
        struct NodeInterfaceBody;
        struct NodeInterfaceBodyDeclaration;
        struct NodeInterfaceMemberDeclaration;
        struct NodeConstDeclaration;
        struct NodeConstantDeclarator;
        struct NodeVariableInitializer;
        struct NodeArrayInitializer;
        struct NodeInterfaceMethodDeclaration;
        struct NodeGenericInterfaceMethodDeclaration;
        struct NodeFormalParameter;
        struct NodeFormalParameterList;
        struct NodeLastFormalParameter;
        struct NodeFormalParameters;
        struct NodeAnnotationTypeDeclaration;
        struct NodeAnnotationTypeBody;
        struct NodeAnnotationTypeElementDeclaration;
        struct NodeAnnotationTypeElementRest;
        struct NodeAnnotationMethodOrConstantRest;
        struct NodeAnnotationMethodRest;
        struct NodeAnnotationConstantRest;
        struct NodeAnnotationIdentifier;
        struct NodeAnnotationName;
        struct NodeAnnotationSequence;
        struct NodeAnnotation;
        struct NodeModifier;
        struct NodeDefaultValue;
        struct NodeElementValue;
        struct NodeElementValueArrayInitializer;
        struct NodeElementValuePair;
        struct NodeElementValuePairs;
        struct NodeBlock;
        struct NodeBlockStatement;
        struct NodeLocalVariableDeclarationStatement;
        struct NodeLocalVariableDeclaration;
        struct NodeVariableDeclaratorId;
        struct NodeVariableDeclarator;
        struct NodeVariableDeclarators;
        struct NodeVariableModifier;
        struct NodeType;
        struct NodeTypeList;
        struct NodeClassOrInterfaceType;
        struct NodeClassOrInterfaceTypePart;
        struct NodeTypeArguments;
        struct NodePrimitiveType;
        struct NodeStatement;
        struct NodeAssertStatement;
        struct NodeIfStatement;
        struct NodeElseStatement;
        struct NodeForStatement;
        struct NodeWhileStatement;
        struct NodeDoStatement;
        struct NodeTryStatement;
        struct NodeTryBlock;
        struct NodeTryResSpec;
        struct NodeSwitchStatement;
        struct NodeSynchronizedStatement;
        struct NodeReturnStatement;
        struct NodeThrowStatement;
        struct NodeBreakStatement;
        struct NodeContinueStatement;
        struct NodeParExpression;
        struct NodeForControl;
        struct NodeForInit;
        struct NodeForUpdate;
        struct NodeEnhancedForControl;
        struct NodeRegularForControl;
        struct NodeCatchClause;
        struct NodeCatchType;
        struct NodeFinallyBlock;
        struct NodeResourceSpecification;
        struct NodeResources;
        struct NodeResource;
        struct NodeSwitchBlockStatementGroup;
        struct NodeSwitchLabel;
        struct NodeConstantExpression;
        struct NodeEnumConstantName;
        struct NodeStatementExpression;
        struct NodeQualifiedName;
        struct NodeQualifiedNameList;
        struct NodeExpression;
        struct NodeExpression2;
        struct NodeExpression3;
        struct NodeExpressionString;
        struct NodeExpressionString2;
        struct NodeExpressionRound;
        struct NodeExpressionBracket;
        struct NodeExpressionPostfix;
        struct NodeExpressionList;
        struct NodeIdentifier;

		#pragma endregion

        #define astVal(type) const type*
        #define astList(type) const AstList<type>&

        struct SourceFileCompilationUnit : public NodeContainer
        {
            AstList<NodePackageDeclaration> packageDeclarationList;
            AstList<NodeImportDeclaration> importDeclarationList;
            AstList<NodeTypeDeclaration> typeDeclarationList;

            astList(NodePackageDeclaration) GetPackageList() const;
            string GetPackageName() const;
            astList(NodeImportDeclaration) GetImportList() const;
            astList(NodeTypeDeclaration) GetTypeDeclarationList() const;
            vector<const NodeClassDeclaration*> GetClassList() const;
            vector<const NodeEnumDeclaration*> GetEnumList() const;
            vector<const NodeInterfaceDeclaration*> GetInterfaceList() const;
        };

        struct NodePackageDeclaration : public NodeContainer
        {
            //AstList<NodeAnnotation> annotationList;
            AstValue<NodeQualifiedName> qualifiedName;

            string GetPackageName() const;
        };

        struct NodeImportDeclaration : public NodeContainer
        {
            AstValue<NodeQualifiedName> qualifiedName;

            string GetImportPackageName() const;
        };

        struct NodeTypeDeclaration : public NodeContainer
        {
            enum NodeTypeDeclarationCategory { UNKNOWN, CLASS, ENUM, INTERFACE, ANNOTATION };

            AstList<NodeClassOrInterfaceModifier> classOrInterfaceModifierList;
            AstValue<NodeClassDeclaration, true> classDeclaration;
            AstValue<NodeEnumDeclaration, true> enumDeclaration;
            AstValue<NodeInterfaceDeclaration, true> interfaceDeclaration;
            //AstValue<NodeAnnotationTypeDeclaration, true> annotationTypeDeclaration;

            vector<string> GetModifiersNames();
            NodeTypeDeclarationCategory GetTypeCategory() const;
            astVal(NodeClassDeclaration) GetClass() const;
            astVal(NodeEnumDeclaration) GetEnum() const;
            astVal(NodeInterfaceDeclaration) GetInterface() const;
        };

        struct NodeClassOrInterfaceModifier : public NodeTerm
        {
            // change NodeTerm to NodeContainer
            //AstValue<NodeAnnotation> anntotation;
        };

        struct NodeTypeBound : public NodeContainer
        {
            AstList<NodeType> typeList;
        };

        struct NodeTypeParameter : public NodeContainer
        {
            AstValue<NodeIdentifier> identifier;
            AstValue<NodeTypeBound, true> typeBound;
        };

        struct NodeTypeParameters : public NodeContainer
        {
            AstList<NodeTypeParameter> typeParameterList;
        };

        struct NodeClassDeclaration : public NodeContainer
        {
            AstValue<NodeIdentifier> identifier;
            AstValue<NodeTypeParameters, true> typeParameters;
            AstValue<NodeClassDeclarationExtend, true> classDeclarationExtend;
            AstValue<NodeClassDeclarationImplement, true> classDeclarationImplement;
            AstValue<NodeClassBody> classBody;

            string GetClassName() const;
            astVal(NodeClassDeclarationExtend) GetClassExtend() const;
            astVal(NodeClassDeclarationImplement) GetClassImplement() const;
            astVal(NodeClassBody) GetClassBody() const;
        };

        struct NodeClassDeclarationExtend : public NodeContainer
        {
            AstValue<NodeType> type;

            string GetExtendsTypeName() const;
        };

        struct NodeClassDeclarationImplement : public NodeContainer
        {
            AstValue<NodeTypeList> typeList;

            vector<string> GetImplementsTypeNames() const;
        };

        struct NodeClassBody : public NodeContainer
        {
            AstList<NodeClassBodyDeclaration> classBodyDeclarationList;

            vector<const NodeBlock*> GetBlockList() const;
            vector<const NodeMemberDeclaration*> GetMembers() const;
            vector<const NodeMethodDeclaration*> GetMethodList() const;
            vector<const NodeGenericMethodDeclaration*> GetGenericMethodList() const;
            vector<const NodeFieldDeclaration*> GetFieldList() const;
            vector<const NodeConstructorDeclaration*> GetConstructorList() const;
            vector<const NodeGenericConstructorDeclaration*> GetGenericConstructorList() const;
            vector<const NodeInterfaceDeclaration*> GetInterfaceList() const;
            vector<const NodeClassDeclaration*> GetClassList() const;
            vector<const NodeEnumDeclaration*> GetEnumList() const;
        };

        struct NodeClassBodyDeclaration : public NodeContainer
        {
            AstValue<NodeBlock, true> block;
            AstValue<NodeMemberDeclaration, true> memberDeclaration;

            astVal(NodeBlock) GetBlock() const;
            astVal(NodeMemberDeclaration) GetMember() const;
        };

        struct NodeMemberDeclaration : public NodeContainer
        {
            enum NodeMemberDeclarationCategory { UNKNOWN, METHOD, GENERIC_METHOD, FIELD, CONSTRUCTOR, GENERIC_CONSTRUCTOR, INTERFACE, ANNOTATION, CLASS, ENUM };

            AstList<NodeModifier> modifierList;
            AstValue<NodeMethodDeclaration, true> methodDeclaration;
            AstValue<NodeGenericMethodDeclaration, true> genericMethodDeclaration;
            AstValue<NodeFieldDeclaration, true> fieldDeclaration;
            AstValue<NodeConstructorDeclaration, true> constructorDeclaration;
            AstValue<NodeGenericConstructorDeclaration, true> genericConstructorDeclaration;
            AstValue<NodeInterfaceDeclaration, true> interfaceDeclaration;
            //AstValue<NodeAnnotationTypeDeclaration, true> annotationTypeDeclaration;
            AstValue<NodeClassDeclaration, true> classDeclaration;
            AstValue<NodeEnumDeclaration, true> enumDeclaration;

            vector<string> GetModifiersNames();
            NodeMemberDeclarationCategory GetMemberCategory() const;
            astVal(NodeMethodDeclaration) GetMethod() const;
            astVal(NodeGenericMethodDeclaration) GetGenericMethod() const;
            astVal(NodeFieldDeclaration) GetField() const;
            astVal(NodeConstructorDeclaration) GetConstructor() const;
            astVal(NodeGenericConstructorDeclaration) GetGenericConstructor() const;
            astVal(NodeInterfaceDeclaration) GetInterface() const;
            astVal(NodeClassDeclaration) GetClass() const;
            astVal(NodeEnumDeclaration) GetEnum() const;
        };

        struct NodeMethodDeclaration : public NodeContainer
        {
            AstValue<NodeType, true> type;
            AstValue<NodeIdentifier> identifier;
            AstValue<NodeFormalParameters> formalParameters;
            AstValue<NodeQualifiedNameList, true> qualifiedNameList;
            AstValue<NodeMethodBody, true> methodBody;

            string GetReturnTypeName() const;
            string GetMethodName() const;
        };

        struct NodeMethodBody : public NodeContainer
        {
            AstValue<NodeBlock> block;
        };

        struct NodeGenericMethodDeclaration : public NodeContainer
        {
            AstValue<NodeTypeParameters> typeParameters;
            AstValue<NodeMethodDeclaration> methodDeclaration;

            string GetGenericMethodName() const;
        };

        struct NodeFieldDeclaration : public NodeContainer
        {

            AstValue<NodeType> type;
            AstValue<NodeVariableDeclarators> variableDeclarators;

            string GetFieldName() const;
        };

        struct NodeConstructorDeclaration : public NodeContainer
        {
            AstValue<NodeIdentifier> identifier;
            AstValue<NodeFormalParameters> formalParameters;
            AstValue<NodeQualifiedNameList, true> qualifiedNameList;
            AstValue<NodeConstructorBody> constructorBody;

            string GetConstructorName() const;
        };

        struct NodeConstructorBody : public NodeContainer
        {
            AstValue<NodeBlock> block;
        };

        struct NodeGenericConstructorDeclaration : public NodeContainer
        {

            AstValue<NodeTypeParameters> typeParameters;
            AstValue<NodeConstructorDeclaration> constructorDeclaration;


            string GetGenericConstructorName() const;
        };

        struct NodeEnumDeclaration : public NodeContainer
        {
            AstValue<NodeIdentifier> identifier;
            AstValue<NodeTypeList, true> typeList;
            AstValue<NodeEnumConstants, true> enumConstants;
            AstValue<NodeEnumBodyDeclarations, true> enumBodyDeclarations;

            string GetEnumName() const;
        };

        struct NodeEnumConstants : public NodeContainer
        {
            AstList<NodeEnumConstant> enumConstantList;
        };

        struct NodeEnumConstant : public NodeContainer
        {
            //AstList<NodeAnnotation> annotationList;
            AstValue<NodeIdentifier> identifier;
            AstValue<NodeArguments, true> arguments;
            AstValue<NodeClassBody, true> classBody;
        };

        struct NodeArguments : public NodeContainer
        {
            AstValue<NodeExpressionList, true> expressionList;
        };

        struct NodeEnumBodyDeclarations : public NodeContainer
        {
            AstList<NodeClassBodyDeclaration> classBodyDeclarationsList;
        };

        struct NodeInterfaceDeclaration : public NodeContainer
        {
            AstValue<NodeIdentifier> identifier;
            AstValue<NodeTypeParameters, true> typeParameters;
            AstValue<NodeTypeList, true> typeList;
            AstValue<NodeInterfaceBody> interfaceBody;

            string GetInterfaceName() const;
        };

        struct NodeInterfaceBody : public NodeContainer
        {
            AstList<NodeInterfaceBodyDeclaration> interfaceBodyDeclarationList;
        };

        struct NodeInterfaceBodyDeclaration : public NodeContainer
        {
            AstValue<NodeInterfaceMemberDeclaration, true> interfaceMemberDeclaration;
        };

        struct NodeInterfaceMemberDeclaration : public NodeContainer
        {
            AstList<NodeModifier> modifierList;
            AstValue<NodeConstDeclaration, true> constDeclaration;
            AstValue<NodeInterfaceMethodDeclaration, true> interfaceMethodDeclaration;
            AstValue<NodeGenericMethodDeclaration, true> genericInterfaceMethodDeclaration;
            AstValue<NodeInterfaceDeclaration, true> interfaceDeclaration;
            //AstValue<NodeAnnotationTypeDeclaration, true> annotationTypeDeclaration;
            AstValue<NodeClassDeclaration, true> classDeclaration;
            AstValue<NodeEnumDeclaration, true> enumDeclaration;
        };

        struct NodeConstDeclaration : public NodeContainer
        {
            AstValue<NodeType> type;
            AstList<NodeConstantDeclarator> constantDeclaratorList;
        };

        struct NodeConstantDeclarator : public NodeContainer
        {
            AstValue<NodeIdentifier> identifier;
            AstValue<NodeVariableInitializer> variableInitializer;
        };

        struct NodeVariableInitializer : public NodeContainer
        {
            AstValue<NodeArrayInitializer, true> arrayInitializer;
            AstValue<NodeExpression, true> expression;
        };

        struct NodeArrayInitializer : public NodeContainer
        {
            AstList<NodeVariableInitializer> variableInitializerList;
        };

        struct NodeInterfaceMethodDeclaration : public NodeContainer
        {
            AstValue<NodeType, true> type;
            AstValue<NodeIdentifier> identifier;
            AstValue<NodeFormalParameters> formalParameters;
            AstValue<NodeQualifiedNameList, true> qualifiedNameList;
        };

        struct NodeGenericInterfaceMethodDeclaration : public NodeContainer
        {
            AstValue<NodeTypeParameters> typeParameters;
            AstValue<NodeInterfaceMethodDeclaration> interfaceMethodDeclaration;
        };

        struct NodeFormalParameter : public NodeContainer
        {
            AstList<NodeVariableModifier> variableModifierList;
            AstValue<NodeType> type;
            AstValue<NodeVariableDeclaratorId> variableDeclaratorId;
        };

        struct NodeFormalParameterList : public NodeContainer
        {
            AstList<NodeFormalParameter> formalParameterList;
            AstValue<NodeLastFormalParameter, true> lastFormalParameter;
        };

        struct NodeLastFormalParameter : public NodeContainer
        {
            AstList<NodeVariableModifier> variableModifierList;
            AstValue<NodeType> type;
            AstValue<NodeVariableDeclaratorId> variableDeclaratorId;
        };

        struct NodeFormalParameters : public NodeContainer
        {
            AstValue<NodeFormalParameterList, true> formalParameterList;
        };

        //struct NodeAnnotationTypeDeclaration : public NodeContainer

        //struct NodeAnnotationTypeBody : public NodeContainer

        //struct NodeAnnotationTypeElementDeclaration : public NodeContainer

        //struct NodeAnnotationTypeElementRest : public NodeContainer

        //struct NodeAnnotationMethodOrConstantRest : public NodeContainer

        //struct NodeAnnotationMethodRest : public NodeContainer

        //struct NodeAnnotationConstantRest : public NodeContainer

        //struct NodeAnnotationIdentifier : public NodeContainer

        //struct NodeAnnotationName : public NodeContainer

        //struct NodeAnnotationSequence : public NodeContainer

        //struct NodeAnnotation : public NodeContainer

        struct NodeModifier : public NodeTerm
        {
        };

        //struct NodeDefaultValue : public NodeContainer

        //struct NodeElementValue : public NodeContainer

        //struct NodeElementValueArrayInitializer : public NodeContainer

        //struct NodeElementValuePair : public NodeContainer

        //struct NodeElementValuePairs : public NodeContainer

        struct NodeBlock : public NodeContainer
        {
            AstList<NodeBlockStatement> blockStatementList;
        };

        struct NodeBlockStatement : public NodeContainer
        {
            AstValue<NodeLocalVariableDeclarationStatement, true> localVariableDeclarationStatement;
            AstValue<NodeStatement, true> statement;
            AstValue<NodeTypeDeclaration, true> typeDeclaration;
        };

        struct NodeLocalVariableDeclarationStatement : public NodeContainer
        {
            AstValue<NodeLocalVariableDeclaration> localVariableDeclaration;
        };

        struct NodeLocalVariableDeclaration : public NodeContainer
        {
            AstList<NodeVariableModifier> variableModifierList;
            AstValue<NodeType> type;
            AstValue<NodeVariableDeclarators> variableDeclarators;
        };

        struct NodeVariableDeclaratorId : public NodeContainer
        {
            AstValue<NodeIdentifier> identifier;

            string GetVariableName() const;
        };

        struct NodeVariableDeclarator : public NodeContainer
        {
            AstValue<NodeVariableDeclaratorId> variableDeclaratorId;
            AstValue<NodeVariableInitializer, true> variableInitializer;

            astVal(NodeVariableDeclaratorId) GetVariableDeclaratorId() const;
        };

        struct NodeVariableDeclarators : public NodeContainer
        {
            AstList<NodeVariableDeclarator> variableDeclaratorList;
        };

        struct NodeVariableModifier : public NodeTerm
        {
            // change NodeTerm to NodeContainer
            //AstValue<NodeAnnotation> anntotation;
        };

        struct NodeType : public NodeContainer
        {
            AstValue<NodeClassOrInterfaceType, true> classOrInterfaceType;
            AstValue<NodePrimitiveType, true> primitiveType;

            string GetTypeName() const;
        };

        struct NodeTypeList : public NodeContainer
        {
            AstList<NodeType> typeList;

            vector<string> GetTypeNames() const;
        };

        struct NodeClassOrInterfaceType : public NodeContainer
        {
            AstList<NodeClassOrInterfaceTypePart> classOrInterfaceTypePartList;

            string GetTypeName() const;
        };

        struct NodeClassOrInterfaceTypePart : public NodeContainer
        {
            AstValue<NodeIdentifier> identifier;
            AstValue<NodeTypeArguments, true> typeArguments;

            string GetTypeNamePart() const;
        };

        struct NodeTypeArguments : public NodeTerm
        {
        };

        struct NodePrimitiveType : public NodeTerm
        {
        };

        struct NodeStatement : public NodeContainer
        {
            AstValue<NodeBlock, true> block;
            AstValue<NodeAssertStatement, true> assertStatement;
            AstValue<NodeIfStatement, true> ifStatement;
            AstValue<NodeForStatement, true> forStatement;
            AstValue<NodeWhileStatement, true> whileStatement;
            AstValue<NodeDoStatement, true> doStatement;
            AstValue<NodeTryStatement, true> tryStatement;
            AstValue<NodeSwitchStatement, true> switchStatement;
            AstValue<NodeSynchronizedStatement, true> synchronizedStatement;
            AstValue<NodeReturnStatement, true> returnStatement;
            AstValue<NodeThrowStatement, true> throwStatement;
            AstValue<NodeBreakStatement, true> breakStatement;
            AstValue<NodeContinueStatement, true> continueStatement;
            AstValue<NodeStatementExpression, true> statementExpression;
            AstValue<NodeIdentifier, true> identifier;
            AstValue<NodeStatement, true> statement;
        };

        struct NodeAssertStatement : public NodeContainer
        {
            AstList<NodeExpression> expressionList;
        };

        struct NodeIfStatement : public NodeContainer
        {
            AstValue<NodeParExpression> parExpression;
            AstValue<NodeStatement> statement;
            AstValue<NodeElseStatement, true> elseStatement;
        };

        struct NodeElseStatement : public NodeContainer
        {
            AstValue<NodeStatement> statement;
        };

        struct NodeForStatement : public NodeContainer
        {
            AstValue<NodeForControl> forControl;
            AstValue<NodeStatement> statement;
        };

        struct NodeWhileStatement : public NodeContainer
        {
            AstValue<NodeParExpression> parExpression;
            AstValue<NodeStatement> statement;
        };

        struct NodeDoStatement : public NodeContainer
        {
            AstValue<NodeStatement> statement;
            AstValue<NodeParExpression> parExpression;
        };

        struct NodeTryStatement : public NodeContainer
        {
            AstValue<NodeTryBlock, true> tryBlock;
            AstValue<NodeTryResSpec, true> tryResSpec;
        };

        struct NodeTryBlock : public NodeContainer
        {
            AstValue<NodeBlock> block;
            AstList<NodeCatchClause> catchClause;
            AstValue<NodeFinallyBlock, true> finallyBlock;
        };

        struct NodeTryResSpec : public NodeContainer
        {
            AstValue<NodeResourceSpecification> resourceSpecification;
            AstValue<NodeBlock> block;
            AstList<NodeCatchClause> catchClause;
            AstValue<NodeFinallyBlock, true> finallyBlock;
        };

        struct NodeSwitchStatement : public NodeContainer
        {
            AstValue<NodeParExpression> parExpression;
            AstList<NodeSwitchBlockStatementGroup> switchBlockStatementGroup;
            AstList<NodeSwitchLabel> switchLabel;
        };

        struct NodeSynchronizedStatement : public NodeContainer
        {
            AstValue<NodeParExpression> parExpression;
            AstValue<NodeBlock> block;
        };

        struct NodeReturnStatement : public NodeContainer
        {
            AstValue<NodeExpression, true> expression;
        };

        struct NodeThrowStatement : public NodeContainer
        {
            AstValue<NodeExpression> expression;
        };

        struct NodeBreakStatement : public NodeContainer
        {
            AstValue<NodeIdentifier, true> identifier;
        };

        struct NodeContinueStatement : public NodeContainer
        {
            AstValue<NodeIdentifier, true> identifier;
        };

        struct NodeParExpression : public NodeContainer
        {
            AstValue<NodeExpression> expression;
        };

        struct NodeForControl : public NodeContainer
        {
            AstValue<NodeEnhancedForControl, true> enhancedForControl;
            AstValue<NodeRegularForControl, true> regularForControl;
        };

        struct NodeForInit : public NodeContainer
        {
            AstValue<NodeLocalVariableDeclaration, true> localVariableDeclaration;
            AstValue<NodeExpressionList, true> expressionList;
        };

        struct NodeForUpdate : public NodeContainer
        {
            AstValue<NodeExpressionList> expressionList;
        };

        struct NodeEnhancedForControl : public NodeContainer
        {
            AstList<NodeVariableModifier> variableModifierList;
            AstValue<NodeType> type;
            AstValue<NodeVariableDeclaratorId> variableDeclaratorId;
            AstValue<NodeExpression> expression;
        };

        struct NodeRegularForControl : public NodeContainer
        {
            AstValue<NodeForInit, true> forInit;
            AstValue<NodeExpression, true> expression;
            AstValue<NodeForUpdate, true> forUpdate;
        };

        struct NodeCatchClause : public NodeContainer
        {
            AstList<NodeVariableModifier> variableModifierList;
            AstValue<NodeCatchType> catchType;
            AstValue<NodeIdentifier> identifier;
            AstValue<NodeBlock> block;
        };

        struct NodeCatchType : public NodeContainer
        {
            AstList<NodeQualifiedName> qualifiedNameList;
        };

        struct NodeFinallyBlock : public NodeContainer
        {
            AstValue<NodeBlock> block;
        };

        struct NodeResourceSpecification : public NodeContainer
        {
            AstValue<NodeResources> resources;
        };

        struct NodeResources : public NodeContainer
        {
            AstList<NodeResource> resourceList;
        };

        struct NodeResource : public NodeContainer
        {
            AstList<NodeVariableModifier> variableModifierList;
            AstValue<NodeClassOrInterfaceType> classOrInterfaceType;
            AstValue<NodeVariableDeclaratorId> variableDeclaratorId;
            AstValue<NodeExpression> expression;
        };

        struct NodeSwitchBlockStatementGroup : public NodeContainer
        {
            AstList<NodeSwitchLabel> switchLabelList;
            AstList<NodeBlockStatement> blockStatementList;
        };

        struct NodeSwitchLabel : public NodeContainer
        {
            AstValue<NodeConstantExpression, true> constantExpression;
            AstValue<NodeEnumConstantName, true> enumConstantName;
        };

        struct NodeConstantExpression : public NodeContainer
        {
            AstValue<NodeExpression3> expression3;
        };

        struct NodeEnumConstantName : public NodeContainer
        {
            AstValue<NodeIdentifier> identifier;
        };

        struct NodeStatementExpression : public NodeContainer
        {
            AstValue<NodeExpression> expression;
        };

        struct NodeQualifiedName : public NodeContainer
        {
            AstList<NodeIdentifier> identifierList;

            string GetValue() const;
        };

        struct NodeQualifiedNameList : public NodeContainer
        {
            AstList<NodeQualifiedName> qualifiedNameList;
        };

        struct NodeExpression : public NodeContainer
        {
            AstValue<NodeExpressionPostfix> expressionPostfix;
        };

        struct NodeExpression2 : public NodeContainer
        {
            AstValue<NodeExpressionString2, true> expressionString2;
            AstValue<NodeExpression2, true> expression2;
            AstValue<NodeExpressionBracket, true> expressionBracket;
        };

        struct NodeExpression3 : public NodeContainer
        {
            AstValue<NodeExpressionPostfix> expressionPostfix;
        };

        struct NodeExpressionString : public NodeContainer
        {
            AstValue<NodeExpression> expression;
        };

        struct NodeExpressionString2 : public NodeContainer
        {
            AstValue<NodeExpression2> expression2;
        };

        struct NodeExpressionRound : public NodeContainer
        {
            AstList<NodeExpression> expressionList;
        };

        struct NodeExpressionBracket : public NodeContainer
        {
            AstValue<NodeExpression2> expression2;
            AstValue<NodeExpression> expression;
        };

        struct NodeExpressionPostfix : public NodeContainer
        {
            AstValue<NodeExpressionString, true> expressionList;
            AstValue<NodeExpressionRound, true> expressionRound;
            AstValue<NodeExpressionBracket, true> expressionBracket;
        };

        struct NodeExpressionList : public NodeContainer
        {
            AstList<NodeExpression> expressionList;
        };

        struct NodeIdentifier : public NodeTerm
        {
        };
	}
}

#pragma once

#include "ParserlibBase.h"

namespace Importer
{
	namespace Parsing
	{
		using parserlib::set;
		using parserlib::nl;
		using parserlib::expr;
		using parserlib::rule;
		using parserlib::term;
		using parserlib::range;

		#pragma region externs

		extern rule whitespace;

		extern rule compilationUnit;
		extern rule packageDeclaration;
		extern rule importDeclaration;
		extern rule typeDeclaration;
		extern rule classOrInterfaceModifier;
		extern rule typeBound;
		extern rule typeParameter;
		extern rule typeParameters;

		extern rule classDeclaration;
		extern rule classDeclarationExtend;
		extern rule classDeclarationImplement;
		extern rule classBody;
		extern rule classBodyDeclaration;
		extern rule memberDeclaration;
		extern rule methodDeclaration;
		extern rule methodBody;
		extern rule genericMethodDeclaration;
		extern rule fieldDeclaration;
		extern rule constructorDeclaration;
		extern rule constructorBody;
		extern rule genericConstructorDeclaration;

		extern rule enumDeclaration;
		extern rule enumConstants;
		extern rule enumConstant;
		extern rule arguments;
		extern rule enumBodyDeclarations;

		extern rule interfaceDeclaration;
		extern rule interfaceBody;
		extern rule interfaceBodyDeclaration;
		extern rule interfaceMemberDeclaration;
		extern rule constDeclaration;
		extern rule constantDeclarator;
		extern rule variableInitializer;
		extern rule arrayInitializer;
		extern rule interfaceMethodDeclaration;
		extern rule genericInterfaceMethodDeclaration;

		extern rule formalParameter;
		extern rule formalParameterList;
		extern rule lastFormalParameter;
		extern rule formalParameters;

		extern rule annotationTypeDeclaration;
		extern rule annotationTypeBody;
		extern rule annotationTypeElementDeclaration;
		extern rule annotationTypeElementRest;
		extern rule annotationMethodOrConstantRest;
		extern rule annotationMethodRest;
		extern rule annotationConstantRest;
		extern rule annotationIdentifier;
		extern rule annotationName;
		extern rule annotationSequence;
		extern rule annotation;

		extern rule modifier;

		extern rule defaultValue;
		extern rule elementValue;
		extern rule elementValueArrayInitializer;
		extern rule elementValuePair;
		extern rule elementValuePairs;

		extern rule block;
		extern rule blockStatement;
		extern rule localVariableDeclarationStatement;
		extern rule localVariableDeclaration;

		extern rule variableDeclaratorId;
		extern rule variableDeclarator;
		extern rule variableDeclarators;
		extern rule variableModifier;

		extern rule type;
		extern rule typeList;
		extern rule classOrInterfaceType;
		extern rule classOrInterfaceTypePart;
		extern rule typeArguments;
		extern rule primitiveType;

		extern rule statement;
		extern rule assertStatement;
		extern rule ifStatement;
		extern rule elseStatement;
		extern rule forStatement;
		extern rule whileStatement;
		extern rule doStatement;
		extern rule tryStatement;
		extern rule tryBlock;
		extern rule tryResSpec;
		extern rule switchStatement;
		extern rule synchronizedStatement;
		extern rule returnStatement;
		extern rule throwStatement;
		extern rule breakStatement;
		extern rule continueStatement;
		extern rule parExpression;
		extern rule forControl;
		extern rule forInit;
		extern rule forUpdate;
		extern rule enhancedForControl;
		extern rule regularForControl;
		extern rule catchClause;
		extern rule catchType;
		extern rule finallyBlock;
		extern rule resourceSpecification;
		extern rule resources;
		extern rule resource;
		extern rule switchBlockStatementGroup;
		extern rule switchLabel;
		extern rule constantExpression;
		extern rule enumConstantName;
		extern rule statementExpression;

		extern rule qualifiedName;
		extern rule qualifiedNameList;

        extern rule expression;
        extern rule expression2;
        extern rule expression3;
        extern rule expressionString;
        extern rule expressionString2;
        extern rule expressionRound;
        extern rule expressionBracket;
        extern rule expressionPostfix;
        extern rule expressionList;

		extern rule identifier;

		#pragma endregion

		#pragma region defines

		#define JAVA_LETTER (range('a', 'z') | range('A', 'Z') | set("$_"))
		#define JAVA_DIGIT range('0', '9')
		#define JAVA_LETTER_OR_DIGIT (JAVA_LETTER | JAVA_DIGIT)
		#define NEWLINE nl(expr("\r\n") | "\n\r" | '\n' | '\r')
		#define WHITECHAR (' ' | set("\t\v\f"))
		#define ANY_CHAR range(0, 255)
		#define EVERYTHING_TO(endChar) term(*(!(endChar) >> (NEWLINE | ANY_CHAR)))
		#define LINE_COMMENT (expr("//") >> EVERYTHING_TO(NEWLINE) >> NEWLINE)
		#define COMMENT (expr("/*") >> EVERYTHING_TO(expr("*/")) >> "*/")

		#define CLASS_OR_INTERFACE_MODIFIER expr("public") | "protected" | "private" | "static" | "abstract" | "final" | "strictfp"

		#pragma endregion

		/* !!! grammar support only limited Java features !!! */

		/*
		LIMITATIONS -->
		- annotations - cannot owns inline round brackets "(...)"
		- typeArguments - cannot owns inline angle brackets "<...>"
		- cannot inline classDeclaration behind enumConstant
		*/

		rule whitespace = *(WHITECHAR | NEWLINE | LINE_COMMENT | COMMENT);

		rule compilationUnit = -packageDeclaration >> *importDeclaration >> *typeDeclaration;
		rule packageDeclaration = *annotation >> "package" >> qualifiedName >> ';';
		rule importDeclaration = "import" >> -expr("static") >> qualifiedName >> -expr(".*") >> ';';
		rule typeDeclaration = (*classOrInterfaceModifier >> (classDeclaration | enumDeclaration | interfaceDeclaration | annotationTypeDeclaration)) | ';';
		rule classOrInterfaceModifier = /*annotation |*/ CLASS_OR_INTERFACE_MODIFIER;
		rule typeBound = type >> *('&' >> type);
		rule typeParameter = identifier >> -("extends" >> typeBound);
		rule typeParameters = "<" >> typeParameter >> *(',' >> typeParameter) >> ">";

		rule classDeclaration = "class" >> identifier >> -typeParameters >> -classDeclarationExtend >> -classDeclarationImplement >> classBody;
		rule classDeclarationExtend = "extends" >> type;
		rule classDeclarationImplement = "implements" >> typeList;
		rule classBody = expr('{') >> *classBodyDeclaration >> '}';
		rule classBodyDeclaration = expr(';') | (-expr("static") >> block) | memberDeclaration;
		rule memberDeclaration = *modifier >> (methodDeclaration | genericMethodDeclaration | fieldDeclaration | constructorDeclaration | genericConstructorDeclaration | interfaceDeclaration | annotationTypeDeclaration | classDeclaration | enumDeclaration);
		rule methodDeclaration = (type | "void") >> identifier >> formalParameters >> *(expr('[') >> ']') >> -("throws" >> qualifiedNameList) >> (methodBody | ';');
		rule methodBody = block;
		rule genericMethodDeclaration = typeParameters >> methodDeclaration;
		rule fieldDeclaration = type >> variableDeclarators >> ';';
		rule constructorDeclaration = identifier >> formalParameters >> -("throws" >> qualifiedNameList) >> constructorBody;
		rule constructorBody = block;
		rule genericConstructorDeclaration = typeParameters >> constructorDeclaration;

		rule enumDeclaration = expr("enum") >> identifier >> -("implements" >> typeList) >> '{' >> -enumConstants >> -expr(',') >> -enumBodyDeclarations >> '}';
		rule enumConstants = enumConstant >> *(',' >> enumConstant);
		rule enumConstant = *annotation >> identifier >> -arguments >> -classBody;
		rule arguments = expr('(') >> -expressionList >> ')';
		rule enumBodyDeclarations = ';' >> *classBodyDeclaration;

		rule interfaceDeclaration = "interface" >> identifier >> -typeParameters >> -("extends" >> typeList) >> interfaceBody;
		rule interfaceBody = expr('{') >> *interfaceBodyDeclaration >> '}';
		rule interfaceBodyDeclaration = interfaceMemberDeclaration | ';';
		rule interfaceMemberDeclaration = *modifier >> (constDeclaration | interfaceMethodDeclaration | genericInterfaceMethodDeclaration | interfaceDeclaration | annotationTypeDeclaration | classDeclaration | enumDeclaration);
		rule constDeclaration = type >> constantDeclarator >> *(',' >> constantDeclarator) >> ';';
		rule constantDeclarator = identifier >> *(expr('[') >> ']') >> '=' >> variableInitializer;
		rule variableInitializer = arrayInitializer | expression;
		rule arrayInitializer = expr('{') >> -(variableInitializer >> *(',' >> variableInitializer) >> -(',')) >> '}';
		rule interfaceMethodDeclaration = (type | "void") >> identifier >> formalParameters >> *(expr('[') >> ']') >> -("throws" >> qualifiedNameList) >> ';';
		rule genericInterfaceMethodDeclaration = typeParameters >> interfaceMethodDeclaration;

		rule formalParameter = *variableModifier >> type >> variableDeclaratorId;
		rule formalParameterList = (formalParameter >> *(',' >> formalParameter) >> -(',' >> lastFormalParameter)) | lastFormalParameter;
		rule lastFormalParameter = *variableModifier >> type >> "..." >> variableDeclaratorId;
		rule formalParameters = expr('(') >> -formalParameterList >> ')';

		rule annotationTypeDeclaration = expr('@') >> "interface" >> identifier >> annotationTypeBody;
		rule annotationTypeBody = '{' >> *annotationTypeElementDeclaration >> '}';
		rule annotationTypeElementDeclaration = (*modifier >> annotationTypeElementRest) | ';';
		rule annotationTypeElementRest = (type >> annotationMethodOrConstantRest >> ';') | ((classDeclaration | interfaceDeclaration | enumDeclaration | annotationTypeDeclaration) >> -expr(';'));
		rule annotationMethodOrConstantRest = annotationMethodRest | annotationConstantRest;
		rule annotationMethodRest = identifier >> '(' >> ')' >> -defaultValue;
		rule annotationConstantRest = variableDeclarators;
		rule annotationIdentifier = term(JAVA_LETTER >> *JAVA_LETTER_OR_DIGIT);
		rule annotationName = annotationIdentifier >> *('.' >> annotationIdentifier);
		rule annotationSequence = EVERYTHING_TO(expr(')'));
		rule annotation = expr('@') >> annotationName >> -('(' >> annotationSequence >> ')');
		//rule annotation = expr('@') >> annotationName >> -('(' >> -(elementValuePairs | elementValue) >> ')');

		rule modifier = CLASS_OR_INTERFACE_MODIFIER | (expr("native") | "synchronized" | "transient" | "volatile");

		rule defaultValue = "default" >> elementValue;
		rule elementValue = expression | annotation | elementValueArrayInitializer;
		rule elementValueArrayInitializer = '{' >> -(elementValue >> *(',' >> elementValue)) >> -expr(',') >> '}';
		rule elementValuePair = identifier >> '=' >> elementValue;
		rule elementValuePairs = elementValuePair >> *(',' >> elementValuePair);

		rule block = '{' >> *blockStatement >> '}';
		rule blockStatement = localVariableDeclarationStatement | statement | typeDeclaration;
		rule localVariableDeclarationStatement = localVariableDeclaration >> ';';
		rule localVariableDeclaration = *variableModifier >> type >> variableDeclarators;

		rule variableDeclaratorId = identifier >> *(expr('[') >> ']');
		rule variableDeclarator = variableDeclaratorId >> -('=' >> variableInitializer);
		rule variableDeclarators = variableDeclarator >> *(',' >> variableDeclarator);
		rule variableModifier = "final" | annotation;

		rule type = (classOrInterfaceType | primitiveType) >> *(expr('[') >> ']');
		rule typeList = type >> *(',' >> type);
		rule classOrInterfaceType = classOrInterfaceTypePart >> *('.' >> classOrInterfaceTypePart);
		rule classOrInterfaceTypePart = identifier >> -typeArguments;
		rule typeArguments = term(expr('<') >> EVERYTHING_TO(expr('>')) >> '>');
		//rule typeArgument = type | ('?' >> -((expr("extends") | "super") >> type));
		//rule typeArguments = expr('<') >> typeArgument >> *(',' >> typeArgument) >> '>';
		rule primitiveType = expr("boolean") | "char" | "byte" | "short" | "int" | "long" | "float" | "double";
		rule statement = block | assertStatement | ifStatement | forStatement | whileStatement | doStatement | tryStatement
			| switchStatement | synchronizedStatement | returnStatement | throwStatement | breakStatement | continueStatement
			| ';' | (statementExpression >> ';') | (identifier >> ':' >> statement);
		rule assertStatement = "assert" >> expression >> -(':' >> expression) >> ';';
		rule ifStatement = "if" >> parExpression >> statement >> -elseStatement;
		rule elseStatement = "else" >> statement;
		rule forStatement = expr("for") >> '(' >> forControl >> ')' >> statement;
		rule whileStatement = "while" >> parExpression >> statement;
		rule doStatement = "do" >> statement >> "while" >> parExpression >> ';';
		rule tryStatement = "try" >> (tryBlock | tryResSpec);
		rule tryBlock = block >> ((+catchClause >> -finallyBlock) | finallyBlock);
		rule tryResSpec = resourceSpecification >> block >> *catchClause >> -finallyBlock;
		rule switchStatement = "switch" >> parExpression >> '{' >> *switchBlockStatementGroup >> *switchLabel >> '}';
		rule synchronizedStatement = "synchronized" >> parExpression >> block;
		rule returnStatement = "return" >> -expression >> ';';
		rule throwStatement = "throw" >> expression >> ';';
		rule breakStatement = "break" >> -identifier >> ';';
		rule continueStatement = "continue" >> -identifier >> ';';
		rule parExpression = '(' >> expression >> ')';
		rule forControl = enhancedForControl | regularForControl;
		rule forInit = localVariableDeclaration | expressionList;
		rule forUpdate = expressionList;
		rule enhancedForControl = *variableModifier >> type >> variableDeclaratorId >> ':' >> expression;
		rule regularForControl = -forInit >> ';' >> -expression >> ';' >> -forUpdate;
		rule catchClause = expr("catch") >> '(' >> *variableModifier >> catchType >> identifier >> ')' >> block;
		rule catchType = qualifiedName >> *('|' >> qualifiedName);
		rule finallyBlock = "finally" >> block;
		rule resourceSpecification = '(' >> resources >> -expr(';') >> ')';
		rule resources = resource >> *(';' >> resource);
		rule resource = *variableModifier >> classOrInterfaceType >> variableDeclaratorId >> '=' >> expression;
		rule switchBlockStatementGroup = +switchLabel >> +blockStatement;
		rule switchLabel = ("case" >> constantExpression >> ':') | ("case" >> enumConstantName >> ':') | (expr("default") >> ':');
        rule constantExpression = expression3;
		rule enumConstantName = identifier;
		rule statementExpression = expression;

		rule qualifiedName = identifier >> *('.' >> identifier);
		rule qualifiedNameList = qualifiedName >> *(',' >> qualifiedName);

        rule expression = EVERYTHING_TO(set("();{}\"")) >> expressionPostfix;
        rule expression2 = EVERYTHING_TO(set("{}\"")) >> -((expressionString2 >> expression2) | expressionBracket);
        rule expression3 = EVERYTHING_TO(set("();:{}\"")) >> expressionPostfix;
        rule expressionString = expr('"') >> EVERYTHING_TO(expr('"')) >> '"' >> expression;
        rule expressionString2 = expr('"') >> EVERYTHING_TO(expr('"')) >> '"' >> expression2;
        rule expressionRound = '(' >> expression >> ')' >> expression;
        rule expressionBracket = '{' >> expression2 >> '}' >> expression;
        rule expressionPostfix = -expressionString >> -(expressionRound | expressionBracket);
        rule expressionList = expression >> *(',' >> expression);

		rule identifier = term(JAVA_LETTER >> *JAVA_LETTER_OR_DIGIT);

	}
}

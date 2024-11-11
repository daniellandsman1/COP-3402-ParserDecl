// Daniel Landsman
// scope_checkv2.c: scope checking file, includes function bodies

#include <stdio.h>
#include "id_use.h"
#include "id_attrs.h"
#include "scope_checkv2.h"
#include "symtabv2.h"

// Pre-Conditions: Block is a valid block AST
// Post-Conditions: Performs declaration checking on block
block_t scope_check_block(block_t block)
{
    symtab_enter_scope(); // Enter scope
    scope_check_constDecls(block.const_decls); // Check const declarations
    scope_check_varDecls(block.var_decls); // Check var declarations
    scope_check_procDecls(block.proc_decls); // Check proc declarations
    block.stmts = scope_check_stmts(block.stmts); // Check statements declarations
    symtab_exit_scope(); // Exit scope
    return block;
}

// Pre-Conditions: varDs is a valid var_decls AST
// Post-Conditions: Performs declaration checking on varDs
void scope_check_varDecls(var_decls_t varDs)
{
    var_decl_t* varDeclPtr = varDs.var_decls; // Start at first varDecl
    
    while (varDeclPtr != NULL) // Iterate through each varDecl
    {
        scope_check_varDecl(*varDeclPtr); // Check varDecl
        varDeclPtr = varDeclPtr->next; // Move to next one
    }
}

// Pre-Conditions: varD is a valid var_decl AST
// Post-Conditions: Performs declaration checking on varD
void scope_check_varDecl(var_decl_t varD)
{
    scope_check_idents(varD.ident_list, variable_idk); // Scope check identifiers
}

// Pre-Conditions: constDecls is a valid const_decls AST
// Post-Conditions: Performs declaration checking on constDecls
void scope_check_constDecls(const_decls_t constDecls)
{
    const_decl_t* constDeclPtr = constDecls.start; // Start at first constDecl

    while (constDeclPtr != NULL) // Iterate through each constDecl
    {
        scope_check_constDecl(*constDeclPtr); // Scope check constDecl
        constDeclPtr = constDeclPtr->next; // Move to next one
    }
}

// Pre-Conditions: constDecl is a valid const_decl AST
// Post-Conditions: Performs declaration checking on constDecl
void scope_check_constDecl(const_decl_t constDecl)
{
    scope_check_constDefList(constDecl.const_def_list); // Scope check constDefList
}

// Pre-Conditions: constDefs is a valid const_def_list AST
// Post-Conditions: Performs declaration checking on constDefs
void scope_check_constDefList(const_def_list_t constDefs)
{
    const_def_t* constDefPtr = constDefs.start; // Start at first constDef

    while (constDefPtr != NULL) // Iterate through each constDef
    {
        scope_check_constDef(*constDefPtr); // Scope check constDef
        constDefPtr = constDefPtr->next; // Move to next one
    }
}

// Pre-Conditions: constDef is a valid const_def AST
// Post-Conditions: Performs declaration checking on constDef
void scope_check_constDef(const_def_t constDef)
{
    scope_check_declare_ident(constDef.ident, constant_idk); // Scope check identifier
}

// Pre-Conditions: procDs is a valid proc_decls AST
// Post-Conditions: Performs declaration checking on procDs
void scope_check_procDecls(proc_decls_t procDs)
{
    proc_decl_t* procDeclPtr = procDs.proc_decls; // Start at first procDecl

    while (procDeclPtr != NULL) // Iterate through each procDecl
    {
        scope_check_procDecl(*procDeclPtr); // Scope check procDecl
        procDeclPtr = procDeclPtr->next; // Move to next one
    }
}

// Pre-Conditions: procD is a valid proc_decl AST
// Post-Conditions: Performs declaration checking on procD
void scope_check_procDecl(proc_decl_t procD)
{
    if (symtab_name_declared_currently(procD.name)) // If duplicate declaration, produce error
    {
        bail_with_prog_error(*(procD.file_loc), "%s \"%s\" is already declared as a %s", 
                             kind2str(procedure_idk), procD.name, kind2str((symtab_lookup(procD.name))->attrs->kind));
    }

    else // No duplicate declaration, add to symbol table
    {
        int ofst_cnt = symtab_scope_loc_count(); // Record offset // FREE THIS
        id_attrs* my_attrs = create_id_attrs(*(procD.file_loc), procedure_idk, ofst_cnt); // Create attributes
        symtab_insert(procD.name, my_attrs); // Insert into symbol table

        procD.block = scope_check_block(procD.block); // Scope check procedure block
    }
}

// Pre-Conditions: idents is a valid ident_list AST
// Post-Conditions: Performs declaration checking on idents
void scope_check_idents(ident_list_t idents, id_kind kind)
{
    ident_t* identPtr = idents.start; // Start at first identifier

    while (identPtr != NULL) // Iterate through each identifier
    {
        scope_check_declare_ident(*identPtr, kind); // Scope check identifier
        identPtr = identPtr->next; // Move to next one
    }
}

// Pre-Conditions: ident is a valid ident AST
// Post-Conditions: Performs declaration checking on ident 
void scope_check_declare_ident(ident_t ident, id_kind kind)
{
    if (symtab_name_declared_currently(ident.name)) // Check for duplicate declaration
    {
        bail_with_prog_error(*(ident.file_loc), "%s \"%s\" is already declared as a %s", 
                             kind2str(kind), ident.name, kind2str((symtab_lookup(ident.name))->attrs->kind));
    }

    else // No duplicate declaration, add to symbol table
    {
        int ofst_cnt = symtab_scope_loc_count(); // Record offset // FREE THIS
        id_attrs* my_attrs = create_id_attrs(*(ident.file_loc), kind, ofst_cnt); // Create attributes
        symtab_insert(ident.name, my_attrs); // Insert into symbol table
    }
}

// Pre-Conditions: statement is a valid stmt AST
// Post-Conditions: Performs declaration checking on statement
stmt_t scope_check_stmt(stmt_t statement)
{
    // Switch statement determines what kind of statement, scope checks accordingly
    switch (statement.stmt_kind)
    {
        case assign_stmt:
            statement.data.assign_stmt = scope_check_assignStmt(statement.data.assign_stmt);
            break;
        case call_stmt:
            statement.data.call_stmt = scope_check_callStmt(statement.data.call_stmt);
            break;
        case if_stmt:
            statement.data.if_stmt = scope_check_ifStmt(statement.data.if_stmt);
            break;
        case while_stmt:
            statement.data.while_stmt = scope_check_whileStmt(statement.data.while_stmt);
            break;
        case read_stmt:
            statement.data.read_stmt = scope_check_readStmt(statement.data.read_stmt);
            break;
        case print_stmt:
            statement.data.print_stmt = scope_check_printStmt(statement.data.print_stmt);
            break;
        case block_stmt:
            statement.data.block_stmt = scope_check_blockStmt(statement.data.block_stmt);
            break;
        default: // If none of the previous cases match, produce error
            bail_with_error("Attempted to scope check an invalid statement kind!");
            break;
    }

    return statement;
}

// Pre-Conditions: aStmt is a valid assign_stmt AST
// Post-Conditions: Performs declaration checking on aStmt
assign_stmt_t scope_check_assignStmt(assign_stmt_t aStmt)
{
    scope_check_ident_declared(*(aStmt.file_loc), aStmt.name); // Make sure ident is declared
    *(aStmt.expr) = scope_check_expr(*(aStmt.expr)); // Scope check expression

    return aStmt;
}

// Pre-Conditions: cStmt is a valid call_stmt AST
// Post-Conditions: Performs declaration checking on cStmt
call_stmt_t scope_check_callStmt(call_stmt_t cStmt)
{
    scope_check_ident_declared(*(cStmt.file_loc), cStmt.name); // Make sure ident is declared

    return cStmt;
}

// Pre-Conditions: iStmt is a valid if_stmt AST
// Post-Conditions: Performs declaration checking on iStmt
if_stmt_t scope_check_ifStmt(if_stmt_t iStmt)
{
    iStmt.condition = scope_check_condition(iStmt.condition); // Scope check condition
    *(iStmt.then_stmts) = scope_check_stmts(*(iStmt.then_stmts)); // Scope check then statements

    if (iStmt.else_stmts != NULL) // If there are else statements, scope check those too
    {
        *(iStmt.else_stmts) = scope_check_stmts(*(iStmt.else_stmts));
    }

    return iStmt;
}

// Pre-Conditions: wStmt is a valid while_stmt AST
// Post-Conditions: Performs declaration checking on wStmt
while_stmt_t scope_check_whileStmt(while_stmt_t wStmt)
{
    wStmt.condition = scope_check_condition(wStmt.condition); // Scope check condition
    *(wStmt.body) = scope_check_stmts(*(wStmt.body)); // Scope check statements in body of loop

    return wStmt;
}

// Pre-Conditions: rStmt is a valid read_stmt AST
// Post-Conditions: Performs declaration checking on rStmt
read_stmt_t scope_check_readStmt(read_stmt_t rStmt)
{
    scope_check_ident_declared(*(rStmt.file_loc), rStmt.name); // Make sure ident is declared

    return rStmt;
}

// Pre-Conditions: pStmt is a valid print_stmt AST
// Post-Conditions: Performs declaration checking on pStmt
print_stmt_t scope_check_printStmt(print_stmt_t pStmt)
{
    pStmt.expr = scope_check_expr(pStmt.expr); // Scope check expression

    return pStmt;
}

// Pre-Conditions: bStmt is a valid block_stmt AST
// Post-Conditions: Performs declaration checking on bStmt
block_stmt_t scope_check_blockStmt(block_stmt_t bStmt)
{
    *(bStmt.block) = scope_check_block(*(bStmt.block)); // Scope check block

    return bStmt;
}

// Pre-Conditions: statements is a valid stmts AST
// Post-Conditions: Performs declaration checking on statements
stmts_t scope_check_stmts(stmts_t statements)
{
    if (statements != NULL) // Takes care of case where statements can be empty
    {
        statements.stmt_list = scope_check_stmtList(statements.stmt_list); // Scope check statements
    }

    return statements;
}

// Pre-Conditions: stmtList is a valid stmt_list AST
// Post-Conditions: Performs declaration checking on stmtList 
stmt_list_t scope_check_stmtList(stmt_list_t stmtList)
{
    stmt_t* stmtListPtr = stmtList.start; // Start at first statement
    
    while (stmtListPtr != NULL) // Iterate through each statement
    {
        *stmtListPtr = scope_check_stmt(*stmtListPtr); // Scope check statement
        stmtListPtr = stmtListPtr->next; // Move to the next one
    }

    return stmtList;
}

// Pre-Conditions: expression is a valid expr AST
// Post-Conditions: Performs declaration checking on expression
expr_t scope_check_expr(expr_t expression)
{
    // Switch statement determines what kind of expression, scope check accordingly
    switch (expression.expr_kind)
    {
        case expr_bin:
            expression.data.binary = scope_check_bin_op_expr(expression.data.binary);
            break;
        case expr_negated:
            expression.data.negated = scope_check_neg_expr(expression.data.negated);
            break;
        case expr_ident:
            expression.data.ident = scope_check_ident_expr(expression.data.ident);
            break;
        case expr_number:
            // Number means no identifiers to scope check, do nothing
            break;
        default: // If none of the previous cases match, produce error
            bail_with_error("Attempted to scope check an invalid expression kind!");
    }

    return expression;
}

// Pre-Conditions: binOpExpr is a valid binary_op_expr AST
// Post-Conditions: Performs declaration checking on binOpExpr
binary_op_expr_t scope_check_bin_op_expr(binary_op_expr_t binOpExpr)
{
    *(binOpExpr.expr1) = scope_check_expr(*(binOpExpr.expr1)); // Scope check first expression
    *(binOpExpr.expr2) = scope_check_expr(*(binOpExpr.expr2)); // Scope check second expression

    return binOpExpr;
}

// Pre-Conditions: negExpr is a valid negated_expr AST
// Post-Conditions: Performs declaration checking on negExpr
negated_expr_t scope_check_neg_expr(negated_expr_t negExpr)
{
    *(negExpr.expr) = scope_check_expr(*(negExpr.expr)); // Scope check the expression

    return negExpr;
}

// Pre-Conditions: ident is a valid ident AST
// Post-Conditions: Performs declaration checking on ident as an expression
ident_t scope_check_ident_expr(ident_t ident)
{
    scope_check_ident_declared(*(ident.file_loc), ident.name); // Scope check identifier

    return ident;
}

// Pre-Conditions: condition is a valid condition AST
// Post-Conditions: Performs declaration checking on condition
extern condition_t scope_check_condition(condition_t condition)
{
    // Switch statement determines what kind of condition, scope check accordingly
    switch (condition.cond_kind)
    {
        case ck_db:
            condition.data.db_cond.dividend = scope_check_expr(condition.data.db_cond.dividend); // Scope check dividend expression
            condition.data.db_cond.divisor = scope_check_expr(condition.data.db_cond.divisor); // Scope check divisor expression
            break;
        case ck_rel:
            condition.data.rel_op_cond.expr1 = scope_check_expr(condition.data.rel_op_cond.expr1); // Scope check first expression
            condition.data.rel_op_cond.expr2 = scope_check_expr(condition.data.rel_op_cond.expr2); // Scope check second expression
            break;
        default: // If none of the previous cases match, produce error
            bail_with_error("Attempted to scope check an invalid condition kind!");
            break;
    }

    return condition;
}

// Pre-Conditions: my_name is not NULL, floc is a valid file location
// Post-Conditions: Checks if the identifier associated with my_name
// has been previously declared in the program
void scope_check_ident_declared(file_location floc, const char* my_name)
{
    if (!symtab_name_declared(my_name)) // If my_name was not declared previously, produce error
    {
        bail_with_prog_error(floc, "identifier \"%s\" is not declared!", my_name);
    }
}
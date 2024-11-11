// Daniel Landsman
// scope_checkv2.h: scope checking file, includes function declarations

#ifndef _SCOPE_CHECKV2_H
#define _SCOPE_CHECKV2_H
#include "ast.h"
#include "id_use.h"

// Pre-Conditions: Block is a valid block AST
// Post-Conditions: Performs declaration checking on block
extern block_t scope_check_block(block_t block);

// Pre-Conditions: varDs is a valid var_decls AST
// Post-Conditions: Performs declaration checking on varDs
extern void scope_check_varDecls(var_decls_t varDs);

// Pre-Conditions: varD is a valid var_decl AST
// Post-Conditions: Performs declaration checking on varD
extern void scope_check_varDecl(var_decl_t varD);

// Pre-Conditions: constDecls is a valid const_decls AST
// Post-Conditions: Performs declaration checking on constDecls
extern void scope_check_constDecls(const_decls_t constDecls);

// Pre-Conditions: constDecl is a valid const_decl AST
// Post-Conditions: Performs declaration checking on constDecl
extern void scope_check_constDecl(const_decl_t constDecl);

// Pre-Conditions: constDefs is a valid const_def_list AST
// Post-Conditions: Performs declaration checking on constDefs
extern void scope_check_constDefList(const_def_list_t constDefs);

// Pre-Conditions: constDef is a valid const_def AST
// Post-Conditions: Performs declaration checking on constDef
extern void scope_check_constDef(const_def_t constDef);

// Pre-Conditions: procDs is a valid proc_decls AST
// Post-Conditions: Performs declaration checking on procDs
extern void scope_check_procDecls(proc_decls_t procDs);

// Pre-Conditions: procD is a valid proc_decl AST
// Post-Conditions: Performs declaration checking on procD
extern void scope_check_procDecl(proc_decl_t procD);

// Pre-Conditions: idents is a valid ident_list AST
// Post-Conditions: Performs declaration checking on idents
extern void scope_check_idents(ident_list_t idents, id_kind kind);

// Pre-Conditions: ident is a valid ident AST
// Post-Conditions: Performs declaration checking on ident 
extern void scope_check_declare_ident(ident_t ident, id_kind kind);

// Pre-Conditions: statement is a valid stmt AST
// Post-Conditions: Performs declaration checking on statement
extern stmt_t scope_check_stmt(stmt_t statement);

// Pre-Conditions: aStmt is a valid assign_stmt AST
// Post-Conditions: Performs declaration checking on aStmt
extern assign_stmt_t scope_check_assignStmt(assign_stmt_t aStmt);

// Pre-Conditions: cStmt is a valid call_stmt AST
// Post-Conditions: Performs declaration checking on cStmt
extern call_stmt_t scope_check_callStmt(call_stmt_t cStmt);

// Pre-Conditions: iStmt is a valid if_stmt AST
// Post-Conditions: Performs declaration checking on iStmt
extern if_stmt_t scope_check_ifStmt(if_stmt_t iStmt);

// Pre-Conditions: wStmt is a valid while_stmt AST
// Post-Conditions: Performs declaration checking on wStmt
extern while_stmt_t scope_check_whileStmt(while_stmt_t wStmt);

// Pre-Conditions: rStmt is a valid read_stmt AST
// Post-Conditions: Performs declaration checking on rStmt
extern read_stmt_t scope_check_readStmt(read_stmt_t rStmt);

// Pre-Conditions: pStmt is a valid print_stmt AST
// Post-Conditions: Performs declaration checking on pStmt
extern print_stmt_t scope_check_printStmt(print_stmt_t pStmt);

// Pre-Conditions: bStmt is a valid block_stmt AST
// Post-Conditions: Performs declaration checking on bStmt
extern block_stmt_t scope_check_blockStmt(block_stmt_t bStmt);

// Pre-Conditions: statements is a valid stmts AST
// Post-Conditions: Performs declaration checking on statements
extern stmts_t scope_check_stmts(stmts_t statements);

// Pre-Conditions: stmtList is a valid stmt_list AST
// Post-Conditions: Performs declaration checking on stmtList 
extern stmt_list_t scope_check_stmtList(stmt_list_t stmtList);

// Pre-Conditions: expression is a valid expr AST
// Post-Conditions: Performs declaration checking on expression
extern expr_t scope_check_expr(expr_t expression);

// Pre-Conditions: binOpExpr is a valid binary_op_expr AST
// Post-Conditions: Performs declaration checking on binOpExpr
extern binary_op_expr_t scope_check_bin_op_expr(binary_op_expr_t binOpExpr);

// Pre-Conditions: negExpr is a valid negated_expr AST
// Post-Conditions: Performs declaration checking on negExpr
extern negated_expr_t scope_check_neg_expr(negated_expr_t negExpr);

// Pre-Conditions: ident is a valid ident AST
// Post-Conditions: Performs declaration checking on ident as an expression
extern ident_t scope_check_ident_expr(ident_t ident);

// Pre-Conditions: condition is a valid condition AST
// Post-Conditions: Performs declaration checking on condition
extern condition_t scope_check_condition(condition_t condition);

// Pre-Conditions: my_name is not NULL, floc is a valid file location
// Post-Conditions: Checks if the identifier associated with my_name
// has been previously declared in the program
extern void scope_check_ident_declared(file_location floc, const char* my_name);

#endif
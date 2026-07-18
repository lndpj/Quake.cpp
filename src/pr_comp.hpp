// pr_comp.hpp -- shared definitions for QuakeC compiler and engine
#pragma once
#include <EASTL/array.h>
#include <cstdint>

using func_t = int;
using string_t = int;

enum etype_t : int {
    ev_void,
    ev_string,
    ev_float,
    ev_vector,
    ev_entity,
    ev_field,
    ev_function,
    ev_pointer
};

constexpr int OFS_NULL = 0;
constexpr int OFS_RETURN = 1;
constexpr int OFS_PARM0 = 4; // leave 3 ofs for each parm to hold vectors
constexpr int OFS_PARM1 = 7;
constexpr int OFS_PARM2 = 10;
constexpr int OFS_PARM3 = 13;
constexpr int OFS_PARM4 = 16;
constexpr int OFS_PARM5 = 19;
constexpr int OFS_PARM6 = 22;
constexpr int OFS_PARM7 = 25;
constexpr int RESERVED_OFS = 28;

enum OpCode : int {
    OP_DONE,
    OP_MUL_F,
    OP_MUL_V,
    OP_MUL_FV,
    OP_MUL_VF,
    OP_DIV_F,
    OP_ADD_F,
    OP_ADD_V,
    OP_SUB_F,
    OP_SUB_V,

    OP_EQ_F,
    OP_EQ_V,
    OP_EQ_S,
    OP_EQ_E,
    OP_EQ_FNC,

    OP_NE_F,
    OP_NE_V,
    OP_NE_S,
    OP_NE_E,
    OP_NE_FNC,

    OP_LE,
    OP_GE,
    OP_LT,
    OP_GT,

    OP_LOAD_F,
    OP_LOAD_V,
    OP_LOAD_S,
    OP_LOAD_ENT,
    OP_LOAD_FLD,
    OP_LOAD_FNC,

    OP_ADDRESS,

    OP_STORE_F,
    OP_STORE_V,
    OP_STORE_S,
    OP_STORE_ENT,
    OP_STORE_FLD,
    OP_STORE_FNC,

    OP_STOREP_F,
    OP_STOREP_V,
    OP_STOREP_S,
    OP_STOREP_ENT,
    OP_STOREP_FLD,
    OP_STOREP_FNC,

    OP_RETURN,
    OP_NOT_F,
    OP_NOT_V,
    OP_NOT_S,
    OP_NOT_ENT,
    OP_NOT_FNC,
    OP_IF,
    OP_IFNOT,
    OP_CALL0,
    OP_CALL1,
    OP_CALL2,
    OP_CALL3,
    OP_CALL4,
    OP_CALL5,
    OP_CALL6,
    OP_CALL7,
    OP_CALL8,
    OP_STATE,
    OP_GOTO,
    OP_AND,
    OP_OR,

    OP_BITAND,
    OP_BITOR
};

struct dstatement_t {
    unsigned short op = 0;
    short a = 0, b = 0, c = 0;
};

struct ddef_t {
    unsigned short type = 0; // if DEF_SAVEGLOBAL bit is set
    // the variable needs to be saved in savegames
    unsigned short ofs = 0;
    int s_name = 0;
};

constexpr int DEF_SAVEGLOBAL = (1 << 15);

constexpr int MAX_PARMS = 8;

struct dfunction_t {
    int first_statement = 0; // negative numbers are builtins
    int parm_start = 0;
    int locals = 0; // total ints of parms + locals

    int profile = 0; // runtime

    int s_name = 0;
    int s_file = 0; // source file defined in

    int numparms = 0;
    eastl::array<uint8_t, MAX_PARMS> parm_size{};
};

constexpr int PROG_VERSION = 6;

struct dprograms_t {
    int version = 0;
    int crc = 0; // check of header file

    int ofs_statements = 0;
    int numstatements = 0; // statement 0 is an error

    int ofs_globaldefs = 0;
    int numglobaldefs = 0;

    int ofs_fielddefs = 0;
    int numfielddefs = 0;

    int ofs_functions = 0;
    int numfunctions = 0; // function 0 is an empty

    int ofs_strings = 0;
    int numstrings = 0; // first string is a null string

    int ofs_globals = 0;
    int numglobals = 0;

    int entityfields = 0;
};

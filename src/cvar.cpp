// cvar.cpp -- dynamic variable tracking

#include "quakedef.hpp"

using namespace CDAudio;
using namespace Client;
using namespace Common;
using namespace Console;
using namespace Render;
using namespace Draw;
using namespace Host;
using namespace Input;
using namespace Keys;
using namespace Math;
using namespace Menu;
using namespace Model;
using namespace Net;
using namespace VM;
using namespace Sbar;
using namespace Screen;
using namespace Server;
using namespace Audio;
using namespace Vid;
using namespace View;
using namespace Wad;
using namespace Cvar;
using namespace Cmd;


namespace Cvar {

/*
============
FindVar
============
*/
cvar_t* FindVar(std::string_view var_name)
{
    for (cvar_t* var = state.vars; var; var = var->next) {
        if (var_name == var->name) {
            return var;
        }
    }
    return nullptr;
}

/*
============
VariableValue
============
*/
float VariableValue(std::string_view var_name)
{
    cvar_t* var = FindVar(var_name);
    if (!var) {
        return 0.0f;
    }
    return Q_atof(var->string);
}

/*
============
VariableString
============
*/
std::string_view VariableString(std::string_view var_name)
{
    cvar_t* var = FindVar(var_name);
    if (!var) {
        return "";
    }
    return var->string;
}

/*
============
CompleteVariable
============
*/
std::string_view CompleteVariable(std::string_view partial)
{
    if (partial.empty()) {
        return "";
    }

    for (cvar_t* var = state.vars; var; var = var->next) {
        if (std::string_view(var->name).starts_with(partial)) {
            return var->name;
        }
    }
    return "";
}

/*
============
Set
============
*/
void Set(std::string_view var_name, std::string_view value)
{
    cvar_t* var = FindVar(var_name);
    if (!var) {
        Con_Printf("Cvar::Set: variable %.*s not found\n", static_cast<int>(var_name.length()), var_name.data());
        return;
    }

    qboolean changed = (value != var->string);

    Z_Free(const_cast<char*>(var->string)); // free the old value string

    char* new_str = (char *) Z_Malloc(value.length() + 1);
    Q_memcpy(new_str, const_cast<char*>(value.data()), value.length());
    new_str[value.length()] = '\0';
    var->string = new_str;
    var->value = Q_atof(var->string);
    if (var->server && changed) {
        if (sv.active) {
            SV_BroadcastPrintf("\"%s\" changed to \"%s\"\n", var->name, var->string);
        }
    }
}

/*
============
SetValue
============
*/
void SetValue(std::string_view var_name, float value)
{
    char val[32];
    std::sprintf(val, "%f", value);
    Set(var_name, val);
}

/*
============
Register
============
*/
void Register(cvar_t* variable)
{
    // check to see if it has allready been defined
    if (FindVar(variable->name)) {
        Con_Printf("Can't register variable %s, allready defined\n", variable->name);
        return;
    }

    // check for overlap with a command
    if (Cmd::Exists(variable->name)) {
        Con_Printf("Cvar::Register: %s is a command\n", variable->name);
        return;
    }

    // copy the value off, because future sets will Z_Free it
    const char* oldstr = variable->string;
    char* new_str = (char *) Z_Malloc(Q_strlen(oldstr) + 1);
    Q_strcpy(new_str, oldstr);
    variable->string = new_str;
    variable->value = Q_atof(variable->string);

    // link the variable in
    variable->next = state.vars;
    state.vars = variable;
}

/*
============
Command
============
*/
qboolean Command(void)
{
    cvar_t* v = FindVar(Cmd::Argv(0));
    if (!v) {
        return false;
    }

    // perform a variable print or set
    if (Cmd::Argc() == 1) {
        Con_Printf("\"%s\" is \"%s\"\n", v->name, v->string);
        return true;
    }

    Set(v->name, Cmd::Argv(1));
    return true;
}

/*
============
WriteVariables
============
*/
void WriteVariables(std::FILE* f)
{
    for (cvar_t* var = state.vars; var; var = var->next) {
        if (var->archive) {
            std::fprintf(f, "%s \"%s\"\n", var->name, var->string);
        }
    }
}

} // namespace Cvar

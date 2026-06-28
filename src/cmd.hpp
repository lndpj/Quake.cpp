// cmd.h -- Command buffer and command execution
#pragma once
#include <string_view>

typedef void (*xcommand_t)(void);

namespace Cmd {

enum class Source {
    Client,
    Command
};

struct State {
    Source source = Source::Command;
};
inline State state;

void BufferInit(void);

void BufferAddText(std::string_view text);

void BufferInsertText(std::string_view text);

void BufferExecute(void);

void Init(void);

void AddCommand(std::string_view cmd_name, xcommand_t function);

bool Exists(std::string_view cmd_name);

std::string_view CompleteCommand(std::string_view partial);

int Argc(void);
std::string_view Argv(int arg);
std::string_view Args(void);

void TokenizeString(std::string_view text);

void ExecuteString(std::string_view text, Source src);

void ForwardToServer(void);

void Print(std::string_view text);

} // namespace Cmd

// cmd.h -- Command buffer and command execution
#pragma once
#include <string_view>
#include <functional>
#include <EASTL/map.h>
#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace Cmd {

using xcommand_t = std::function<void()>;

enum class Source {
    Client,
    Command
};

struct State {
    Source source = Source::Command;
};

struct CaseInsensitiveLess {
    using is_transparent = void;
    template <typename T, typename U>
    bool operator()(const T& lhs, const U& rhs) const {
        std::string_view sv_lhs(lhs.data(), lhs.size());
        std::string_view sv_rhs(rhs.data(), rhs.size());
        return std::lexicographical_compare(
            sv_lhs.begin(), sv_lhs.end(),
            sv_rhs.begin(), sv_rhs.end(),
            [](char a, char b) {
                return std::tolower(static_cast<unsigned char>(a)) <
                       std::tolower(static_cast<unsigned char>(b));
            }
        );
    }
};

class CommandRegistry {
public:
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

    State& GetState() { return state_; }
    const State& GetState() const { return state_; }

    const eastl::map<eastl::string, eastl::string, CaseInsensitiveLess>& GetAliases() const { return aliases_; }
    eastl::map<eastl::string, eastl::string, CaseInsensitiveLess>& GetAliases() { return aliases_; }
    bool& GetCmdWait() { return cmd_wait_; }

    void AddAlias(std::string_view name, std::string_view value) {
        aliases_[eastl::string(name.data(), name.length())] = eastl::string(value.data(), value.length());
    }

private:
    State state_;
    eastl::string cmd_text_;
    bool cmd_wait_ = false;
    eastl::map<eastl::string, eastl::string, CaseInsensitiveLess> aliases_;
    eastl::map<eastl::string, xcommand_t, CaseInsensitiveLess> commands_;
    eastl::vector<eastl::string> cmd_argv_;
    std::string_view cmd_args_;
};

CommandRegistry& GetCommandRegistry();

inline State& state = GetCommandRegistry().GetState();

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

void ExecuteString(std::string_view text, Source src);

void ForwardToServer(void);

} // namespace Cmd

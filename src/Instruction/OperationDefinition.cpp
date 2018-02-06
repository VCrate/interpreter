#include <bytec/Instruction/OperationDefinition.hpp>

#include <map>
#include <iostream>

namespace bytec {

OperationDefinition::OperationDefinition(std::string const& abbr) : abbr(abbr), arguments_count(0) {}
OperationDefinition::OperationDefinition(std::string const& abbr, bool first_is_writable)
     : abbr(abbr), arguments_count(1), first_is_writable(first_is_writable) {}
OperationDefinition::OperationDefinition(std::string const& abbr, bool first_is_writable, bool second_is_writable)
     : abbr(abbr), arguments_count(2), first_is_writable(first_is_writable), second_is_writable(second_is_writable) {}

OperationDefinition OperationDefinition::get_definition(Operations operation) {
    static std::map<Operations, OperationDefinition> defs = {
        { Operations::ADD,      OperationDefinition{ "ADD",     true, false     }},
        { Operations::SUB,      OperationDefinition{ "SUB",     true, false     }},
        { Operations::MOD,      OperationDefinition{ "MOD",     true, false     }},
        { Operations::MUL,      OperationDefinition{ "MUL",     true, false     }},
        { Operations::MULU,     OperationDefinition{ "MULU",    true, false     }},
        { Operations::DIV,      OperationDefinition{ "DIV",     true, false     }},
        { Operations::DIVU,     OperationDefinition{ "DIVU",    true, false     }},
        { Operations::MOV,      OperationDefinition{ "MOV",     true, false     }},
        { Operations::POP,      OperationDefinition{ "POP",     true            }},
        { Operations::PUSH,     OperationDefinition{ "PUSH",    false           }},
        { Operations::JMP,      OperationDefinition{ "JMP",     false           }},
        { Operations::JMPE,     OperationDefinition{ "JMPE",    false           }},
        { Operations::JMPNE,    OperationDefinition{ "JMPNE",   false           }},
        { Operations::JMPG,     OperationDefinition{ "JMPG",    false           }},
        { Operations::JMPGE,    OperationDefinition{ "JMPGE",   false           }},
        { Operations::AND,      OperationDefinition{ "AND",     true, false     }},
        { Operations::OR,       OperationDefinition{ "OR",      true, false     }},
        { Operations::XOR,      OperationDefinition{ "XOR",     true, false     }},
        { Operations::NEG,      OperationDefinition{ "NEG",     true            }},
        { Operations::SHL,      OperationDefinition{ "SHL",     true, false     }},
        { Operations::RTL,      OperationDefinition{ "RTL",     true, false     }},
        { Operations::SHR,      OperationDefinition{ "SHR",     true, false     }},
        { Operations::RTR,      OperationDefinition{ "RTR",     true, false     }},
        { Operations::SWP,      OperationDefinition{ "SWP",     true, true      }},
        { Operations::CMP,      OperationDefinition{ "CMP",     false, false    }},
        { Operations::INC,      OperationDefinition{ "INC",     true            }},
        { Operations::DEC,      OperationDefinition{ "DEC",     true            }},
        { Operations::NEW,      OperationDefinition{ "NEW",     true, false     }},
        { Operations::DEL,      OperationDefinition{ "DEL",     false           }},
        { Operations::CALL,     OperationDefinition{ "CALL",    false           }},
        { Operations::RET,      OperationDefinition{ "RET"                      }},
        { Operations::ETR,      OperationDefinition{ "ETR"                      }},
        { Operations::LVE,      OperationDefinition{ "LVE"                      }},
        { Operations::HLT,      OperationDefinition{ "HLT"                      }},
        { Operations::OUT,      OperationDefinition{ "OUT",     false           }}
    };

    try {
        return defs.at(operation);
    } catch(...) {
        std::cout << "Operation unknown : " << static_cast<ui32>(operation) << std::endl;
        throw;
        //return { "???" };
    }
}

}
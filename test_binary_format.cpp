#include <stdio.h>
#include <string.h>

struct CMD
{
    unsigned int imm : 1;
    unsigned int reg : 1;
    unsigned int mem : 1;
    unsigned int cmd : 5;
};

enum CMD_INFO
{
    IMM  = 0b00000001,
    REG  = 0b00000010,
    RAM  = 0b00000100
};

enum PROCESSOR_COMMANDS
{
    ERROR =  0,
    HLT   =  1,
    OUT   =  2,
    PUSH  =  3,
    POP   =  4,
    ADD   =  5,
    SUB   =  6,
    MULT  =  7,
    DIV   =  8,
    PUSHR =  9,
    POPR  = 10,
    IN    = 11
};

char buffer[30] = {};
// push == 00001;

int main()
{
    FILE* file = fopen("new.asm", "wb");
    CMD Cmd = {};
    const char* command = "push [50]";
    int offset = 0;
    int reg    = 0;
    int value  = 0;
    char register_id = -1;

    char command_name[10] = {};
    sscanf(command, "%s%n", command_name, &offset);
    if (strcmp(command_name, "push") == 0)
    {
        buffer[0] |= PUSH;

        char tmp[10] = {};
        int character_quantity = 0;
        if (sscanf(command + offset, " [%[a-z]]", &tmp) == 1)
        {
            if (strchr(command + offset, '+'))
            {
                buffer[0] |= REG | RAM | IMM;
                sscanf(command + offset, " [+ %d]", &value);
                sscanf(command + offset, " [r%cx]", &register_id);
            }
            else
            {
                Cmd.mem = 1;
                Cmd.reg = 1;
                buffer[0] |= RAM | REG;
                sscanf(command + offset, " [r%cx]", &register_id);
            }
        }
        else if (sscanf(command + offset, " [%d]", &value) == 1)
        {
            Cmd.imm = 1;
            Cmd.mem = 1;
            buffer[0] |= IMM | RAM;
        }
        else if (sscanf(command + offset, " r%cx", &register_id))
        {
            Cmd.reg = 0b1;
            buffer[0] |= REG;
            if (strchr(command + offset, '+'))
            {
                if (sscanf(command + offset, " + %d", &value))
                {
                    Cmd.imm = 1;
                    buffer[0] |= IMM;
                }
            }
        }
        else if (sscanf(command + offset, " %d", &value))
        {
            Cmd.imm = 1;
            buffer[0] |= IMM;
        }
        if (register_id == 'a') register_id = 0;
        if (register_id == 'b') register_id = 1;
        if (register_id == 'c') register_id = 2;
        if (register_id == 'd') register_id = 3;


    }
    printf("cmd            %x\n", Cmd.cmd);
    printf("mem            %x\n", Cmd.mem);
    printf("reg            %x\n", Cmd.reg);
    printf("imm            %x\n", Cmd.imm);
    printf("register_value %x\n", register_id);
    printf("value          %x\n", value);

    char check  = 0;
    char check2 = 0;
    check  |= Cmd.cmd;
    check2 |= Cmd.imm;
    printf("check          %x\n", check);
    printf("check2         %x\n", check2);
    printf("cmd in buffer  %x\n", buffer[0]);
    *(int *)(buffer + 1) = value;

    fwrite(buffer, sizeof(char), 30, file);

    return 0;
}

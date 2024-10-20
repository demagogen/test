#include <stdio.h>
#include <string.h>

struct CMD
{
    unsigned int imm : 1;
    unsigned int reg : 1;
    unsigned int mem : 1;
    unsigned int cmd : 5;
};

char buffer[30] = {};
// push == 00001;

int main()
{
    FILE* file = fopen("new.asm", "wb");
    CMD Cmd = {};
    const char* command = "push 50";
    int offset = 0;
    int reg    = 0;
    int value  = 0;
    char register_id = -1;

    char command_name[10] = {};
    sscanf(command, "%s%n", command_name, &offset);
    if (strcmp(command_name, "push") == 0)
    {
        Cmd.cmd = 0b00001;

        char tmp[10] = {};
        int character_quantity = 0;
        if (sscanf(command + offset, " [%[a-z]]", &tmp) == 1)
        {
            if (strchr(command + offset, '+'))
            {
                Cmd.imm = 0b1;
                Cmd.reg = 0b1;
                Cmd.mem = 0b1;
                sscanf(command + offset, " [+ %d]", &value);
                sscanf(command + offset, " [r%cx]", &register_id);
            }
            else
            {
                Cmd.mem = 0b1;
                Cmd.reg = 0b1;
                sscanf(command + offset, " [r%cx]", &register_id);
            }
            if (register_id == 'a') register_id = 0;
            if (register_id == 'b') register_id = 1;
            if (register_id == 'c') register_id = 2;
            if (register_id == 'd') register_id = 3;
        }
        else if (sscanf(command + offset, " [%d]", &value) == 1)
        {
            Cmd.imm = 0b1;
            Cmd.mem = 0b1;
        }
        else if (sscanf(command + offset, " r%cx", &register_id))
        {
            Cmd.reg = 0b1;
            if (strchr(command + offset, '+'))
            {
                if (sscanf(command + offset, " + %d", &value))
                {
                    Cmd.imm = 0b1;
                }
            }
        }
        else if (sscanf(command + offset, " %d", &value))
        {
            Cmd.imm = 0b1;
        }

        buffer[0] |= Cmd.mem | Cmd.reg | Cmd.imm | Cmd.cmd;
    }
    printf("cmd            %x\n", Cmd.cmd);
    printf("mem            %x\n", Cmd.mem);
    printf("reg            %x\n", Cmd.reg);
    printf("imm            %x\n", Cmd.imm);
    printf("register_value %x\n", register_id);
    printf("value          %x\n", value);

    buffer[0] |= Cmd.cmd | Cmd.mem | Cmd.reg | Cmd.imm;
    printf("%b\n", buffer[0]);
    *(int *)(buffer + 1) = value;

    fwrite(buffer, sizeof(char), 30, file);

    return 0;
}

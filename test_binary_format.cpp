#include <stdio.h>
#include <string.h>

struct CMD
{
    unsigned int mem : 1;
    unsigned int reg : 1;
    unsigned int imm : 1;
    unsigned int cmd : 5;
};
// push == 00001;

int main()
{
    CMD Cmd = {};
    const char* command = "push 50";
    int offset = 0;
    int reg    = 0;
    int value  = 0;

    char command_name[10] = {};
    sscanf(command, "%s%n", command_name, &offset);
    printf("%s\n", command_name);
    printf("%d\n", offset);
    if (strcmp(command_name, "push") == 0)
    {
        Cmd.cmd = 0b00001;

        char tmp = 0;
        int character_quantity = 0;
        if (sscanf(command + offset, "_%c%n", &tmp, &character_quantity) == 2){
            if (tmp == '[')
            {
                offset += character_quantity;
                Cmd.mem = 0b1;
                tmp = 0;
                character_quantity = 0;
            }
        }
        if (sscanf(command + offset, "r%cx%n", &tmp, &character_quantity) == 2)
        {
            offset += character_quantity;
            Cmd.reg = 0b1;
            if (tmp = 'a') reg = 0;
            if (tmp = 'b') reg = 1;
            if (tmp = 'c') reg = 2;
            if (tmp = 'd') reg = 3;
            tmp = 0;
            character_quantity = 0;
        }
        if (sscanf(command + offset, "_%c%n", &tmp, &character_quantity) == 2)
        {
            if (tmp == '+')
            {
                Cmd.imm = 0b1;
                offset += character_quantity;
                tmp = 0;
                character_quantity = 0;
            }
        }
        if (sscanf(command + offset, "_%d%n", &value, &character_quantity) == 2)
        {
            offset += character_quantity;
        }
        if (sscanf(command + offset, "_%c%n", &tmp, &character_quantity) == 2)
        {
            if (tmp != ']')
            {
                printf("syntax error in line\n%s\nno ']'\n", command);
            }
        }
    }
    printf("cmd %b\n", Cmd.cmd);
    printf("mem %b\n", Cmd.mem);
    printf("reg %b\n", Cmd.reg);
    printf("imm %b\n", Cmd.imm);
    printf("value %d\n", value);

    return 0;
}

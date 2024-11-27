#ifndef COMMAND_H
#define COMMAND_H

typedef void (*CommandFunction)();

struct Command {
    char identifier;
    CommandFunction execute;
};

#endif
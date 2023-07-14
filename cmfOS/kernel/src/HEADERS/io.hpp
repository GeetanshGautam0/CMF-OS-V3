#pragma once
#include "std.hpp"

#ifndef _IO_HPP
    #define _IO_HPP
#endif

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void io_wait();

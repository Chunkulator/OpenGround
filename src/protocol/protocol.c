/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Deviation is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Deviation.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "interface.h"
#include <stdlib.h>

#define PROTODEF(proto, module, map, cmd, name) {module, cmd, name},
const struct{
    enum Radio module;
    uintptr_t (*cmd)(enum ProtoCmds);
    const char* name;
}Protocols[PROTOCOL_COUNT] = {
    { 0, NULL, "None"},
    #include "protocol.h"
};
#undef PROTODEF

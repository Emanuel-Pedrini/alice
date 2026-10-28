x : i32 = 1 + 2 * 3 / 6
/*
long int x = 1 + 2 * 3 / 6;
*/

class Character
    name : string
    fn say(msg : string) -> null
        print(self.name .. ": " .. msg)
    end
end

/*
#include <stdio.h>
#include <stdlib.h>
typedef struct StructCharacter
{
    name : __AliceString
} Character;

void Character_say(Character* self, msg : __AliceString)
{
    __AliceString _1 = __ConcatenateAString(self -> name, __AliceStringFrom(": "));
    __AliceString _2 = __ConcatenateAString(_1, msg);
    printf("%s", _2 -> Chars);
    free(_1);
    free(_2);
}
*/
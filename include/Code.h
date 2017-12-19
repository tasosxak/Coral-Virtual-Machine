#ifndef CODE_H
#define CODE_H
#include <string>
#include "symbol.h"
class Code: public symbol
{
    public:
        Code();
        Code(const std::string& code);
        virtual ~Code();
        std::string className();
        std::string getCode();
    protected:

    private:
        std::string code;
};

#endif // CODE_H

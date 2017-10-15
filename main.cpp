#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <regex>

#include "Integer.h"
#include "Double.h"
#include "Literal.h"
#include "vs_stack.h"
#include "Code.h"
#include "symbol.h"
#include "hash_table.h"
#include "Class.h"

using namespace std;

//Integer Registers
int eax = 0;
int ebx = 0;
int ecx = 0;
int edx = 0;

//Real Registers
double feax = 0;
double febx = 0;
double fecx = 0;
double fedx = 0;




//End of Code register
int eoc = -1;

//Next Instruction Counter
int ic = 0;

int ebp = 0;

//Flags
bool ZF = 0;
bool LF = 0;
bool GF = 0;
bool EF = 0;
bool LEF = 0;
bool GEF = 0;
bool NEF = 0;

vs_stack st;
hash_table hs;
hash_table class_defs;

hash_table* cur_hs = &hs;
Class* cur_class;

template <class Container>
void split(const std::string& str, Container& cont)
{
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(cont));
}

int Loader(char* filename)
{

    ifstream bytecode(filename);
    string line;


    while (getline(bytecode,line))
    {
        cout<<"CODE: "<<line<<endl;
        st.push(new Code(line));
        eoc++;

    }


    cout<<"EOC : "<<eoc<<endl;
    return 0;
}


int compile()
{
    int main_location;
    string linecode;
    std::smatch tokens;

    while(ic < eoc)
    {

        linecode = ((Code *) st.see(ic))->getCode();

        if(linecode == "call _main:")
            main_location = ic;

        if(std::regex_search(linecode,tokens,std::regex("class (.*)"))){


            symbol* _class = new symbol(linecode);
            _class->setLine(ic);
            cur_hs->add(_class);

            cur_class = new Class(tokens[0]);
            cur_class->mht = new hash_table();
            class_defs.add(cur_class);
            cur_hs = cur_class->mht;

        }
        if(std::regex_match(linecode, std::regex("endclass"))){

            cur_hs = &hs;
            cur_class = NULL;
        }

        if (std::regex_match (linecode, std::regex("(_)(.*)(:)") ))
        {


            symbol* method = new symbol(linecode);

            method->setLine(ic);
            cur_hs->add(method);
        }
        else if (std::regex_match (linecode, std::regex("(@)(.*)(:)") ))
        {

            symbol* label = new symbol(linecode);
            label->setLine(ic);
            cur_hs->add(label);
        }

        ic++;
    }
    cout<<"MAIN_CALL: "<<main_location;
    ebp = eoc;
    ic = main_location;

    return 1;
}

inline bool isInteger(const std::string & s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

    char * p ;
    strtol(s.c_str(), &p, 10) ;

    return (*p == 0) ;
}

bool isFloat( string myString )
{
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}



int main()
{
    symbol* s,s1,s2,s3;
    string linecode;
    string p,p1,p2,p3,p4;

    Loader("filename.txt");
    compile();
    cout<<"ESP: "<<st.esp()<<endl;
    while(ic < eoc)
    {

        std::vector<std::string> tokens;
        cout<<"IC : "<<ic << endl;
        linecode = ((Code *) st.see(ic))->getCode();
        cout<<linecode<<endl;
        split(linecode,tokens);

        p = tokens[0];


        if(p == "mov")
        {

            // mov eax 5

            p1 = tokens[1];
            p2 = tokens[2];

            if(p1 == "eax")
            {

                if(isInteger(p2))
                {

                    eax = stoi(p2);
                    cout<<"eax is: "<<eax<<endl;

                }
                else if (p2 == "feax")
                {


                    eax = (int) stoi(p2);
                    cout<<"eax is: "<<eax<<endl;

                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    eax = ((Integer*) s) ->getValue();
                    cout<<"eax is: "<<eax<<endl;
                }

            }
            else if(p1 == "ebx")
            {
                if(isInteger(p2))
                {

                    ebx = stoi(p2);
                    cout<<"ebx is: "<<ebx<<endl;

                }
                else if (p2 == "febx")
                {


                    ebx = (int) stoi(p2);
                    cout<<"ebx is: "<<ebx<<endl;

                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    ebx = ((Integer*) s) ->getValue();
                    cout<<"ebx is: "<<ebx<<endl;
                }


            }
            else if(p1 == "ecx")
            {

                if(isInteger(p2))
                {

                    ecx = stoi(p2);
                    cout<<"ecx is: "<<ecx<<endl;

                }
                else if (p2 == "fecx")
                {


                    ecx = (int) stoi(p2);
                    cout<<"ecx is: "<<ecx<<endl;

                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    ecx = ((Integer*) s)->getValue();

                }

            }
            else if(p1 == "edx")
            {
                if(isInteger(p2))
                {

                    ebx = stoi(p2);
                    cout<<"edx is: "<<edx<<endl;

                }
                else if (p2 == "fedx")
                {


                    edx = (int) stoi(p2);
                    cout<<"edx is: "<<edx<<endl;

                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    edx = ((Integer*) s) ->getValue();
                    cout<<"edx is: "<<edx<<endl;
                }


            }
            else if(p1 == "ebp")
            {
                if(isInteger(p2))
                {

                    ebp = stoi(p2);
                    cout<<"ebp is: "<<ebp<<endl;

                }
                else if( p2 == "esp")
                {

                    ebp = st.esp();
                    cout<<"ebp is: "<<ebp<<endl;


                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    ebp = ((Integer*) s) ->getValue();
                    cout<<"ebp is: "<<ebp<<endl;
                }


            }
            else if(p1 == "esp")
            {
                if(isInteger(p2))
                {

                    st.setesp(stoi(p2));
                    cout<<"esp is: "<<st.esp()<<endl;

                }
                else if( p2 == "ebp")
                {

                    st.setesp(ebp);
                    cout<<"esp is: "<<st.esp()<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    st.setesp(((Integer*) s) ->getValue());
                    cout<<"esp is: "<<st.esp()<<endl;
                }


            }
            else if( p1 == "feax")
            {


                if(isFloat(p2))
                {

                    feax = stod(p2);
                    cout<<"feax is: "<<feax<<endl;

                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    feax = ((Double*) s) ->getValue();
                    cout<<"feax is: "<<feax<<endl;
                }

            }
            else if( p1 == "febx")
            {


                if(isFloat(p2))
                {

                    febx = stod(p2);
                    cout<<"feax is: "<<feax<<endl;

                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    febx = ((Double*) s) ->getValue();
                    cout<<"febx is: "<<febx<<endl;
                }

            }
            else if( p1 == "fecx")
            {


                if(isFloat(p2))
                {

                    feax = stod(p2);
                    cout<<"fecx is: "<<fecx<<endl;

                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    fecx = ((Double*) s) ->getValue();
                    cout<<"fecx is: "<<fecx<<endl;
                }

            }
            else if( p1 == "fedx")
            {


                if(isFloat(p2))
                {

                    feax = stod(p2);
                    cout<<"fedx is: "<<fedx<<endl;

                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    fedx = ((Double*) s) ->getValue();
                    cout<<"fedx is: "<<fedx<<endl;
                }

            }
            else
            {

                s = cur_hs->lookfor(p1);
                if(p2 =="eax")
                {
                    ((Integer*) s)->setValue(eax);
                    cout<<((Integer*) s)->getName()<<"is :"<<((Integer*) s)->getValue();

                }
                else if(p2 == "ebx")
                {
                    ((Integer*) s)->setValue(ebx);
                    cout<<((Integer*) s)->getName()<<"is :"<<((Integer*) s)->getValue();
                }
                else if(p2 == "ecx")
                {
                    ((Integer*) s)->setValue(ecx);
                    cout<<((Integer*) s)->getName()<<"is :"<<((Integer*) s)->getValue();
                }
                else if(p2 == "edx")
                {
                    ((Integer*) s)->setValue(edx);
                    cout<<((Integer*) s)->getName()<<"is :"<<((Integer*) s)->getValue();
                }
                else if(p2 == "feax")
                {
                    ((Double*) s)->setValue(feax);
                    cout<<((Double*) s)->getName()<<"is :"<<((Double*) s)->getValue();
                }
                else if(p2 == "febx")
                {
                    ((Double*) s)->setValue(febx);
                    cout<<((Double*) s)->getName()<<"is :"<<((Double*) s)->getValue();
                }
                else if(p2 == "fecx")
                {
                    ((Double*) s)->setValue(fecx);
                    cout<<((Double*) s)->getName()<<"is :"<<((Double*) s)->getValue();
                }
                else if(p2 == "fedx")
                {
                    ((Double*) s)->setValue(fedx);
                    cout<<((Double*) s)->getName()<<"is :"<<((Double*) s)->getValue();
                }



            }


        }
        if(p == "movs")
        {
            // movs eax ebp + 4

            p1 = tokens[1];
            p2 = tokens[2];
            p3 = tokens[3];
            p4 = tokens[4];

            if(p2 == "ebp")
            {

                if(p3 == "+")
                {

                    if(p1 == "eax")
                    {

                        eax = ((Integer*)st.see(ebp - stoi(p4) - 2))->getValue();
                    }
                    else if(p4 == "ebx")
                    {

                        ebx = ((Integer*)st.see(ebp - stoi(p4) - 2))->getValue();
                    }
                    else if(p4 == "ecx")
                    {

                        ecx = ((Integer*)st.see(ebp - stoi(p4) - 2))->getValue();
                    }
                    else if(p4 == "edx")
                    {

                        edx = ((Integer*)st.see(ebp - stoi(p4) - 2))->getValue();
                    }
                    else if(p4 == "feax")
                    {

                        feax = ((Double*)st.see(ebp - stoi(p4) - 2))->getValue();
                    }
                    else if(p4 == "febx")
                    {

                        febx = ((Double*)st.see(ebp - stoi(p4) - 2))->getValue();
                    }
                    else if(p4 == "fecx")
                    {

                        fecx = ((Double*)st.see(ebp - stoi(p4) - 2))->getValue();
                    }
                    else if(p4 == "fedx")
                    {

                        fedx = ((Integer*)st.see(ebp - stoi(p4) - 2))->getValue();
                    }

                }
                else if(p3 == "-")
                {

                    if(p1 == "eax")
                    {

                        eax = ((Integer*)st.see(ebp + stoi(p4)))->getValue();
                    }
                    else if(p4 == "ebx")
                    {

                        ebx = ((Integer*)st.see(ebp + stoi(p4)))->getValue();
                    }
                    else if(p4 == "ecx")
                    {

                        ecx = ((Integer*)st.see(ebp + stoi(p4)))->getValue();
                    }
                    else if(p4 == "edx")
                    {

                        edx = ((Integer*)st.see(ebp + stoi(p4)))->getValue();
                    }
                    else if(p4 == "feax")
                    {

                        feax = ((Double*)st.see(ebp + stoi(p4)))->getValue();
                    }
                    else if(p4 == "febx")
                    {

                        febx = ((Double*)st.see(ebp + stoi(p4)))->getValue();
                    }
                    else if(p4 == "fecx")
                    {

                        fecx = ((Double*)st.see(ebp + stoi(p4)))->getValue();
                    }
                    else if(p4 == "fedx")
                    {

                        fedx = ((Double*)st.see(ebp + stoi(p4)))->getValue();
                    }


                }


            }


        }
        else if(p == "strs")
        {

            //str ebp - 4 eax

            p1 = tokens[1];
            p2 = tokens[2];
            p3 = tokens[3];
            p4 = tokens[4];

            if(p1 == "ebp")
            {

                if(p2 == "+")
                {

                    if(p4 == "eax")
                    {

                        ((Integer*)st.see(ebp - stoi(p3) - 2))->setValue(eax);
                    }
                    else if(p4 == "ebx")
                    {

                        ((Integer*)st.see(ebp - stoi(p3) - 2))->setValue(ebx);
                    }
                    else if(p4 == "ecx")
                    {

                        ((Integer*)st.see(ebp - stoi(p3) - 2))->setValue(ecx);
                    }
                    else if(p4 == "edx")
                    {

                        ((Integer*)st.see(ebp - stoi(p3) - 2))->setValue(edx);
                    }
                    else if(p4 == "feax")
                    {

                        ((Double*)st.see(ebp - stoi(p3) - 2))->setValue(feax);
                    }
                    else if(p4 == "febx")
                    {

                        ((Double*)st.see(ebp - stoi(p3) - 2))->setValue(febx);
                    }
                    else if(p4 == "fecx")
                    {

                        ((Double*)st.see(ebp - stoi(p3) - 2))->setValue(fecx);
                    }
                    else if(p4 == "fedx")
                    {

                        ((Double*)st.see(ebp - stoi(p3) - 2))->setValue(fedx);
                    }

                }
                else if(p2 == "-")
                {

                    if(p4 == "eax")
                    {

                        ((Integer*)st.see(ebp + stoi(p3)))->setValue(eax);
                    }
                    else if(p4 == "ebx")
                    {

                        ((Integer*)st.see(ebp + stoi(p3)))->setValue(ebx);
                    }
                    else if(p4 == "ecx")
                    {

                        ((Integer*)st.see(ebp + stoi(p3)))->setValue(ecx);
                    }
                    else if(p4 == "edx")
                    {

                        ((Integer*)st.see(ebp + stoi(p3)))->setValue(edx);
                    }
                    else if(p4 == "feax")
                    {

                        ((Double*)st.see(ebp + stoi(p3)))->setValue(feax);
                    }

                    else if(p4 == "febx")
                    {

                        ((Double*)st.see(ebp + stoi(p3)))->setValue(febx);
                    }

                    else if(p4 == "fecx")
                    {

                        ((Double*)st.see(ebp + stoi(p3)))->setValue(fecx);
                    }

                    else if(p4 == "fedx")
                    {

                        ((Double*)st.see(ebp + stoi(p3)))->setValue(fedx);
                    }



                }

            }



        }
        else if(p == "set")
        {
            p1 = tokens[1];
            p2 = tokens[2];
            p3 = tokens[3];

            if(p2 == "i")
                cur_hs->add(new Integer(p1,stoi(p3)));
            else if ( p2 == "d")
                cur_hs->add(new Double(p1,stod(p3)));
            else if( p2 == "r")
                cur_hs->add(new Reference(p1,class_defs.lookfor(p3));

        }
        else if(p == "push")
        {

            p1 = tokens[1];
            p2 = tokens[2];

            if(p1 == "r")
            {
                if(p2 == "eax")
                    st.push(new Integer(eax));
                else if(p2 == "ebx")
                    st.push(new Integer(ebx));
                else if(p2 == "ecx")
                    st.push(new Integer(ecx));
                else if(p2 == "edx")
                    st.push(new Integer(edx));
                else if(p2 == "ebp")
                {
                    st.push(new Integer(ebp));
                    cout<<"ebp is: "<<ebp<<endl;

                }
                else if(p2 == "feax")
                    st.push(new Double(feax));
                else if(p2 == "febx")
                    st.push(new Double(febx));
                else if(p2 == "fecx")
                    st.push(new Double(fecx));
                else if(p2 == "fedx")
                    st.push(new Double(fedx));
            }
            else if(p1 == "m")
            {

                s = cur_hs->lookfor(p2);
                st.push(s);

            }
            else if(p1 == "i")
            {

                st.push(new Integer(stoi(p2)));

            }
            else if(p1 == "d")
            {

                st.push(new Double(stod(p2)));

            }
            else if(p1 == "s")
            {

                st.push(new Literal(p2));

            }


        }
        else if(p == "pop")
        {


            p1 = tokens[1];
            p2 = tokens[2];

            if(p1 == "r")
            {

                if(p2 == "eax")
                {
                    eax = ((Integer*)st.pop())->getValue();
                    cout<<"eax is: "<<eax<<endl;

                }
                else if(p2 == "ebx")
                {
                    ebx = ((Integer*)st.pop())->getValue();
                    cout<<"ebx is: "<<ebx<<endl;


                }
                else if(p2 == "ecx")
                {

                    ecx = ((Integer*)st.pop())->getValue();
                    cout<<"ecx is: "<<ecx<<endl;

                }
                else if(p2 == "edx")
                {
                    edx = ((Integer*)st.pop())->getValue();
                    cout<<"edx is: "<<edx<<endl;

                }
                else if(p2 == "ebp")
                {
                    ebp = ((Integer*)st.pop())->getValue();
                    cout<<"ebp is: "<<ebp<<endl;

                }
                else if(p2 == "feax")
                {
                    feax = ((Double*)st.pop())->getValue();

                }
                else if(p2 == "febx")
                {
                    febx = ((Double*)st.pop())->getValue();

                }
                else if(p2 == "fecx")
                {
                    fecx = ((Double*)st.pop())->getValue();

                }
                else if(p2 == "fedx")
                {
                    fedx = ((Double*)st.pop())->getValue();

                }

            }
            else if(p1 == "i")
            {


                ((Integer *) cur_hs->lookfor(p2))->setValue(((Integer*)st.pop())->getValue());

            }
            else if(p1 == "d")
            {


                ((Double *) cur_hs->lookfor(p2))->setValue(((Double*)st.pop())->getValue());

            }

        }
        else if(p == "add")
        {
            p1 = tokens[1];
            p2 = tokens[2];

            if(p1 == "eax")
            {
                if(isInteger(p2))
                {

                    eax += stoi(p2);
                    cout<<"eax is: "<<eax<<endl;

                }
                else if(p2 == "eax")
                {

                    eax+=eax;
                    cout<<"eax is: "<<eax<<endl;
                }
                else if(p2 == "ebx")
                {

                    eax+=ebx;
                    cout<<"eax is: "<<eax<<endl;
                }
                else if(p2 == "ecx")
                {

                    eax+=ecx;
                    cout<<"eax is: "<<eax<<endl;
                }
                else if(p2 == "edx")
                {

                    eax+=edx;
                    cout<<"eax is: "<<eax<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    eax += ((Integer*) s) ->getValue();
                    cout<<"eax is: "<<eax<<endl;
                }


            }
            else if(p1 == "ebx")
            {
                if(isInteger(p2))
                {

                    ebx += stoi(p2);
                    cout<<"ebx is: "<<ebx<<endl;

                }
                else if(p2 == "eax")
                {

                    ebx+=eax;
                    cout<<"ebx is: "<<ebx<<endl;
                }
                else if(p2 == "ebx")
                {

                    ebx+=ebx;
                    cout<<"ebx is: "<<ebx<<endl;
                }
                else if(p2 == "ecx")
                {

                    ebx+=ecx;
                    cout<<"ebx is: "<<ebx<<endl;
                }
                else if(p2 == "edx")
                {

                    ebx+=edx;
                    cout<<"ebx is: "<<ebx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    ebx += ((Integer*) s) ->getValue();
                    cout<<"ebx is: "<<ebx<<endl;
                }

            }
            else if(p1 == "ecx")
            {
                if(isInteger(p2))
                {

                    ecx += stoi(p2);
                    cout<<"ecx is: "<<ecx<<endl;

                }
                else if(p2 == "eax")
                {

                    ecx+=eax;
                    cout<<"ecx is: "<<ecx<<endl;
                }
                else if(p2 == "ebx")
                {

                    ecx+=ebx;
                    cout<<"ecx is: "<<ecx<<endl;
                }
                else if(p2 == "ecx")
                {

                    ecx+=ecx;
                    cout<<"eax is: "<<ecx<<endl;
                }
                else if(p2 == "edx")
                {

                    ecx+=edx;
                    cout<<"ecx is: "<<ecx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    ecx += ((Integer*) s) ->getValue();
                    cout<<"ecx is: "<<ecx<<endl;
                }


            }
            else if(p1 == "edx")
            {

                if(isInteger(p2))
                {

                    edx += stoi(p2);
                    cout<<"edx is: "<<edx<<endl;

                }
                else if(p2 == "eax")
                {

                    edx+=eax;
                    cout<<"edx is: "<<edx<<endl;
                }
                else if(p2 == "ebx")
                {

                    edx+=ebx;
                    cout<<"edx is: "<<edx<<endl;
                }
                else if(p2 == "ecx")
                {

                    edx+=ecx;
                    cout<<"edx is: "<<edx<<endl;
                }
                else if(p2 == "edx")
                {

                    edx+=edx;
                    cout<<"edx is: "<<edx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    edx += ((Integer*) s) ->getValue();
                    cout<<"edx is: "<<edx<<endl;
                }


            }
            else if(p1 == "feax")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    feax += stod(p2);
                    cout<<"feax is: "<<feax<<endl;

                }
                else if(p2 == "eax")
                {

                    feax+=eax;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "ebx")
                {

                    feax+=ebx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "ecx")
                {

                    feax+=ecx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "edx")
                {

                    feax+=edx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "feax")
                {

                    feax+=feax;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "febx")
                {

                    feax+=febx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "fecx")
                {

                    feax+=fecx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "fedx")
                {

                    feax+=fedx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    feax += ((Double*) s) ->getValue();
                    cout<<"feax is: "<<feax<<endl;
                }


            }
            else if(p1 == "febx")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    febx += stod(p2);
                    cout<<"febx is: "<<febx<<endl;

                }
                else if(p2 == "eax")
                {

                    febx+=eax;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "ebx")
                {

                    febx+=ebx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "ecx")
                {

                    febx+=ecx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "edx")
                {

                    febx+=edx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "febx")
                {

                    febx+=febx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "febx")
                {

                    febx+=febx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "fecx")
                {

                    febx+=fecx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "fedx")
                {

                    febx+=fedx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    febx += ((Double*) s) ->getValue();
                    cout<<"febx is: "<<febx<<endl;
                }


            }
            else if(p1 == "fecx")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    fecx += stod(p2);
                    cout<<"fecx is: "<<fecx<<endl;

                }
                else if(p2 == "eax")
                {

                    fecx+=eax;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "ebx")
                {

                    fecx+=ebx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "ecx")
                {

                    fecx+=ecx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "edx")
                {

                    fecx+=edx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "fecx")
                {

                    fecx+=fecx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "fecx")
                {

                    fecx+=fecx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "fecx")
                {

                    fecx+=fecx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "fedx")
                {

                    fecx+=fedx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    fecx += ((Double*) s) ->getValue();
                    cout<<"fecx is: "<<fecx<<endl;
                }


            }
            else if(p1 == "fedx")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    fedx += stod(p2);
                    cout<<"fedx is: "<<fedx<<endl;

                }
                else if(p2 == "eax")
                {

                    fedx+=eax;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "ebx")
                {

                    fedx+=ebx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "ecx")
                {

                    fedx+=ecx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "edx")
                {

                    fedx+=edx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "fedx")
                {

                    fedx+=fedx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "fedx")
                {

                    fedx+=fedx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "fedx")
                {

                    fedx+=fedx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "fedx")
                {

                    fedx+=fedx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    fedx += ((Double*) s) ->getValue();
                    cout<<"fedx is: "<<fedx<<endl;
                }


            }


        }
        else if(p == "sub")
        {
            p1 = tokens[1];
            p2 = tokens[2];

            if(p1 == "eax")
            {
                if(isInteger(p2))
                {

                    eax -= stoi(p2);
                    cout<<"eax is: "<<eax<<endl;

                }
                else if(p2 == "eax")
                {

                    eax-=eax;
                    cout<<"eax is: "<<eax<<endl;
                }
                else if(p2 == "ebx")
                {

                    eax-=ebx;
                    cout<<"eax is: "<<eax<<endl;
                }
                else if(p2 == "ecx")
                {

                    eax-=ecx;
                    cout<<"eax is: "<<eax<<endl;
                }
                else if(p2 == "edx")
                {

                    eax-=edx;
                    cout<<"eax is: "<<eax<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    eax -= ((Integer*) s) ->getValue();
                    cout<<"eax is: "<<eax<<endl;
                }


            }
            else if(p1 == "ebx")
            {
                if(isInteger(p2))
                {

                    ebx -= stoi(p2);
                    cout<<"ebx is: "<<ebx<<endl;

                }
                else if(p2 == "eax")
                {

                    ebx-=eax;
                    cout<<"ebx is: "<<ebx<<endl;
                }
                else if(p2 == "ebx")
                {

                    ebx-=ebx;
                    cout<<"ebx is: "<<ebx<<endl;
                }
                else if(p2 == "ecx")
                {

                    ebx-=ecx;
                    cout<<"ebx is: "<<ebx<<endl;
                }
                else if(p2 == "edx")
                {

                    ebx-=edx;
                    cout<<"ebx is: "<<ebx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    ebx -= ((Integer*) s) ->getValue();
                    cout<<"ebx is: "<<ebx<<endl;
                }

            }
            else if(p1 == "ecx")
            {
                if(isInteger(p2))
                {

                    ecx -= stoi(p2);
                    cout<<"ecx is: "<<ecx<<endl;

                }
                else if(p2 == "eax")
                {

                    ecx-=eax;
                    cout<<"ecx is: "<<ecx<<endl;
                }
                else if(p2 == "ebx")
                {

                    ecx-=ebx;
                    cout<<"ecx is: "<<ecx<<endl;
                }
                else if(p2 == "ecx")
                {

                    ecx-=ecx;
                    cout<<"eax is: "<<ecx<<endl;
                }
                else if(p2 == "edx")
                {

                    ecx-=edx;
                    cout<<"ecx is: "<<ecx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    ecx -= ((Integer*) s) ->getValue();
                    cout<<"ecx is: "<<ecx<<endl;
                }


            }
            else if(p1 == "edx")
            {

                if(isInteger(p2))
                {

                    edx -= stoi(p2);
                    cout<<"edx is: "<<edx<<endl;

                }
                else if(p2 == "eax")
                {

                    edx-=eax;
                    cout<<"edx is: "<<edx<<endl;
                }
                else if(p2 == "ebx")
                {

                    edx-=ebx;
                    cout<<"edx is: "<<edx<<endl;
                }
                else if(p2 == "ecx")
                {

                    edx-=ecx;
                    cout<<"edx is: "<<edx<<endl;
                }
                else if(p2 == "edx")
                {

                    edx-=edx;
                    cout<<"edx is: "<<edx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    edx -= ((Integer*) s) ->getValue();
                    cout<<"edx is: "<<edx<<endl;
                }


            }
            else if(p1 == "feax")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    feax -= stod(p2);
                    cout<<"feax is: "<<feax<<endl;

                }
                else if(p2 == "eax")
                {

                    feax-=eax;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "ebx")
                {

                    feax-=ebx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "ecx")
                {

                    feax-=ecx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "edx")
                {

                    feax-=edx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "feax")
                {

                    feax-=feax;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "feax")
                {

                    feax-=feax;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "feax")
                {

                    feax-=feax;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "feax")
                {

                    feax-=feax;
                    cout<<"feax is: "<<feax<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    feax -= ((Double*) s) ->getValue();
                    cout<<"feax is: "<<feax<<endl;
                }


            }
            else if(p1 == "febx")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    febx -= stod(p2);
                    cout<<"febx is: "<<febx<<endl;

                }
                else if(p2 == "eax")
                {

                    febx-=eax;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "ebx")
                {

                    febx-=ebx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "ecx")
                {

                    febx-=ecx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "edx")
                {

                    febx-=edx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "febx")
                {

                    febx-=febx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "febx")
                {

                    febx-=febx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "febx")
                {

                    febx-=febx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "febx")
                {

                    febx-=febx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    febx -= ((Double*) s) ->getValue();
                    cout<<"febx is: "<<febx<<endl;
                }


            }
            else if(p1 == "fecx")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    fecx -= stod(p2);
                    cout<<"fecx is: "<<fecx<<endl;

                }
                else if(p2 == "eax")
                {

                    fecx-=eax;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "ebx")
                {

                    fecx-=ebx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "ecx")
                {

                    fecx-=ecx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "edx")
                {

                    fecx-=edx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "fecx")
                {

                    fecx-=fecx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "fecx")
                {

                    fecx-=fecx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "fecx")
                {

                    fecx-=fecx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "fecx")
                {

                    fecx-=fecx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    fecx -= ((Double*) s) ->getValue();
                    cout<<"fecx is: "<<fecx<<endl;
                }


            }
            else if(p1 == "fedx")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    fedx -= stod(p2);
                    cout<<"fedx is: "<<fedx<<endl;

                }
                else if(p2 == "eax")
                {

                    fedx-=eax;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "ebx")
                {

                    fedx-=ebx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "ecx")
                {

                    fedx-=ecx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "edx")
                {

                    fedx-=edx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "fedx")
                {

                    fedx-=fedx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "fedx")
                {

                    fedx-=fedx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "fedx")
                {

                    fedx-=fedx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "fedx")
                {

                    fedx-=fedx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    fedx -= ((Double*) s) ->getValue();
                    cout<<"fedx is: "<<fedx<<endl;
                }


            }


        }

        else if(p == "div")
        {
            double num;
            p1 = tokens[1];
            p2 = tokens[2];

            if(p1 == "eax")
            {
                if(isInteger(p2))
                {
                    num = (double)eax / stoi(p2);
                    eax = (int) num;
                    feax = num;
                    cout<<"eax is: "<<eax<<endl;
                    cout<<"feax is: "<<feax<<endl;

                }
                else if(p2 == "eax")
                {
                    eax = 1;
                    feax = 1;
                    cout<<"eax is: "<<eax<<endl;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "ebx")
                {
                    num = eax / ebx ;
                    eax = (int) num;
                    feax = num;
                    cout<<"eax is: "<<eax<<endl;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "ecx")
                {
                    num = eax / ecx ;
                    eax = (int) num;
                    feax = num;
                    cout<<"eax is: "<<eax<<endl;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "edx")
                {
                    num = eax / edx ;
                    eax = (int) num;
                    feax = num;
                    cout<<"eax is: "<<eax<<endl;
                    cout<<"feax is: "<<feax<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    num = eax / ((Integer*) s) ->getValue();
                    eax = (int) num;
                    feax = num;
                    cout<<"eax is: "<<eax<<endl;
                    cout<<"feax is: "<<feax<<endl;
                }


            }
            else if(p1 == "feax")
            {
                if(isInteger(p2) || isFloat(p2))
                {
                    num = feax / stod(p2);
                    feax = num;
                    cout<<"eax is: "<<eax<<endl;
                    cout<<"feax is: "<<feax<<endl;

                }
                else if(p2 == "feax")
                {
                    feax = 1;
                    cout<<"eax is: "<<eax<<endl;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "febx")
                {
                    num = feax / febx ;
                    feax = num;
                    cout<<"eax is: "<<eax<<endl;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "fecx")
                {
                    num = feax / fecx ;
                    feax = num;

                    cout<<"eax is: "<<eax<<endl;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "fedx")
                {
                    num = feax / fedx ;
                    feax = num;
                    cout<<"eax is: "<<eax<<endl;
                    cout<<"feax is: "<<feax<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    num = feax / ((Double*) s) ->getValue();
                    feax = num;
                    cout<<"eax is: "<<eax<<endl;
                    cout<<"feax is: "<<feax<<endl;
                }


            }


        }
        else if(p == "mul")
        {
            p1 = tokens[1];
            p2 = tokens[2];

            if(p1 == "eax")
            {
                if(isInteger(p2))
                {

                    eax *= stoi(p2);
                    cout<<"eax is: "<<eax<<endl;

                }
                else if(p2 == "eax")
                {

                    eax*=eax;
                    cout<<"eax is: "<<eax<<endl;
                }
                else if(p2 == "ebx")
                {

                    eax*=ebx;
                    cout<<"eax is: "<<eax<<endl;
                }
                else if(p2 == "ecx")
                {

                    eax*=ecx;
                    cout<<"eax is: "<<eax<<endl;
                }
                else if(p2 == "edx")
                {

                    eax*=edx;
                    cout<<"eax is: "<<eax<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    eax *= ((Integer*) s) ->getValue();
                    cout<<"eax is: "<<eax<<endl;
                }


            }
            else if(p1 == "ebx")
            {
                if(isInteger(p2))
                {

                    ebx *= stoi(p2);
                    cout<<"ebx is: "<<ebx<<endl;

                }
                else if(p2 == "eax")
                {

                    ebx*=eax;
                    cout<<"ebx is: "<<ebx<<endl;
                }
                else if(p2 == "ebx")
                {

                    ebx*=ebx;
                    cout<<"ebx is: "<<ebx<<endl;
                }
                else if(p2 == "ecx")
                {

                    ebx*=ecx;
                    cout<<"ebx is: "<<ebx<<endl;
                }
                else if(p2 == "edx")
                {

                    ebx*=edx;
                    cout<<"ebx is: "<<ebx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    ebx *= ((Integer*) s) ->getValue();
                    cout<<"ebx is: "<<ebx<<endl;
                }

            }
            else if(p1 == "ecx")
            {
                if(isInteger(p2))
                {

                    ecx *= stoi(p2);
                    cout<<"ecx is: "<<ecx<<endl;

                }
                else if(p2 == "eax")
                {

                    ecx*=eax;
                    cout<<"ecx is: "<<ecx<<endl;
                }
                else if(p2 == "ebx")
                {

                    ecx*=ebx;
                    cout<<"ecx is: "<<ecx<<endl;
                }
                else if(p2 == "ecx")
                {

                    ecx*=ecx;
                    cout<<"eax is: "<<ecx<<endl;
                }
                else if(p2 == "edx")
                {

                    ecx*=edx;
                    cout<<"ecx is: "<<ecx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    ecx *= ((Integer*) s) ->getValue();
                    cout<<"ecx is: "<<ecx<<endl;
                }


            }
            else if(p1 == "edx")
            {

                if(isInteger(p2))
                {

                    edx *= stoi(p2);
                    cout<<"edx is: "<<edx<<endl;

                }
                else if(p2 == "eax")
                {

                    edx*=eax;
                    cout<<"edx is: "<<edx<<endl;
                }
                else if(p2 == "ebx")
                {

                    edx*=ebx;
                    cout<<"edx is: "<<edx<<endl;
                }
                else if(p2 == "ecx")
                {

                    edx*=ecx;
                    cout<<"edx is: "<<edx<<endl;
                }
                else if(p2 == "edx")
                {

                    edx*=edx;
                    cout<<"edx is: "<<edx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    edx *= ((Integer*) s) ->getValue();
                    cout<<"edx is: "<<edx<<endl;
                }


            }
            else if(p1 == "feax")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    feax *= stod(p2);
                    cout<<"feax is: "<<feax<<endl;

                }
                else if(p2 == "eax")
                {

                    feax*=eax;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "ebx")
                {

                    feax*=ebx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "ecx")
                {

                    feax*=ecx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "edx")
                {

                    feax*=edx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "feax")
                {

                    feax*=feax;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "febx")
                {

                    feax*=febx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "fecx")
                {

                    feax*=fecx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else if(p2 == "fedx")
                {

                    feax*=fedx;
                    cout<<"feax is: "<<feax<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    feax *= ((Double*) s) ->getValue();
                    cout<<"feax is: "<<feax<<endl;
                }


            }
            else if(p1 == "febx")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    febx *= stod(p2);
                    cout<<"febx is: "<<febx<<endl;

                }
                else if(p2 == "eax")
                {

                    febx*=eax;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "ebx")
                {

                    febx*=ebx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "ecx")
                {

                    febx*=ecx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "edx")
                {

                    febx*=edx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "febx")
                {

                    febx*=febx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "feax")
                {

                    febx*=feax;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "fecx")
                {

                    febx*=fecx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else if(p2 == "fedx")
                {

                    febx*=fedx;
                    cout<<"febx is: "<<febx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    febx *= ((Double*) s) ->getValue();
                    cout<<"febx is: "<<febx<<endl;
                }


            }
            else if(p1 == "fecx")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    fecx *= stod(p2);
                    cout<<"fecx is: "<<fecx<<endl;

                }
                else if(p2 == "eax")
                {

                    fecx*=eax;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "ebx")
                {

                    fecx*=ebx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "ecx")
                {

                    fecx*=ecx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "edx")
                {

                    fecx*=edx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "fecx")
                {

                    fecx*=fecx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "feax")
                {

                    fecx*=feax;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "febx")
                {

                    fecx*=febx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else if(p2 == "fedx")
                {

                    fecx*=fedx;
                    cout<<"fecx is: "<<fecx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    fecx *= ((Double*) s) ->getValue();
                    cout<<"fecx is: "<<fecx<<endl;
                }


            }
            else if(p1 == "fedx")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    fedx *= stod(p2);
                    cout<<"fedx is: "<<fedx<<endl;

                }
                else if(p2 == "eax")
                {

                    fedx*=eax;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "ebx")
                {

                    fedx*=ebx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "ecx")
                {

                    fedx*=ecx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "edx")
                {

                    fedx*=edx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "fedx")
                {

                    fedx*=fedx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "feax")
                {

                    fedx*=feax;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "febx")
                {

                    fedx*=febx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else if(p2 == "fecx")
                {

                    fedx*=fecx;
                    cout<<"fedx is: "<<fedx<<endl;
                }
                else
                {

                    s = cur_hs->lookfor(p2);
                    fedx *= ((Double*) s) ->getValue();
                    cout<<"fedx is: "<<fedx<<endl;
                }


            }


        }

        else if( p == "ret")
        {

            p1 = tokens[1];
            ic = ((Integer *) st.pop())->getValue();
            cout<<"RET:"<<ic;
            st.setesp(st.esp() - stoi(p1));
            cout<<" SET:"<<st.esp();
            continue;

        }
        else if( p == "call")
        {

            p1 = tokens[1];
            st.push(new Integer(ic + 1));
            cout<<"CALL: "<<ic+1<<endl;
            ic = ((Code *)cur_hs->lookfor(p1))->getLine();
            continue;
        }
        else if( p == "cmp")
        {
            p1 = tokens[1];
            p2 = tokens[2];

            if(p1 == "eax")
            {
                if(isInteger(p2))
                {
                    if(eax < stoi(p2))
                        LF = 1;
                    else if(eax <= stoi(p2))
                        LEF = 1;
                    else if (eax > stoi(p2))
                        GF = 1;
                    else if (eax >= stoi(p2))
                        GEF = 1;
                    else if(eax == stoi(p2))
                        EF = 1;
                    else
                        NEF = 1;


                }
                else if(p2 == "eax")
                {
                    EF = 1;
                }
                else if(p2 == "ebx")
                {

                    if(eax < ebx)
                        LF = 1;
                    else if(eax <= ebx)
                        LEF = 1;
                    else if (eax > ebx)
                        GF = 1;
                    else if (eax >= ebx)
                        GEF = 1;
                    else if(eax == ebx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "ecx")
                {

                    if(eax < ecx)
                        LF = 1;
                    else if(eax <= ecx)
                        LEF = 1;
                    else if (eax > ecx)
                        GF = 1;
                    else if (eax >= ecx)
                        GEF = 1;
                    else if(eax == ecx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "edx")
                {

                    if(eax < edx)
                        LF = 1;
                    else if(eax <= edx)
                        LEF = 1;
                    else if (eax > edx)
                        GF = 1;
                    else if (eax >= edx)
                        GEF = 1;
                    else if(eax == edx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else
                {
                    int temp_val;
                    s = cur_hs->lookfor(p2);
                    temp_val = ((Integer*) s) ->getValue();

                    if(eax < temp_val)
                        LF = 1;
                    else if(eax <= temp_val)
                        LEF = 1;
                    else if (eax > temp_val)
                        GF = 1;
                    else if (eax >= temp_val)
                        GEF = 1;
                    else if(eax == temp_val)
                        EF = 1;
                    else
                        NEF = 1;
                }


            }
            else if(p1 == "ebx")
            {
                if(isInteger(p2))
                {
                    if(ebx < stoi(p2))
                        LF = 1;
                    else if(ebx <= stoi(p2))
                        LEF = 1;
                    else if (ebx > stoi(p2))
                        GF = 1;
                    else if (ebx >= stoi(p2))
                        GEF = 1;
                    else if(ebx == stoi(p2))
                        EF = 1;
                    else
                        NEF = 1;


                }
                else if(p2 == "ebx")
                {
                    EF = 1;
                }
                else if(p2 == "eax")
                {

                    if(ebx < eax)
                        LF = 1;
                    else if(ebx <= eax)
                        LEF = 1;
                    else if (ebx > eax)
                        GF = 1;
                    else if (ebx >= eax)
                        GEF = 1;
                    else if(ebx == eax)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "ecx")
                {

                    if(ebx < ecx)
                        LF = 1;
                    else if(ebx <= ecx)
                        LEF = 1;
                    else if (ebx > ecx)
                        GF = 1;
                    else if (ebx >= ecx)
                        GEF = 1;
                    else if(ebx == ecx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "edx")
                {

                    if(ebx < edx)
                        LF = 1;
                    else if(ebx <= edx)
                        LEF = 1;
                    else if (ebx > edx)
                        GF = 1;
                    else if (ebx >= edx)
                        GEF = 1;
                    else if(ebx == edx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else
                {
                    int temp_val;
                    s = cur_hs->lookfor(p2);
                    temp_val = ((Integer*) s) ->getValue();

                    if(ebx < temp_val)
                        LF = 1;
                    else if(ebx <= temp_val)
                        LEF = 1;
                    else if (ebx > temp_val)
                        GF = 1;
                    else if (ebx >= temp_val)
                        GEF = 1;
                    else if(ebx == temp_val)
                        EF = 1;
                    else
                        NEF = 1;
                }
            }
            else if(p1 == "ecx")
            {
                if(isInteger(p2))
                {
                    if(ecx < stoi(p2))
                        LF = 1;
                    else if(ecx <= stoi(p2))
                        LEF = 1;
                    else if (ecx > stoi(p2))
                        GF = 1;
                    else if (ecx >= stoi(p2))
                        GEF = 1;
                    else if(ecx == stoi(p2))
                        EF = 1;
                    else
                        NEF = 1;


                }
                else if(p2 == "ecx")
                {
                    EF = 1;
                }
                else if(p2 == "eax")
                {

                    if(ecx < eax)
                        LF = 1;
                    else if(ecx <= eax)
                        LEF = 1;
                    else if (ecx > eax)
                        GF = 1;
                    else if (ecx >= eax)
                        GEF = 1;
                    else if(ecx == eax)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "ebx")
                {

                    if(ecx < ebx)
                        LF = 1;
                    else if(ecx <= ebx)
                        LEF = 1;
                    else if (ecx > ebx)
                        GF = 1;
                    else if (ecx >= ebx)
                        GEF = 1;
                    else if(ecx == ebx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "edx")
                {

                    if(ecx < edx)
                        LF = 1;
                    else if(ecx <= edx)
                        LEF = 1;
                    else if (ecx > edx)
                        GF = 1;
                    else if (ecx >= edx)
                        GEF = 1;
                    else if(ecx == edx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else
                {
                    int temp_val;
                    s = cur_hs->lookfor(p2);
                    temp_val = ((Integer*) s) ->getValue();

                    if(ecx < temp_val)
                        LF = 1;
                    else if(ecx <= temp_val)
                        LEF = 1;
                    else if (ecx > temp_val)
                        GF = 1;
                    else if (ecx >= temp_val)
                        GEF = 1;
                    else if(ecx == temp_val)
                        EF = 1;
                    else
                        NEF = 1;
                }


            }
            else if(p1 == "edx")
            {

                if(isInteger(p2))
                {
                    if(edx < stoi(p2))
                        LF = 1;
                    else if(edx <= stoi(p2))
                        LEF = 1;
                    else if (edx > stoi(p2))
                        GF = 1;
                    else if (edx >= stoi(p2))
                        GEF = 1;
                    else if(edx == stoi(p2))
                        EF = 1;
                    else
                        NEF = 1;


                }
                else if(p2 == "edx")
                {
                    EF = 1;
                }
                else if(p2 == "eax")
                {

                    if(edx < eax)
                        LF = 1;
                    else if(edx <= eax)
                        LEF = 1;
                    else if (edx > eax)
                        GF = 1;
                    else if (edx >= eax)
                        GEF = 1;
                    else if(edx == eax)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "ebx")
                {

                    if(edx < ebx)
                        LF = 1;
                    else if(edx <= ebx)
                        LEF = 1;
                    else if (edx > ebx)
                        GF = 1;
                    else if (edx >= ebx)
                        GEF = 1;
                    else if(edx == ebx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "ecx")
                {

                    if(edx < ecx)
                        LF = 1;
                    else if(edx <= ecx)
                        LEF = 1;
                    else if (edx > ecx)
                        GF = 1;
                    else if (edx >= ecx)
                        GEF = 1;
                    else if(edx == ecx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else
                {
                    int temp_val;
                    s = cur_hs->lookfor(p2);
                    temp_val = ((Integer*) s) ->getValue();

                    if(edx < temp_val)
                        LF = 1;
                    else if(edx <= temp_val)
                        LEF = 1;
                    else if (edx > temp_val)
                        GF = 1;
                    else if (edx >= temp_val)
                        GEF = 1;
                    else if(edx == temp_val)
                        EF = 1;
                    else
                        NEF = 1;
                }

            }
            else if(p1 == "feax")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    if(feax < stod(p2))
                        LF = 1;
                    else if(feax <= stod(p2))
                        LEF = 1;
                    else if (feax > stod(p2))
                        GF = 1;
                    else if (feax >= stod(p2))
                        GEF = 1;
                    else if(feax  == stod(p2))
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "eax")
                {

                    if(feax < eax)
                        LF = 1;
                    else if(feax <= eax)
                        LEF = 1;
                    else if (feax > eax)
                        GF = 1;
                    else if (feax >= eax)
                        GEF = 1;
                    else if(feax == eax)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "ebx")
                {

                    if(feax < ebx)
                        LF = 1;
                    else if(feax <= ebx)
                        LEF = 1;
                    else if (feax > ebx)
                        GF = 1;
                    else if (feax >= ebx)
                        GEF = 1;
                    else if(feax == ebx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "ecx")
                {

                    if(feax < ecx)
                        LF = 1;
                    else if(feax <= ecx)
                        LEF = 1;
                    else if (feax > ecx)
                        GF = 1;
                    else if (feax >= ecx)
                        GEF = 1;
                    else if(feax == ecx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "edx")
                {

                    if(feax < edx)
                        LF = 1;
                    else if(feax <= edx)
                        LEF = 1;
                    else if (feax > edx)
                        GF = 1;
                    else if (feax >= edx)
                        GEF = 1;
                    else if(feax == edx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "feax")
                {
                    EF = 1;

                }
                else if(p2 == "febx")
                {

                    if(feax < febx)
                        LF = 1;
                    else if(feax <= febx)
                        LEF = 1;
                    else if (feax > febx)
                        GF = 1;
                    else if (feax >= febx)
                        GEF = 1;
                    else if(feax == febx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "fecx")
                {

                    if(feax < fecx)
                        LF = 1;
                    else if(feax <= fecx)
                        LEF = 1;
                    else if (feax > fecx)
                        GF = 1;
                    else if (feax >= fecx)
                        GEF = 1;
                    else if(feax == fecx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "fedx")
                {

                    if(feax < fedx)
                        LF = 1;
                    else if(feax <= fedx)
                        LEF = 1;
                    else if (feax > fedx)
                        GF = 1;
                    else if (feax >= fedx)
                        GEF = 1;
                    else if(feax == fedx)
                        EF = 1;
                    else
                        NEF = 1;


                }
                else
                {

                    double temp_val;
                    s = cur_hs->lookfor(p2);
                    temp_val = ((Double*) s) ->getValue();

                    if(feax < temp_val)
                        LF = 1;
                    else if(feax <= temp_val)
                        LEF = 1;
                    else if (feax > temp_val)
                        GF = 1;
                    else if (feax >= temp_val)
                        GEF = 1;
                    else if(feax == temp_val)
                        EF = 1;
                    else
                        NEF = 1;
                }


            }
            else if(p1 == "febx")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    if(febx < stod(p2))
                        LF = 1;
                    else if(febx <= stod(p2))
                        LEF = 1;
                    else if (febx > stod(p2))
                        GF = 1;
                    else if (febx >= stod(p2))
                        GEF = 1;
                    else if(febx  == stod(p2))
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "eax")
                {

                    if(febx < eax)
                        LF = 1;
                    else if(febx <= eax)
                        LEF = 1;
                    else if (febx > eax)
                        GF = 1;
                    else if (febx >= eax)
                        GEF = 1;
                    else if(febx == eax)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "ebx")
                {

                    if(febx < ebx)
                        LF = 1;
                    else if(febx <= ebx)
                        LEF = 1;
                    else if (febx > ebx)
                        GF = 1;
                    else if (febx >= ebx)
                        GEF = 1;
                    else if(febx == ebx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "ecx")
                {

                    if(febx < ecx)
                        LF = 1;
                    else if(febx <= ecx)
                        LEF = 1;
                    else if (febx > ecx)
                        GF = 1;
                    else if (febx >= ecx)
                        GEF = 1;
                    else if(febx == ecx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "edx")
                {

                    if(febx < edx)
                        LF = 1;
                    else if(febx <= edx)
                        LEF = 1;
                    else if (febx > edx)
                        GF = 1;
                    else if (febx >= edx)
                        GEF = 1;
                    else if(febx == edx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "febx")
                {
                    EF = 1;

                }
                else if(p2 == "feax")
                {

                    if(febx < feax)
                        LF = 1;
                    else if(febx <= feax)
                        LEF = 1;
                    else if (febx > feax)
                        GF = 1;
                    else if (febx >= feax)
                        GEF = 1;
                    else if(febx == feax)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "fecx")
                {

                    if(febx < fecx)
                        LF = 1;
                    else if(febx <= fecx)
                        LEF = 1;
                    else if (febx > fecx)
                        GF = 1;
                    else if (febx >= fecx)
                        GEF = 1;
                    else if(febx == fecx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "fedx")
                {

                    if(febx < fedx)
                        LF = 1;
                    else if(febx <= fedx)
                        LEF = 1;
                    else if (febx > fedx)
                        GF = 1;
                    else if (febx >= fedx)
                        GEF = 1;
                    else if(febx == fedx)
                        EF = 1;
                    else
                        NEF = 1;


                }
                else
                {

                    double temp_val;
                    s = cur_hs->lookfor(p2);
                    temp_val = ((Double*) s) ->getValue();

                    if(febx < temp_val)
                        LF = 1;
                    else if(febx <= temp_val)
                        LEF = 1;
                    else if (febx > temp_val)
                        GF = 1;
                    else if (febx >= temp_val)
                        GEF = 1;
                    else if(febx == temp_val)
                        EF = 1;
                    else
                        NEF = 1;
                }



            }
            else if(p1 == "fecx")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    if(fecx < stod(p2))
                        LF = 1;
                    else if(fecx <= stod(p2))
                        LEF = 1;
                    else if (fecx > stod(p2))
                        GF = 1;
                    else if (fecx >= stod(p2))
                        GEF = 1;
                    else if(fecx  == stod(p2))
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "eax")
                {

                    if(fecx < eax)
                        LF = 1;
                    else if(fecx <= eax)
                        LEF = 1;
                    else if (fecx > eax)
                        GF = 1;
                    else if (fecx >= eax)
                        GEF = 1;
                    else if(fecx == eax)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "ebx")
                {

                    if(fecx < ebx)
                        LF = 1;
                    else if(fecx <= ebx)
                        LEF = 1;
                    else if (fecx > ebx)
                        GF = 1;
                    else if (fecx >= ebx)
                        GEF = 1;
                    else if(fecx == ebx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "ecx")
                {

                    if(fecx < ecx)
                        LF = 1;
                    else if(fecx <= ecx)
                        LEF = 1;
                    else if (fecx > ecx)
                        GF = 1;
                    else if (fecx >= ecx)
                        GEF = 1;
                    else if(fecx == ecx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "edx")
                {

                    if(fecx < edx)
                        LF = 1;
                    else if(fecx <= edx)
                        LEF = 1;
                    else if (fecx > edx)
                        GF = 1;
                    else if (fecx >= edx)
                        GEF = 1;
                    else if(fecx == edx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "fecx")
                {
                    EF = 1;

                }
                else if(p2 == "feax")
                {

                    if(fecx < feax)
                        LF = 1;
                    else if(fecx <= feax)
                        LEF = 1;
                    else if (fecx > feax)
                        GF = 1;
                    else if (fecx >= feax)
                        GEF = 1;
                    else if(fecx == feax)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "febx")
                {

                    if(fecx < febx)
                        LF = 1;
                    else if(fecx <= febx)
                        LEF = 1;
                    else if (fecx > febx)
                        GF = 1;
                    else if (fecx >= febx)
                        GEF = 1;
                    else if(fecx == febx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "fedx")
                {

                    if(fecx < fedx)
                        LF = 1;
                    else if(fecx <= fedx)
                        LEF = 1;
                    else if (fecx > fedx)
                        GF = 1;
                    else if (fecx >= fedx)
                        GEF = 1;
                    else if(fecx == fedx)
                        EF = 1;
                    else
                        NEF = 1;


                }
                else
                {

                    double temp_val;
                    s = cur_hs->lookfor(p2);
                    temp_val = ((Double*) s) ->getValue();

                    if(fecx < temp_val)
                        LF = 1;
                    else if(fecx <= temp_val)
                        LEF = 1;
                    else if (fecx > temp_val)
                        GF = 1;
                    else if (fecx >= temp_val)
                        GEF = 1;
                    else if(fecx == temp_val)
                        EF = 1;
                    else
                        NEF = 1;
                }


            }
            else if(p1 == "fedx")
            {

                if(isInteger(p2) || isFloat(p2))
                {

                    if(fedx < stod(p2))
                        LF = 1;
                    else if(fedx <= stod(p2))
                        LEF = 1;
                    else if (fedx > stod(p2))
                        GF = 1;
                    else if (fedx >= stod(p2))
                        GEF = 1;
                    else if(fedx  == stod(p2))
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "eax")
                {

                    if(fedx < eax)
                        LF = 1;
                    else if(fedx <= eax)
                        LEF = 1;
                    else if (fedx > eax)
                        GF = 1;
                    else if (fedx >= eax)
                        GEF = 1;
                    else if(fedx == eax)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "ebx")
                {

                    if(fedx < ebx)
                        LF = 1;
                    else if(fedx <= ebx)
                        LEF = 1;
                    else if (fedx > ebx)
                        GF = 1;
                    else if (fedx >= ebx)
                        GEF = 1;
                    else if(fedx == ebx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "ecx")
                {

                    if(fedx < ecx)
                        LF = 1;
                    else if(fedx <= ecx)
                        LEF = 1;
                    else if (fedx > ecx)
                        GF = 1;
                    else if (fedx >= ecx)
                        GEF = 1;
                    else if(fedx == ecx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "edx")
                {

                    if(fedx < edx)
                        LF = 1;
                    else if(fedx <= edx)
                        LEF = 1;
                    else if (fedx > edx)
                        GF = 1;
                    else if (fedx >= edx)
                        GEF = 1;
                    else if(fedx == edx)
                        EF = 1;
                    else
                        NEF = 1;
                }
                else if(p2 == "fedx")
                {
                    EF = 1;

                }
                else if(p2 == "feax")
                {

                    if(fedx < feax)
                        LF = 1;
                    else if(fedx <= feax)
                        LEF = 1;
                    else if (fedx > feax)
                        GF = 1;
                    else if (fedx >= feax)
                        GEF = 1;
                    else if(fedx == feax)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "febx")
                {

                    if(fedx < febx)
                        LF = 1;
                    else if(fedx <= febx)
                        LEF = 1;
                    else if (fedx > febx)
                        GF = 1;
                    else if (fedx >= febx)
                        GEF = 1;
                    else if(fedx == febx)
                        EF = 1;
                    else
                        NEF = 1;

                }
                else if(p2 == "fecx")
                {

                    if(fedx < fecx)
                        LF = 1;
                    else if(fedx <= fecx)
                        LEF = 1;
                    else if (fedx > fecx)
                        GF = 1;
                    else if (fedx >= fecx)
                        GEF = 1;
                    else if(fedx == fecx)
                        EF = 1;
                    else
                        NEF = 1;


                }
                else
                {

                    double temp_val;
                    s = cur_hs->lookfor(p2);
                    temp_val = ((Double*) s) ->getValue();

                    if(fedx < temp_val)
                        LF = 1;
                    else if(fedx <= temp_val)
                        LEF = 1;
                    else if (fedx > temp_val)
                        GF = 1;
                    else if (fedx >= temp_val)
                        GEF = 1;
                    else if(fedx == temp_val)
                        EF = 1;
                    else
                        NEF = 1;
                }

            }


        }
        else if(p == "jle")
        {

            p1 = tokens[1];

            if(LEF == 1)
            {

                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
                LEF = 0;
                continue;
            }


        }
        else if(p == "je")
        {

            p1 = tokens[1];

            if(EF == 1)
            {

                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
                EF = 0;
                continue;
            }


        }
        else if(p == "jl")
        {

            p1 = tokens[1];

            if(LF == 1)
            {

                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
                LF = 0;
                continue;
            }


        }
        else if(p == "jge")
        {

            p1 = tokens[1];

            if(GEF == 1)
            {

                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
                GEF = 0;
                continue;
            }


        }
        else if(p == "jg")
        {

            p1 = tokens[1];

            if(GF == 1)
            {

                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
                GF = 0;
                continue;
            }


        }
        else if(p == "jne")
        {

            p1 = tokens[1];

            if(NEF == 1)
            {

                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
                NEF = 0;
                continue;
            }


        }
        else if(p == "jcxz")
        {

            p1 = tokens[1];

            if(ecx == 0)
            {

                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
                continue;
            }


        }
        else if(p == "jcxnz")
        {

            p1 = tokens[1];

            if(ecx != 0)
            {

                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
                continue;
            }


        }
        else if(p == "jmp")
        {

            p1 = tokens[1];
            ic = ((Code*)cur_hs->lookfor(p1))->getLine();
            continue;


        }
        else if(p == "$jle")
        {

            p1 = tokens[1];

            if(LEF == 1)
            {

                ic += stoi(p1);
                LEF = 0;
                continue;
            }


        }
        else if(p == "$je")
        {

            p1 = tokens[1];

            if(EF == 1)
            {

                ic += stoi(p1);
                EF = 0;
                continue;

            }


        }
        else if(p == "$jl")
        {

            p1 = tokens[1];

            if(LF == 1)
            {

                ic += stoi(p1);
                LF = 0;
                continue;
            }


        }
        else if(p == "$jge")
        {

            p1 = tokens[1];

            if(GEF == 1)
            {

                ic += stoi(p1);
                GEF = 0;
                continue;
            }


        }
        else if(p == "$jg")
        {

            p1 = tokens[1];

            if(GF == 1)
            {

                ic += stoi(p1);
                GF = 0;
                continue;
            }


        }
        else if(p == "$jne")
        {

            p1 = tokens[1];

            if(NEF == 1)
            {

                ic += stoi(p1);
                NEF = 0;
                continue;
            }


        }
        else if(p == "$jcxz")
        {

            p1 = tokens[1];

            if(ecx == 0)
            {

                ic += stoi(p1);
                continue;
            }


        }
        else if(p == "$jcxnz")
        {

            p1 = tokens[1];

            if(ecx != 0)
            {

                ic += stoi(p1);
                continue;
            }


        }
        else if(p == "$jmp")
        {

            p1 = tokens[1];
            ic += stoi(p1);
            continue;


        }
        else if(p == "print")
        {

            p1 = tokens[1];

            if(p1 == "i")
            {
                printf("%d",((Integer *)st.pop())->getValue());
            }
            else if(p1 == "d")
            {

               printf("%lf",((Double *)st.pop())->getValue());
            }
            else if(p1 == "s")
            {

                cout<<((Literal *)st.pop())->getValue();


            }


        }
         else if(p == "nl")
        {
            printf("\n");
        }
        else if(p == "class"){

            p1 = tokens[1];
            cur_class = (Class *)class_defs.lookfor(p1);
            cur_hs = cur_class->mht;

        }
        else if(p == "endclass"){


            cur_class = NULL;
            cur_hs = &hs;

        }
        else if(p == "")


        //cout<<"THIS: "<< ((Integer *)st.see(st.esp()))->getValue()<<endl;

        ic++;
    }
    return 0;
}

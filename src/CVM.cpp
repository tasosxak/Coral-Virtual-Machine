#include "CVM.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <regex>

/* Built-in classes */
#include "Integer.h"
#include "Method.h"
#include "Object.h"
#include "Field.h"
#include "Int.h"
#include "Double.h"
#include "Float.h"
#include "Literal.h"
#include "vs_stack.h"
#include "Code.h"
#include "symbol.h"
#include "hash_table.h"
#include "Class.h"
#include "Reference.h"

#define COMPILATION 0
#define EXECUTION 0
#define LOAD 0

CVM::CVM(std::string bytecodefile)
{

    this->bytecodefile = bytecodefile;
}

int CVM::start()
{

    this->execute();

    return 0;
}

template <class Container>
void CVM::split(const std::string& str, Container& cont)
{
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(cont));
}


std::string trim(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;

    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
        rit++;

    return std::string(it, rit.base());
}


/*
Loader loads the program, push the code into the stack
*/
int CVM::Loader(string filename)
{
    //cout<<"Start loading ..."<<endl;

    ifstream bytecode(filename);
    std::smatch tokens;
    string line;


    while (getline(bytecode,line))
    {
        line = trim(line);
#if LOAD
        cout<<"CODE: "<<line<<endl;
#endif
        if(std::regex_search(line,tokens,std::regex("use NAME=(.*)")))
        {

            // eoc++;
            Loader(tokens[1]);

            continue;

        }

        st.push(new Code(line));
        eoc++;

    }

#if LOAD
    cout<<"EOC : "<<eoc<<endl;
#endif

    return 0;
}




/*

Compile compiles the program

*/
int CVM::compile()
{
    //cout<<"Start compiling..."<<endl;
    int main_location;
    string linecode;
    std::smatch tokens;

    while(ic <= eoc)
    {

        linecode = ((Code *) st.see(ic))->getCode();

#if COMPILATION
        cout<<"COMPILED: "<<linecode<<endl;
#endif
        if(linecode == "invoke_static main")
        {
            main_location = ic;

        }
        else if(std::regex_search(linecode,tokens,std::regex("//.*"))) //==== Comment
        {

            ic++;
            continue;
        }
        else if(std::regex_search(linecode,tokens,std::regex("(public|abstract|final) class (.*) extends (.*):"))) // ==== Class Definition
        {

            // symbol* _class = new symbol(tokens[1]);
            // _class->setLine(ic);
            // cur_hs->add(_class);

            //---- CLASS INITIALIZATION ---- //
            Class* superclass = NULL;

            if( tokens[2] != "Entity" )
            {
                superclass = (Class*) class_defs.lookfor(tokens[3]);

                if(superclass == NULL)
                {

                    cout<<"Undefined class";
                    exit(1);

                }

                if(superclass->get_acc() == 4){

                    cout<<"final class";
                    exit(1);

                }
            }

            cur_class = new Class(tokens[2],tokens[1],superclass);
            cur_class->setLine(ic);
            cur_class->mht = new hash_table();

            //---- ADD CLASS IN CURRENT SYMBOL TABLE AND CLASS POOL----//
            cur_hs->add(cur_class);
            class_defs.add(cur_class);

            //---- CHANGE CURRENT SYMBOL TABLE ----//
            cur_hs = cur_class->mht;


        }
        else if(std::regex_match(linecode, std::regex("endclass"))) // == End of class definition
        {

            cur_hs = &hs;
            cur_class = NULL;
        }
        else if (std::regex_search(linecode,tokens,std::regex("(private|public) (.*) (.*)[(](.*)[)];"))) //== Method definition
        {

            //---- METHOD INITIALIZATION ---- //
            Method* method = new Method(tokens[3],tokens[1],tokens[2]);
            method->setLine(ic);
            cur_hs->add(method);
        }
        else if (std::regex_search(linecode,tokens,std::regex("(.*) (.*)[(](.*)[)];"))) //== Function definition
        {
            //---- FUNCTION INITIALIZATION ---//
            Method* func = new Method(tokens[2],"public",tokens[1]);
            func->setLine(ic);
            cur_hs->add(func);
        }
        else if (std::regex_search(linecode,tokens,std::regex("label NAME=(.*):"))) //== Label definition
        {
            // cout<<"LABEL :"<<tokens[1]<< " " << ic <<endl;

            //---- LABEL INITIALIZATION FOR LOOPS,IFS ---//
            symbol* label = new symbol(tokens[1]);
            label->setLine(ic);
            cur_hs->add(label);
        }
        else if(std::regex_search(linecode,tokens,std::regex("(public|private) (.*) (.*);"))) //== Field definition
        {

            if( tokens[2] == "I") //=== Integer Field of a class
            {
                cur_hs->add(new Field(tokens[3],tokens[1],new Int(0)));
            }
            else if ( tokens[2] == "D") //=== Double Field of a class
            {
                cur_hs->add(new Field(tokens[3],tokens[1], new Double(0.0)));
            }
            else //=== Reference Field of a class
            {
                Class* y;
                Object* x;

                y = (Class *)class_defs.lookfor(tokens[2]); //Looking for type of reference in current symbol table

                if( y!= NULL)
                {
                    x = NULL;

                }
                else
                {

                    // Exception(ClassNotFound);
                }


                //  cur_hs->add(new Field(tokens[2],tokens[1],new Reference(tokens[2], x)));

            }

        }
        else if( std::regex_search(linecode,tokens,std::regex( "(.*) (.*) = (.*);"))) //== Variable definition
        {

            if( tokens[1] == "I")
            {

                cur_hs->add(new Field(tokens[2],"public",new Int(stoi(tokens[3]))));
            }
            else if ( tokens[1] == "D")
            {
                cur_hs->add(new Field(tokens[2],"public", new Double(stod(tokens[3]))));
            }
            else
            {
                Class* y;
                Object* x;

                y = (Class *)class_defs.lookfor(tokens[1]);

                if( y!= NULL)
                {

                    if( tokens[3] != "NULL")
                    {
                        x = y->createInstance();
                    }
                    else
                    {
                        x = NULL;
                    }
                }
                else
                {

                    // Exception(ClassNotFound);
                }


                cur_hs->add(new Field(tokens[2],"public",new Reference(tokens[2],y, x)));

            }


        }


        ic++;
    }




//cout<<"MAIN_CALL: "<<main_location<<endl;
    ebp = eoc;
    ic = main_location;

// cout<<"End compiling..."<<endl;
    return 1;
}



int CVM::execute()
{

    symbol* s,s1,s2,s3;
    string linecode;
    string init_linecode;
    string p,p1,p2,p3,p4;

    Loader(this->bytecodefile);
    compile();

#if EXECUTION
    cout<<"Start executing..."<<endl;
    cout<<"ESP: "<<st.esp()<<endl;
#endif
    while(ic <= eoc)
    {

        std::vector<std::string> tokens;
#if EXECUTION
        cout<<"IC : "<<ic << endl;
#endif
        linecode = ((Code *) st.see(ic))->getCode();
        init_linecode = linecode;

#if EXECUTION
        cout<<"EXECUTE:"<<linecode<<endl;
#endif
        split(linecode,tokens);

        p = tokens[0];


        if( p == "istore" )

        {
            // istore ivar

            int value;

            p1 = tokens[1];
            s = cur_hs->lookfor(p1);

            value = ((Int*)st.pop())->getValue();

            if( s != NULL)
            {

                ((Int*)(((Field*)s)->get_data()))->setValue(value);

            }
            else
            {

                cout<<"Undefined Variable";
                exit(0);

            }


        }

        else if( p == "dstore" )

        {
            // dstore dvar

            p1 = tokens[1];
            s = cur_hs->lookfor(p1);

            ((Double*)(((Field*)s)->get_data()))->setValue( ((Double*)st.pop())->getValue() );

        }
        else if( p == "fstore" )

        {
            // fstore dvar

            p1 = tokens[1];
            s = cur_hs->lookfor(p1);

            ((Float*)(((Field*)s)->get_data()))->setValue( ((Float*)st.pop())->getValue() );

        }
        else if( p == "rstore" )

        {
            // rstore objref

            p1 = tokens[1];
            s = cur_hs->lookfor(p1);

            ((Reference*)((Field*) s)->get_data())->setPointer((Object*)st.pop());

        }
        else if( p == "istore_" )

        {
            // istore_ index

            p1 = tokens[1];

            if( stoi(p1) > 0 )
            {

                ((Int*)(st.see(ebp - stoi(p1) - 1)))->setValue( ((Int*)st.pop())->getValue());

            }
            else if ( stoi(p1) < 0 )
            {

                ((Int*)(st.see(ebp + stoi(p1))))->setValue( ((Int*)st.pop())->getValue());
            }



        }
        else if( p == "dstore_" )

        {
            // dstore_ index

            p1 = tokens[1];

            if( stoi(p1) > 0 )
            {

                ((Double*)(st.see(ebp - stoi(p1) - 1)))->setValue( ((Double*)st.pop())->getValue());

            }
            else if ( stoi(p1) < 0 )
            {

                ((Double*)(st.see(ebp + stoi(p1))))->setValue( ((Double*)st.pop())->getValue());
            }



        }
        else if( p == "fstore_" )

        {
            // fstore_ index

            p1 = tokens[1];

            if( stoi(p1) > 0 )
            {

                ((Float*)(st.see(ebp - stoi(p1) - 1)))->setValue( ((Float*)st.pop())->getValue());

            }
            else if ( stoi(p1) < 0 )
            {

                ((Float*)(st.see(ebp + stoi(p1))))->setValue( ((Float*)st.pop())->getValue());
            }



        }
        else if( p == "rstore_" )

        {
            // rstore_ index

            p1 = tokens[1];

            if( stoi(p1) > 0 )
            {

                ((Reference*)(st.see(ebp - stoi(p1) - 1)))->setPointer((Object*)st.pop());

            }
            else if ( stoi(p1) < 0 )
            {

                ((Reference*)(st.see(ebp + stoi(p1))))->setPointer((Object*)st.pop());
            }



        }

        else if(p == "iload_")
        {
            // iload_ 5

            p1 = tokens[1];

            if( stoi(p1) > 0 )
            {
                st.push(new Int(((Int*)st.see(ebp - stoi(p1)-1))->getValue()));
            }
            else if ( stoi(p1) < 0 )
            {

                st.push(new Int(((Int*)st.see(ebp + stoi(p1)))->getValue()));
            }

        }
        else if(p == "dload_")
        {

            // dload_ 5

            p1 = tokens[1];

            if( stoi(p1) > 0 )
            {

                st.push(new Double(((Double*)(st.see(ebp - stoi(p1) - 1)))->getValue()));
            }
            else if ( stoi(p1) < 0 )
            {

                st.push(new Double(((Double*)(st.see(ebp + stoi(p1))))->getValue()));
            }


        }
        else if(p == "fload_")
        {

            // dload_ 5

            p1 = tokens[1];

            if( stoi(p1) > 0 )
            {

                st.push(new Float(((Float*)(st.see(ebp - stoi(p1) - 1)))->getValue()));
            }
            else if ( stoi(p1) < 0 )
            {

                st.push(new Float(((Float*)(st.see(ebp + stoi(p1))))->getValue()));
            }


        }
        else if(p == "rload_")
        {
            // rload_ 5

            p1 = tokens[1];

            if( stoi(p1) > 0 )
            {
                st.push((Object*)st.see(ebp - stoi(p1)-1));
            }
            else if ( stoi(p1) < 0 )
            {

                st.push((Object*)st.see(ebp + stoi(p1)));
            }

        }
        else if(p == "iload")
        {
            // iload ivar

            p1 = tokens[1];

            if( s = cur_hs->lookfor(p1))
            {

                st.push(new Int(((Int*)(((Field*)s)->get_data()))->getValue()));
            }

        }
        else if(p == "dload")
        {
            // dload dvar

            p1 = tokens[1];

            if( s = cur_hs->lookfor(p1))
            {

                st.push(new Double(((Double*)(((Field*)s)->get_data()))->getValue()));
            }

        }
        else if(p == "fload")
        {
            // dload dvar

            p1 = tokens[1];

            if( s = cur_hs->lookfor(p1))
            {

                st.push(new Float(((Float*)(((Field*)s)->get_data()))->getValue()));
            }

        }
        else if(p == "rload")
        {
            // rload objref

            p1 = tokens[1];

            if( s = cur_hs->lookfor(p1))
            {
                st.push(  ( (Object*)( ((Reference*)(((Field*)s)->get_data()))->getPointer() ) )  );
            }

        }
        else if(p == "iconst")
        {
            // iconst 5
            p1 = tokens[1];

            st.push(new Int(stoi(p1)));

        }
        else if(p == "dconst")
        {
            // dconst 5.6
            p1 = tokens[1];

            st.push(new Double(stod(p1)));

        }
        else if(p == "fconst")
        {
            // fconst 5.6
            p1 = tokens[1];

            st.push(new Float(stof(p1)));

        }
        else if(p == "rconst")
        {
            // rconst Null
            p1 = tokens[1];

            if( p1 == "Null")
            {

                st.push(new symbol());

            }

        }
        else if(p == "sconst")
        {

            std::smatch tokens;

            if(std::regex_search(init_linecode,tokens,std::regex("sconst \"(.*)\"")))
            {
                st.push(new Literal(tokens[1]));
            }

        }
        else if( p == "pop")
        {

            st.pop();

        }

        else if( p == "iadd")
        {
            int left, right;

            left = ((Int*)st.pop())->getValue();
            right = ((Int*)st.pop())->getValue();

            st.push(new Int(left + right));
        }
        else if( p == "dadd")
        {
            double left, right;

            left = ((Double*)st.pop())->getValue();
            right = ((Double*)st.pop())->getValue();

            st.push(new Double(left+right));

        }
        else if( p == "fadd")
        {
            float left, right;

            left = ((Float*)st.pop())->getValue();
            right = ((Float*)st.pop())->getValue();

            st.push(new Float(left+right));

        }
        else if( p == "sadd")
        {


        }
        else if( p == "isub")
        {
            int left, right;

            right = ((Int*)st.pop())->getValue();
            left = ((Int*)st.pop())->getValue();


            st.push(new Int(left - right));

        }
        else if( p == "dsub")
        {
            double left, right;

            right = ((Double*)st.pop())->getValue();
            left = ((Double*)st.pop())->getValue();


            st.push(new Double(left-right));

        }
        else if( p == "fsub")
        {
            float left, right;

            right = ((Float*)st.pop())->getValue();
            left = ((Float*)st.pop())->getValue();


            st.push(new Float(left-right));

        }
        else if( p == "ssub")
        {


        }
        else if( p == "imul")
        {

            int left, right;

            right = ((Int*)st.pop())->getValue();
            left = ((Int*)st.pop())->getValue();


            st.push(new Int(left * right));

        }
        else if( p == "dmul")
        {
            double left, right;


            right = ((Int*)st.pop())->getValue();
            left = ((Double*)st.pop())->getValue();


            st.push(new Double(left*right));

        }
        else if( p == "fmul")
        {
            float left, right;


            right = ((Float*)st.pop())->getValue();
            left = ((Float*)st.pop())->getValue();


            st.push(new Float(left*right));

        }
        else if( p == "smul")
        {


        }
        else if( p == "idiv")
        {
            int left, right;


            right = ((Int*)st.pop())->getValue();
            left = ((Int*)st.pop())->getValue();


            if(right != 0 )
            {
                st.push(new Int(left / right));
            }
            else
            {

                cout<<"Exception: DivisionByZero";
                exit(0);
            }


        }
        else if( p == "ddiv")
        {
            double left, right;


            right = ((Double*)st.pop())->getValue();
            left = ((Double*)st.pop())->getValue();


            if(right != 0 )
            {
                st.push(new Double(left / right));
            }
            else
            {

                cout<<"Exception: DivisionByZero";
                exit(0);
            }

        }
        else if( p == "fdiv")
        {
            float left, right;


            right = ((Float*)st.pop())->getValue();
            left = ((Float*)st.pop())->getValue();


            if(right != 0 )
            {
                st.push(new Float(left / right));
            }
            else
            {

                cout<<"Exception: DivisionByZero";
                exit(0);
            }

        }
        else if( p == "sdiv")
        {


        }
        else if( p == "i2d")
        {

            Int* x = (Int*)st.pop();
            st.push(new Double((double) x->getValue()));
        }
        else if( p == "i2f")
        {

            Int* x = (Int*)st.pop();
            st.push(new Float((float) x->getValue()));
        }
        else if( p == "d2i")
        {

            Double* x = (Double*)st.pop();
            st.push(new Int((int) x->getValue()));
        }
        else if( p == "d2f")
        {

            Double* x = (Double*)st.pop();
            st.push(new Float((float) x->getValue()));
        }
        else if( p == "f2d")
        {

            Float* x = (Float*)st.pop();
            st.push(new Double((double) x->getValue()));
        }

        else if( p == "ret")
        {
            int val;

            // par1
            // par2
            // ret val
            // ebp
            // local variables
            // Returned object
            // val

            //Get the operand of ret (is the number of parameters)
            val = ((Int*)st.pop())->getValue();

            //Get the returned object
            s = (symbol *)st.pop();

            // mov esp ebp
            st.setesp(ebp);

            // pop ebp
            ebp = ((Int*)st.pop())->getValue();

            //Get the next instruction line
            ic = ((Int*) st.pop())->getValue();


            //Free the memory of parameters
            st.setesp(st.esp() - val);


            cur_hs = (hash_table*) hts.pop();


            //Push the returned object
            st.push(s);

            continue;

        }
        else if( p == "new")
        {

            Class* y;
            Object* x;

            p1 = tokens[1];
            y = (Class *)class_defs.lookfor(p1);

            if(y != NULL)
            {
                if(y->get_acc() == 3){

                    cout<<"Abstract class";
                    exit(1);
                }

                x = y->createInstance();
                x->setClass(y);


            }

            st.push(x);
        }
        else if( p == "invoke_virtual")
        {

            // invoke_virtual Class method
            // par1 <--- this object
            // par2
            // ret val
            // ebp
            // local variables
            // Returned object
            // val

            Method* y;
            Class* cclass;


            cclass = (Class *)class_defs.lookfor(tokens[1]);

            do
            {
                y = (Method *) cclass->mht->lookfor(tokens[2]);

                if ( y == NULL)
                {

                    cclass = cclass->getSuperClass();
                    if(cclass == NULL)
                    {

                        cout<<"Dynamic binding error";
                        exit(1);
                    }

                }
                else
                {

                    break;
                }


            }
            while(true);

            //Check accessibility

            if(cur_hs != cclass->mht)
            {

                if(cur_hs == &hs)
                {

                    if( y->get_acc() != 0)
                    {

                        cout<<"Access to private or protected method";
                        exit(1);
                    }

                }
                else
                {

                    if( y->get_acc() == 2)
                    {

                        cout<<"Access to private method";
                        exit(1);
                    }


                }

            }


            hts.push(cur_hs);
            //hts.push(new hash_table("*"));

            cur_hs = cclass->mht;

            //Push line of next instruction after call
            //cout<<ic;
            st.push(new Int(ic + 1));

            // push ebp & ebp = esp
            st.push(new Int(ebp));
            ebp = st.esp();


            ic = y->getLine();

            continue;


        }
        else if( p == "invoke_special")
        {

            // invoke_special Class method
            // par1 <--- this object
            // par2
            // ret val
            // ebp
            // local variables
            // Returned object
            // val

            Class* superclass = (Class*) ((Object*)st.see(st.esp()))->getClass()->getSuperClass();

            do
            {

                if (superclass->getName() != tokens[1])
                {

                    superclass = superclass->getSuperClass();
                    if(superclass == NULL)
                    {

                        cout<<"Inheretance error";
                        exit(1);
                    }
                }
                else
                {

                    break;
                }


            }
            while(true);


            Method* y =  (Method*)(superclass->mht->lookfor(tokens[2]));

            if(y == NULL)
            {
                cout<<"Undefined method\n";
                break;
            }

            //Check accessibility

            if(cur_hs != superclass->mht)
            {

                if(cur_hs == &hs)
                {

                    if( y->get_acc() != 0)
                    {

                        cout<<"Access to private or protected method";
                        exit(1);
                    }

                }
                else
                {

                    if( y->get_acc() == 2)
                    {

                        cout<<"Access to private method";
                        exit(1);
                    }


                }

            }


            hts.push(cur_hs);
            //hts.push(new hash_table("*"));

            cur_hs = superclass->mht;

            //Push line of next instruction after call
            //cout<<ic;
            st.push(new Int(ic + 1));

            // push ebp & ebp = esp
            st.push(new Int(ebp));
            ebp = st.esp();


            ic = y->getLine();

            continue;


        }
        else if( p == "invoke_static")
        {
            //invoke_static functionname

            p1 = tokens[1]; //function's name
            Method* x = (Method*) (cur_hs->lookfor(p1)); //looking for function in current symbol table
            if(x == NULL)
            {
                cout<<"Undeclared function\n";
                break;
            }

            hts.push(cur_hs);
            //hts.push(new hash_table("*"));

            cur_hs = &hs;


            // cout<<ic;
            st.push(new Int(ic + 1)); //Push line of next instruction after call
            st.push(new Int(ebp)); // push ebp & ebp = esp
            ebp = st.esp();


            ic = x->getLine(); //jumps to function

            continue;
        }/*
        else if( p == "call")
        {
            //call obj _method:
            //call obj obj2 ... method_name
            p1 = tokens[tokens.size() - 1];

            for(int i=1; i< tokens.size()-1; i++)
            {
                //cout<<"MPIKA!"<<endl;
                //cout<<"X"<<endl;
                //cout<<tokens[i]<<endl;
                Reference* x = (Reference *) (((Field*) cur_hs->lookfor(tokens[i]))->get_data());
                // cout<<x->getName();
                //cout<<x;
                //cout<<"k"<<endl;
                if(x == NULL)
                {

                    cout<<"Object doensn't define in current hs\n";
                    break;
                }

                Object* c = (Object *)((x)->getPointer());
                hts.push(cur_hs);
                if(i == 1) hts.push(new hash_table("*"));
//                cout<<(c->get
                cur_hs = (c->getClass())->mht;


            }

            if(tokens.size() == 2)
            {
                hts.push(cur_hs);
                hts.push(nsnew hash_table("*"));
            }

            //Push line of next instruction after call
            //cout<<ic;
            st.push(new Int(ic + 1));

            // push ebp & ebp = esp
            st.push(new Int(ebp));
            ebp = st.esp();

            // cout<<"CALL: "<<ic+1<<endl;
            //cout<<cur_hs->lookfor(p1)<<endl;
            ic = ((symbol *)cur_hs->lookfor(p1))->getLine();

            continue;
        }*/
        else if(p == "print")
        {

            s = (symbol *)st.pop();
            cout<<s->print();

        }
        else if(p == "read")
        {


        }
        else if(p == "getfield")
        {

            //getfield fieldname

            p1 = tokens[1];
            Object* x = (Object*)st.pop();

            if(x != NULL)
            {

                Field* f = (Field*)(x->mht->lookfor(p1));

                if( f != NULL)
                {

                     //Check accessibility

                    if(cur_hs != x->getClass()->mht){

                                if( f->get_acc() != 0){

                                    cout<<"Access to private or protected field";
                                    exit(1);
                                }

                     }



                    string classname = (f->get_data())->className();
                    if( classname == "Int")
                    {

                        st.push( new Int(((Int*)f->get_data())->getValue()) );
                    }
                    else if( classname == "Double")
                    {

                        st.push( new Double(((Double*)f->get_data())->getValue()) );
                    }
                    else if( classname == "Float")
                    {

                        st.push( new Float(((Float*)f->get_data())->getValue()) );
                    }
                }
            }

        }
        else if(p == "putfield")
        {
            //objectref
            //value

            //putfield fieldname

            p1 = tokens[1];
            symbol* value = (symbol*) st.pop();
            Object* x = (Object*)st.pop();

            if(x != NULL)
            {

                Field* f = (Field*)(x->mht->lookfor(p1));

                if( f != NULL )
                {

                     //Check accessibility

                    if(cur_hs != x->getClass()->mht){


                                if( f->get_acc() != 0){

                                    cout<<"Access to private or protected field";
                                    exit(1);
                                }

                     }


                    string classname = (f->get_data())->className();
                    if( classname == "Int")
                    {

                        ((Int*)f->get_data())->setValue(((Int*)value)->getValue());
                    }
                    else if( classname == "Double")
                    {

                        ((Double*)f->get_data())->setValue(((Double*)value)->getValue());
                    }
                    else if( classname == "Float")
                    {

                        ((Float*)f->get_data())->setValue(((Float*)value)->getValue());
                    }

                }
            }

        }
        else if ( p == "dup")
        {

            int nloops;
            p = tokens[1];
            s = st.pop();

            nloops = stoi(p);

            for(int i=0; i<nloops; i++)
                st.push(s);

        }
        else if( p == "icmpg")
        {

            int right = ((Int*) st.pop())->getValue();
            int left = ((Int*) st.pop())->getValue();

            if(left>right)
                st.push(new Int(1));
            else
                st.push(new Int(0));


        }
        else if( p == "icmp")
        {

            int right = ((Int*) st.pop())->getValue();
            int left = ((Int*) st.pop())->getValue();

            if(left>right)
                st.push(new Int(1));
            else if(left == right)
                st.push(new Int(0));
            else
                st.push(new Int(-1));


        }
        else if( p == "dcmp")
        {

            double right = ((Double*) st.pop())->getValue();
            double left = ((Double*) st.pop())->getValue();

            if(left>right)
                st.push(new Int(1));
            else if(left == right)
                st.push(new Int(0));
            else
                st.push(new Int(-1));


        }
        else if( p == "ifeq")
        {

            // ifeq label label2


            p1 = tokens[1];
            p2 = tokens[2];

            int res = ((Int*)st.pop())->getValue();

            if(res == 0)
                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
            else
                ic = ((Code*)cur_hs->lookfor(p2))->getLine();

            continue;

        }
        else if( p == "ifne")
        {

            // ifne label label2


            p1 = tokens[1];
            p2 = tokens[2];

            int res = ((Int*)st.pop())->getValue();

            if(res != 0)
                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
            else
                ic = ((Code*)cur_hs->lookfor(p2))->getLine();

            continue;

        }
        else if( p == "ifg")
        {

            // ifg label label2


            p1 = tokens[1];
            p2 = tokens[2];

            int res = ((Int*)st.pop())->getValue();

            if(res == 1)
                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
            else
                ic = ((Code*)cur_hs->lookfor(p2))->getLine();

            continue;

        }
        else if( p == "ifge")
        {

            // ifge label label2


            p1 = tokens[1];
            p2 = tokens[2];

            int res = ((Int*)st.pop())->getValue();

            if(res == 1 || res == 0)
                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
            else
                ic = ((Code*)cur_hs->lookfor(p2))->getLine();

            continue;

        }
        else if( p == "ifl")
        {

            // ifle label label2


            p1 = tokens[1];
            p2 = tokens[2];

            int res = ((Int*)st.pop())->getValue();

            if(res == -1)
                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
            else
                ic = ((Code*)cur_hs->lookfor(p2))->getLine();

            continue;

        }
        else if( p == "ifle")
        {

            // ifle label label2


            p1 = tokens[1];
            p2 = tokens[2];

            int res = ((Int*)st.pop())->getValue();

            if(res == -1 || res == 0)
                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
            else
                ic = ((Code*)cur_hs->lookfor(p2))->getLine();

            continue;

        }
        else if( p == "ifnull")
        {

            // ifge label label2


            p1 = tokens[1];
            p2 = tokens[2];

            symbol* s = st.pop();

            if(!s)
                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
            else
                ic = ((Code*)cur_hs->lookfor(p2))->getLine();

            continue;

        }
        else if( p == "ifnotnull")
        {

            // ifge label label2


            p1 = tokens[1];
            p2 = tokens[2];

            symbol* s = st.pop();

            if(s)
                ic = ((Code*)cur_hs->lookfor(p1))->getLine();
            else
                ic = ((Code*)cur_hs->lookfor(p2))->getLine();

            continue;

        }
        else if( p == "jmp")
        {

            // jmp label

            p1 = tokens[1];

            ic = ((Code*)cur_hs->lookfor(p1))->getLine();
            continue;


        }

        /*

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
                 else if(p == "read")
                {

                    p1 = tokens[1];

                    if(p1 == "i")
                    {
                        int x;
                        cin>>x;
                        ((Integer *)st.pop())->setValue(x);
                    }
                    else if(p1 == "d")
                    {
                       double x;
                       cin>>x;
                       ((Double *)st.pop())->setValue(x);

                    }
                    else if(p1 == "s")
                    {
                       string x;
                       cin>>x;
                       ((Literal *)st.pop())->setValue(x);
                    }


                }
                 else if(p == "nl")
                {
                    printf("\n");
                }
                else if(p == "undef")
                {
                    //undef name
                    p1 = tokens[1];
                    cur_hs->discard(p1);

                }
            */

        ic++;
    }


}


CVM::~CVM()
{
    //dtor
}

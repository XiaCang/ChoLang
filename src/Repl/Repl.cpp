#include<evaluator/Evaluator.h>
#include<fstream>
class Repl{
public:
    static void Start(){
        Environment env;
        Evaluator eval;
        string line;
        while(true){
            cout<<">> ";
            getline(cin,line);
            if(line=="exit"){
                break;
            }

            Lexer lexer(line);
            Parser parser(lexer);
            auto program=parser.ParseProgram();
            if(parser.Errors().size()>0){
                for(auto err:parser.Errors()){
                    cout<<err<<endl;
                }
                continue;
            }

            auto res=eval.EvalProgram(*(program.get()),env);

            if(res)
                cout<<res->ToString()<<endl;
        }


    };

    static void File(string path){
        Environment env;
        Evaluator eval;
        string pg;
        string line;
        ifstream file(path);
        while (getline(file,line))
        {
            line+="\n";
            pg+=line;
        }

        Lexer lexer(pg);
        Parser parser(lexer);
        auto program=parser.ParseProgram();
        if(parser.Errors().size()>0){
            for(auto err:parser.Errors()){
                cout<<err<<endl;
            }
            return;
        }
        
        auto res=eval.EvalProgram(*(program.get()),env);

        if(res)
            cout<<res->ToString()<<endl;
        
    }
};
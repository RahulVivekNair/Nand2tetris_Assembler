#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include<bitset>

class Assembler {
public:
    Assembler(const std::string& inputFile)
        : inputFileName(inputFile) {
        outputFileName = inputFileName.substr(0, inputFileName.find_last_of('.')) + ".hack";
    }

    void assemble() {
        loadfile();
        cleanInstructions();
        printcleanfile();
        parseAndTranslate();
        writeOutput();
    }

private:
    std::string inputFileName;
    std::string outputFileName;
    std::string instruction;
    std::vector<std::string> instructionList;
    std::vector<std::string> cleanInstructionList;
    std::vector<std::string> machineInstructionList;

    void loadfile(){
        std::ifstream infile(inputFileName);        
        while(getline(infile,instruction)){
            instructionList.push_back(instruction);
        }
    }
    // debuggin function - TO REMOVE   
    void printcleanfile(){
        for(auto &i: cleanInstructionList)
            std::cout<< i <<"\n";
    }
    void cleanInstructions(){
        for(auto &ins: instructionList){
            //remove everything after // comment
            size_t commentPos=ins.find("//");
            if(commentPos!=std::string::npos){
                ins = ins.substr(0,commentPos);
            }
            //remove trailing whitespace
            ins.erase(std::find_if(ins.rbegin(), ins.rend(), [](char ch){ return !isspace(ch);}).base(), ins.end());
            //remove leading whitespace
            ins.erase(ins.begin(), std::find_if(ins.begin(), ins.end(), [](char ch){ return !isspace(ch);}));
            //if the instruction is not an empty line after cleaning add it to the clean instruction list
            if(!ins.empty()){
                cleanInstructionList.push_back(ins);
            }

        }
    }
    void parseAndTranslate(){
        for(auto &ins: cleanInstructionList){
            //handling A instruction
            if(ins[0]=='@'){
                std::string valueStr = ins.substr(1);
                std::string binaryCode = std::bitset<16>(std::stoi(valueStr)).to_string();
                machineInstructionList.push_back(binaryCode);
            }
            //Handling C-instructions
            
        }
    }
    void writeOutput(){

    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file.asm>\n";
        return 1;
    }

    Assembler assembler(argv[1]);
    assembler.assemble();

    return 0;
}

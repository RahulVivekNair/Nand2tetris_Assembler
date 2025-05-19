#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <bitset>

class Assembler
{
public:
    Assembler(const std::string &inputFile)
        : inputFileName(inputFile)
    {
        outputFileName = inputFileName.substr(0, inputFileName.find_last_of('.')) + ".hack";
    }

    void assemble()
    {
        loadfile();
        cleanInstructions();
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
    std::unordered_map<std::string, int> symbolTable;

    std::unordered_map<std::string, std::string> jumpLookup = {{"", "000"}, {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"}, {"JLT", "100"}, {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"}};
    std::unordered_map<std::string, std::string> destLookup = {
        {"", "000"},
        {"M", "001"},
        {"D", "010"},
        {"MD", "011"},
        {"A", "100"},
        {"AM", "101"},
        {"AD", "110"},
        {"AMD", "111"}};

    std::unordered_map<std::string, std::string> compLookup = {
        {"0", "0101010"},
        {"1", "0111111"},
        {"-1", "0111010"},
        {"D", "0001100"},
        {"A", "0110000"},
        {"!D", "0001101"},
        {"!A", "0110001"},
        {"-D", "0001111"},
        {"-A", "0110011"},
        {"D+1", "0011111"},
        {"A+1", "0110111"},
        {"D-1", "0001110"},
        {"A-1", "0110010"},
        {"D+A", "0000010"},
        {"D-A", "0010011"},
        {"A-D", "0000111"},
        {"D&A", "0000000"},
        {"D|A", "0010101"},
        {"M", "1110000"},
        {"!M", "1110001"},
        {"-M", "1110011"},
        {"M+1", "1110111"},
        {"M-1", "1110010"},
        {"D+M", "1000010"},
        {"D-M", "1010011"},
        {"M-D", "1000111"},
        {"D&M", "1000000"},
        {"D|M", "1010101"}};

    void loadfile()
    {
        std::ifstream infile(inputFileName);
        while (getline(infile, instruction))
        {
            instructionList.push_back(instruction);
        }
        infile.close();
    }
    void cleanInstructions()
    {
        for (auto &ins : instructionList)
        {
            // remove everything after // comment
            size_t commentPos = ins.find("//");
            if (commentPos != std::string::npos)
            {
                ins = ins.substr(0, commentPos);
            }
            // remove trailing whitespace
            ins.erase(std::find_if(ins.rbegin(), ins.rend(), [](char ch)
                                   { return !isspace(ch); })
                          .base(),
                      ins.end());
            // remove leading whitespace
            ins.erase(ins.begin(), std::find_if(ins.begin(), ins.end(), [](char ch)
                                                { return !isspace(ch); }));
            // if the instruction is not an empty line after cleaning add it to the clean instruction list
            if (!ins.empty())
            {
                cleanInstructionList.push_back(ins);
            }
        }
    }
    void parseAndTranslate()
    {
        for (auto &ins : cleanInstructionList)
        {
            // handling A instruction
            if (ins[0] == '@')
            {
                std::string valueStr = ins.substr(1);
                std::string binaryCode = std::bitset<16>(std::stoi(valueStr)).to_string();
                machineInstructionList.push_back(binaryCode);
            }
            // Handling C-instructions
            else
            {
                size_t equalPos = ins.find("=");
                size_t semicolonPos = ins.find(";");
                std::string dest = "";
                std::string comp = "";
                std::string jump = "";
                if (equalPos != std::string::npos)
                    dest = ins.substr(0, equalPos);
                if (semicolonPos != std::string::npos)
                    jump = ins.substr(semicolonPos + 1);
                if (equalPos != std::string::npos && semicolonPos != std::string::npos)
                    comp = ins.substr(equalPos + 1, semicolonPos - equalPos + 1);
                else if (equalPos != std::string::npos)
                    comp = ins.substr(equalPos + 1);
                else if (semicolonPos != std::string::npos)
                    comp = ins.substr(0, semicolonPos);
                else
                    comp = ins;

                std::string compBits = compLookup[comp];
                std::string destBits = destLookup[dest];
                std::string jumpBits = jumpLookup[jump];
                std::string binaryCode = "111"+ compBits+destBits+jumpBits;
                machineInstructionList.push_back(binaryCode);
            }
        }
    }
    void writeOutput()
    {
        std::ofstream outfile(outputFileName);
        for(auto &mins: machineInstructionList)
            outfile << mins << "\n";
        outfile.close();
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file.asm>\n";
        return 1;
    }

    Assembler assembler(argv[1]);
    assembler.assemble();

    return 0;
}

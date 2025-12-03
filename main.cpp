#include <iostream>
#include <fstream>
#include <string>
#include "BlockChain.h"
#include "Transaction.h" 
#include "Utilities.h"


int main(int argc, char** argv) {
    if(argc != 4){
        std::cerr << getErrorMessage() << std::endl;
        return 1;
    }

    string operation = argv[1];
    string source = argv[2];
    string target = argv[3];

    if(operation == "format"){
        ifstream in_file(source);
        ofstream out_file(target);
        BlockChain blockchain = BlockChainLoad(in_file);
        BlockChainDump(blockchain, out_file);
    }else if(operation == "hash"){
        std::ifstream ifile(source);
        if (!ifile) {
            std::cerr << "Error: could not open file 'test'\n";
            return 1;
        }
        std::ofstream ofile(target);

        if (!ofile) {
            std::cerr << "Error: could not open file 'test' for writing\n";
            return 1;
        }
        BlockChain blockchain=BlockChainLoad(ifile);
        BlockChainDumpHashed(blockchain, ofile);
    }else if(operation == "compress"){

    }else if(operation == "verify"){

    }else{
        std::cerr << "unvalid operation, you can use format, hash, compress or verify only" << std::endl;
        return 1;
    }

    return 0;
}
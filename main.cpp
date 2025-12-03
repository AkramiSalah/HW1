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

    }else if(operation == "compress"){

    }else if(operation == "verify"){

    }else{
        std::cerr << "unvalid operation, you can use format, hash, compress or verify only" << std::endl;
        return 1;
    }

    return 0;
}
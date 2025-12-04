#include <iostream>
#include <fstream>
#include <string>
#include "BlockChain.h"
#include "Utilities.h"


int main(int argc, char** argv) {
    if(argc != 4){
        std::cerr << getErrorMessage() << std::endl;
        return 1;
    }

    string operation = argv[1];
    string source = argv[2];
    string target = argv[3];

    ifstream in_file(source);
    if (!in_file) {
        std::cerr << "could not open source file" << std::endl;
        return 1;
    }

    BlockChain blockchain = BlockChainLoad(in_file);
    if(operation == "format"){
        ofstream out_file(target);
        if (!out_file) {
            std::cerr << "could not open target file" << std::endl;
            return 1;
        }
        BlockChainDump(blockchain, out_file);

    }else if(operation == "hash"){
        ofstream out_file(target);
        if (!out_file) {
            std::cerr << "could not open target file" << std::endl;
            return 1;
        }
        BlockChainDumpHashed(blockchain, out_file);

    }else if(operation == "compress"){
        BlockChainCompress(blockchain);
        ofstream out_file(target);
        if (!out_file) {
            std::cerr << "could not open target file" << std::endl;
            return 1;
        }
        BlockChainDump(blockchain, out_file);
        
    }else if(operation == "verify"){
        ifstream target_file(target);
        if (!target_file) {
            std::cerr << "could not open target file" << std::endl;
            return 1;
        }
        bool verify_res = BlockChainVerifyFile(blockchain, target_file);
        string verification_status = verify_res ? "passed" : "failed";
        std::cout << "Verification " << verification_status << std::endl;
    }else{
        std::cerr << "unvalid operation, you can use format, hash, compress or verify only" << std::endl;
        return 1;
    }
    BlockChainDestroy(blockchain);
    delete blockchain.head;
    return 0;
}
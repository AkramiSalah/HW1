#include <iostream>
#include <fstream>
#include <string>
#include "BlockChain.h"
#include "Utilities.h"


int main(int argc, char** argv) {
    if (argc != 4){
        std::cout << getErrorMessage() << std::endl;
        return 1;
    }

    const string operation = argv[1];
    const string source = argv[2];
    const string target = argv[3];

    ifstream in_file(source);
    if (!in_file) {
        std::cout << "could not open source file" << std::endl;
        return 1;
    }
    BlockChain blockchain = BlockChainLoad(in_file);

    if (operation == "verify"){
        ifstream target_file(target);
        if (!target_file) {
            std::cout << "could not open target file" << std::endl;
            BlockChainDestroy(blockchain);
            return 1;
        }

        bool verify_res = BlockChainVerifyFile(blockchain, target_file);
        string verification_status = verify_res ? "passed" : "failed";
        std::cout << "Verification " << verification_status << std::endl;
    }else{
        ofstream out_file(target);
        if (!out_file) {
            std::cout << "could not open target file" << std::endl;
            BlockChainDestroy(blockchain);
            return 1;
        }

        if (operation == "format"){
            BlockChainDump(blockchain, out_file);
        }else if (operation == "hash"){
            BlockChainDumpHashed(blockchain, out_file);
        }else if (operation == "compress"){
            BlockChainCompress(blockchain);
            BlockChainDump(blockchain, out_file);
        }else{
            std::cout << getErrorMessage() << std::endl;
            BlockChainDestroy(blockchain);
            return 1;
        }
    }
    
    BlockChainDestroy(blockchain);
    return 0;
}
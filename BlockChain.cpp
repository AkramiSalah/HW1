#include "BlockChain.h"

BlockChain BlockChainInit(){
    BlockChain blockChain;
    blockChain.head = nullptr;
    blockChain.tail = nullptr;
    blockChain.size = 0;
    return blockChain;
}

int BlockChainGetSize(const BlockChain& blockChain){
    return blockChain.size;
}

void BlockChainAppendTransaction(
        BlockChain& blockChain,
        unsigned int value,
        const string& sender,
        const string& receiver,
        const string& timestamp
){
    const Transaction* transaction = new Transaction({value, sender, receiver});
    const Block newBlock = {transaction timestamp};
    BlockLink* newHeadLink = new BlockLink({newBlock, blockChain.head});

    if(BlockChainGetSize(blockChain) == 0) blockChain.tail = newHeadLink;

    blockChain.head = newHeadLink;
    blockChain.size += 1;
    
}

#include"HeadData.h"

void HeadData::baseParse() {
    this->protocolId = parseInt(PROTOCOL_ID_SIZE);
    this->id = parseInt(ID_SIZE);
    this->dataType = parseInt(DATA_TYPE_SIZE);
    this->dataLength = parseInt(DATA_SIZE);
}

unsigned int HeadData::parseInt(int len) {
    unsigned int sum = 0;
    unsigned int i = 0;
    for(const char *end = bp + len - 1; bp <= end; end--) {
        sum += (((unsigned int) ((unsigned char) (*end))) << i);
        i += 8;
    }
    bp += len;
    return sum;
}

bool HeadData::readBuf() {
    if(read(fd, buffer, BASE_BUFFER_SIZE) == 0) {
        return false;
    }
    bp = buffer;
    baseParse();
    return true;
}

HeadData::HeadData(int fd) {
    this->fd = fd;
}

HeadData::HeadData() {

}

bool HeadData::parse(const char *buffer) {
    bp = buffer;
    baseParse();
    return true;
}

unsigned int HeadData::getProtocolId() {
    return this->protocolId;
}

unsigned int HeadData::getId() {
    return this->id;
}

unsigned int HeadData::getDataType() {
    return this->dataType;
}

unsigned int HeadData::getDataLength() {
    return this->dataLength;
}
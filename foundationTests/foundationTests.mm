//
//  foundationTests.mm
//  foundationTests
//
//  Created by dx on 12-7-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "RefBase.h"
#include "Mutex.h"
#include "Condition.h"
#define LOG_TAG "foundationTests"
#include "Log.h"

#include "ALooper.h"
#include "AMessage.h"
#include "ABuffer.h"
#include "ABitReader.h"
#include "ALooper.h"
#include "AHandler.h"
#include "AString.h"
#include "String8.h"
#include "Vector.h"
#include "List.h"
#include "String16.h"
#include "Parcel.h"
#include "ADebug.h"

using namespace android;

class MyHandler: public AHandler {
    protected:
        virtual void onMessageReceived(const sp<AMessage> &msg) {
            switch(msg->what()) {
                case 'test': 
                    {
                        Mutex::Autolock autoLock(mLock);
                        ALOGI("test");
                        mCond.signal();
                    }
            }
        };
    private:
        Mutex mLock;
        Condition mCond;
};

void testLog() {
    LOGV("demon LOGV");
    LOGD("demon LOGD");
    LOGI("demon LOGI");
    LOGW("demon LOGW");
    LOGE("demon LOGE");

    ALOGV("demon ALOGV");
    ALOGD("demon ALOGD");
    ALOGI("demon ALOGI");
    ALOGW("demon ALOGW");
    ALOGE("demon ALOGE");
}

void testAMessage() {
    sp<AMessage> msg = new AMessage(0);
    sp<ABuffer> buffer = new ABuffer(256);
    int32_t a = -1;
    int64_t b = -1;
    float c = -1.0;
    double d = -1.0;
    AString e;
    sp<ABuffer> f;
    sp<AMessage> g;

    msg->setInt32("a", 1);
    msg->setInt64("b", 2);
    msg->setFloat("c", 3.0);
    msg->setDouble("d", 4.0);
    msg->setString("e", "5");
    msg->setBuffer("f", buffer);
    msg->setMessage("g", msg);

    msg->findInt32("a", &a);
    msg->findInt64("b", &b);
    msg->findFloat("c", &c);
    msg->findDouble("d", &d);
    msg->findString("e", &e);
    msg->findBuffer("f", &f);
    msg->findMessage("g", &g);

    CHECK_EQ(msg->what(), 0);
    CHECK_EQ(a, 1);
    CHECK_EQ(b, 2);
    CHECK_EQ(c, 3.0);
    CHECK_EQ(d, 4.0);
    CHECK(!strcmp(e.c_str(), "5"));
    CHECK_EQ(f.get(), buffer.get());
    CHECK_EQ(g.get(), msg.get());
}

void testVector() {
    Vector<int> vector;
    Vector<int> other;
    vector.setCapacity(8);

    vector.add(1);
    vector.add(2);
    vector.add(3);

    CHECK_EQ(vector.size(), 3);

    // copy the vector
    other = vector;

    CHECK_EQ(other.size(), 3);

    // add an element to the first vector
    vector.add(4);

    // make sure the sizes are correct
    CHECK_EQ(vector.size(), 4);
    CHECK_EQ(other.size(), 3);

    // add an element to the copy
    other.add(5);

    // make sure the sizes are correct
    CHECK_EQ(vector.size(), 4);
    CHECK_EQ(other.size(), 4);

    // make sure the content of both vectors are correct
    CHECK_EQ(vector[3], 4);
    CHECK_EQ(other[3], 5);

    vector.push(3);
    CHECK_EQ(vector.top(), 3);
    vector.pop();
    
    for(int i=0; i<32; i++) {
        vector.push(1);
    }
    CHECK_EQ(vector.size(), 36);
}

void testString() {
    const char *a = "Hello";
    const char *b = " ";
    const char *c = "Demon";
    const char *d = "Hello Demon";
    const char16_t e[] = {'H', 'e', 'l', 'l', 'o', ' ', 'D', 'e', 'm', 'o', 'n', '\0'};
    // can not use u"Hello Demon" ???

    String8 s8 = String8(a);
    String16 s16 = String16(a);
    AString sa = AString(a);

    String8 x8 = s8 + b + c;
    String16 x16 = s16 + String16(b) + String16(c);
    AString xa = StringPrintf("%s%s%s", sa.c_str(), b, c);
    
    CHECK(!strcmp(x8, d));
    CHECK(!strcmp16(x16.string(), e));
    CHECK(!strcmp(xa.c_str(), d));
    
}

void testParcel() {
    Parcel parcel;
    int32_t a = -1;
    int64_t b = -1;
    float c = -1.0;
    double d = -1.0;
    const char *e;

    parcel.writeInt32(1);
    parcel.writeInt64(2);
    parcel.writeFloat(3.0);
    parcel.writeDouble(4.0);
    parcel.writeCString("5");

    parcel.setDataPosition(0);
    a = parcel.readInt32();
    b = parcel.readInt64();
    c = parcel.readFloat();
    d = parcel.readDouble();
    e = parcel.readCString();

    CHECK_EQ(a, 1);
    CHECK_EQ(b, 2);
    CHECK_EQ(c, 3.0);
    CHECK_EQ(d, 4.0);
    CHECK(!strcmp(e, "5"));
}

void testALooper() {
}

#import "foundationTests.h"

@implementation foundationTests

- (void)setUp
{
    [super setUp];

    // Set-up code here.
}

- (void)tearDown
{
    // Tear-down code here.

    [super tearDown];
}

- (void)testExample
{
    testLog();
    testAMessage();
    testVector();
    testString();
    testParcel();
    //STFail(@"Unit tests are not implemented yet in foundationTests");
}

@end



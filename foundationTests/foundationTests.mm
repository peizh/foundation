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

  LOGV("demon %d", msg->what());
  LOGD("demon %d", a);
  LOGI("demon %lld", b);
  LOGW("demon %f", c);
  LOGE("demon %f", d);
  LOGI("demon %s", e.c_str());
  LOGI("demon %p %p", f.get(), buffer.get());
  LOGI("demon %p %p", g.get(), msg.get());
  NSLog(@"aaa");
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
  testAMessage();
  //STFail(@"Unit tests are not implemented yet in foundationTests");
}

@end



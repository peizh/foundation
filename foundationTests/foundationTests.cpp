//
//  foundationTests.m
//  foundationTests
//
//  Created by dx on 12-7-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "RefBase.h"
#include "Mutex.h"
#include "Condition.h"
#include "Log.h"

#include "ALooper.h"
#include "AMessage.h"
#include "ABuffer.h"
#include "ABitReader.h"
#include "ALooper.h"
#include "AHandler.h"
#include "AString.h"

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

int main() {
  sp<ALooper> looper = new ALooper();
  sp<ABuffer> buffer = new ABuffer(256);
  AString string = "aaaa";
  sp<AMessage> message = new AMessage('test');
  message->setInt32("aaa", 1);
  message->setBuffer("bbb", buffer);
  message->setString("ccc", string.c_str());
  sp<AHandler> handler = new MyHandler();
  looper->registerHandler(handler);
  looper->start();
  message->post();
  Vector<int> a;
  a.push_back(1);
  List<int> b;
  b.clear();


  return 0;
}

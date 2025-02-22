/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#import <GRPCClient/GRPCCall+Tests.h>
#import <GRPCClient/internal_testing/GRPCCall+InternalTests.h>

#import <Cronet/Cronet.h>
#import <GRPCClient/GRPCCall+Cronet.h>

#import "../ConfigureCronet.h"
#import "InteropTests.h"

// The server address is derived from preprocessor macro, which is
// in turn derived from environment variable of the same name.
#define NSStringize_helper(x) #x
#define NSStringize(x) @NSStringize_helper(x)
static NSString *const kRemoteSSLHost = NSStringize(HOST_PORT_REMOTE);

// The Protocol Buffers encoding overhead of remote interop server. Acquired
// by experiment. Adjust this when server's proto file changes.
static int32_t kRemoteInteropServerOverhead = 12;

/** Tests in InteropTests.m, sending the RPCs to a remote SSL server. */
@interface InteropTestsRemoteWithCronet : InteropTests
@end

@implementation InteropTestsRemoteWithCronet

+ (void)setUp {
  configureCronet();
  [GRPCCall useCronetWithEngine:[Cronet getGlobalEngine]];

  [super setUp];
}

+ (NSString *)host {
  return kRemoteSSLHost;
}

+ (GRPCTransportId)transport {
  return gGRPCCoreCronetId;
}

- (int32_t)encodingOverhead {
  return kRemoteInteropServerOverhead;  // bytes
}

@end

/*
 *
 * Copyright 2016 gRPC authors.
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

#import "GRPCCallLegacy.h"
#import "GRPCTypes.h"

typedef struct stream_engine stream_engine;

// Transport id for Cronet transport
extern const GRPCTransportId gGRPCCoreCronetId;

// Deprecated class. Please use the gGRPCCoreCronetId with GRPCCallOptions.transport instead.
@interface GRPCCall (Cronet)

+ (void)useCronetWithEngine:(stream_engine*)engine;
+ (stream_engine*)cronetEngine;
+ (BOOL)isUsingCronet;

@end

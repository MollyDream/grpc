/*
 *
 * Copyright 2019 gRPC authors.
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

// The interface for a transport implementation

#import "GRPCInterceptor.h"

NS_ASSUME_NONNULL_BEGIN

#pragma mark Transport ID

/**
 * The default transport implementations available in gRPC. These implementations will be provided
 * by gRPC by default unless explicitly excluded.
 */
extern const struct GRPCDefaultTransportImplList {
  const GRPCTransportId core_secure;
  const GRPCTransportId core_insecure;
} GRPCDefaultTransportImplList;

/** Returns whether two transport id's are identical. */
BOOL TransportIdIsEqual(GRPCTransportId lhs, GRPCTransportId rhs);

/** Returns the hash value of a transport id. */
NSUInteger TransportIdHash(GRPCTransportId);

#pragma mark Transport and factory

@protocol GRPCInterceptorInterface;
@protocol GRPCResponseHandler;
@class GRPCTransportManager;
@class GRPCRequestOptions;
@class GRPCCallOptions;
@class GRPCTransport;

/** The factory method to create a transport. */
@protocol GRPCTransportFactory<NSObject>

- (GRPCTransport *)createTransportWithManager:(GRPCTransportManager *)transportManager;

@end

/** The registry of transport implementations. */
@interface GRPCTransportRegistry : NSObject

+ (instancetype)sharedInstance;

/**
 * Register a transport implementation with the registry. All transport implementations to be used
 * in a process must register with the registry on process start-up in its +load: class method.
 * Parameter \a transportId is the identifier of the implementation, and \a factory is the factory
 * object to create the corresponding transport instance.
 */
- (void)registerTransportWithId:(GRPCTransportId)transportId
                        factory:(id<GRPCTransportFactory>)factory;

@end

/**
 * Base class for transport implementations. All transport implementation should inherit from this
 * class.
 */
@interface GRPCTransport : NSObject<GRPCInterceptorInterface>

@end

NS_ASSUME_NONNULL_END

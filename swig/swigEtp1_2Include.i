/*-----------------------------------------------------------------------
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"; you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-----------------------------------------------------------------------*/
%module(directors="1") fetpapi
%catches(Swig::DirectorException);

#ifdef WITH_FESAPI
%import "../../fesapi-2.1.1.0/src/common/DataObjectReference.h"
%import "../../fesapi-2.1.1.0/src/common/HdfProxyFactory.h"
#endif

%{
#include <stdint.h>		// Use the C99 official header
%}

%include <swigarch.i>

/* Exact integral types.  */

/* Signed.  */

typedef signed char		int8_t;
typedef short int		int16_t;
typedef int			int32_t;
#if defined(SWIGWORDSIZE64)
typedef long int		int64_t;
#else
typedef long long int		int64_t;
#endif

/* Unsigned.  */
typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		uint32_t;
#ifndef SWIGJAVA	
#if defined(SWIGWORDSIZE64)
typedef unsigned long int	uint64_t;
#else
typedef unsigned long long int	uint64_t;
#endif
#endif

%include "std_string.i"

%include "../src/nsDefinitions.h"

//************************/
// JAVA
//************************/

#ifdef SWIGJAVA		
	// We don't want to use BigInteger in java.
	#if defined(SWIGWORDSIZE64)
	typedef long int		uint64_t;
	#else
	typedef long long int		uint64_t;
	#endif	
	
	/*
	 When using multiple modules or the nspace feature it is common to invoke SWIG with a different -package command line option for each module.
	 However, by default the generated code may not compile if generated classes in one package use generated classes in another package.
	 The visibility of the getCPtr() and pointer constructor generated from the javabody typemaps needs changing.
	 The default visibility is protected but it needs to be public for access from a different package. Just changing 'protected' to 'public' in the typemap achieves this.
	 Two macros are available in java.swg to make this easier and using them is the preferred approach over simply copying the typemaps and modifying as this is forward compatible with any changes in the javabody typemap in future versions of SWIG.
	 The macros are for the proxy and typewrapper classes and can respectively be used to make the method and constructor public:
	  SWIG_JAVABODY_PROXY(public, public, SWIGTYPE)
	  SWIG_JAVABODY_TYPEWRAPPER(public, public, public, SWIGTYPE)
	*/
	SWIG_JAVABODY_PROXY(public, public, SWIGTYPE)
	SWIG_JAVABODY_TYPEWRAPPER(public, public, public, SWIGTYPE)
	#define SWIG_SHARED_PTR_TYPEMAPS(CONST, TYPE...) SWIG_SHARED_PTR_TYPEMAPS_IMPLEMENTATION(public, public, CONST, TYPE)
	
	// Director threading issues
	%insert("runtime") %{
		#define SWIG_JAVA_ATTACH_CURRENT_THREAD_AS_DAEMON
	%}
	
	// http://www.swig.org/Doc4.0/SWIGDocumentation.html#Java_proper_enums
	%include "enums.swg"
	%javaconst(1);
#endif

//************************/
// CSHARP
//************************/

#ifdef SWIGCSHARP
	SWIG_CSBODY_PROXY(public, public, SWIGTYPE)
	SWIG_CSBODY_TYPEWRAPPER(public, public, public, SWIGTYPE)
#endif

//************************/
// PYTHON
//************************/

#ifdef SWIGPYTHON

	%{
	#define SWIG_FILE_WITH_INIT // In case we use Python Swig Wrapping
	%}

#endif

// Don't try to create vector of polymorphic data unless you really know what you are doing.
// Use C array approach instead.
%include "std_vector.i"

%template(StringVector) std::vector< std::string >;
%template(Int32Vector) std::vector< int >;
%template(Int64Vector) std::vector< long long >;
%template(FloatVector) std::vector< float >;
%template(DoubleVector) std::vector< double >;
%template(BoolVector) std::vector< bool >;

//************************/
// EXCEPTIONS
//************************/

%include "exception.i"
%exception {
    try {
        $action
    }
    catch (std::invalid_argument & e) {
        SWIG_exception(SWIG_TypeError,const_cast<char*>(e.what()));
    } 
    catch (std::out_of_range& e) {
        SWIG_exception(SWIG_IndexError,const_cast<char*>(e.what()));
    }
    catch (std::logic_error & e) {
        SWIG_exception(SWIG_SyntaxError,const_cast<char*>(e.what()));
    } 
    catch(...) {
        SWIG_exception(SWIG_RuntimeError,"Unknown exception at F2I C++ API level");
    }
}

typedef long long 				time_t;

%{
#include "../src/etp/ClientSessionLaunchers.h"
#include "../src/etp/EtpHelpers.h"
#include "../src/etp/PlainServerSession.h"
#include "../src/etp/Server.h"
%}

#ifdef WITH_FESAPI
%{
#include "../src/etp/fesapi/FesapiHdfProxy.h"
%}
#endif

#if defined(SWIGJAVA) || defined(SWIGCSHARP)
	%nspace ETP_NS::ProtocolHandlers;
	%nspace ETP_NS::CoreHandlers;
	%nspace ETP_NS::DiscoveryHandlers;
	%nspace ETP_NS::StoreHandlers;
	%nspace ETP_NS::StoreNotificationHandlers;
	%nspace ETP_NS::DataArrayHandlers;
	%nspace ETP_NS::TransactionHandlers;
	%nspace ETP_NS::DataspaceHandlers;
	%nspace ETP_NS::AbstractSession;
	%nspace ETP_NS::PlainClientSession;
	%nspace ETP_NS::InitializationParameters;
	%nspace ETP_NS::ServerInitializationParameters;
	%nspace ETP_NS::Server;
#ifdef WITH_FESAPI
	%nspace ETP_NS::FesapiHdfProxyFactory;
#endif
	
	%nspace Energistics::Etp::v12::Datatypes::SupportedDataObject;
	%nspace Energistics::Etp::v12::Datatypes::Uuid;
	%nspace Energistics::Etp::v12::Datatypes::Version;
	%nspace Energistics::Etp::v12::Datatypes::SupportedProtocol;
	%nspace Energistics::Etp::v12::Datatypes::DataValue;
	%nspace Energistics::Etp::v12::Datatypes::DataValueitem_t;
	%nspace Energistics::Etp::v12::Datatypes::ErrorInfo;
	%nspace Energistics::Etp::v12::Datatypes::AnyArrayType;
	%nspace Energistics::Etp::v12::Datatypes::AnyArray;
	%nspace Energistics::Etp::v12::Datatypes::AnyArrayitem_t;
	%nspace Energistics::Etp::v12::Datatypes::ArrayOfBoolean;
	%nspace Energistics::Etp::v12::Datatypes::ArrayOfInt;
	%nspace Energistics::Etp::v12::Datatypes::ArrayOfLong;
	%nspace Energistics::Etp::v12::Datatypes::ArrayOfFloat;
	%nspace Energistics::Etp::v12::Datatypes::ArrayOfDouble;
	%nspace Energistics::Etp::v12::Datatypes::ArrayOfString;
	
	%nspace Energistics::Etp::v12::Datatypes::Object::ContextInfo;
	%nspace Energistics::Etp::v12::Datatypes::Object::ContextScopeKind;
	%nspace Energistics::Etp::v12::Datatypes::Object::DataObject;
	%nspace Energistics::Etp::v12::Datatypes::Object::Resource;
	%nspace Energistics::Etp::v12::Datatypes::Object::DeletedResource;
	%nspace Energistics::Etp::v12::Datatypes::Object::Dataspace;
	%nspace Energistics::Etp::v12::Datatypes::Object::Edge;
	%nspace Energistics::Etp::v12::Datatypes::Object::RelationshipKind;
	
	%nspace Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier;
	%nspace Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray;
	%nspace Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata;
	%nspace Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataArraysType;
	%nspace Energistics::Etp::v12::Datatypes::DataArrayTypes::GetDataSubarraysType;
	%nspace Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataSubarraysType;
	%nspace Energistics::Etp::v12::Datatypes::DataArrayTypes::PutUninitializedDataArrayType;
	
	%nspace Energistics::Etp::v12::Protocol::Core::OpenSession;
	%nspace Energistics::Etp::v12::Protocol::Core::RequestSession;
	%nspace Energistics::Etp::v12::Protocol::Core::CloseSession;
	%nspace Energistics::Etp::v12::Protocol::Core::ProtocolException;
	%nspace Energistics::Etp::v12::Protocol::Core::Acknowledge;
	%nspace Energistics::Etp::v12::Protocol::Core::Ping;
	%nspace Energistics::Etp::v12::Protocol::Core::Pong;
	%nspace Energistics::Etp::v12::Protocol::Core::RenewSecurityToken;
	%nspace Energistics::Etp::v12::Protocol::Core::RenewSecurityTokenResponse;
	
	%nspace Energistics::Etp::v12::Protocol::Discovery::GetResources;
	%nspace Energistics::Etp::v12::Protocol::Discovery::GetResourcesResponse;
	%nspace Energistics::Etp::v12::Protocol::Discovery::GetResourcesEdgesResponse;
	%nspace Energistics::Etp::v12::Protocol::Discovery::GetDeletedResources;
	%nspace Energistics::Etp::v12::Protocol::Discovery::GetDeletedResourcesResponse;
	
	%nspace Energistics::Etp::v12::Protocol::Store::GetDataObjects;
	%nspace Energistics::Etp::v12::Protocol::Store::GetDataObjectsResponse;
	%nspace Energistics::Etp::v12::Protocol::Store::PutDataObjects;
	%nspace Energistics::Etp::v12::Protocol::Store::PutDataObjectsResponse;
	%nspace Energistics::Etp::v12::Protocol::Store::DeleteDataObjects;
	%nspace Energistics::Etp::v12::Protocol::Store::DeleteDataObjectsResponse;
	%nspace Energistics::Etp::v12::Protocol::Store::Chunk;
	
	%nspace Energistics::Etp::v12::Protocol::DataArray::GetDataArrays;
	%nspace Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse;
	%nspace Energistics::Etp::v12::Protocol::DataArray::PutDataArrays;
	%nspace Energistics::Etp::v12::Protocol::DataArray::PutDataArraysResponse;
	%nspace Energistics::Etp::v12::Protocol::DataArray::GetDataSubarrays;
	%nspace Energistics::Etp::v12::Protocol::DataArray::GetDataSubarraysResponse;
	%nspace Energistics::Etp::v12::Protocol::DataArray::PutDataSubarrays;
	%nspace Energistics::Etp::v12::Protocol::DataArray::PutDataSubarraysResponse;
	%nspace Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata;
	%nspace Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadataResponse;
	%nspace Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArrays;
	%nspace Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArraysResponse;
	
	%nspace Energistics::Etp::v12::Protocol::Transaction::StartTransaction;
	%nspace Energistics::Etp::v12::Protocol::Transaction::StartTransactionResponse;
	%nspace Energistics::Etp::v12::Protocol::Transaction::CommitTransaction;
	%nspace Energistics::Etp::v12::Protocol::Transaction::CommitTransactionResponse;
	%nspace Energistics::Etp::v12::Protocol::Transaction::RollbackTransaction;
	%nspace Energistics::Etp::v12::Protocol::Transaction::RollbackTransactionResponse;
	
	%nspace Energistics::Etp::v12::Protocol::Dataspace::GetDataspaces;
	%nspace Energistics::Etp::v12::Protocol::Dataspace::GetDataspacesResponse;
	%nspace Energistics::Etp::v12::Protocol::Dataspace::PutDataspaces;
	%nspace Energistics::Etp::v12::Protocol::Dataspace::PutDataspacesResponse;
	%nspace Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspaces;
	%nspace Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspacesResponse;
#endif

%include "std_map.i"
%template(MapStringString) std::map<std::string, std::string>;

%include "stdint.i"
%include "std_array.i"

%typemap(javaimports) SWIGTYPE %{
	import com.f2i_consulting.fetpapi.*;
%}
				
namespace Energistics {
	namespace Etp {	
		namespace v12 {		
			namespace Datatypes {				
				struct SupportedDataObject{				
					std::string qualifiedType;
					std::vector<std::string> dataObjectCapabilities;
				};
				
				struct Uuid{				
					std::array<uint8_t, 16> array;
				};
				
				struct Version{				
					int32_t major;
					int32_t minor;
					int32_t revision;
					int32_t patch;
				};
				
				struct SupportedProtocol{				
					int32_t protocol;
					Energistics::Etp::v12::Datatypes::Version protocolVersion;
					std::string role;
				};
				
				struct ArrayOfBoolean{				
					std::vector<bool> values;
				};
				
				struct ArrayOfInt{				
					std::vector<int32_t> values;
				};
				
				struct ArrayOfLong{				
					std::vector<int64_t> values;
				};
				
				struct ArrayOfFloat{				
					std::vector<float> values;
				};
				
				struct ArrayOfDouble{				
					std::vector<double> values;
				};
				
				struct ArrayOfString{				
					std::vector<std::string> values;
				};
				
				struct DataValueitem_t {
				public:				
					size_t idx() const;
					
					bool is_null() const;
					void set_null();
									
					bool get_boolean();				
					void set_boolean(const bool& v);
					
					int32_t get_int();			
					void set_int(const int32_t& v);	
					
					int64_t get_long();				
					void set_long(const int64_t& v);
					
					float get_float();				
					void set_float(const float& v);
					
					double get_double();				
					void set_double(const double& v);
					
					std::string get_string();				
					void set_string(const std::string& v);
					
					Energistics::Etp::v12::Datatypes::ArrayOfBoolean get_ArrayOfBoolean();		
					void set_ArrayOfBoolean(const Energistics::Etp::v12::Datatypes::ArrayOfBoolean& v);
					
					Energistics::Etp::v12::Datatypes::ArrayOfInt get_ArrayOfInt();			
					void set_ArrayOfInt(const Energistics::Etp::v12::Datatypes::ArrayOfInt& v);
					
					Energistics::Etp::v12::Datatypes::ArrayOfLong get_ArrayOfLong();
					void set_ArrayOfLong(const Energistics::Etp::v12::Datatypes::ArrayOfLong& v);	
					
					Energistics::Etp::v12::Datatypes::ArrayOfFloat get_ArrayOfFloat();				
					void set_ArrayOfFloat(const Energistics::Etp::v12::Datatypes::ArrayOfFloat& v);
					
					Energistics::Etp::v12::Datatypes::ArrayOfDouble get_ArrayOfDouble();	
					void set_ArrayOfDouble(const Energistics::Etp::v12::Datatypes::ArrayOfDouble& v);
					
					Energistics::Etp::v12::Datatypes::ArrayOfString get_ArrayOfString();			
					void set_ArrayOfString(const Energistics::Etp::v12::Datatypes::ArrayOfString& v);	
					
					std::string get_bytes();				
					void set_bytes(const std::string& v);		
				};
				
				struct DataValue{				
					Energistics::Etp::v12::Datatypes::DataValueitem_t item;
				};
				
				struct ErrorInfo{				
					std::string message;
					int32_t code;
				};
				
				struct AnyArrayitem_t {
				public:				
					size_t idx() const;
					
					Energistics::Etp::v12::Datatypes::ArrayOfBoolean get_ArrayOfBoolean();
					void set_ArrayOfBoolean(const Energistics::Etp::v12::Datatypes::ArrayOfBoolean& v);
					
					Energistics::Etp::v12::Datatypes::ArrayOfInt get_ArrayOfInt();
					void set_ArrayOfInt(const Energistics::Etp::v12::Datatypes::ArrayOfInt& v);
					
					Energistics::Etp::v12::Datatypes::ArrayOfLong get_ArrayOfLong();
					void set_ArrayOfLong(const Energistics::Etp::v12::Datatypes::ArrayOfLong& v);
					
					Energistics::Etp::v12::Datatypes::ArrayOfFloat get_ArrayOfFloat();
					void set_ArrayOfFloat(const Energistics::Etp::v12::Datatypes::ArrayOfFloat& v);
					
					Energistics::Etp::v12::Datatypes::ArrayOfDouble get_ArrayOfDouble();
					void set_ArrayOfDouble(const Energistics::Etp::v12::Datatypes::ArrayOfDouble& v);
					
					Energistics::Etp::v12::Datatypes::ArrayOfString get_ArrayOfString();
					void set_ArrayOfString(const Energistics::Etp::v12::Datatypes::ArrayOfString& v);
					
					std::string get_bytes();
					void set_bytes(const std::string& v);
				};
				
				struct AnyArray{				
					Energistics::Etp::v12::Datatypes::AnyArrayitem_t item;
				};	
				
				enum AnyArrayType {				
					arrayOfBoolean=0,
					arrayOfInt=1,
					arrayOfLong=2,
					arrayOfFloat=3,
					arrayOfDouble=4,
					arrayOfString=5,
					bytes=6
				};
			}
		}
	}
}
%template(SupportedDataObjectVector) std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject>;
%template(uint8_t16Array) std::array<uint8_t, 16>;
%template(SupportedProtocolVector) std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol>;
%template(MapStringDataValue) std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue>;
%template(MapStringErrorInfo) std::map<std::string, Energistics::Etp::v12::Datatypes::ErrorInfo>;

namespace Energistics {
	namespace Etp {	
		namespace v12 {		
			namespace Datatypes {			
				namespace Object {				
					struct ContextInfo{					
						std::string uri;
						int32_t depth;
						std::vector<std::string> dataObjectTypes;
					};
					
					enum ContextScopeKind {
						self=0,
						sources=1,
						targets=2,
						sourcesOrSelf=3,
						targetsOrSelf=4
					};

					struct Resource{					
						std::string uri;
						std::vector<std::string> alternateUris;
						std::string dataObjectType;
						std::string name;
						boost::optional<int32_t> sourceCount;
						boost::optional<int32_t> targetCount;
						int64_t lastChanged;
						int64_t storeLastWrite;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};
					
					struct DeletedResource{					
						std::string uri;
						std::string dataObjectType;
						int64_t deletedTime;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};

					struct DataObject{					
						Energistics::Etp::v12::Datatypes::Object::Resource resource;
						std::string format;
						boost::optional<Energistics::Etp::v12::Datatypes::Uuid> blobId;
						std::string data;
					};

					struct Dataspace{					
						std::string uri;
						std::string path;
						boost::optional<int64_t> lastChanged;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};
					
					struct Edge{					
						std::string sourceUri;
						std::string targetUri;
						Energistics::Etp::v12::Datatypes::Object::RelationshipKind relationshipKind;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};
					
					enum RelationshipKind {					
						Primary=0,
						Secondary=1,
						Both=2
					};
				}
			}
		}
	}
}
%template(ResourceVector) std::vector<Energistics::Etp::v12::Datatypes::Object::Resource>;
%template(DeletedResourceVector) std::vector<Energistics::Etp::v12::Datatypes::Object::DeletedResource>;
%template(DataspaceVector) std::vector<Energistics::Etp::v12::Datatypes::Object::Dataspace>;
%template(EdgeVector) std::vector<Energistics::Etp::v12::Datatypes::Object::Edge>;
%template(MapStringDataObject) std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject>;
%template(MapStringDataspace) std::map<std::string, Energistics::Etp::v12::Datatypes::Object::Dataspace>;

namespace Energistics {
	namespace Etp {	
		namespace v12 {		
			namespace Datatypes {			
				namespace DataArrayTypes {			
					struct DataArrayIdentifier{					
						std::string uri;
						std::string pathInResource;
					};		
					
					struct DataArray{					
						std::vector<int64_t> dimensions;
						Energistics::Etp::v12::Datatypes::AnyArray data;
					};

					struct DataArrayMetadata{					
						std::vector<int64_t> dimensions;
						Energistics::Etp::v12::Datatypes::AnyArrayType arrayType;
					};

					struct PutDataArraysType{
						Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier uid;
						Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray array;
					};

					struct GetDataSubarraysType{					
						Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier uid;
						std::vector<int64_t> starts;
						std::vector<int64_t> counts;
					};

					struct PutDataSubarraysType{					
						Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier uid;
						Energistics::Etp::v12::Datatypes::AnyArray data;
						std::vector<int64_t> starts;
						std::vector<int64_t> counts;
					};

					struct PutUninitializedDataArrayType{					
						Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier uid;
						Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata metadata;
					};					
				}
			}
		}
	}
}
%template(MapStringDataArrayIdentifier) std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier>;
%template(MapStringDataArray) std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray>;
%template(MapStringDataArrayMetadata) std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata>;
%template(MapStringPutDataArraysType) std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataArraysType>;
%template(MapStringGetDataSubarraysType) std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::GetDataSubarraysType>;
%template(MapStringPutDataSubarraysType) std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataSubarraysType>;
%template(MapStringPutUninitializedDataArrayType) std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutUninitializedDataArrayType>;

/**************** CORE PROTOCOL  *****************/

namespace Energistics {
	namespace Etp {	
		namespace v12 {		
			namespace Protocol {			
				namespace Core {				
					struct RequestSession{					
						std::string applicationName;
						std::string applicationVersion;
						Energistics::Etp::v12::Datatypes::Uuid clientInstanceId;
						std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> requestedProtocols;
						std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> supportedDataObjects;
						std::vector<std::string> supportedCompression;
						std::vector<std::string> supportedFormats;
						int64_t currentDateTime;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> endpointCapabilities;
						static const int messageTypeId=1;
					};
					
					struct OpenSession{					
						std::string applicationName;
						std::string applicationVersion;
						Energistics::Etp::v12::Datatypes::Uuid serverInstanceId;
						std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> supportedProtocols;
						std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> supportedDataObjects;
						std::string supportedCompression;
						std::vector<std::string> supportedFormats;
						Energistics::Etp::v12::Datatypes::Uuid sessionId;
						int64_t currentDateTime;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> endpointCapabilities;
						static const int messageTypeId=2;
					};
					
					struct CloseSession{					
						std::string reason;
						static const int messageTypeId=5;
					};
					
					struct Ping{					
						int64_t currentDateTime;
						static const int messageTypeId=8;
					};
					
					struct Pong{					
						int64_t currentDateTime;
						static const int messageTypeId=9;
					};
					
					struct ProtocolException{					
						boost::optional<Energistics::Etp::v12::Datatypes::ErrorInfo> error;
						std::map<std::string, Energistics::Etp::v12::Datatypes::ErrorInfo> errors;
						static const int messageTypeId=1000;
					};
					
					struct Acknowledge{					
						static const int messageTypeId=1001;
						int protocolId;
					};
				}
			}
		}
	}
}

/**************** DISCOVERY PROTOCOL  *****************/

namespace Energistics {
	namespace Etp {	
		namespace v12 {		
			namespace Protocol {			
				namespace Discovery {				
					struct GetResources{					
						Energistics::Etp::v12::Datatypes::Object::ContextInfo context;
						Energistics::Etp::v12::Datatypes::Object::ContextScopeKind scope;
						bool countObjects=false;
						boost::optional<int64_t> storeLastWriteFilter;
						boost::optional<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind> activeStatusFilter;
						static const int messageTypeId=1;
					};
					
					struct GetResourcesResponse{					
						std::vector<Energistics::Etp::v12::Datatypes::Object::Resource> resources;
						static const int messageTypeId=4;
					};
					
					struct GetDeletedResources{					
						std::string dataspaceUri;
						boost::optional<int64_t> deleteTimeFilter;
						std::vector<std::string> dataObjectTypes;
						static const int messageTypeId=5;
					};
					
					struct GetDeletedResourcesResponse{					
						std::vector<Energistics::Etp::v12::Datatypes::Object::DeletedResource> deletedResources;
						static const int messageTypeId=6;
					};
					
					struct GetResourcesEdgesResponse{					
						std::vector<Energistics::Etp::v12::Datatypes::Object::Edge> edges;
						static const int messageTypeId=7;
					};
				}
			}
		}
	}
}

/**************** STORE PROTOCOL  *****************/

namespace Energistics {
	namespace Etp {	
		namespace v12 {		
			namespace Protocol {			
				namespace Store {				
					struct GetDataObjects{					
						std::map<std::string, std::string> uris;
						std::string format;
						static const int messageTypeId=1;
					};
					
					struct PutDataObjects{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> dataObjects;
						bool pruneContainedObjects=false;
						static const int messageTypeId=2;
					};
					
					struct DeleteDataObjects{					
						std::map<std::string, std::string> uris;
						bool pruneContainedObjects=false;
						static const int messageTypeId=3;
					};
					
					struct GetDataObjectsResponse{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> dataObjects;
						static const int messageTypeId=4;
					};
					
					struct Chunk{					
						Energistics::Etp::v12::Datatypes::Uuid blobId;
						std::string data;
						bool final=false;
						static const int messageTypeId=8;
					};
					
					struct PutDataObjectsResponse{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::PutResponse> success;
						static const int messageTypeId=9;
					};
					
					struct DeleteDataObjectsResponse{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::ArrayOfString> deletedUris;
						static const int messageTypeId=10;
					};
				}
			}
		}
	}
}

/**************** DATA ARRAY PROTOCOL  *****************/

namespace Energistics {
	namespace Etp {	
		namespace v12 {		
			namespace Protocol {			
				namespace DataArray {				
					struct GetDataArrays{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier> dataArrays;
						static const int messageTypeId=2;
					};
					
					struct GetDataArraysResponse{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray> dataArrays;
						static const int messageTypeId=1;
					};
					
					struct GetDataSubarrays{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::GetDataSubarraysType> dataSubarrays;
						static const int messageTypeId=3;
					};
					
					struct GetDataSubarraysResponse{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray> dataSubarrays;
						static const int messageTypeId=8;
					};
					
					struct PutDataArrays{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataArraysType> dataArrays;
						static const int messageTypeId=4;
					};
					
					struct PutDataArraysResponse{					
						std::map<std::string, std::string> success;
						static const int messageTypeId=10;
					};
					
					struct PutDataSubarrays{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataSubarraysType> dataSubarrays;
						static const int messageTypeId=5;
					};
					
					struct PutDataSubarraysResponse{					
						std::map<std::string, std::string> success;
						static const int messageTypeId=11;
					};
					
					struct PutUninitializedDataArrays{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutUninitializedDataArrayType> dataArrays;
						static const int messageTypeId=9;
					};
					
					struct PutUninitializedDataArraysResponse{					
						std::map<std::string, std::string> success;
						static const int messageTypeId=12;
					};
					
					struct GetDataArrayMetadata{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier> dataArrays;
						static const int messageTypeId=6;
					};
					
					struct GetDataArrayMetadataResponse{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata> arrayMetadata;
						static const int messageTypeId=7;
					};
				}
			}
		}
	}
}

/**************** TRANSACTION PROTOCOL  *****************/

namespace Energistics {
	namespace Etp {	
		namespace v12 {		
			namespace Protocol {			
				namespace Transaction {				
					struct StartTransaction{					
						bool readOnly=false;
						std::string message;
						std::vector<std::string> dataspaceUris;
						static const int messageTypeId=1;
					};
					
					struct StartTransactionResponse{					
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						bool successful=false;
						std::string failureReason;
						static const int messageTypeId=2;
					};
					
					struct CommitTransaction{					
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						static const int messageTypeId=3;
					};
					
					struct RollbackTransaction{					
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						static const int messageTypeId=4;
					};
					
					struct CommitTransactionResponse{					
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						bool successful=false;
						std::string failureReason;
						static const int messageTypeId=5;
					};
					
					struct RollbackTransactionResponse{					
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						bool successful=false;
						std::string failureReason;
						static const int messageTypeId=6;
					};
				}
			}
		}
	}
}

/**************** DATASPACE PROTOCOL  *****************/

namespace Energistics {
	namespace Etp {	
		namespace v12 {		
			namespace Protocol {			
				namespace Dataspace {				
					struct GetDataspaces{					
						boost::optional<int64_t> lastChangedFilter;
						static const int messageTypeId=1;
					};
					
					struct GetDataspacesResponse{					
						std::vector<Energistics::Etp::v12::Datatypes::Object::Dataspace> dataspaces;
						static const int messageTypeId=2;
					};
					
					struct PutDataspaces{					
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::Dataspace> dataspaces;
						static const int messageTypeId=3;
					};
					
					struct PutDataspacesResponse{					
						std::map<std::string, std::string> success;
						static const int messageTypeId=6;
					};
					
					struct DeleteDataspaces{					
						std::map<std::string, std::string> uris;
						static const int messageTypeId=4;
					};
					
					struct DeleteDataspacesResponse{					
						std::map<std::string, std::string> success;
						static const int messageTypeId=5;
					};
				}
			}
		}
	}
}

%include <std_shared_ptr.i>
%shared_ptr(ETP_NS::ProtocolHandlers)
%shared_ptr(ETP_NS::CoreHandlers)
%shared_ptr(ETP_NS::DiscoveryHandlers)
%shared_ptr(ETP_NS::StoreHandlers)
%shared_ptr(ETP_NS::StoreNotificationHandlers)
%shared_ptr(ETP_NS::DataArrayHandlers)
%shared_ptr(ETP_NS::TransactionHandlers)
%shared_ptr(ETP_NS::DataspaceHandlers)
%shared_ptr(ETP_NS::AbstractSession)
%shared_ptr(ETP_NS::PlainClientSession)

%feature("director") ETP_NS::CoreHandlers;
%feature("director") ETP_NS::DiscoveryHandlers;
%feature("director") ETP_NS::StoreHandlers;
%feature("director") ETP_NS::StoreNotificationHandlers;
%feature("director") ETP_NS::DataArrayHandlers;
%feature("director") ETP_NS::TransactionHandlers;
%feature("director") ETP_NS::DataspaceHandlers;
%feature("director") ETP_NS::InitializationParameters;
%feature("director") ETP_NS::ServerInitializationParameters;

/* Following extensions aims at preventing the Python garbage collector from 
   garbage collecting a protocol handler that may be still used by a session. */
#ifdef SWIGPYTHON
	%fragment("server_initialization_parameters_reference_init", "init") {
		server_initialization_parameters_reference();
	}
	%fragment("server_initialization_parameters_reference_function", "header", fragment="server_initialization_parameters_reference_init") {
		static PyObject *server_initialization_parameters_reference() {
		  static PyObject *server_initialization_parameters_reference_string = SWIG_Python_str_FromChar("__server_initialization_parameters_reference");
		  return server_initialization_parameters_reference_string;
		}
	}
	%extend ETP_NS::Server {
		%typemap(ret, fragment="server_initialization_parameters_reference_function") void listen(ServerInitializationParameters* serverInitializationParams, int threadCount) %{
		  PyObject_SetAttr($self, server_initialization_parameters_reference(), args);
		%}
	}
	
	%fragment("core_handler_reference_init", "init") {
		core_handler_reference();
	}
	%fragment("core_handler_reference_function", "header", fragment="core_handler_reference_init") {
		static PyObject *core_handler_reference() {
		  static PyObject *core_handler_reference_string = SWIG_Python_str_FromChar("__core_handler_reference");
		  return core_handler_reference_string;
		}
	}
	%extend ETP_NS::AbstractSession {
		%typemap(ret, fragment="core_handler_reference_function") void setCoreProtocolHandlers(std::shared_ptr<ETP_NS::CoreHandlers> coreHandlers) %{
		  PyObject_SetAttr($self, core_handler_reference(), args);
		%}
	}
	
	%fragment("discovery_handler_reference_init", "init") {
		discovery_handler_reference();
	}
	%fragment("discovery_handler_reference_function", "header", fragment="discovery_handler_reference_init") {
		static PyObject *discovery_handler_reference() {
		  static PyObject *discovery_handler_reference_string = SWIG_Python_str_FromChar("__discovery_handler_reference");
		  return discovery_handler_reference_string;
		}
	}
	%extend ETP_NS::AbstractSession {
		%typemap(ret, fragment="discovery_handler_reference_function") void setDiscoveryProtocolHandlers(std::shared_ptr<DiscoveryHandlers> discoveryHandlers) %{
		  PyObject_SetAttr($self, discovery_handler_reference(), args);
		%}
	}
	
	%fragment("store_handler_reference_init", "init") {
	  store_handler_reference();
	}

	%fragment("store_handler_reference_function", "header", fragment="store_handler_reference_init") {

	static PyObject *store_handler_reference() {
	  static PyObject *store_handler_reference_string = SWIG_Python_str_FromChar("__store_handler_reference");
	  return store_handler_reference_string;
	}

	}

	%extend ETP_NS::AbstractSession {
	%typemap(ret, fragment="store_handler_reference_function") void setStoreProtocolHandlers(std::shared_ptr<StoreHandlers> storeHandlers) %{
	  PyObject_SetAttr($self, store_handler_reference(), args);
	%}
	}
	
	%fragment("store_notification_handler_reference_init", "init") {
	  store_notification_handler_reference();
	}

	%fragment("store_notification_handler_reference_function", "header", fragment="store_notification_handler_reference_init") {

	static PyObject *store_notification_handler_reference() {
	  static PyObject *store_notification_handler_reference_string = SWIG_Python_str_FromChar("__store_notification_handler_reference");
	  return store_notification_handler_reference_string;
	}

	}

	%extend ETP_NS::AbstractSession {
	%typemap(ret, fragment="store_notification_handler_reference_function") void setStoreNotificationProtocolHandlers(std::shared_ptr<ETP_NS::StoreNotificationHandlers> storeNotificationHandlers) %{
	  PyObject_SetAttr($self, store_notification_handler_reference(), args);
	%}
	}

// DataArray	
%fragment("data_array_handler_reference_init", "init") {
	  data_array_handler_reference();
}
%fragment("data_array_handler_reference_function", "header", fragment="data_array_handler_reference_init") {

	static PyObject *data_array_handler_reference() {
	  static PyObject *data_array_handler_reference_string = SWIG_Python_str_FromChar("__data_array_handler_reference");
	  return data_array_handler_reference_string;
	}

}

%extend ETP_NS::AbstractSession {
%typemap(ret, fragment="data_array_handler_reference_function") void setDataArrayProtocolHandlers(std::shared_ptr<DataArrayHandlers> dataArrayHandlers) %{
	  PyObject_SetAttr($self, data_array_handler_reference(), args);
	%}
}
	
	// Transaction	
%fragment("transaction_handler_reference_init", "init") {
	  transaction_handler_reference();
}
%fragment("transaction_handler_reference_function", "header", fragment="transaction_handler_reference_init") {

	static PyObject *transaction_handler_reference() {
	  static PyObject *transaction_handler_reference_string = SWIG_Python_str_FromChar("__transaction_handler_reference");
	  return transaction_handler_reference_string;
	}

}

%extend ETP_NS::AbstractSession {
%typemap(ret, fragment="transaction_handler_reference_function") void setTransactionProtocolHandlers(std::shared_ptr<TransactionHandlers> transactionHandlers) %{
	  PyObject_SetAttr($self, transaction_handler_reference(), args);
	%}
}
	
	// Dataspace	
%fragment("dataspace_handler_reference_init", "init") {
	  dataspace_handler_reference();
}
%fragment("dataspace_handler_reference_function", "header", fragment="dataspace_handler_reference_init") {

	static PyObject *dataspace_handler_reference() {
	  static PyObject *dataspace_handler_reference_string = SWIG_Python_str_FromChar("__dataspace_handler_reference");
	  return dataspace_handler_reference_string;
	}

}

%extend ETP_NS::AbstractSession {
%typemap(ret, fragment="dataspace_handler_reference_function") void setDataspaceProtocolHandlers(std::shared_ptr<DataspaceHandlers> dataspaceHandlers) %{
	  PyObject_SetAttr($self, dataspace_handler_reference(), args);
	%}
}
#endif 
	
#ifdef SWIGCSHARP
%typemap(cscode) ETP_NS::Server %{
  private ServerInitializationParameters serverInitializationParametersReference;
%}

%typemap(csin, 
         post="      serverInitializationParametersReference = $csinput;"
         ) ETP_NS::ServerInitializationParameters* serverInitializationParams "ServerInitializationParameters.getCPtr($csinput)"	 


%typemap(cscode) ETP_NS::AbstractSession %{
  private CoreHandlers coreHandlersReference;
  private DiscoveryHandlers discoveryHandlersReference;
  private StoreHandlers storeHandlersReference;
  private StoreNotificationHandlers storeNotificationHandlersReference;
  private DataArrayHandlers dataArrayHandlersReference;
  private TransactionHandlers transactionHandlersReference;
  private DataspaceHandlers dataspaceHandlersReference;
%}

%typemap(csin,
         post="      coreHandlersReference = $csinput;"
         ) std::shared_ptr<ETP_NS::CoreHandlers> coreHandlers "CoreHandlers.getCPtr($csinput)"
		 
%typemap(csin,
         post="      discoveryHandlersReference = $csinput;"
         ) std::shared_ptr<ETP_NS::DiscoveryHandlers> discoveryHandlers "DiscoveryHandlers.getCPtr($csinput)"

%typemap(csin,
         post="      storeHandlersReference = $csinput;"
         ) std::shared_ptr<ETP_NS::StoreHandlers> storeHandlers "StoreHandlers.getCPtr($csinput)"
		 
%typemap(csin,
         post="      storeNotificationHandlersReference = $csinput;"
         ) std::shared_ptr<ETP_NS::StoreNotificationHandlers> storeNotificationHandlers "StoreNotificationHandlers.getCPtr($csinput)"
		 
%typemap(csin,
         post="      dataArrayHandlersReference = $csinput;"
         ) std::shared_ptr<ETP_NS::DataArrayHandlers> dataArrayHandlers "DataArrayHandlers.getCPtr($csinput)"
		 
%typemap(csin,
         post="      transactionHandlersReference = $csinput;"
         ) std::shared_ptr<ETP_NS::TransactionHandlers> transactionHandlers "TransactionHandlers.getCPtr($csinput)"
		 
%typemap(csin,
         post="      dataspaceHandlersReference = $csinput;"
         ) std::shared_ptr<ETP_NS::DataspaceHandlers> dataspaceHandlers "DataspaceHandlers.getCPtr($csinput)"
#endif	  
	  
#ifdef SWIGJAVA
%typemap(javacode) ETP_NS::Server %{
  private ServerInitializationParameters serverInitializationParametersReference;
%}

%typemap(javain, 
         post="      serverInitializationParametersReference = $javainput;"
         ) ETP_NS::ServerInitializationParameters* serverInitializationParams "ServerInitializationParameters.getCPtr($javainput)"	 

%typemap(javacode) ETP_NS::AbstractSession %{
  private CoreHandlers coreHandlersReference;
  private DiscoveryHandlers discoveryHandlersReference;
  private StoreHandlers storeHandlersReference;
  private StoreNotificationHandlers storeNotificationHandlersReference;
  private DataArrayHandlers dataArrayHandlersReference;
  private TransactionHandlers transactionHandlersReference;
  private DataspaceHandlers dataspaceHandlersReference;
%}

%typemap(javain, 
         post="      coreHandlersReference = $javainput;"
         ) std::shared_ptr<ETP_NS::CoreHandlers> coreHandlers "CoreHandlers.getCPtr($javainput)"
		 
%typemap(javain, 
         post="      discoveryHandlersReference = $javainput;"
         ) std::shared_ptr<ETP_NS::DiscoveryHandlers> discoveryHandlers "DiscoveryHandlers.getCPtr($javainput)"
		 
%typemap(javain, 
         post="      storeHandlersReference = $javainput;"
         ) std::shared_ptr<ETP_NS::StoreHandlers> storeHandlers "StoreHandlers.getCPtr($javainput)"
		 
%typemap(javain, 
         post="      storeNotificationHandlersReference = $javainput;"
         ) std::shared_ptr<ETP_NS::StoreNotificationHandlers> storeNotificationHandlers "StoreNotificationHandlers.getCPtr($javainput)"
		 
%typemap(javain, 
         post="      dataArrayHandlersReference = $javainput;"
         ) std::shared_ptr<ETP_NS::DataArrayHandlers> dataArrayHandlers "DataArrayHandlers.getCPtr($javainput)"
		 
%typemap(javain,
         post="      transactionHandlersReference = $javainput;"
         ) std::shared_ptr<ETP_NS::TransactionHandlers> transactionHandlers "TransactionHandlers.getCPtr($javainput)"
		 
%typemap(javain, 
         post="      dataspaceHandlersReference = $javainput;"
         ) std::shared_ptr<ETP_NS::DataspaceHandlers> dataspaceHandlers "DataspaceHandlers.getCPtr($javainput)"
#endif

namespace ETP_NS
{
	class AbstractSession;

	%nodefaultctor ProtocolHandlers;
	class ProtocolHandlers
	{
	public:
		AbstractSession* getSession();
	};

	class CoreHandlers : public ProtocolHandlers
	{
	public:
		CoreHandlers(AbstractSession* mySession);
		virtual ~CoreHandlers();
		
		virtual void on_RequestSession(const Energistics::Etp::v12::Protocol::Core::RequestSession & rs, int64_t correlationId);
	    virtual void on_OpenSession(const Energistics::Etp::v12::Protocol::Core::OpenSession & os, int64_t correlationId);
	    virtual void on_CloseSession(const Energistics::Etp::v12::Protocol::Core::CloseSession & cs, int64_t correlationId);
	    virtual void on_ProtocolException(const Energistics::Etp::v12::Protocol::Core::ProtocolException & pe, int64_t correlationId);
		virtual void on_Acknowledge(const Energistics::Etp::v12::Protocol::Core::Acknowledge & ack, int64_t correlationId);
	};
	
	class DiscoveryHandlers : public ProtocolHandlers
	{
	public:
		DiscoveryHandlers(AbstractSession* mySession);
		virtual ~DiscoveryHandlers();

		virtual void on_GetResources(const Energistics::Etp::v12::Protocol::Discovery::GetResources & msg, int64_t correlationId);
		virtual void on_GetResourcesResponse(const Energistics::Etp::v12::Protocol::Discovery::GetResourcesResponse & msg, int64_t correlationId);
	};
	
	class StoreHandlers : public ProtocolHandlers
	{
	public:
		StoreHandlers(AbstractSession* mySession);
		virtual ~StoreHandlers();

	    virtual void on_GetDataObjects(const Energistics::Etp::v12::Protocol::Store::GetDataObjects & msg, int64_t correlationId);
	    virtual void on_GetDataObjectsResponse(const Energistics::Etp::v12::Protocol::Store::GetDataObjectsResponse & msg, int64_t correlationId);
	    virtual void on_PutDataObjects(const Energistics::Etp::v12::Protocol::Store::PutDataObjects & msg, int64_t correlationId);
		virtual void on_PutDataObjectsResponse(const Energistics::Etp::v12::Protocol::Store::PutDataObjectsResponse & msg, int64_t correlationId);
	    virtual void on_DeleteDataObjects(const Energistics::Etp::v12::Protocol::Store::DeleteDataObjects & msg, int64_t correlationId);
		virtual void on_DeleteDataObjectsResponse(const Energistics::Etp::v12::Protocol::Store::DeleteDataObjectsResponse & msg, int64_t correlationId);
	};
	
	class StoreNotificationHandlers : public ProtocolHandlers
	{
	public:
		StoreNotificationHandlers(AbstractSession* mySession);
		virtual ~StoreNotificationHandlers();

	    virtual void on_SubscribeNotifications(const Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotifications & msg, int64_t messageId);
	    virtual void on_UnsubscribeNotifications(const Energistics::Etp::v12::Protocol::StoreNotification::UnsubscribeNotifications & msg, int64_t messageId, int64_t correlationId);
		virtual void on_UnsolicitedStoreNotifications(const Energistics::Etp::v12::Protocol::StoreNotification::UnsolicitedStoreNotifications & msg, int64_t correlationId);
		virtual void on_SubscriptionEnded(const Energistics::Etp::v12::Protocol::StoreNotification::SubscriptionEnded & msg, int64_t correlationId);
	    virtual void on_ObjectChanged(const Energistics::Etp::v12::Protocol::StoreNotification::ObjectChanged & msg, int64_t correlationId);
	    virtual void on_ObjectDeleted(const Energistics::Etp::v12::Protocol::StoreNotification::ObjectDeleted & msg, int64_t correlationId);
		virtual void on_ObjectAccessRevoked(const Energistics::Etp::v12::Protocol::StoreNotification::ObjectAccessRevoked & msg, int64_t correlationId);
	};
	
	class DataArrayHandlers : public ProtocolHandlers
	{
	public:
		DataArrayHandlers(AbstractSession* mySession);
		virtual ~DataArrayHandlers();

	    virtual void on_GetDataArrays(const Energistics::Etp::v12::Protocol::DataArray::GetDataArrays & gda, int64_t correlationId);
		virtual void on_GetDataArraysResponse(Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse & gdar, int64_t correlationId);
	    virtual void on_PutDataArrays(const Energistics::Etp::v12::Protocol::DataArray::PutDataArrays & pda, int64_t correlationId);
		virtual void on_PutDataArraysResponse(const Energistics::Etp::v12::Protocol::DataArray::PutDataArraysResponse & msg, int64_t correlationId);
	    virtual void on_GetDataSubarrays(const Energistics::Etp::v12::Protocol::DataArray::GetDataSubarrays & msg, int64_t correlationId);
		virtual void on_GetDataSubarraysResponse(const Energistics::Etp::v12::Protocol::DataArray::GetDataSubarraysResponse & msg, int64_t correlationId);
	    virtual void on_PutDataSubarrays(const Energistics::Etp::v12::Protocol::DataArray::PutDataSubarrays & msg, int64_t correlationId);
		virtual void on_PutDataSubarraysResponse(const Energistics::Etp::v12::Protocol::DataArray::PutDataSubarraysResponse & msg, int64_t correlationId);
		virtual void on_GetDataArrayMetadata(const Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata & gdam, int64_t correlationId);
		virtual void on_GetDataArrayMetadataResponse(const Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadataResponse & gdamr, int64_t correlationId);
		virtual void on_PutUninitializedDataArrays(const Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArrays & msg, int64_t correlationId);
		virtual void on_PutUninitializedDataArraysResponse(const Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArraysResponse & msg, int64_t correlationId);
	};
	
	class TransactionHandlers : public ProtocolHandlers
	{
	public:
		TransactionHandlers(AbstractSession* mySession);
		virtual ~TransactionHandlers();

	    virtual void on_StartTransaction(const Energistics::Etp::v12::Protocol::Transaction::StartTransaction & msg, int64_t correlationId);
		virtual void on_StartTransactionResponse(const Energistics::Etp::v12::Protocol::Transaction::StartTransactionResponse & msg, int64_t correlationId);
	    virtual void on_CommitTransaction(const Energistics::Etp::v12::Protocol::Transaction::CommitTransaction & msg, int64_t correlationId);
		virtual void on_CommitTransactionResponse(const Energistics::Etp::v12::Protocol::Transaction::CommitTransactionResponse & msg, int64_t correlationId);
	    virtual void on_RollbackTransaction(const Energistics::Etp::v12::Protocol::Transaction::RollbackTransaction & msg, int64_t correlationId);
		virtual void on_RollbackTransactionResponse(const Energistics::Etp::v12::Protocol::Transaction::RollbackTransactionResponse & msg, int64_t correlationId);
	};
	
	class DataspaceHandlers : public ProtocolHandlers
	{
	public:
		DataspaceHandlers(AbstractSession* mySession);
		virtual ~DataspaceHandlers();

	    virtual void on_GetDataspaces(const Energistics::Etp::v12::Protocol::Dataspace::GetDataspaces & msg, int64_t correlationId);
		virtual void on_GetDataspacesResponse(const Energistics::Etp::v12::Protocol::Dataspace::GetDataspacesResponse & msg, int64_t correlationId);
	    virtual void on_PutDataspaces(const Energistics::Etp::v12::Protocol::Dataspace::PutDataspaces & msg, int64_t correlationId);
		virtual void on_PutDataspacesResponse(const Energistics::Etp::v12::Protocol::Dataspace::PutDataspacesResponse & msg, int64_t correlationId);
	    virtual void on_DeleteDataspaces(const Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspaces & msg, int64_t correlationId);
		virtual void on_DeleteDataspacesResponse(const Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspacesResponse & msg, int64_t correlationId);
	};

	%nodefaultctor AbstractSession;
	class AbstractSession
	{
	public:
		void setCoreProtocolHandlers(std::shared_ptr<ETP_NS::CoreHandlers> coreHandlers);
		void setDiscoveryProtocolHandlers(std::shared_ptr<DiscoveryHandlers> discoveryHandlers);
		void setStoreProtocolHandlers(std::shared_ptr<StoreHandlers> storeHandlers);
		void setStoreNotificationProtocolHandlers(std::shared_ptr<ETP_NS::StoreNotificationHandlers> storeNotificationHandlers);
		void setDataArrayProtocolHandlers(std::shared_ptr<DataArrayHandlers> dataArrayHandlers);
		void setTransactionProtocolHandlers(std::shared_ptr<TransactionHandlers> transactionHandlers);
		void setDataspaceProtocolHandlers(std::shared_ptr<DataspaceHandlers> dataspaceHandlers);
		
		template<typename T> int64_t sendWithSpecificHandler(const T & mb, std::shared_ptr<ETP_NS::ProtocolHandlers> specificHandler, int64_t correlationId = 0, int32_t messageFlags = 0)
		{
			int64_t msgId = encode(mb, correlationId, messageFlags); // put the message to write in the queue

			if (sendingQueue.size() == 1) {
				do_write();
			}
			specificProtocolHandlers[msgId] = specificHandler;

			return msgId;
		}		
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Core::RequestSession>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Core::OpenSession>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Core::CloseSession>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Core::ProtocolException>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Core::Acknowledge>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Core::Ping>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Core::Pong>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Core::RenewSecurityToken>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Core::RenewSecurityTokenResponse>;
		
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Discovery::GetResources>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Discovery::GetResourcesResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Discovery::GetResourcesEdgesResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Discovery::GetDeletedResources>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Discovery::GetDeletedResourcesResponse>;
		
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Store::GetDataObjects>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Store::GetDataObjectsResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Store::PutDataObjects>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Store::PutDataObjectsResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Store::DeleteDataObjects>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Store::DeleteDataObjectsResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Store::Chunk>;
		
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::DataArray::GetDataArrays>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::DataArray::PutDataArrays>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::DataArray::PutDataArraysResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::DataArray::GetDataSubarrays>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::DataArray::GetDataSubarraysResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::DataArray::PutDataSubarrays>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::DataArray::PutDataSubarraysResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadataResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArrays>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArraysResponse>;
		
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Transaction::StartTransaction>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Transaction::StartTransactionResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Transaction::CommitTransaction>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Transaction::CommitTransactionResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Transaction::RollbackTransaction>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Transaction::RollbackTransactionResponse>;
		
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Dataspace::GetDataspaces>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Dataspace::GetDataspacesResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Dataspace::PutDataspaces>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Dataspace::PutDataspacesResponse>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspaces>;
		%template(sendWithSpecificHandler) sendWithSpecificHandler<Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspacesResponse>;
		
		template<typename T> int64_t send(const T & mb, int64_t correlationId = 0, int32_t messageFlags = 0)
		{
			return sendWithSpecificHandler(mb, protocolHandlers[mb.protocolId], correlationId, messageFlags);
		}
		%template(send) send<Energistics::Etp::v12::Protocol::Core::RequestSession>;
		%template(send) send<Energistics::Etp::v12::Protocol::Core::OpenSession>;
		%template(send) send<Energistics::Etp::v12::Protocol::Core::CloseSession>;
		%template(send) send<Energistics::Etp::v12::Protocol::Core::ProtocolException>;
		%template(send) send<Energistics::Etp::v12::Protocol::Core::Acknowledge>;
		%template(send) send<Energistics::Etp::v12::Protocol::Core::Ping>;
		%template(send) send<Energistics::Etp::v12::Protocol::Core::Pong>;
		%template(send) send<Energistics::Etp::v12::Protocol::Core::RenewSecurityToken>;
		%template(send) send<Energistics::Etp::v12::Protocol::Core::RenewSecurityTokenResponse>;
		
		%template(send) send<Energistics::Etp::v12::Protocol::Discovery::GetResources>;
		%template(send) send<Energistics::Etp::v12::Protocol::Discovery::GetResourcesResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::Discovery::GetResourcesEdgesResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::Discovery::GetDeletedResources>;
		%template(send) send<Energistics::Etp::v12::Protocol::Discovery::GetDeletedResourcesResponse>;
		
		%template(send) send<Energistics::Etp::v12::Protocol::Store::GetDataObjects>;
		%template(send) send<Energistics::Etp::v12::Protocol::Store::GetDataObjectsResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::Store::PutDataObjects>;
		%template(send) send<Energistics::Etp::v12::Protocol::Store::PutDataObjectsResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::Store::DeleteDataObjects>;
		%template(send) send<Energistics::Etp::v12::Protocol::Store::DeleteDataObjectsResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::Store::Chunk>;
		
		%template(send) send<Energistics::Etp::v12::Protocol::DataArray::GetDataArrays>;
		%template(send) send<Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::DataArray::PutDataArrays>;
		%template(send) send<Energistics::Etp::v12::Protocol::DataArray::PutDataArraysResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::DataArray::GetDataSubarrays>;
		%template(send) send<Energistics::Etp::v12::Protocol::DataArray::GetDataSubarraysResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::DataArray::PutDataSubarrays>;
		%template(send) send<Energistics::Etp::v12::Protocol::DataArray::PutDataSubarraysResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata>;
		%template(send) send<Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadataResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArrays>;
		%template(send) send<Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArraysResponse>;
		
		%template(send) send<Energistics::Etp::v12::Protocol::Transaction::StartTransaction>;
		%template(send) send<Energistics::Etp::v12::Protocol::Transaction::StartTransactionResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::Transaction::CommitTransaction>;
		%template(send) send<Energistics::Etp::v12::Protocol::Transaction::CommitTransactionResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::Transaction::RollbackTransaction>;
		%template(send) send<Energistics::Etp::v12::Protocol::Transaction::RollbackTransactionResponse>;
		
		%template(send) send<Energistics::Etp::v12::Protocol::Dataspace::GetDataspaces>;
		%template(send) send<Energistics::Etp::v12::Protocol::Dataspace::GetDataspacesResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::Dataspace::PutDataspaces>;
		%template(send) send<Energistics::Etp::v12::Protocol::Dataspace::PutDataspacesResponse>;
		%template(send) send<Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspaces>;
		%template(send) send<Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspacesResponse>;
		
		bool isMessageStillProcessing(int64_t msgId) const;
		void close();
	};

	namespace EtpHelpers {
		Energistics::Etp::v12::Datatypes::ErrorInfo validateUri(const std::string & uri, ETP_NS::AbstractSession* session = nullptr);
		Energistics::Etp::v12::Datatypes::ErrorInfo validateDataObjectUri(const std::string & uri, ETP_NS::AbstractSession* session = nullptr);
		Energistics::Etp::v12::Protocol::Core::ProtocolException buildSingleMessageProtocolException(int32_t m_code, const std::string & m_message);
	}

	/******************* CLIENT ***************************/

	%nodefaultctor PlainClientSession;
	class PlainClientSession : public AbstractSession
	{
	public:
		bool run();
	};
	
	class InitializationParameters
	{
	public:
		InitializationParameters(const std::string & instanceUuid, const std::string & host, unsigned short port);
		virtual ~InitializationParameters();

		void setMaxWebSocketMessagePayloadSize(int64_t value);

		virtual std::string getApplicationName() const;
		virtual std::string getApplicationVersion() const;

		virtual std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> makeSupportedDataObjects() const;
		virtual std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> makeSupportedProtocols() const;

		/**
		* Override this method in order to register some dedicated protocol handlers for a session.
		*/
		virtual void postSessionCreationOperation(AbstractSession* session) const;
	};

	namespace ClientSessionLaunchers
	{
		std::shared_ptr<ETP_NS::PlainClientSession> createWsClientSession(InitializationParameters* initializationParams, const std::string & target, const std::string & authorization,
			std::size_t preferredMaxFrameSize = 4096);
	}
	
	/******************* SERVER ***************************/
	
	class ServerInitializationParameters : public InitializationParameters
	{
	public:
		ServerInitializationParameters(const std::string & serverUuid, const std::string & host, unsigned short port);
		virtual ~ServerInitializationParameters();

		virtual std::string getContactEmail() const;
		virtual std::string getContactName() const;
		virtual std::string getContactPhone() const;
		virtual std::string getOrganizationName() const;

		virtual std::vector<std::string> makeSupportedEncodings() const;
	};
	
	class Server
	{
	public:
		Server();
		std::vector< std::shared_ptr<AbstractSession> >& getSessions();
		void listen(ServerInitializationParameters* serverInitializationParams, int threadCount);
	};
	
#ifdef WITH_FESAPI
%typemap(javaimports) FesapiHdfProxyFactory %{
	import com.f2i_consulting.fesapi.common.HdfProxyFactory;
%}	
	class FesapiHdfProxyFactory : public COMMON_NS::HdfProxyFactory
	{
	public:
		FesapiHdfProxyFactory(PlainClientSession* session);
	};
#endif
}

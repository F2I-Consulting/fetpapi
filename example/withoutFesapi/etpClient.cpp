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
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>

#include "etp/ClientSessionLaunchers.h"
#include "etp/InitializationParameters.h"
#include "etp/ProtocolHandlers/DataArrayHandlers.h"
#include "etp/ProtocolHandlers/StoreNotificationHandlers.h"

#include "MyOwnCoreProtocolHandlers.h"
#include "MyOwnDiscoveryProtocolHandlers.h"
#include "MyOwnStoreProtocolHandlers.h"

using namespace ETP_NS;

void setProtocolHandlers(std::shared_ptr<ETP_NS::AbstractSession> session) {
	session->setCoreProtocolHandlers(std::make_shared<MyOwnCoreProtocolHandlers>(session.get()));
	session->setDiscoveryProtocolHandlers(std::make_shared<MyOwnDiscoveryProtocolHandlers>(session.get()));
	session->setStoreProtocolHandlers(std::make_shared<MyOwnStoreProtocolHandlers>(session.get()));
	session->setDataArrayProtocolHandlers(std::make_shared<ETP_NS::DataArrayHandlers>(session.get()));
	session->setStoreNotificationProtocolHandlers(std::make_shared<ETP_NS::StoreNotificationHandlers>(session.get()));
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "The command must be : fetpapiClient ipAddress port [target]" << std::endl;
		std::cerr << "The command must be : fetpapiClient etpServerUrl (where etpServerUrl must start with ws(s)://...)" << std::endl;
		return 1;
	}

	if (argc == 4 && (argv[3][0] != '/')) {
		std::cerr << "Please put a slash at the start of the target " << argv[3] << std::endl;
		return 1;
	}

	std::cout << "Give your authorization to pass to the server " << argv[1] << " (or hit enter if no authorization)" << std::endl;
	std::string authorization;
	std::getline(std::cin, authorization);

	bool successfulConnection = false;

	boost::uuids::random_generator gen;
	ETP_NS::InitializationParameters initializationParams = argc == 2
		? ETP_NS::InitializationParameters(gen(), argv[1]) // URL based
		: ETP_NS::InitializationParameters(gen(), argv[1], std::stoi(argv[2]), argc < 4 ? "/" : argv[3]); // IP Port and target based
	std::map< std::string, std::string > additionalHeaderField = { {"data-partition-id", "osdu"} }; // Example for OSDU RDDMS
	initializationParams.setAdditionalHandshakeHeaderFields(additionalHeaderField);

	std::cerr << "Creating a client session..." << std::endl;
	auto clientSession = ClientSessionLaunchers::createClientSession(&initializationParams, authorization);
	setProtocolHandlers(clientSession);

	std::cerr << "Running the client session..." << std::endl;
	successfulConnection = clientSession->run();
	
	if (!successfulConnection) {
		if (argc > 2) {
			std::cout << "Could not connect to the server " << argv[1] << " " << argv[2] << std::endl;
		}
		else {
			std::cout << "Could not connect to the server " << argv[1] << std::endl;
		}
	}

#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif
	
	return 0;
}

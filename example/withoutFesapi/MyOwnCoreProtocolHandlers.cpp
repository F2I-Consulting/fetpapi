/*-----------------------------------------------------------------------
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agceements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"; you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agceed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-----------------------------------------------------------------------*/
#include "MyOwnCoreProtocolHandlers.h"

#include <thread>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "etp/EtpHelpers.h"
#include "etp/PlainClientSession.h"

#include "MyOwnDiscoveryProtocolHandlers.h"
#include "MyOwnStoreProtocolHandlers.h"

void printHelp()
{
	std::cout << "List of available commands :" << std::endl;
	std::cout << "\tPing" << std::endl << "\t\tPing the server" << std::endl << std::endl;
	std::cout << "\tList" << std::endl << "\t\tList the objects which have been got from ETP to the in-memory epc" << std::endl << std::endl;
	std::cout << "\tPutXmldAndHdfAtOnce" << std::endl << "\t\tPut a dummy point set representation to the store sending XML and HDF5 points at once." << std::endl << std::endl;
	std::cout << "\tGetResources URI scope(default self) depth(default 1) countObjects(true or false, default is true) dataTypeFilter,dataTypeFilter,...(default noFilter)" << std::endl << std::endl;
	std::cout << "\tGetDataObjects dataObjectURI,dataObjectURI,..." << std::endl << "\t\tGet the objects from an ETP store and store them into the in memory epc (only create partial TARGET relationships, not any SOURCE relationships)" << std::endl << std::endl;
	std::cout << "\tGetXYZPoints URI" << std::endl << "\t\tGet the XYZ points of a rep from store and print some of them." << std::endl << std::endl;
	std::cout << "\tPutDataObject UUID" << std::endl << "\t\tPut the XML part of a dataobject which is on the client side (use \"Load\" command to laod some dataobjects on client side) to the store" << std::endl << std::endl;
	std::cout << "\tGetDataArray epcExternalPartURI datasetPathInEpcExternalPart" << std::endl << "\t\tGet the numerical values from a dataset included in an EpcExternalPart over ETP." << std::endl << std::endl;
	std::cout << "\tPutDataArray epcExternalPartURI datasetPathInEpcExternalPart" << std::endl << "\t\tPut a dummy {0,1,2,3,4,5,6,7,8,9} integer array in a particular store epcExternalPartURI at a particular dataset path" << std::endl << std::endl;
	std::cout << "\tSubscribeNotif URI scope(default self) depth(default 1) receiveXML(true or false, default is true) dataTypeFilter,dataTypeFilter,...(default noFilter)" << std::endl << "\t\tSubscribe to notifications." << std::endl << std::endl;
	std::cout << "\tLoad epcDocument" << std::endl << "\t\tLoad an EPC document into the DataObjectRepository." << std::endl << std::endl;
	std::cout << "\tSetVerbosity 0or1" << std::endl << "\t\tSet the sesion verbosity to true or false" << std::endl << std::endl;
	std::cout << "\tquit" << std::endl << "\t\tQuit the session." << std::endl << std::endl;
}

void askUser(ETP_NS::AbstractSession* session)
{
	std::string buffer;

	std::cout << "What is your command (\"quit\" for closing connection)?" << std::endl;
	std::string command;
	while (command != "quit")
	{
		if (session->isEtpSessionClosed()) {
			command = "quit";
		}
		else {
			std::getline(std::cin, command);
		}
		auto commandTokens = tokenize(command, ' ');

		if (commandTokens.empty()) {
			printHelp();
			continue;
		}
		if (commandTokens[0] == "quit") {
			continue;
		}
			
		if (commandTokens[0] == "GetResources") {
			Energistics::Etp::v12::Protocol::Discovery::GetResources mb;
			mb.context.uri = commandTokens[1];
			mb.scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::self;
			mb.context.depth = 1;
			mb.context.navigableEdges = Energistics::Etp::v12::Datatypes::Object::RelationshipKind::Primary;
			mb.countObjects = true;

			if (commandTokens.size() > 2) {
				if (commandTokens[2] == "self")
					mb.scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::self;
				else if (commandTokens[2] == "sources")
					mb.scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::sources;
				else if (commandTokens[2] == "sourcesOrSelf")
					mb.scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::sourcesOrSelf;
				else if (commandTokens[2] == "targets")
					mb.scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targets;
				else if (commandTokens[2] == "targetsOrSelf")
					mb.scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targetsOrSelf;

				if (commandTokens.size() > 3) {
					mb.context.depth = std::stoi(commandTokens[3]);

					if (commandTokens.size() > 4) {
						if (commandTokens[4] == "false" || commandTokens[4] == "False" || commandTokens[4] == "FALSE") {
							mb.countObjects = false;
						}

						if (commandTokens.size() > 5) {
							mb.context.dataObjectTypes = tokenize(commandTokens[5], ',');
						}
					}
				}
			}

			session->send(mb, 0, 0x02);

			continue;
		}
		else if (commandTokens[0] == "SetVerbosity") {
			if (commandTokens[1].size() == 1 && commandTokens[1][0] == '1') {
				std::cout << "Set the verbosity ON" << std::endl;
				session->setVerbose(true);
			}
			else {
				std::cout << "Set the verbosity OFF" << std::endl;
			}
			session->setVerbose(commandTokens[1].size() == 1 && commandTokens[1][0] == '1');
		}
		else if (commandTokens[0] == "GetDataObjects") {
			Energistics::Etp::v12::Protocol::Store::GetDataObjects getO;
			std::vector<std::string> tokens = tokenize(commandTokens[1], ',');
			std::map<std::string, std::string> tokenMaps;
			for (size_t i = 0; i < tokens.size(); ++i) {
				tokenMaps[std::to_string(i)] = tokens[i];
			}
			getO.uris = tokenMaps;
			session->send(getO, 0, 0x02);
		}
		else if (commandTokens[0] == "SubscribeNotif") {
			Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotifications mb;
			Energistics::Etp::v12::Datatypes::Object::SubscriptionInfo subscriptionInfo;
			subscriptionInfo.context.uri = commandTokens[1];
			subscriptionInfo.scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::self;
			subscriptionInfo.context.depth = 1;
			boost::uuids::random_generator gen;
			boost::uuids::uuid uuid = gen();
			std::move(std::begin(uuid.data), std::end(uuid.data), subscriptionInfo.requestUuid.array.begin());

			if (commandTokens.size() > 2) {
				if (commandTokens[2] == "self")
					subscriptionInfo.scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::self;
				else if (commandTokens[2] == "sources")
					subscriptionInfo.scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::sources;
				else if (commandTokens[2] == "sourcesOrSelf")
					subscriptionInfo.scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::sourcesOrSelf;
				else if (commandTokens[2] == "targets")
					subscriptionInfo.scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targets;
				else if (commandTokens[2] == "targetsOrSelf")
					subscriptionInfo.scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targetsOrSelf;

				if (commandTokens.size() > 3) {
					subscriptionInfo.context.depth = std::stoi(commandTokens[3]);

					if (commandTokens.size() > 4) {
						subscriptionInfo.includeObjectData = commandTokens[4] == "true";

						if (commandTokens.size() > 5) {
							subscriptionInfo.context.dataObjectTypes = tokenize(commandTokens[5], ',');
						}
					}
				}
			}

			mb.request["0"] = subscriptionInfo;

			session->send(mb, 0, 0x02);

			continue;
		}

		if (commandTokens.size() == 1) {
			if (commandTokens[0] == "Ping") {
				Energistics::Etp::v12::Protocol::Core::Ping ping;
				ping.currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				session->send(ping, 0, 0x02); // 0x10 requires Acknowledge from the store
				std::cout << "PING at " << ping.currentDateTime << std::endl;
			}
		}
		else if (commandTokens.size() == 3) {
			if (commandTokens[0] == "GetDataArray") {
				Energistics::Etp::v12::Protocol::DataArray::GetDataArrays gda;
				gda.dataArrays["0"].uri = commandTokens[1];
				gda.dataArrays["0"].pathInResource = commandTokens[2];
				std::cout << gda.dataArrays["0"].pathInResource << std::endl;
				session->send(gda);
			}
			else if (commandTokens[0] == "PutDataArray") {
				Energistics::Etp::v12::Protocol::DataArray::PutDataArrays pda;
				pda.dataArrays["0"].uid.uri = commandTokens[1];
				pda.dataArrays["0"].uid.pathInResource = commandTokens[2];

				std::vector<int64_t> dimensions = { 10 };
				pda.dataArrays["0"].array.dimensions = dimensions;

				Energistics::Etp::v12::Datatypes::AnyArray data;
				Energistics::Etp::v12::Datatypes::ArrayOfInt arrayOfInt;
				arrayOfInt.values = { 0,1,2,3,4,5,6,7,8,9 };
				data.item.set_ArrayOfInt(arrayOfInt);
				pda.dataArrays["0"].array.data = data;

				session->send(pda, 0, 0x02);
			}
		}
	}

	session->close();
}

void MyOwnCoreProtocolHandlers::on_OpenSession(const Energistics::Etp::v12::Protocol::Core::OpenSession & os, int64_t correlationId)
{
	// Ask the user about what he wants to do on another thread
	// The main thread is on reading mode
	std::thread askUserThread(askUser, session);
	askUserThread.detach(); // Detach the thread since we don't want it to be a blocking one.
}

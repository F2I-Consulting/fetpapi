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

#include <fesapi/common/EpcDocument.h>
#include <fesapi/resqml2/BoundaryFeature.h>
#include <fesapi/resqml2/PointSetRepresentation.h>
#include <fesapi/resqml2_0_1/LocalDepth3dCrs.h>

#include "etp/ClientSessionLaunchers.h"
#include "etp/fesapi/FesapiHdfProxy.h"
#include "etp/fesapi/FesapiHelpers.h"

void printHelp()
{
	std::cout << "List of available commands :" << std::endl;
	std::cout << "\tBlockingImport" << std::endl << "\t\tList all dataobjects from the project/study named dataspace (or the first dataspace) and get the first one in a blocking way" << std::endl << std::endl;
	std::cout << "\tBlockingExport" << std::endl << "\t\tPut a dummy horizon feature into a dummy dataspace which is also created" << std::endl << std::endl;
	std::cout << "\tPing" << std::endl << "\t\tPing the server" << std::endl << std::endl;
	std::cout << "\tList" << std::endl << "\t\tList the objects which have been got from ETP to the in-memory Dataobject repository" << std::endl << std::endl;
	std::cout << "\tPutXmlAndHdfAtOnce" << std::endl << "\t\tPut a dummy point set representation to the store sending XML and HDF5 points at once." << std::endl << std::endl;
	std::cout << "\tGetDataspaces" << std::endl << "\t\tGet all store dataspaces" << std::endl << std::endl;
	std::cout << "\tGetResources URI scope(default self) depth(default 1) countObjects(true or false, default is true) includeSecondaryTargets(true or false, default is false) includeSecondarySources(true or false, default is false) dataTypeFilter,dataTypeFilter,...(default noFilter)" << std::endl << std::endl;
	std::cout << "\tGetDataObjects dataObjectURI,dataObjectURI,..." << std::endl << "\t\tGet the objects from an ETP store and store them into the in memory Dataobject repository (only create partial TARGET relationships, not any SOURCE relationships)" << std::endl << std::endl;
	std::cout << "\tGetXYZPoints URI" << std::endl << "\t\tGet the XYZ points of a rep from store and print some of them." << std::endl << std::endl;
	std::cout << "\tPutDataObject UUID" << std::endl << "\t\tPut the XML part of a dataobject which is on the client side (use \"Load\" command to laod some dataobjects on client side) to the store" << std::endl << std::endl;
	std::cout << "\tGetDataArrayMetadata epcExternalPartURI datasetPathInEpcExternalPart" << std::endl << "\t\tGet the metadata of a dataset included in an EpcExternalPart over ETP." << std::endl << std::endl;
	std::cout << "\tGetDataArray epcExternalPartURI datasetPathInEpcExternalPart" << std::endl << "\t\tGet the numerical values from a dataset included in an EpcExternalPart over ETP." << std::endl << std::endl;
	std::cout << "\tPutDataArray epcExternalPartURI datasetPathInEpcExternalPart" << std::endl << "\t\tPut a dummy {0,1,2,3,4,5,6,7,8,9} integer array in a particular store epcExternalPartURI at a particular dataset path" << std::endl << std::endl;
	std::cout << "\tSubscribeNotif URI scope(default self) depth(default 1) receiveXML(true or false, default is true) dataTypeFilter,dataTypeFilter,...(default noFilter)" << std::endl << "\t\tSubscribe to notifications." << std::endl << std::endl;
	std::cout << "\tLoad epcDocument" << std::endl << "\t\tLoad an EPC document into the Dataobject repository." << std::endl << std::endl;
	std::cout << "\tSetVerbosity 0or1" << std::endl << "\t\tSet the sesion verbosity to true or false" << std::endl << std::endl;
	std::cout << "\tDeleteDataObject URI" << std::endl << "\t\tDelete a dataobject" << std::endl << std::endl;
	std::cout << "\tDeleteDataspace URI" << std::endl << "\t\tDelete a dataspace" << std::endl << std::endl;
	std::cout << "\tGetDeletedResources dataspaceURI" << std::endl << "\t\tGet all deleted resources" << std::endl << std::endl;
	std::cout << "\tquit" << std::endl << "\t\tQuit the session." << std::endl << std::endl;
}

void setFetpapiHandlers(std::shared_ptr<ETP_NS::AbstractSession> session) {
	session->setDiscoveryProtocolHandlers(std::make_shared<ETP_NS::DiscoveryHandlers>(session.get()));
	session->setStoreProtocolHandlers(std::make_shared<ETP_NS::StoreHandlers>(session.get()));
	session->setDataArrayProtocolHandlers(std::make_shared<ETP_NS::DataArrayHandlers>(session.get()));
	session->setStoreNotificationProtocolHandlers(std::make_shared<ETP_NS::StoreNotificationHandlers>(session.get()));
	session->setDataspaceProtocolHandlers(std::make_shared<ETP_NS::DataspaceHandlers>(session.get()));
	session->setTransactionProtocolHandlers(std::make_shared<ETP_NS::TransactionHandlers>(session.get()));
}

void askUser(std::shared_ptr<ETP_NS::AbstractSession> session, COMMON_NS::DataObjectRepository& repo)
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

		if (commandTokens[0] == "Load") {
			COMMON_NS::EpcDocument epcDoc(commandTokens[1]);
			epcDoc.deserializeInto(repo);
			std::cout << "LOADED!" << std::endl;
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
		else if (commandTokens[0] == "GetDeletedResources") {
			const auto resources = session->getDeletedResources(commandTokens[1]);
			for (auto& resource : resources) {
				std::cout << resource.uri << " deleted on " << resource.deletedTime << std::endl;
			}
			continue;
		}
		else if (commandTokens[0] == "DeleteDataObject" && commandTokens.size() > 1) {
			std::map< std::string, std::string > query;
			query["0"] = commandTokens[1];
			const auto deleted = session->deleteDataObjects(query);
			if (deleted.empty()) {
				std::cout << "NOT deleted" << std::endl;
			}
			else {
				std::cout << "DELETED" << std::endl;
			}
		}
		else if (commandTokens[0] == "DeleteDataspace" && commandTokens.size() > 1) {
			std::map< std::string, std::string > query;
			query["0"] = commandTokens[1];
			const auto deleted = session->deleteDataspaces(query);
			if (deleted.empty()) {
				std::cout << "NOT deleted" << std::endl;
			}
			else {
				std::cout << "DELETED" << std::endl;
			}
		}
		else if (commandTokens[0] == "GetResources") {
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
							if (commandTokens[5] == "true" || commandTokens[5] == "True" || commandTokens[5] == "TRUE") {
								mb.context.includeSecondaryTargets = true;
							}

							if (commandTokens.size() > 6) {

								if (commandTokens[6] == "true" || commandTokens[6] == "True" || commandTokens[6] == "TRUE") {
									mb.context.includeSecondarySources = true;
								}

								if (commandTokens.size() > 7) {
									mb.context.dataObjectTypes = tokenize(commandTokens[7], ',');
								}
							}
						}
					}
				}
			}

			const auto resources = session->getResources(mb.context, mb.scope);
			for (auto& resource : resources) {
				std::cout << resource.uri << std::endl;
				if (resource.has_sourceCount()) std::cout << "Source count: " << resource.sourceCount.get() << std::endl;
				if (resource.has_targetCount()) std::cout << "Target count: " << resource.targetCount.get() << std::endl;
			}
			continue;
		}
		else if (commandTokens[0] == "GetDataObjects") {
			if (commandTokens.size() == 1) {
				std::cerr << "Please provide some ETP URIs" << std::endl;
				continue;
			}
			std::vector<std::string> tokens = tokenize(commandTokens[1], ',');
			std::map< std::string, std::string > query;
			for (size_t i = 0; i < tokens.size(); ++i) {
				query[std::to_string(i)] = tokens[i];
			}
			const auto dataobjects = session->getDataObjects(query);
			for (const auto& graphResource : dataobjects) {
				std::cout << "*************************************************" << std::endl;
				std::cout << "Resource received : " << std::endl;
				std::cout << "uri : " << graphResource.second.resource.uri << std::endl;
				std::cout << "name : " << graphResource.second.resource.name << std::endl;
				std::cout << "xml : " << graphResource.second.data << std::endl;

				repo.addOrReplaceGsoapProxy(graphResource.second.data, ETP_NS::EtpHelpers::getDataObjectType(graphResource.second.resource.uri), ETP_NS::EtpHelpers::getDataspaceUri(graphResource.second.resource.uri));
			}
		}
		else if (commandTokens[0] == "GetXYZPoints") {
			if (commandTokens.size() == 1) {
				std::cerr << "Please provide some ETP URIs of a RESQML representation" << std::endl;
				continue;
			}
			/* This works in a blocking way i.e. getXyzPointCountOfPatch will return only when the store would have answered back.
			HDF proxy factory and custom HDF proxy are used for that. See main.cpp for setting the custom HDF proxy factory.
			You should also look at MyOwnStoreProtocolHandlers::on_GetDataObjectsResponse which allows to set the session information to the HDF proxy.
			We could have hard set those information thanks to HDF proxy factory.

			If you would want non blocking approach, please see GetDataArrays which require more work to fill in the arguments.
			*/
			std::string uuid = commandTokens[1].substr(commandTokens[1].rfind("(") + 1, 36);
			auto* rep = repo.getDataObjectByUuid<RESQML2_NS::AbstractRepresentation>(uuid);
			if (rep == nullptr) {
				std::cerr << " The UUID " << uuid << " from URI " << commandTokens[1] << " does not correspond to a representation which is on client side. Please get first this dataobject from the store before to call GetXYZPoints on it." << std::endl;
				continue;
			}
			auto xyzPointCount = rep->getXyzPointCountOfPatch(0);
			std::unique_ptr<double[]> xyzPoints(new double[xyzPointCount * 3]);
			rep->getXyzPointsOfPatch(0, xyzPoints.get());
			for (auto xyzPointIndex = 0; xyzPointIndex < xyzPointCount && xyzPointIndex < 20; ++xyzPointIndex) {
				std::cout << "XYZ Point Index " << xyzPointIndex << " : " << xyzPoints[xyzPointIndex * 3] << "," << xyzPoints[xyzPointIndex * 3 + 1] << "," << xyzPoints[xyzPointIndex * 3 + 2] << std::endl;
			}
		}
		else if (commandTokens[0] == "PutDataObject") {
			auto* dataObj = repo.getDataObjectByUuid(commandTokens[1]);
			if (dataObj != nullptr) {
				Energistics::Etp::v12::Protocol::Store::PutDataObjects putDataObjects;
				Energistics::Etp::v12::Datatypes::Object::DataObject dataObject = ETP_NS::FesapiHelpers::buildEtpDataObjectFromEnergisticsObject(dataObj);
				putDataObjects.dataObjects["0"] = dataObject;

				session->send(putDataObjects, 0, 0x10 | 0x02); // 0x10 requires Acknowledge from the store
			}
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
			if (commandTokens[0] == "BlockingImport") {
				std::cout << "************ LIST DATASPACES ************" << std::endl;
				const auto dataspaces = session->getDataspaces();
				for (auto& dataspace : dataspaces) {
					std::cout << dataspace.uri << std::endl;
				}
				std::cout << "************ LIST RESOURCES ************" << std::endl;
				const auto dsIter = std::find_if(dataspaces.begin(), dataspaces.end(),
					[](const Energistics::Etp::v12::Datatypes::Object::Dataspace& ds) { return ds.uri == "eml:///dataspace('project/study')"; });
				Energistics::Etp::v12::Datatypes::Object::ContextInfo ctxInfo;
				ctxInfo.uri = dsIter == dataspaces.end()
					? dataspaces[0].uri
					: dsIter->uri;
				ctxInfo.depth = 0;
				ctxInfo.navigableEdges = Energistics::Etp::v12::Datatypes::Object::RelationshipKind::Both;
				ctxInfo.includeSecondaryTargets = false;
				ctxInfo.includeSecondarySources = false;
				const auto resources = session->getResources(ctxInfo, Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targets);
				for (auto& resource : resources) {
					std::cout << resource.uri << std::endl;
				}
				std::cout << "************ GET FIRST DATAOBJECT ************" << std::endl;
				if (!resources.empty()) {
					std::map< std::string, std::string > query = { { "0", resources[0].uri } };
					const auto dataobject = session->getDataObjects(query);
					if (dataobject.size() == 1) {
						std::cout << dataobject.at("0").data << std::endl;
					}
				}
				else {
					std::cout << "There is no dataobject in this dataspace" << std::endl;
				}
			}
			else if (commandTokens[0] == "BlockingExport") {
				const auto existingDataspaces = session->getDataspaces();
				if (std::find_if(existingDataspaces.begin(), existingDataspaces.end(),
					[](const Energistics::Etp::v12::Datatypes::Object::Dataspace& ds) { return ds.uri == "eml:///dataspace('project/study')"; }) == existingDataspaces.end()) {
					std::cout << "************ PUT DUMMY DATASPACE ************" << std::endl;
					Energistics::Etp::v12::Datatypes::Object::Dataspace dataspace;
					dataspace.uri = "eml:///dataspace('project/study')";
					dataspace.path = "project/study";
					dataspace.storeCreated = 0;
					dataspace.storeLastWrite = 0;
					std::map<std::string, Energistics::Etp::v12::Datatypes::Object::Dataspace> dataspaces = { {"0", dataspace} };
					auto dsAnswer = session->putDataspaces(dataspaces);
					if (std::find(dsAnswer.begin(), dsAnswer.end(), "0") == dsAnswer.end()) {
						std::cerr << "Error when putting the dataspace into the store" << std::endl;
						continue;
					}
				}
				std::cout << "************ PUT DUMMY DATAOBJECT ************" << std::endl;
				COMMON_NS::AbstractObject* horizon = repo.getDataObjectByUuid("6f8c8aa5-7472-4433-a309-74113a419948");
				if (horizon == nullptr) {
					horizon = repo.createHorizon("6f8c8aa5-7472-4433-a309-74113a419948", "dummy horizon");
				}
				horizon->setUriSource("eml:///dataspace('project/study')");
				std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> dataObjects = { {"0", ETP_NS::FesapiHelpers::buildEtpDataObjectFromEnergisticsObject(horizon)} };
				const auto doAnswer = session->putDataObjects(dataObjects);
				if (std::find(doAnswer.begin(), doAnswer.end(), "0") == doAnswer.end()) {
					std::cerr << "Error when putting the dataobject into the store" << std::endl;
				}

				// comment/uncomment below two lines as you want
				//std::cout << "************ DELETE DATASPACE ************" << std::endl;
				//session->deleteDataspaces({ {"0", "eml:///dataspace('project/study')"} });
			}
			else if (commandTokens[0] == "Ping") {
				Energistics::Etp::v12::Protocol::Core::Ping ping;
				ping.currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				session->send(ping, 0, 0x02);
				std::cout << "PING at " << ping.currentDateTime << std::endl;
				std::cout << "Please Set Verbosity to 1 if you don't see anything" << std::endl;
			}
			else if (commandTokens[0] == "GetDataspaces") {
				const auto dataspaces = session->getDataspaces();
				for (auto& dataspace : dataspaces) {
					std::cout << dataspace.uri << std::endl;
				}
			}
			else if (commandTokens[0] == "List") {
				std::cout << "*** START LISTING ***" << std::endl;
				for (const auto& entryPair : repo.getDataObjects()) {
					for (const auto* obj : entryPair.second) {
						if (!obj->isPartial()) {
							std::cout << "******************" << entryPair.first << " : " << obj->getTitle() << "******************" << std::endl;
							std::cout << "*** SOURCE REL ***" << std::endl;
							for (auto srcObj : obj->getRepository()->getSourceObjects(obj)) {
								std::cout << srcObj->getUuid() << " : " << srcObj->getXmlTag() << std::endl;
							}
							std::cout << "*** TARGET REL ***" << std::endl;
							for (auto targetObj : obj->getRepository()->getTargetObjects(obj)) {
								std::cout << targetObj->getUuid() << " : " << targetObj->getXmlTag() << std::endl;
							}
							std::cout << std::endl;
						}
						else {
							std::cout << "PARTIAL " << entryPair.first << " : " << obj->getTitle() << std::endl;
						}
					}
				}
				std::cout << "*** END LISTING ***" << std::endl;
			}
			else if (commandTokens[0] == "PutXmlAndHdfAtOnce") {
				// Create the point set representation, an ETP HDF proxy if necessary and a partial crs
				RESQML2_NS::PointSetRepresentation* h1i1PointSetRep = repo.createPointSetRepresentation("", "Horizon1 Interp1 PointSetRep");
				auto* crs = repo.createPartial<RESQML2_0_1_NS::LocalDepth3dCrs>("", "");
				if (repo.getDefaultHdfProxy() == nullptr) {
					auto* etpHdfProxy = repo.createHdfProxy("", "", "", "", COMMON_NS::DataObjectRepository::openingMode::READ_WRITE);
					repo.setDefaultHdfProxy(etpHdfProxy);
				}

				// Create and push the numerical values to the store
				// Internally it uses the ETP Hdf proxy set as the default HDF proxy of the repository in main.cpp.
				// pushBackGeometryPatch is a blocking method. If you want non blocking method, you need to use PutDataArray directly.
				double pointCoords[18] = { 10, 70, 301, 11, 21, 299, 150, 30, 301, 400, 0, 351, 450, 75, 340, 475, 100, 350 };
				h1i1PointSetRep->pushBackGeometryPatch(6, pointCoords, nullptr, crs);

				// Now send the XML part
				Energistics::Etp::v12::Protocol::Store::PutDataObjects putDataObjects;
				Energistics::Etp::v12::Datatypes::Object::DataObject dataObject = ETP_NS::FesapiHelpers::buildEtpDataObjectFromEnergisticsObject(h1i1PointSetRep);
				putDataObjects.dataObjects["0"] = dataObject;

				session->send(putDataObjects, 0, 0x02 | 0x10); // 0x10 requires Acknowledge from the store
			}
		}
		else if (commandTokens.size() == 3) {
			if (commandTokens[0] == "GetDataArray") {
				Energistics::Etp::v12::Protocol::DataArray::GetDataArrays gda;
				gda.dataArrays["0"].uri = commandTokens[1];
				gda.dataArrays["0"].pathInResource = commandTokens[2];
				std::cout << gda.dataArrays["0"].pathInResource << std::endl;
				session->send(gda, 0, 0x02);
				std::cout << "Please Set Verbosity to 1 if you don't see anything" << std::endl;
			}
			if (commandTokens[0] == "GetDataArrayMetadata") {
				Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata msg;
				msg.dataArrays["0"].uri = commandTokens[1];
				msg.dataArrays["0"].pathInResource = commandTokens[2];
				std::cout << msg.dataArrays["0"].pathInResource << std::endl;
				session->send(msg, 0, 0x02);
				std::cout << "Please Set Verbosity to 1 if you don't see anything" << std::endl;
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

	for (auto* hdfProxy : repo.getHdfProxySet()) {
		hdfProxy->close();
	}
	session->close();
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

	std::cout << "Give your authorization to pass to the server (including Bearer or Basic) " << argv[1] << " (or hit enter if no authorization)" << std::endl;
	std::string authorization;
	std::getline(std::cin, authorization);

	bool successfulConnection = false;

	COMMON_NS::DataObjectRepository repo;
	repo.setDefaultStandard(COMMON_NS::DataObjectRepository::EnergisticsStandard::RESQML2_0_1);
	repo.setDefaultStandard(COMMON_NS::DataObjectRepository::EnergisticsStandard::EML2_0);

	boost::uuids::random_generator gen;
	ETP_NS::InitializationParameters initializationParams = argc == 2
		? ETP_NS::InitializationParameters(gen(), argv[1]) // URL based
		: ETP_NS::InitializationParameters(gen(), argv[1], std::stoi(argv[2]), argc < 4 ? "/" : argv[3]); // IP Port and target based
	std::map< std::string, std::string > additionalHeaderField = { {"data-partition-id", "osdu"} }; // Example for OSDU RDDMS
	initializationParams.setAdditionalHandshakeHeaderFields(additionalHeaderField);

	std::cerr << "Creating a client session..." << std::endl;
	auto clientSession = ETP_NS::ClientSessionLaunchers::createClientSession(&initializationParams, authorization);

	setFetpapiHandlers(clientSession);
	repo.setHdfProxyFactory(new ETP_NS::FesapiHdfProxyFactory(clientSession.get()));

	std::thread sessionThread(&ETP_NS::ClientSession::run, clientSession);
	sessionThread.detach();
	
	// Wait for the ETP session to be opened
	auto t_start = std::chrono::high_resolution_clock::now();
	while (clientSession->isEtpSessionClosed()) {
		auto timeOut = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count();
		if (timeOut > 5000) {
			throw std::invalid_argument("Time out : " + std::to_string(timeOut) + " ms.\n");
		}
	}

	clientSession->setTimeOut(1500000);
	askUser(clientSession, repo);

#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif
	
	return 0;
}

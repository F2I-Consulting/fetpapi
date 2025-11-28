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
#include <fesapi/resqml2/ContinuousProperty.h>
#include <fesapi/resqml2/Grid2dRepresentation.h>
#include <fesapi/resqml2/HorizonInterpretation.h>
#include <fesapi/resqml2/PointSetRepresentation.h>
#include <fesapi/resqml2_0_1/ContinuousProperty.h>
#include <fesapi/resqml2_0_1/LocalDepth3dCrs.h>

#include "etp/ClientSessionLaunchers.h"
#include "etp/fesapi/FesapiHdfProxy.h"
#include "etp/fesapi/FesapiHelpers.h"

namespace {
	std::vector<std::string> tokenize(const std::string& str, char delimiter) {
		std::vector<std::string> tokens;
		std::stringstream ss(str);
		std::string token;
		while (getline(ss, token, delimiter)) {
			tokens.push_back(token);
		}

		return tokens;
	}
}

void printHelp()
{
	std::cout << "List of available commands :" << std::endl;
	std::cout << "\tBlockingImport" << std::endl << "\t\tList all dataobjects from the project/study named dataspace (or the first dataspace) and get the first one in a blocking way" << std::endl << std::endl;
	std::cout << "\tBlockingExport" << std::endl << "\t\tPut a dummy horizon feature into a dummy dataspace which is also created" << std::endl << std::endl;
	std::cout << "\tPing" << std::endl << "\t\tPing the server" << std::endl << std::endl;
	std::cout << "\tPutDummyHorizon" << std::endl << "\t\tPut a dummy horizon to the store" << std::endl << std::endl;
	std::cout << "\tList" << std::endl << "\t\tList the objects which have been got from ETP to the in-memory Dataobject repository" << std::endl << std::endl;
	std::cout << "\tPutXmlAndHdfAtOnce" << std::endl << "\t\tPut a dummy point set representation to the store sending XML and HDF5 points at once." << std::endl << std::endl;
	std::cout << "\tGetDataspaces" << std::endl << "\t\tGet all store dataspaces" << std::endl << std::endl;
	std::cout << "\tPutDataspace dataspaceUri" << std::endl << "\t\tPut a dataspace" << std::endl << std::endl;
	std::cout << "\tGetDataspaceInfo dataspace" << std::endl << "\t\tGet info on a particular dataspace" << std::endl << std::endl;
	std::cout << "\tCopyDataspace sourceDataspace targetDataspace" << std::endl << "\t\tCopy by reference a dataspace into another one" << std::endl << std::endl;
	std::cout << "\tGetResources URI scope(default self) depth(default 1) countObjects(true or false, default is true) includeSecondaryTargets(true or false, default is false) includeSecondarySources(true or false, default is false) dataTypeFilter,dataTypeFilter,...(default noFilter)" << std::endl << std::endl;
	std::cout << "\tGetDataObjects dataObjectURI,dataObjectURI,..." << std::endl << "\t\tGet the objects from an ETP store and store them into the in memory Dataobject repository (only create partial TARGET relationships, not any SOURCE relationships)" << std::endl << std::endl;
	std::cout << "\tGetXYZPoints URI sourcesDepth(default 0)" << std::endl << "\t\tGet the XYZ points of a rep from store and print some of them." << std::endl << std::endl;
	std::cout << "\tCopyDataObjectsByValue URI" << std::endl << "\t\tCopy a dataobject by value on server side." << std::endl << std::endl;
	std::cout << "\tPutDataObject UUID" << std::endl << "\t\tPut the XML part of a dataobject which is on the client side (use \"Load\" command to load some dataobjects on client side) to the store" << std::endl << std::endl;
	std::cout << "\tPutAllDataObjects" << std::endl << "\t\tPut the XML part of all dataobjects which is on the client side (use \"Load\" command to load some dataobjects on client side) to the store" << std::endl << std::endl;
	std::cout << "\tGetDataArrayMetadata epcExternalPartURI datasetPathInEpcExternalPart" << std::endl << "\t\tGet the metadata of a dataset included in an EpcExternalPart over ETP." << std::endl << std::endl;
	std::cout << "\tGetDataArray epcExternalPartURI datasetPathInEpcExternalPart" << std::endl << "\t\tGet the numerical values from a dataset included in an EpcExternalPart over ETP." << std::endl << std::endl;
	std::cout << "\tPutDataArray epcExternalPartURI datasetPathInEpcExternalPart" << std::endl << "\t\tPut a dummy {0,1,2,3,4,5,6,7,8,9} integer array in a particular store epcExternalPartURI at a particular dataset path" << std::endl << std::endl;
	std::cout << "\tSubscribeNotif URI scope(default self) depth(default 1) receiveXML(true or false, default is true) dataTypeFilter,dataTypeFilter,...(default noFilter)" << std::endl << "\t\tSubscribe to notifications." << std::endl << std::endl;
	std::cout << "\tLoad epcDocument" << std::endl << "\t\tLoad an EPC document into the Dataobject repository." << std::endl << std::endl;
	std::cout << "\tSetVerbosity 0or1" << std::endl << "\t\tSet the sesion verbosity to true or false" << std::endl << std::endl;
	std::cout << "\tDeleteDataObject URI" << std::endl << "\t\tDelete a dataobject" << std::endl << std::endl;
	std::cout << "\tDeleteDataspace URI" << std::endl << "\t\tDelete a dataspace" << std::endl << std::endl;
	std::cout << "\tGetDeletedResources dataspaceURI" << std::endl << "\t\tGet all deleted resources" << std::endl << std::endl;
	std::cout << "\tGetAllRepsAndProps dataspace" << std::endl << "\t\tGet all the representations and all their properties from a dataspace" << std::endl << std::endl;
	std::cout << "\tquit" << std::endl << "\t\tQuit the session." << std::endl << std::endl;
}

void askUser(std::shared_ptr<ETP_NS::AbstractSession> session, COMMON_NS::DataObjectRepository& repo)
{
	std::string buffer;

	std::cout << "What is your command (\"quit\" for closing connection)?" << std::endl;
	std::string command;
	while (command != "quit")
	{
		if (session->isEtpSessionClosed()) {
			std::cout << "The ETP session has been lost. You should quit if FETPAPI cannot reconnect by itself" << std::endl;
		}

		std::getline(std::cin, command);
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
				if (resource.has_sourceCount()) std::cout << "Source count: " << resource.sourceCount.value() << std::endl;
				if (resource.has_targetCount()) std::cout << "Target count: " << resource.targetCount.value() << std::endl;
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

			for (std::string warning : repo.getWarnings()) {
				std::cerr << warning << std::endl;
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
			auto xyzPointCount = rep->getXyzPointCountOfAllPatches();
			std::unique_ptr<double[]> xyzPoints(new double[xyzPointCount * 3]);
			rep->getXyzPointsOfAllPatches(xyzPoints.get());
			for (auto xyzPointIndex = 0; xyzPointIndex < xyzPointCount && xyzPointIndex < 20; ++xyzPointIndex) {
				std::cout << "XYZ Point Index " << xyzPointIndex << " : " << xyzPoints[xyzPointIndex * 3] << "," << xyzPoints[xyzPointIndex * 3 + 1] << "," << xyzPoints[xyzPointIndex * 3 + 2] << std::endl;
			}
		}
		else if (commandTokens[0] == "CopyDataObjectsByValue") {
			if (commandTokens.size() == 1) {
				std::cerr << "Please provide some ETP URIs of an Energistics dataobject" << std::endl;
				continue;
			}

			auto copiedUris = session->copyDataObjectsByValue(commandTokens[1], std::stoi(commandTokens.size() > 2 ? commandTokens[2] : 0));
			for (auto& copiedUri : copiedUris) {
				std::cout << "copied into " << copiedUri << std::endl;
			}
		}
		else if (commandTokens[0] == "GetDataspaceInfo") {
			if (commandTokens.size() == 1) {
				std::cerr << "Please provide some ETP URIs of a dataspace" << std::endl;
				continue;
			}
			std::map<std::string, std::string> dataspaceUris;
			std::cout << "Getting dataspace info on " << commandTokens[1] << std::endl;
			dataspaceUris["0"] = commandTokens[1];
			auto dataspaces = session->getDataspaceInfo(dataspaceUris);
			for (auto& dataspace : dataspaces) {
				std::cout << dataspace.uri << std::endl;
			}
		}
		else if (commandTokens[0] == "GetAllRepsAndProps") {
			if (commandTokens.size() == 1) {
				std::cerr << "Please provide some ETP URIs of a dataspace" << std::endl;
				continue;
			}
			std::map<std::string, std::string> dataspaceUris;
			dataspaceUris["0"] = commandTokens[1];

			Energistics::Etp::v12::Datatypes::Object::ContextInfo ctxInfo;
			ctxInfo.uri = dataspaceUris["0"];
			ctxInfo.depth = 0;
			ctxInfo.navigableEdges = Energistics::Etp::v12::Datatypes::Object::RelationshipKind::Both;
			ctxInfo.includeSecondaryTargets = false;
			ctxInfo.includeSecondarySources = false;
			const auto resources = session->getResources(ctxInfo, Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targets);
			std::cout << "************ GET ALL " << resources.size() << " DATAOBJECTS ************" << std::endl;
			if (!resources.empty()) {
				std::map< std::string, std::string > query;
				size_t index = 0;
				for (auto& resource : resources) {
					query[std::to_string(index++)] = resource.uri;
				}
				const auto dataobjects = session->getDataObjects(query);
				for (const auto& dataobjectEntry : dataobjects) {
					repo.addOrReplaceGsoapProxy(dataobjectEntry.second.data, ETP_NS::EtpHelpers::getDataObjectType(dataobjectEntry.second.resource.uri), ETP_NS::EtpHelpers::getDataspaceUri(dataobjectEntry.second.resource.uri));
				}
				// Parse reps
				auto global_start = std::chrono::high_resolution_clock::now();
				for (auto* rep : repo.getDataObjects<RESQML2_NS::AbstractRepresentation>()) {
					std::cout << "Representation " << rep->getTitle() << std::endl;
					std::unique_ptr<double[]> ijkGridPoints(new double[rep->getXyzPointCountOfAllPatches() * 3]);
					auto t_start = std::chrono::high_resolution_clock::now();
					try {
						rep->getXyzPointsOfAllPatches(ijkGridPoints.get());
						std::cout << "XYZ POINTS IN  " << std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() << " ms" << std::endl;
					}
					catch (...) {
						std::cerr << "Error reading XYZ points." << std::endl;
					}

					auto allProps = rep->getValuesPropertySet();
					size_t propIndex = 1;
					for (auto* prop : allProps) {
						size_t valuesCount = prop->getValuesCountOfPatch(0);
						if (dynamic_cast<RESQML2_NS::ContinuousProperty*>(prop) != nullptr) {
							std::cout << "Continuous Prop " << propIndex++ << "/" << allProps.size() << " : " << prop->getTitle() << std::endl;
							std::unique_ptr<double[]> propValues(new double[valuesCount]);
							t_start = std::chrono::high_resolution_clock::now();
							prop->getDoubleValuesOfPatch(0, propValues.get());
							std::cout << "Continuous Prop IN  " << std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() << " ms" << std::endl;
						}
						else {
							std::cout << "Non Continuous Prop " << propIndex++ << "/" << allProps.size() << " : " << prop->getTitle() << std::endl;
							std::unique_ptr<int[]> propValues(new int[valuesCount]);
							t_start = std::chrono::high_resolution_clock::now();
							prop->getInt32ValuesOfPatch(0, propValues.get());
							std::cout << "Non Continuous Prop IN  " << std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() << " ms" << std::endl;
						}
					}
				}
				std::cout << "GLOBALLY DONE IN  " << std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - global_start).count() << " ms" << std::endl;
			}
			else {
				std::cout << "There is no dataobject in this dataspace" << std::endl;
			}
		}
		else if (commandTokens[0] == "PutDataspace") {
			if (commandTokens.size() == 1) {
				std::cerr << "Please provide some ETP URI of a dataspace" << std::endl;
				continue;
			}
			Energistics::Etp::v12::Datatypes::Object::Dataspace dsToPut;
			dsToPut.uri = commandTokens[1];
			size_t pos1 = commandTokens[1].find('\'');
			size_t pos2 = commandTokens[1].rfind('\'');
			dsToPut.path = commandTokens[1].substr(pos1+1, pos2 - pos1 -1);
			std::map<std::string, Energistics::Etp::v12::Datatypes::Object::Dataspace> dssToPut;
			dssToPut["0"] = dsToPut;
			auto result = session->putDataspaces(dssToPut);
			if (result.size() == 1) std::cout << "SUCCESS!" << std::endl;
			else std::cout << "FAILURE!" << std::endl;
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
			std::move(uuid.begin(), uuid.end(), subscriptionInfo.requestUuid.array.begin());

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
				std::cout << "************ GET ALL DATAOBJECTS ************" << std::endl;
				if (!resources.empty()) {
					std::map< std::string, std::string > query;
					size_t index = 0;
					for (auto& resource : resources) {
						std::cout << resource.uri << std::endl;
						query[std::to_string(index++)] = resource.uri;
					}
					const auto dataobject = session->getDataObjects(query);
					std::cout << "************ ONLY SHOW FIRST ONE ************" << std::endl;
					if (dataobject.size() >= 1) {
						std::cout << dataobject.at("0").data << std::endl;
					}
					else {
						std::cout << "No dataobject returned..." << std::endl;
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
			else if (commandTokens[0] == "PutDummyHorizon") {
				Energistics::Etp::v12::Datatypes::Object::Dataspace dataspace;
				dataspace.path = "demo/PutHorizon3";
				dataspace.uri = "eml:///dataspace('" + dataspace.path + "')";
				Energistics::Etp::v12::Datatypes::DataValue dataValue;
				Energistics::Etp::v12::Datatypes::ArrayOfString aos;

				aos.values.push_back("data.default.viewers@osdu.example.com");
				//aos.values.push_back("data.default.viewers@opendes.contoso.com");
				dataValue.item.set_ArrayOfString(aos);
				dataspace.customData["viewers"] = dataValue;

				aos.values[0] = "data.default.owners@osdu.example.com";
				//aos.values[0] = "data.default.owners@opendes.contoso.com";
				dataValue.item.set_ArrayOfString(aos);
				dataspace.customData["owners"] = dataValue;

				aos.values[0] = "osdu-public-usa-dataset";
				//aos.values[0] = "opendes-ReservoirDDMS-Legal-Tag";
				dataValue.item.set_ArrayOfString(aos);
				dataspace.customData["legaltags"] = dataValue;

				aos.values[0] = "US";
				dataValue.item.set_ArrayOfString(aos);
				dataspace.customData["otherRelevantDataCountries"] = dataValue;

				std::map<std::string, std::string> deleteDs;
				deleteDs["0"] = dataspace.uri;
				session->deleteDataspaces(deleteDs);

				std::map<std::string, Energistics::Etp::v12::Datatypes::Object::Dataspace> dataspaces;
				dataspaces["0"] = dataspace;
				auto successKeys = session->putDataspaces(dataspaces);
				if (successKeys.size() == 1) std::cout << "Dataspace has been put" << std::endl;
				else std::cout << "Error when putting dataspace" << std::endl;
				
				COMMON_NS::DataObjectRepository tmpRepo;
				tmpRepo.setDefaultStandard(COMMON_NS::DataObjectRepository::EnergisticsStandard::RESQML2_0_1);
				tmpRepo.setDefaultStandard(COMMON_NS::DataObjectRepository::EnergisticsStandard::EML2_0);
				auto* local_3d_crs = tmpRepo.createLocalDepth3dCrs("b2129512-b8f9-4721-8a70-1abac53ef406", "Default CRS",
					0.0, 0.0, 0.0, 0.0,
					gsoap_resqml2_0_1::eml20__LengthUom::m, 5215,
					gsoap_resqml2_0_1::eml20__LengthUom::m, "Unknown",
					false);
				tmpRepo.setDefaultCrs(local_3d_crs);
				tmpRepo.setHdfProxyFactory(new ETP_NS::FesapiHdfProxyFactory(session.get()));
				auto* hdf_proxy = tmpRepo.createHdfProxy("f8160b8f-0517-4c55-ab6e-ed8bcdc87111", "Hdf Proxy",
					".", "fake.h5",
					COMMON_NS::DataObjectRepository::openingMode::OVERWRITE);
				hdf_proxy->setUriSource(dataspace.uri);
				tmpRepo.setDefaultHdfProxy(hdf_proxy);
				auto* horizon_feature = tmpRepo.createHorizon("c0f12836-41f4-44a8-a3fd-95ac78f6232d", "My horizon feature");
				auto* horizon_interpretation = tmpRepo.createHorizonInterpretation(horizon_feature, "dc217b29-8ceb-4b77-bdcc-6bcfd9cd3baf", "My horizon interpretation");
				auto* horizon_grid_2d_representation = tmpRepo.createGrid2dRepresentation(horizon_interpretation, "7721fb3c-39ba-4d59-ba0b-f9451706a94c", "My horizon representation");

				std::vector< std::string > dataspacesToLock;
				dataspacesToLock.push_back(dataspace.uri);
				
				auto transaction_start = std::chrono::high_resolution_clock::now();

				session->startTransaction(dataspacesToLock);

				const size_t ni = 10000;
				const size_t nj = 1000;
				std::unique_ptr<double[]> resqml_points(new double[ni * nj]);
				for (double i = 0; i < ni * nj; ++i) {
					resqml_points[(int)i] = i * 100;
				}
				horizon_grid_2d_representation->setGeometryAsArray2dOfExplicitZ(resqml_points.get(), ni, nj, hdf_proxy,
					0.0, 0.0, 0.0,
					1.0, 0.0, 0.0, 25.0,
					0.0, 1.0, 0.0, 50.0);

				for (size_t propIndex = 0; propIndex < 1; ++propIndex) {
					auto t_start = std::chrono::high_resolution_clock::now();
					auto* prop = tmpRepo.createContinuousProperty(horizon_grid_2d_representation, "", "", 1, gsoap_eml2_3::eml23__IndexableElement::nodes, gsoap_resqml2_0_1::resqml20__ResqmlUom::m,
						gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind::length);
					std::unique_ptr<double[]> prop_values(new double[ni * nj]);
					prop->pushBackDoubleHdf5Array2dOfValues(prop_values.get(), ni, nj, hdf_proxy);
					std::cout << " Pushed prop " << propIndex << " in " << std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() << " ms" << std::endl;
					std::cout << " Global time  " << std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - transaction_start).count() << " s" << std::endl;
				}

				tmpRepo.setUriSource(dataspace.uri);
				std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> dataobjects;
				auto allUuids = tmpRepo.getUuids();
				int index = 0;
				for (auto& uuid : allUuids)
					dataobjects[std::to_string(index++)] = ETP_NS::FesapiHelpers::buildEtpDataObjectFromEnergisticsObject(tmpRepo, uuid);
				successKeys = session->putDataObjects(dataobjects);
				for (std::string& str : successKeys)
					std::cout << "successKey : " << str << std::endl;

				std::cout << "commit : " << session->commitTransaction() << std::endl;

				if (session != nullptr && !session->isWebSocketSessionClosed())
					horizon_grid_2d_representation->getZValues(resqml_points.get());

				tmpRepo.clear();
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
					for (std::unique_ptr<COMMON_NS::AbstractObject> const& obj : entryPair.second) {
						if (!obj->isPartial()) {
							std::cout << "******************" << entryPair.first << " : " << obj->getTitle() << "******************" << std::endl;
							std::cout << "*** SOURCE REL ***" << std::endl;
							for (auto srcObj : obj->getRepository()->getSourceObjects(obj.get())) {
								std::cout << srcObj->getUuid() << " : " << srcObj->getXmlTag() << std::endl;
							}
							std::cout << "*** TARGET REL ***" << std::endl;
							for (auto targetObj : obj->getRepository()->getTargetObjects(obj.get())) {
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
				h1i1PointSetRep->pushBackXyzGeometryPatch(6, pointCoords, nullptr, crs);

				// Now send the XML part
				Energistics::Etp::v12::Protocol::Store::PutDataObjects putDataObjects;
				Energistics::Etp::v12::Datatypes::Object::DataObject dataObject = ETP_NS::FesapiHelpers::buildEtpDataObjectFromEnergisticsObject(h1i1PointSetRep);
				putDataObjects.dataObjects["0"] = dataObject;

				session->send(putDataObjects, 0, 0x02 | 0x10); // 0x10 requires Acknowledge from the store
			}
			else if (commandTokens[0] == "PutAllDataObjects") {
				std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> putDataObjectsMap;
				auto allUuids = repo.getUuids();
				for (size_t i = 0; i < allUuids.size(); ++i) {
					auto* dataObj = repo.getDataObjectByUuid(allUuids[i]);
					dataObj->setUriSource("eml:///dataspace('pv/test')");
					putDataObjectsMap[std::to_string(i)] = ETP_NS::FesapiHelpers::buildEtpDataObjectFromEnergisticsObject(dataObj);
				}
				auto results = session->putDataObjects(putDataObjectsMap);
				std::cout << "Has put " << results.size() << " on " << putDataObjectsMap.size() << " sent dataobjects into eml:///dataspace('pwls/3.0')";
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
			else if (commandTokens[0] == "GetDataArrayMetadata") {
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
				data.item.set_ArrayOfInt(std::move(arrayOfInt));
				pda.dataArrays["0"].array.data = data;
				std::cout << "Start sending the array" << std::endl;

				session->send(pda, 0, 0x02);
			}
			else if (commandTokens[0] == "CopyDataspace") {
				std::map<std::string, std::string> sourceDataspaceUris;
				sourceDataspaceUris["0"] = commandTokens[1];

				auto result = session->lockDataspaces(sourceDataspaceUris, true);
				if (result.size() == 1) std::cout << "SUCCESS ON LOCK!" << std::endl;
				else std::cout << "FAILURE ON LOCK!" << std::endl;

				result = session->copyDataspacesContent(sourceDataspaceUris, commandTokens[2]);
				if (result.size() == 1) std::cout << "SUCCESS ON COPY!" << std::endl;
				else std::cout << "FAILURE ON COPY!" << std::endl;
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

	std::cout << "Give the data partition id you want to direct your requests (or hit enter if no data partition)" << std::endl;
	std::string dataPartition;
	std::getline(std::cin, dataPartition);

	COMMON_NS::DataObjectRepository repo;
	repo.setDefaultStandard(COMMON_NS::DataObjectRepository::EnergisticsStandard::RESQML2_0_1);
	repo.setDefaultStandard(COMMON_NS::DataObjectRepository::EnergisticsStandard::EML2_0);

	boost::uuids::random_generator gen;
	ETP_NS::InitializationParameters initializationParams = argc == 2
		? ETP_NS::InitializationParameters(gen(), argv[1]) // URL based
		: ETP_NS::InitializationParameters(gen(), argv[1], std::stoi(argv[2]), argc < 4 ? "/" : argv[3]); // IP Port and target based
	std::map< std::string, std::string > additionalHeaderField = { {"data-partition-id", dataPartition} }; // Example for OSDU RDDMS
	initializationParams.setAdditionalHandshakeHeaderFields(additionalHeaderField);

	std::cout << "Creating a client session..." << std::endl;
	auto clientSession = ETP_NS::ClientSessionLaunchers::createClientSession(&initializationParams, authorization);
	clientSession->setVerbose(false);

	repo.setHdfProxyFactory(new ETP_NS::FesapiHdfProxyFactory(clientSession.get()));

	std::thread sessionThread(&ETP_NS::ClientSession::run, clientSession);
	
	// Wait for the ETP session to be opened
	auto t_start = std::chrono::high_resolution_clock::now();
	while (clientSession->isEtpSessionClosed()) {
		auto timeOut = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count();
		if (timeOut > 5000) {
			throw std::invalid_argument("Time out : " + std::to_string(timeOut) + " ms.\n");
		}
	}

	clientSession->setTimeOut(60000);
	askUser(clientSession, repo);

	sessionThread.join();

#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif
	
	return 0;
}
